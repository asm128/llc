 #include "llc_process.h"

// ----------- EXPLAIN, JUSTIFY EACH OF THE INCLUDES, REMOVE THOSE ALREADY INCLUDED INDIRECTLY
// #if defined(__linux__) && !defined(_GNU_SOURCE)
// #	define _GNU_SOURCE
// #endif
// #include "llc_keyval.h"
// 
// #ifdef LLC_WINDOWS
// #	include <Windows.h> // THIS ONE IS NEVER NEEDED. 
// #else
// #	include <unistd.h>
// #endif
// #ifdef LLC_LINUX
// #	include <errno.h>
// #	include <fcntl.h>
// #	include <pthread.h>
// #	include <signal.h>
// #	include <spawn.h>
// #	include <sys/wait.h>
// #endif

sttc	llc::err_t		processHandleClose		(HANDLE & handle) {
	if(0 == handle || INVALID_HANDLE_VALUE == handle) {
		handle					= INVALID_HANDLE_VALUE;
		rtrn 0;
	}
	HANDLE						closing					= handle;
	handle					= INVALID_HANDLE_VALUE;
	rtrn CloseHandle(closing) ? 0 : ::llc::OS_ERROR;
}

llc::err_t			llc::processClose			(SProcess & process) {
	bool					failedToClose				= false;
	HANDLE					stdoutWrite				= process.Startup.hStdOutput;
	if(failed(::processHandleClose(process.Startup.hStdInput)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.Startup.hStdOutput)))
		failedToClose			= true;
	if(process.Startup.hStdError == stdoutWrite)
		process.Startup.hStdError = INVALID_HANDLE_VALUE;
	else if(failed(::processHandleClose(process.Startup.hStdError)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.StdinWrite)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.StdoutRead)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.Info.hThread)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.Info.hProcess)))
		failedToClose			= true;
	process.Info				= {};
	process.Startup			= {sizeof(STARTUPINFOA)};
	rtrn failedToClose ? OS_ERROR : 0;
}

llc::err_t			llc::processRedirect			(SProcess & process) {
	llc_necs(processClose(process));
	SECURITY_ATTRIBUTES		securityAttributes			= {sizeof(SECURITY_ATTRIBUTES), 0, TRUE};
	if(FALSE == CreatePipe(&process.StdoutRead, &process.Startup.hStdOutput, &securityAttributes, 0)) {
		processClose(process);
		rtrn OS_ERROR;
	}
	process.Startup.hStdError = process.Startup.hStdOutput;
	if(FALSE == SetHandleInformation(process.StdoutRead, HANDLE_FLAG_INHERIT, 0)) {
		processClose(process);
		rtrn OS_ERROR;
	}
	if(FALSE == CreatePipe(&process.Startup.hStdInput, &process.StdinWrite, &securityAttributes, 0)) {
		processClose(process);
		rtrn OS_ERROR;
	}
	if(FALSE == SetHandleInformation(process.StdinWrite, HANDLE_FLAG_INHERIT, 0)) {
		processClose(process);
		rtrn OS_ERROR;
	}
	process.Startup.dwFlags |= STARTF_USESTDHANDLES;
	rtrn 0;
}

llc::err_t		llc::processCreate
	( SProcess		& process
	, vcst_t		application
	, vcst_t		commandLine
	, vcs0_t		environment
	, u2_t			creationFlags
	) {
	if(process.Info.hProcess || process.Info.hThread)
		rtrn OS_BUSY;
	string			applicationStorage			= application;
	string			commandLineStorage			= commandLine;
	as0_t			environmentStorage			= environment;
	cnst BOOL				inheritHandles				= (process.Startup.dwFlags & STARTF_USESTDHANDLES) ? TRUE : FALSE;
	if(FALSE == CreateProcessA
		( applicationStorage.size() ? &applicationStorage[0] : 0
		, commandLineStorage.size() ? &commandLineStorage[0] : 0
		, 0
		, 0
		, inheritHandles
		, creationFlags
		, environmentStorage.size() ? &environmentStorage[0] : 0
		, 0
		, &process.Startup
		, &process.Info
		))
		rtrn OS_ERROR;
	HANDLE					stdoutWrite				= process.Startup.hStdOutput;
	bool					failedToClose				= false;
	if(failed(::processHandleClose(process.Startup.hStdInput)))
		failedToClose			= true;
	if(failed(::processHandleClose(process.Startup.hStdOutput)))
		failedToClose			= true;
	if(process.Startup.hStdError == stdoutWrite)
		process.Startup.hStdError = INVALID_HANDLE_VALUE;
	else if(failed(::processHandleClose(process.Startup.hStdError)))
		failedToClose			= true;
	rtrn failedToClose ? OS_ERROR : 0;
}

llc::err_t			llc::processWrite			(SProcess & process, vcu0_t input) {
	if(0 == process.StdinWrite || INVALID_HANDLE_VALUE == process.StdinWrite)
		rtrn input.size() ? OS_ERROR : 0;
	u2_t			offset						= 0;
	while(offset < input.size()) {
		DWORD				written						= 0;
		if(FALSE == WriteFile(process.StdinWrite, input.begin() + offset, input.size() - offset, &written, 0) || 0 == written) {
			::processHandleClose(process.StdinWrite);
			rtrn OS_ERROR;
		}
		offset					+= written;
	}
	rtrn ::processHandleClose(process.StdinWrite);
}

llc::err_t			llc::processRead			(SProcess & process, au0_t & output) {
	if(0 == process.StdoutRead || INVALID_HANDLE_VALUE == process.StdoutRead)
		rtrn OS_ERROR;
	u0_t			buffer[4096]					= {};
	while(true) {
		DWORD				readCount						= 0;
		if(FALSE == ReadFile(process.StdoutRead, buffer, sizeof(buffer), &readCount, 0)) {
			cnst DWORD		lastError						= GetLastError();
			::processHandleClose(process.StdoutRead);
			rtrn ERROR_BROKEN_PIPE == lastError ? 0 : OS_ERROR;
		}
		if(0 == readCount)
			break;
		if(failed(output.append(buffer, readCount))) {
			::processHandleClose(process.StdoutRead);
			rtrn OS_ERROR;
		}
	}
	rtrn ::processHandleClose(process.StdoutRead);
}

llc::err_t			llc::processWait			(SProcess & process, u2_t & exitCode) {
	if(0 == process.Info.hProcess || INVALID_HANDLE_VALUE == process.Info.hProcess)
		rtrn OS_ERROR;
	if(WAIT_OBJECT_0 != WaitForSingleObject(process.Info.hProcess, INFINITE))
		rtrn OS_ERROR;
	DWORD					processExitCode				= 0;
	if(FALSE == GetExitCodeProcess(process.Info.hProcess, &processExitCode))
		rtrn OS_ERROR;
	exitCode				= processExitCode;
	rtrn 0;
}

stct SProcessInput {
	SProcess		& Process;
	vcu0_t		Input;
	llc::err_t		Result = OS_ERROR;
};

// Native thread entry point; the operation's err_t stays in the shared context.
sttc DWORD WINAPI processInputWrite(void * parameter) {
	SProcessInput		& context = *(SProcessInput*)parameter;
	context.Result = processWrite(context.Process, context.Input);
	rtrn 0;
}

sttc llc::err_t processCommunicate(SProcess & process, vcu0_t input, au0_t & output) {
	if(0 == input.size()) {
		llc_necs(processWrite(process, input));
		rtrn processRead(process, output);
	}
	SProcessInput		context = {process, input};
	HANDLE				writer = CreateThread(0, 0, ::processInputWrite, &context, 0, 0);
	if_null_fef(writer, "Cannot start process input writer: %lu.", GetLastError());
	llc::err_t		result = processRead(process, output);
	// Cancellation can race the writer entering WriteFile; retry until it has exited.
	for(;;) {
		cnst DWORD waited = WaitForSingleObject(writer, failed(result) ? 10 : INFINITE);
		if(WAIT_OBJECT_0 == waited)
			break;
		if(WAIT_TIMEOUT != waited)
			result = OS_ERROR;
		if(FALSE == CancelSynchronousIo(writer) && ERROR_NOT_FOUND != GetLastError())
			result = OS_ERROR;
	}
	cnst llc::err_t closeResult = ::processHandleClose(writer);
	rtrn failed(result) ? result : failed(context.Result) ? context.Result : closeResult;
}

llc::err_t			llc::processExecute
	( vcst_t			application
	, vcst_t			commandLine
	, vcu0_t			input
	, au0_t			& output
	, u2_t				& exitCode
	, vcs0_t			environment
	) {
	SProcess			process						= {};
	llc::err_t			result						= processRedirect(process);
	if(succeeded(result)) result = processCreate(process, application, commandLine, environment);
	if(succeeded(result)) result = ::processCommunicate(process, input, output);
	if(succeeded(result)) result = processWait(process, exitCode);
	cnst llc::err_t		closeResult					= processClose(process);
	rtrn failed(result) ? result : closeResult;
}

llc::err_t			llc::processLaunch			(vcst_t application, vcst_t commandLine, vcs0_t environment) {
	SProcess			process						= {};
	llc::err_t			result						= processCreate(process, application, commandLine, environment);
	cnst llc::err_t		closeResult					= processClose(process);
	rtrn failed(result) ? result : closeResult;
}
#endif

#ifdef LLC_LINUX
sttc llc::err_t processDescriptorClose(int & descriptor) {
	if(0 > descriptor)
		rtrn 0;
	int cancellation = 0;
	if(0 != pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancellation))
		rtrn OS_ERROR;
	cnst int closing = descriptor;
	descriptor = -1;
	// Linux releases the descriptor even on EINTR; retrying could close a reused fd.
	cnst int result = close(closing);
	cnst int restored = pthread_setcancelstate(cancellation, 0);
	rtrn result || restored ? OS_ERROR : 0;
}

sttc llc::err_t processPipesClose(SProcess & process) {
	llc::err_t result = 0;
	if(failed(::processDescriptorClose(process.StdinRead))) result = OS_ERROR;
	if(failed(::processDescriptorClose(process.StdinWrite))) result = OS_ERROR;
	if(failed(::processDescriptorClose(process.StdoutRead))) result = OS_ERROR;
	if(failed(::processDescriptorClose(process.StdoutWrite))) result = OS_ERROR;
	rtrn result;
}

sttc void * processReap(void * parameter) {
	cnst pid_t child = (pid_t)(intptr_t)parameter;
	while(0 > waitpid(child, 0, 0) && EINTR == errno) {}
	rtrn 0;
}

llc::err_t llc::processClose(SProcess & process) {
	llc::err_t result = ::processPipesClose(process);
	if(0 >= process.Id)
		rtrn result;
	pid_t waited = -1;
	do { waited = waitpid(process.Id, 0, WNOHANG); } while(0 > waited && EINTR == errno);
	if(0 == waited) {
		pthread_attr_t attributes;
		int error = pthread_attr_init(&attributes);
		if(0 == error) {
			error = pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_DETACHED);
			pthread_t waiter;
			if(0 == error)
				error = pthread_create(&waiter, &attributes, ::processReap, (void*)(intptr_t)process.Id);
			if(0 != pthread_attr_destroy(&attributes)) result = OS_ERROR;
		}
		if(error) {
			// Resource exhaustion must not leave a zombie; fall back to a blocking reap.
			::processReap((void*)(intptr_t)process.Id);
			result = OS_ERROR;
		}
	}
	else if(0 > waited && ECHILD != errno)
		rtrn OS_ERROR;
	process.Id = -1;
	rtrn result;
}

sttc llc::err_t processPipe(int & readEnd, int & writeEnd) {
	int descriptors[2] = {-1, -1};
	if(0 != pipe2(descriptors, O_CLOEXEC))
		rtrn OS_ERROR;
	// Keep spawn's source descriptors distinct from stdin/stdout/stderr, even if closed.
	for(int & descriptor : descriptors) {
		if(descriptor > STDERR_FILENO)
			continue;
		cnst int moved = fcntl(descriptor, F_DUPFD_CLOEXEC, STDERR_FILENO + 1);
		cnst llc::err_t closed = ::processDescriptorClose(descriptor);
		descriptor = moved;
		if(0 > moved || failed(closed)) {
			::processDescriptorClose(descriptors[0]);
			::processDescriptorClose(descriptors[1]);
			rtrn OS_ERROR;
		}
	}
	readEnd = descriptors[0];
	writeEnd = descriptors[1];
	rtrn 0;
}

llc::err_t llc::processRedirect(SProcess & process) {
	if(0 < process.Id)
		rtrn OS_BUSY;
	llc_necs(::processPipesClose(process));
	if(failed(::processPipe(process.StdinRead, process.StdinWrite))
	|| failed(::processPipe(process.StdoutRead, process.StdoutWrite))) {
		::processPipesClose(process);
		rtrn OS_ERROR;
	}
	rtrn 0;
}

// Compact one writable command line into argv storage; no per-argument strings or shell.
// Match MS CRT double quotes/backslashes, including argv[0]'s literal backslashes.
sttc llc::err_t processArguments(as0_t & storage, apod<char*> & arguments) {
	u2_t read = 0, write = 0;
	while(read + 1 < storage.size()) {
		while(' ' == storage[read] || '\t' == storage[read]) ++read;
		if(!storage[read]) break;
		cnst bool programName = 0 == arguments.size();
		llc_necs(arguments.push_back(storage.begin() + write));
		bool quoted = false;
		while(storage[read] && (quoted || (' ' != storage[read] && '\t' != storage[read]))) {
			u2_t slashes = 0;
			if(!programName)
				while('\\' == storage[read]) { ++slashes; ++read; }
			cnst bool quote = '"' == storage[read];
			for(u2_t i = 0, count = quote ? slashes / 2 : slashes; i < count; ++i)
				storage[write++] = '\\';
			if(quote) {
				++read;
				if(slashes & 1U)
					storage[write++] = '"';
				else if(!programName && quoted && '"' == storage[read]) {
					storage[write++] = '"';
					++read;
				}
				else
					quoted = !quoted;
			}
			else if(storage[read] && (quoted || (' ' != storage[read] && '\t' != storage[read])))
				storage[write++] = storage[read++];
		}
		// Advance before replacing the separator when read and write coincide.
		while(' ' == storage[read] || '\t' == storage[read]) ++read;
		storage[write++] = 0;
	}
	if_true_fef(!arguments.size() || !arguments[0][0], "%s", "Command line must include a nonempty program name.");
	llc_necs(arguments.push_back(0));
	rtrn 0;
}

llc::err_t llc::processCreate(SProcess & process, vcst_t application, vcst_t commandLine, vcs0_t environment, u2_t creationFlags) {
	if(0 < process.Id)
		rtrn OS_BUSY;
	if_true_fef(creationFlags, "%s", "Windows process creation flags are unsupported on Linux.");
	if_true_fef(application.size() && memchr(application.begin(), 0, application.size()), "%s", "Application contains an embedded zero.");
	if_true_fef(commandLine.size() && memchr(commandLine.begin(), 0, commandLine.size()), "%s", "Command line contains an embedded zero.");
	as0_t argumentStorage, environmentStorage = environment;
	apod<char*> argv, envp;
	llc_necs(argumentStorage.append(commandLine.begin(), commandLine.size()));
	llc_necs(argumentStorage.push_back(0));
	llc_necs(::processArguments(argumentStorage, argv));
	string executable = application;
	for(u2_t offset = 0; offset < environmentStorage.size() && environmentStorage[offset]; ) {
		char * end = (char*)memchr(environmentStorage.begin() + offset, 0, environmentStorage.size() - offset);
		if_null_fef(end, "%s", "Unterminated process environment entry.");
		llc_necs(envp.push_back(environmentStorage.begin() + offset));
		offset = (u2_t)(end - environmentStorage.begin()) + 1;
	}
	llc_necs(envp.push_back(0));
	cnst bool redirected = 0 <= process.StdinRead && 0 <= process.StdinWrite && 0 <= process.StdoutRead && 0 <= process.StdoutWrite;
	if_true_fef(!redirected && (0 <= process.StdinRead || 0 <= process.StdinWrite || 0 <= process.StdoutRead || 0 <= process.StdoutWrite), "%s", "Incomplete process pipe setup.");
	posix_spawn_file_actions_t actions;
	if(0 != posix_spawn_file_actions_init(&actions))
		rtrn OS_ERROR;
	int error = 0;
	if(redirected) {
		error = posix_spawn_file_actions_adddup2(&actions, process.StdinRead, STDIN_FILENO);
		if(!error) error = posix_spawn_file_actions_adddup2(&actions, process.StdoutWrite, STDOUT_FILENO);
		if(!error) error = posix_spawn_file_actions_adddup2(&actions, process.StdoutWrite, STDERR_FILENO);
		if(!error) error = posix_spawn_file_actions_addclose(&actions, process.StdinRead);
		if(!error) error = posix_spawn_file_actions_addclose(&actions, process.StdinWrite);
		if(!error) error = posix_spawn_file_actions_addclose(&actions, process.StdoutRead);
		if(!error) error = posix_spawn_file_actions_addclose(&actions, process.StdoutWrite);
	}
	if(!error) {
		if(application.size())
			error = posix_spawn(&process.Id, executable.begin(), &actions, 0, argv.begin(), environment.size() ? envp.begin() : environ);
		else
			error = posix_spawnp(&process.Id, argv[0], &actions, 0, argv.begin(), environment.size() ? envp.begin() : environ);
	}
	cnst int destroyed = posix_spawn_file_actions_destroy(&actions);
	if(error) {
		process.Id = -1;
		rtrn OS_ERROR;
	}
	cnst llc::err_t closedInput = ::processDescriptorClose(process.StdinRead);
	cnst llc::err_t closedOutput = ::processDescriptorClose(process.StdoutWrite);
	rtrn destroyed || failed(closedInput) || failed(closedOutput) ? OS_ERROR : 0;
}

llc::err_t llc::processWrite(SProcess & process, vcu0_t input) {
	if(0 > process.StdinWrite)
		rtrn input.size() ? OS_ERROR : 0;
	// Block SIGPIPE only in this thread; never change the application's signal disposition.
	sigset_t blocked, previous, pending;
	sigemptyset(&blocked);
	sigaddset(&blocked, SIGPIPE);
	if(0 != pthread_sigmask(SIG_BLOCK, &blocked, &previous))
		rtrn OS_ERROR;
	llc::err_t result = 0;
	cnst int pendingResult = sigpending(&pending);
	cnst bool alreadyPending = 0 == pendingResult && 1 == sigismember(&pending, SIGPIPE);
	if(pendingResult)
		result = OS_ERROR;
	bool brokenPipe = false;
	for(u2_t offset = 0; succeeded(result) && offset < input.size(); ) {
		cnst ssize_t written = write(process.StdinWrite, input.begin() + offset, input.size() - offset);
		if(0 > written && EINTR == errno)
			continue;
		if(0 >= written) {
			brokenPipe = 0 > written && EPIPE == errno;
			result = OS_ERROR;
			break;
		}
		offset += (u2_t)written;
	}
	if(brokenPipe && !alreadyPending) {
		cnst timespec noWait = {};
		while(0 > sigtimedwait(&blocked, 0, &noWait) && EINTR == errno) {}
	}
	if(0 != pthread_sigmask(SIG_SETMASK, &previous, 0)) result = OS_ERROR;
	cnst llc::err_t closed = ::processDescriptorClose(process.StdinWrite);
	rtrn failed(result) ? result : closed;
}

llc::err_t llc::processRead(SProcess & process, au0_t & output) {
	if(0 > process.StdoutRead)
		rtrn OS_ERROR;
	u0_t buffer[4096] = {};
	llc::err_t result = 0;
	for(;;) {
		cnst ssize_t count = read(process.StdoutRead, buffer, sizeof(buffer));
		if(0 > count && EINTR == errno)
			continue;
		if(0 >= count) {
			if(0 > count) result = OS_ERROR;
			break;
		}
		if(failed(output.append(buffer, (u2_t)count))) {
			result = OS_ERROR;
			break;
		}
	}
	cnst llc::err_t closed = ::processDescriptorClose(process.StdoutRead);
	rtrn failed(result) ? result : closed;
}

llc::err_t llc::processWait(SProcess & process, u2_t & exitCode) {
	if(0 >= process.Id)
		rtrn OS_ERROR;
	int status = 0;
	pid_t waited;
	do { waited = waitpid(process.Id, &status, 0); } while(0 > waited && EINTR == errno);
	if(0 > waited) {
		if(ECHILD == errno) process.Id = -1;
		rtrn OS_ERROR;
	}
	process.Id = -1;
	exitCode = WIFEXITED(status) ? (u2_t)WEXITSTATUS(status) : 128U + (u2_t)WTERMSIG(status);
	rtrn 0;
}

stct SProcessInput {
	SProcess & Process;
	vcu0_t Input;
	llc::err_t Result = OS_ERROR;
};

sttc void * processInputWrite(void * parameter) {
	SProcessInput & context = *(SProcessInput*)parameter;
	context.Result = processWrite(context.Process, context.Input);
	rtrn 0;
}

llc::err_t llc::processExecute(vcst_t application, vcst_t commandLine, vcu0_t input, au0_t & output, u2_t & exitCode, vcs0_t environment) {
	SProcess process;
	llc::err_t result = processRedirect(process);
	if(succeeded(result)) result = processCreate(process, application, commandLine, environment);
	if(succeeded(result) && input.size()) {
		SProcessInput context = {process, input};
		pthread_t writer;
		if(0 != pthread_create(&writer, 0, ::processInputWrite, &context))
			result = OS_ERROR;
		else {
			result = processRead(process, output);
			if(failed(result) && 0 != pthread_cancel(writer)) result = OS_ERROR;
			cnst int joined = pthread_join(writer, 0);
			if(joined || failed(context.Result)) result = OS_ERROR;
		}
	}
	else if(succeeded(result)) {
		result = processWrite(process, input);
		if(succeeded(result)) result = processRead(process, output);
	}
	if(succeeded(result)) result = processWait(process, exitCode);
	cnst llc::err_t closed = processClose(process);
	rtrn failed(result) ? result : closed;
}

llc::err_t llc::processLaunch(vcst_t application, vcst_t commandLine, vcs0_t environment) {
	SProcess process;
	cnst llc::err_t result = processCreate(process, application, commandLine, environment);
	cnst llc::err_t closed = processClose(process);
	rtrn failed(result) ? result : closed;
}
