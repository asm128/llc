#include "llc_process_internal.h"

#ifdef LLC_WINDOWS
// Windows declarations already arrive through LLC's synchronization headers.
namespace llc::process_detail {
sttc	llc::err_t		processHandleClose		(TWindowsHandle & handle) {
	if(0 == handle || INVALID_WINDOWS_HANDLE == handle) {
		handle					= INVALID_WINDOWS_HANDLE;
		rtrn 0;
	}
	TWindowsHandle				closing					= handle;
	handle					= INVALID_WINDOWS_HANDLE;
	rtrn CloseHandle((HANDLE)closing) ? 0 : -1;
}

stct auto_process_handle : auto_handler<TWindowsHandle*, nullptr> {
	explicit auto_process_handle(TWindowsHandle & handle) nxpt { Handle = &handle; }
	~auto_process_handle() { if_fail_e(Close()); }
	err_t Close() {
		if(!Handle) rtrn 0;
		TWindowsHandle * closing = Handle;
		Handle = 0;
		rtrn processHandleClose(*closing);
	}
};

llc::err_t			processClose			(SProcessWin32 & process) {
	bool					failedToClose				= false;
	TWindowsHandle					stdoutWrite				= process.Startup.HandleStdOutput;
	if(failed(processHandleClose(process.Startup.HandleStdInput)))
		failedToClose			= true;
	if(failed(processHandleClose(process.Startup.HandleStdOutput)))
		failedToClose			= true;
	if(process.Startup.HandleStdError == stdoutWrite)
		process.Startup.HandleStdError = INVALID_WINDOWS_HANDLE;
	else if(failed(processHandleClose(process.Startup.HandleStdError)))
		failedToClose			= true;
	if(failed(processHandleClose(process.StdinWrite)))
		failedToClose			= true;
	if(failed(processHandleClose(process.StdoutRead)))
		failedToClose			= true;
	if(failed(processHandleClose(process.StderrRead)))
		failedToClose			= true;
	if(failed(processHandleClose(process.Info.hThread)))
		failedToClose			= true;
	if(failed(processHandleClose(process.Info.hProcess)))
		failedToClose			= true;
	process.Info				= {};
	process.Startup			= {sizeof(STARTUPINFOA)};
	rtrn failedToClose ? -1 : 0;
}

llc::err_t processRedirect(SProcessWin32 & process, bool separateError) {
	if(process.Info.hProcess || process.Info.hThread)
		rtrn -1;
	if_fail_fe(processClose(process));
	SProcessNativeScope<SProcessWin32> rollback{process};
	SECURITY_ATTRIBUTES security = {sizeof(SECURITY_ATTRIBUTES), 0, TRUE};
	HANDLE readEnd = 0, writeEnd = 0;
	if(!CreatePipe(&readEnd, &writeEnd, &security, 0))
		rtrn -1;
	process.StdoutRead = (TWindowsHandle)readEnd;
	process.Startup.HandleStdOutput = (TWindowsHandle)writeEnd;
	if_true_ve(-1, !SetHandleInformation(readEnd, HANDLE_FLAG_INHERIT, 0));
	if(separateError) {
		if_true_ve(-1, !CreatePipe(&readEnd, &writeEnd, &security, 0));
		process.StderrRead = (TWindowsHandle)readEnd;
		process.Startup.HandleStdError = (TWindowsHandle)writeEnd;
		if_true_ve(-1, !SetHandleInformation(readEnd, HANDLE_FLAG_INHERIT, 0));
	}
	else
		process.Startup.HandleStdError = process.Startup.HandleStdOutput;
	if_true_ve(-1, !CreatePipe(&readEnd, &writeEnd, &security, 0));
	process.Startup.HandleStdInput = (TWindowsHandle)readEnd;
	process.StdinWrite = (TWindowsHandle)writeEnd;
	if_true_ve(-1, !SetHandleInformation(writeEnd, HANDLE_FLAG_INHERIT, 0));
	process.Startup.Flags |= STARTF_USESTDHANDLES;
	rollback.Inactive = true;
	rtrn 0;
}

tplT ndsi	T *	data_or_null		(view<T> yourView)	nxpt	{ return yourView.size() ? &yourView[0] : 0; }

llc::err_t		processCreate
	( SProcessWin32	& process
	, vcst_t		application
	, vcst_t		commandLine
	, vcs0_t		environment
	, u2_t			creationFlags
	) {
	if(process.Info.hProcess || process.Info.hThread)
		rtrn -1;
	string					applicationStorage			= application;
	string					commandLineStorage			= commandLine;
	as0_t					environmentStorage			= environment;
	cnst BOOL				inheritHandles				= (process.Startup.Flags & STARTF_USESTDHANDLES) ? TRUE : FALSE;
	STARTUPINFOA			startup						= {};
	::llc::cpycst(startup, process.Startup);
	PROCESS_INFORMATION		info						= {};
	if_zero_vef(-1, CreateProcessA(data_or_null(applicationStorage), data_or_null(commandLineStorage), 0, 0, inheritHandles, creationFlags, data_or_null(environmentStorage), 0, &startup, &info)
		, "\"%.*s\", \"%.*s\"", LLC_VIEW_ARGS(applicationStorage), LLC_VIEW_ARGS(commandLineStorage)
	);
	llc::cpycst(process.Info, info);
	TWindowsHandle			stdoutWrite					= process.Startup.HandleStdOutput;
	bool					failedToClose				= false;
	if(failed(processHandleClose(process.Startup.HandleStdInput)))
		failedToClose			= true;
	if(failed(processHandleClose(process.Startup.HandleStdOutput)))
		failedToClose			= true;
	if(process.Startup.HandleStdError == stdoutWrite)
		process.Startup.HandleStdError = INVALID_WINDOWS_HANDLE;
	else if(failed(processHandleClose(process.Startup.HandleStdError)))
		failedToClose			= true;
	rtrn failedToClose ? -1 : 0;
}

llc::err_t			processWrite			(SProcessWin32 & process, vcu0_t input) {
	if(0 == process.StdinWrite || INVALID_WINDOWS_HANDLE == process.StdinWrite)
		rtrn input.size() ? -1 : 0;
	auto_process_handle scope{process.StdinWrite};
	u2_t			offset						= 0;
	while(offset < input.size()) {
		DWORD				written						= 0;
		if_true_ve(-1, FALSE == WriteFile((HANDLE)process.StdinWrite, input.begin() + offset, input.size() - offset, &written, 0) || 0 == written);
		offset					+= written;
	}
	rtrn scope.Close();
}

sttc llc::err_t		processPipeRead		(TWindowsHandle & pipe, au0_t & output) {
	if(0 == pipe || INVALID_WINDOWS_HANDLE == pipe)
		rtrn -1;
	auto_process_handle scope{pipe};
	u0_t					buffer[4096]					= {};
	while(true) {
		DWORD					readCount						= 0;
		if(FALSE == ReadFile((HANDLE)pipe, buffer, sizeof(buffer), &readCount, 0)) {
			cnst DWORD				lastError						= GetLastError();
			rtrn ERROR_BROKEN_PIPE == lastError ? 0 : -1;
		}
		if(0 == readCount)
			break;
		if_fail_ve(-1, output.append(buffer, readCount));
	}
	rtrn scope.Close();
}

llc::err_t			processRead			(SProcessWin32 & process, au0_t & output) {
	rtrn processPipeRead(process.StdoutRead, output);
}

stct SProcessOutput {
	TWindowsHandle	& Pipe;
	au0_t			& Output;
	llc::err_t	Result		= -1;
};

sttc DWORD WINAPI threadOutputRead(void * parameter) {
	SProcessOutput & context = *(SProcessOutput*)parameter;
	context.Result = processPipeRead(context.Pipe, context.Output);
	rtrn 0;
}

stct auto_process_reader : auto_handler<HANDLE, nullptr> {
	~auto_process_reader() { if_fail_e(Close()); }
	err_t Close() {
		if(!Handle) rtrn 0;
		cnst DWORD waited = WaitForSingleObject(Handle, INFINITE);
		cnst HANDLE closing = Handle;
		Handle = 0;
		cnst BOOL closed = CloseHandle(closing);
		rtrn WAIT_OBJECT_0 == waited && closed ? 0 : -1;
	}
};

llc::err_t processRead(SProcessWin32 & process, au0_t & output, au0_t & errorOutput) {
	if(0 == process.StderrRead || INVALID_WINDOWS_HANDLE == process.StderrRead)
		rtrn -1;
	SProcessOutput context = {process.StderrRead, errorOutput};
	auto_process_reader reader;
	reader.Handle = CreateThread(0, 0, threadOutputRead, &context, 0, 0);
	if_null_fef(reader.Handle, "Cannot start process error reader: %lu.", GetLastError());
	cnst err_t outputResult = processPipeRead(process.StdoutRead, output);
	cnst err_t closeResult = reader.Close();
	rtrn failed(outputResult) ? outputResult : failed(context.Result) ? context.Result : closeResult;
}

llc::err_t			processWait			(SProcessWin32 & process, u2_t & exitCode) {
	if(0 == process.Info.hProcess || INVALID_WINDOWS_HANDLE == process.Info.hProcess)
		rtrn -1;
	if(WAIT_OBJECT_0 != WaitForSingleObject((HANDLE)process.Info.hProcess, INFINITE))
		rtrn -1;
	DWORD					processExitCode		= 0;
	if(FALSE == GetExitCodeProcess((HANDLE)process.Info.hProcess, &processExitCode))
		rtrn -1;
	exitCode			= processExitCode;
	rtrn 0;
}

stct SProcessInput {
	SProcessWin32	& Process;
	vcu0_t			Input;
	llc::err_t		Result		= -1;
};

// Native thread entry point; the operation's err_t stays in the shared context.
sttc DWORD WINAPI threadInputWrite(void * parameter) {
	SProcessInput		& context = *(SProcessInput*)parameter;
	context.Result = processWrite(context.Process, context.Input);
	rtrn 0;
}

stct auto_process_writer : auto_handler<HANDLE, nullptr> {
	err_t Result = -1;
	~auto_process_writer() { if_fail_e(Close()); }
	err_t Close() {
		if(!Handle) rtrn 0;
		// Cancellation can race the writer entering WriteFile; retry until it has exited.
		for(;;) {
			cnst DWORD waited = WaitForSingleObject(Handle, failed(Result) ? 10 : INFINITE);
			if(WAIT_OBJECT_0 == waited)
				break;
			if(WAIT_TIMEOUT != waited)
				Result = -1;
			if(FALSE == CancelSynchronousIo(Handle) && ERROR_NOT_FOUND != GetLastError())
				Result = -1;
		}
		cnst HANDLE closing = Handle;
		Handle = 0;
		rtrn CloseHandle(closing) ? 0 : -1;
	}
};

llc::err_t processCommunicate(SProcessWin32 & process, vcu0_t input, au0_t & output) {
	if(0 == input.size()) {
		if_fail_fe(processWrite(process, input));
		rtrn processRead(process, output);
	}
	SProcessInput context = {process, input};
	auto_process_writer writer;
	writer.Handle = CreateThread(0, 0, threadInputWrite, &context, 0, 0);
	if_null_fef(writer.Handle, "Cannot start process input writer: %lu.", GetLastError());
	writer.Result = processRead(process, output);
	cnst err_t closeResult = writer.Close();
	rtrn failed(writer.Result) ? writer.Result : failed(context.Result) ? context.Result : closeResult;
}

llc::err_t processCommunicate(SProcessWin32 & process, vcu0_t input, au0_t & output, au0_t & errorOutput) {
	if(0 == input.size()) {
		if_fail_fe(processWrite(process, input));
		rtrn processRead(process, output, errorOutput);
	}
	SProcessInput context = {process, input};
	auto_process_writer writer;
	writer.Handle = CreateThread(0, 0, threadInputWrite, &context, 0, 0);
	if_null_fef(writer.Handle, "Cannot start process input writer: %lu.", GetLastError());
	writer.Result = processRead(process, output, errorOutput);
	cnst err_t closeResult = writer.Close();
	rtrn failed(writer.Result) ? writer.Result : failed(context.Result) ? context.Result : closeResult;
}

}
#endif

