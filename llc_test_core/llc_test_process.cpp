#include "llc_process.h"
#include "llc_args.h"
#include "llc_string_compose.h"
#include "llc_test_core.h"
#ifdef LLC_LINUX
#	include <errno.h>
#	include <pthread.h>
#	include <signal.h>
#	include <unistd.h>
#endif

GDEFINE_ENUM_TYPE(PROCESS_TEST_RESULT, ::llc::u0_t);
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_RESULT			, 1, "environmentBlockViews() rejected a valid environment block.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_COUNT			, 2, "environmentBlockViews() returned the wrong variable count.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_KEY			, 3, "environmentBlockViews() returned the wrong variable key.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_VALUE			, 4, "environmentBlockViews() returned the wrong variable value.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BLOCK_ALIAS			, 5, "environmentBlockViews() copied or misplaced a variable view.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, BARE_KEY			, 6, "environmentBlockViews() mishandled a variable without a value.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EMPTY_BLOCK			, 7, "environmentBlockViews() modified output for an empty block.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, UNTERMINATED_RESULT	, 8, "environmentBlockViews() accepted an unterminated variable.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, UNTERMINATED_OUTPUT	, 9, "environmentBlockViews() modified output after rejecting an unterminated variable.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EXECUTE_RESULT		, 10, "SProcessManager::Execute() failed to run the test process.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EXECUTE_OUTPUT		, 11, "SProcessManager::Execute() failed to capture the child process' standard error.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EXECUTE_EXIT		, 12, "SProcessManager::Execute() returned the wrong child exit code.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, LOOKUP				, 13, "An empty application name must enable executable lookup from the command line.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, DUPLEX				, 14, "Large stdin and stdout transfers must complete without pipe deadlock or data loss.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, EARLY_EXIT			, 15, "A child closing stdin early must return an input error without hanging.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, ARGUMENTS			, 16, "Argument boundaries and shell metacharacters must be preserved literally.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, ENVIRONMENT			, 17, "A supplied environment block must replace the inherited environment.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, CREATE_FAILURE		, 18, "Failed process creation must leave a closable owner with no child id.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, SIGNAL_POLICY		, 19, "Pipe writes must preserve the caller's SIGPIPE disposition and mask.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, LAUNCH				, 20, "An asynchronous process launch must succeed and arrange child reaping.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, MANAGER				, 21, "Manager ownership, retained completion or process-id lifecycle is incorrect.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, COPYCAST			, 22, "cpycst() must preserve equal-sized object representations and process flag bits.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, SCOPE				, 23, "Scoped process cleanup must run once, preserve errors and allow ownership transfer.");
GDEFINE_ENUM_VALUED(PROCESS_TEST_RESULT, COMMUNICATE			, 24, "Communicate must drain an already-launched child without relaunching or reaping it.");

stct STestProcessOwner {
	::llc::u2_t Calls = 0, Order = 0;
	::llc::err_t Result = 0;
	::llc::err_t Close(::llc::u2_t id) { ++Calls; Order = Order * 10 + id; rtrn Result; }
};

sttc ::llc::err_t testProcessScopeReturn(STestProcessOwner & owner) {
	::llc::SProcessScope<STestProcessOwner> scope{owner, 1};
	rtrn -19;
}

sttc ::llc::err_t testProcessScope(::llc::ATestError & errors) {
	STestProcessOwner owner;
	{
		::llc::SProcessScope<STestProcessOwner> scope{owner, 1};
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, owner.Calls != 1 || owner.Order != 1);
	{
		::llc::SProcessScope<STestProcessOwner> scope{owner, 2};
		scope.Inactive = true;
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, owner.Calls != 1 || owner.Order != 1);
	{
		::llc::SProcessScope<STestProcessOwner> scope{owner, 3};
		LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, scope.Close() || scope.Close() || owner.Calls != 2 || owner.Order != 13);
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, owner.Calls != 2);
	owner.Result = -37;
	{
		::llc::SProcessScope<STestProcessOwner> scope{owner, 4};
		LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, scope.Close() != -37 || scope.Close() || owner.Calls != 3 || owner.Order != 134);
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, owner.Calls != 3);
	cnst ::llc::err_t failure = testProcessScopeReturn(owner);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, failure != -19 || owner.Calls != 4);
	owner = {};
	{
		::llc::SProcessScope<STestProcessOwner> first{owner, 1};
		::llc::SProcessScope<STestProcessOwner> second{owner, 2};
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, owner.Order != 21);
	rtrn 0;
}

#if defined(LLC_WINDOWS) || defined(LLC_LINUX)
sttc ::llc::err_t testProcessCommunicate(::llc::ATestError & errors, ::llc::vcst_t application, ::llc::vcst_t commandLine, ::llc::vcu0_t input, ::llc::vcu0_t expected) {
	::llc::SProcessManager<> manager;
	cnst ::llc::err_t id = manager.Create(application, commandLine);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::failed(id));
	::llc::SProcessManager<>::TProcessScope scope{manager, (::llc::u2_t)id};
	::llc::au0_t output;
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::succeeded(manager.Communicate(id, input, output)) || output.size());
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::failed(manager.Redirect(id)) || ::llc::failed(manager.Launch(id)));
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::failed(manager.Communicate(id, input, output)), "Child:%u, input:%u bytes.", (::llc::u2_t)id, input.size());
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COMMUNICATE, !manager.KnownProcesses[id].State.Started || manager.KnownProcesses[id].State.Finished);
	::llc::u2_t exitCode = 0;
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::failed(manager.Wait(id, exitCode)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COMMUNICATE, exitCode || output.size() != expected.size() || memcmp(output.begin(), expected.begin(), expected.size()));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::succeeded(manager.Communicate(id, input, output)));
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::failed(scope.Close()));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COMMUNICATE, ::llc::succeeded(manager.Communicate(id, input, output)));
	rtrn 0;
}

sttc ::llc::err_t testProcessManager(::llc::ATestError & errors, ::llc::vcst_t application, ::llc::vcst_t commandLine) {
	::llc::SProcessManager<> manager;
	::llc::string ownedCommand = commandLine;
	cnst ::llc::err_t id = manager.Create(application, ownedCommand);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(id));
	ownedCommand.clear();
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, manager.KnownProcesses[id].State.Started || manager.KnownProcesses[id].CommandLine != commandLine);
	::llc::u2_t exitCode = 0;
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::succeeded(manager.Wait(id, exitCode)));
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Execute(id)));
	cnst ::llc::vcst_t marker = "llc process child";
	cnst auto & record = manager.KnownProcesses[id];
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, !record.State.Started || !record.State.Finished || record.ExitCode != EXIT_FAILURE);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, record.Output.size() != marker.size() || memcmp(record.Output.begin(), marker.begin(), marker.size()));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Wait(id, exitCode)) || exitCode != EXIT_FAILURE);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::succeeded(manager.Launch(id)) || ::llc::succeeded(manager.Execute(id)));
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Close(id)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::succeeded(manager.Wait(id, exitCode)) || ::llc::succeeded(manager.Close(id)));
	cnst ::llc::err_t next = manager.Create(application, commandLine);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, next <= id);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Redirect(next)) || ::llc::failed(manager.Launch(next)));
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Write(next, {})));
	::llc::au0_t output;
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Read(next, output)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Wait(next, exitCode)) || exitCode != EXIT_FAILURE);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, output.size() != marker.size() || memcmp(output.begin(), marker.begin(), marker.size()));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::succeeded(manager.Launch((::llc::u2_t)-1)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_MANAGER, ::llc::failed(manager.Close(next)));
	cnst ::llc::u2_t originalCount = manager.KnownProcesses.size();
	for(::llc::u2_t stage = 0; stage < 3; ++stage) {
		{
			llc_necs(manager.PlatformState.push_back({}));
			::llc::SProcessManager<>::SCreateScope rollback{manager, originalCount};
			if(stage) {
				llc_necs(manager.KnownProcesses.emplace_back());
				if(stage > 1) {
					llc_necs(manager.KnownProcesses[originalCount].Application.append(application.begin(), application.size()));
				}
			}
		}
		LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_SCOPE, manager.KnownProcesses.size() != originalCount || manager.PlatformState.size() != originalCount, "Rollback stage:%u.", stage);
	}
	cnst ::llc::err_t scopedId = manager.Create(application, commandLine);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_SCOPE, ::llc::failed(scopedId));
	{
		::llc::SProcessManager<>::TProcessScope scope{manager, (::llc::u2_t)scopedId};
	}
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SCOPE, !manager.KnownProcesses[scopedId].State.Closed);
	rtrn 0;
}
#endif

#ifdef LLC_WINDOWS
sttc DWORD WINAPI processChildTimeout(void * completed) {
	if(WAIT_OBJECT_0 != WaitForSingleObject((HANDLE)completed, 15000))
		ExitProcess(124);
	rtrn 0;
}

::llc::err_t testProcessEchoChild() {
	HANDLE completed = CreateEventA(0, TRUE, FALSE, 0);
	if_null_fe(completed);
	HANDLE watchdog = CreateThread(0, 0, ::processChildTimeout, completed, 0, 0);
	if(0 == watchdog) {
		CloseHandle(completed);
		rtrn -1;
	}
	cnst ::llc::err_t result = []() -> ::llc::err_t {
		::llc::u0_t buffer[4096] = {};
		DWORD transferred = 0;
		for(::llc::u2_t i = 0; i < 16; ++i) {
			if(FALSE == WriteFile(GetStdHandle(STD_ERROR_HANDLE), buffer, sizeof(buffer), &transferred, 0) || transferred != sizeof(buffer))
				rtrn -1;
		}
		while(true) {
			if(FALSE == ReadFile(GetStdHandle(STD_INPUT_HANDLE), buffer, sizeof(buffer), &transferred, 0))
				rtrn ERROR_BROKEN_PIPE == GetLastError() ? 0 : -1;
			if(0 == transferred)
				rtrn 0;
			DWORD written = 0;
			if(FALSE == WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), buffer, transferred, &written, 0) || written != transferred)
				rtrn -1;
		}
	}();
	cnst BOOL signaled = SetEvent(completed);
	cnst DWORD waited = WaitForSingleObject(watchdog, INFINITE);
	cnst BOOL closedThread = CloseHandle(watchdog);
	cnst BOOL closedEvent = CloseHandle(completed);
	rtrn signaled && WAIT_OBJECT_0 == waited && closedThread && closedEvent ? result : -1;
}
#elif defined(LLC_LINUX)
sttc ::llc::err_t processTestWrite(int descriptor, ::llc::vcu0_t bytes) {
	for(::llc::u2_t offset = 0; offset < bytes.size(); ) {
		cnst ssize_t written = write(descriptor, bytes.begin() + offset, bytes.size() - offset);
		if(0 > written && EINTR == errno)
			continue;
		if(0 >= written)
			rtrn -1;
		offset += (::llc::u2_t)written;
	}
	rtrn 0;
}

::llc::err_t testProcessMarkerChild() {
	stxp ::llc::sc_t marker[] = "llc process child";
	if_fail_fe(::processTestWrite(STDERR_FILENO, {(cnst ::llc::u0_t*)marker, sizeof(marker) - 1}));
	rtrn -1;
}

::llc::err_t testProcessEchoChild() {
	// A regression must fail in the child instead of hanging the parent test suite.
	if(SIG_ERR == signal(SIGALRM, SIG_DFL))
		rtrn -1;
	alarm(15);
	::llc::u0_t buffer[4096] = {};
	for(::llc::u2_t i = 0; i < 16; ++i)
		if_fail_fe(::processTestWrite(STDERR_FILENO, {buffer, sizeof(buffer)}));
	for(;;) {
		cnst ssize_t count = read(STDIN_FILENO, buffer, sizeof(buffer));
		if(0 > count && EINTR == errno)
			continue;
		if(0 > count)
			rtrn -1;
		if(0 == count)
			break;
		if_fail_fe(::processTestWrite(STDOUT_FILENO, {buffer, (::llc::u2_t)count}));
	}
	alarm(0);
	rtrn 0;
}

::llc::err_t testProcessSignalChild() {
	if(SIG_ERR == signal(SIGTERM, SIG_DFL))
		rtrn -1;
	raise(SIGTERM);
	rtrn -1;
}
#endif

::llc::err_t testProcess(::llc::ATestError & errors, [[maybe_unused]] ::llc::vcst_t outputFolder) {
	llc_necs(::testProcessScope(errors));
	::llc::SProcessCreationFlags copiedFlags = {};
	cnst ::llc::u2_t originalFlags = 0x08000004U;
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COPYCAST, &::llc::cpycst(copiedFlags, originalFlags) != &copiedFlags);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COPYCAST, !copiedFlags.Suspended || !copiedFlags.NoWindow || copiedFlags.NewConsole);
	::llc::u2_t copiedBits = 0;
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COPYCAST, &::llc::cpycst(copiedBits, copiedFlags) != &copiedBits || copiedBits != originalFlags);
	cnst ::llc::u0_t sourceBytes[] = {0, 127, 128, 255};
	::llc::s0_t destinationBytes[4] = {};
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_COPYCAST, &::llc::cpycst(destinationBytes, sourceBytes) != &destinationBytes || memcmp(destinationBytes, sourceBytes, sizeof(sourceBytes)));
	cnst ::llc::sc_t environment[] = {'A', '=', '1', 0, 'B', '=', 't', 'w', 'o', 0, 0};
	::llc::aobj<::llc::kv<::llc::vcst_t>> variables = {};
	cnst ::llc::err_t result = ::llc::environmentBlockViews(environment, variables);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BLOCK_RESULT, ::llc::failed(result), "Result:%i.", result);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BLOCK_COUNT, variables.size() != 2, "Variable count:%u, expected:2.", variables.size());
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_KEY, variables[0].Key != "A", "First key:%.*s, expected:A.", LLC_VIEW_ARGS(variables[0].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_VALUE, variables[0].Val != "1", "First value:%.*s, expected:1.", LLC_VIEW_ARGS(variables[0].Val));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_KEY, variables[1].Key != "B", "Second key:%.*s, expected:B.", LLC_VIEW_ARGS(variables[1].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_VALUE, variables[1].Val != "two", "Second value:%.*s, expected:two.", LLC_VIEW_ARGS(variables[1].Val));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[0].Key.begin() != &environment[0], "First key begin:%p, expected:%p.", variables[0].Key.begin(), &environment[0]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[0].Val.begin() != &environment[2], "First value begin:%p, expected:%p.", variables[0].Val.begin(), &environment[2]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[1].Key.begin() != &environment[4], "Second key begin:%p, expected:%p.", variables[1].Key.begin(), &environment[4]);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BLOCK_ALIAS, variables[1].Val.begin() != &environment[6], "Second value begin:%p, expected:%p.", variables[1].Val.begin(), &environment[6]);

	cnst ::llc::sc_t bareStorage[] = {'F', 'L', 'A', 'G', 0, 0};
	::llc::aobj<::llc::kv<::llc::vcst_t>> bare = {};
	cnst ::llc::err_t bareResult = ::llc::environmentBlockViews(bareStorage, bare);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BARE_KEY, ::llc::failed(bareResult), "Result:%i.", bareResult);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare.size() != 1, "Variable count:%u, expected:1.", bare.size());
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare[0].Key != "FLAG", "Key:%.*s, expected:FLAG.", LLC_VIEW_ARGS(bare[0].Key));
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_BARE_KEY, bare[0].Val.size(), "Value count:%u, expected:0.", bare[0].Val.size());

	cnst ::llc::u2_t countBeforeEmpty = variables.size();
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EMPTY_BLOCK, ::llc::environmentBlockViews({}, variables) < 0, "%s", "Empty block was rejected.");
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EMPTY_BLOCK, variables.size() != countBeforeEmpty, "Variable count:%u, expected:%u.", variables.size(), countBeforeEmpty);

	cnst ::llc::sc_t unterminated[] = {'X', '=', '9', 0, 'Y', '=', '8'};
	cnst ::llc::u2_t countBeforeInvalid = variables.size();
	cnst ::llc::err_t invalidResult = ::llc::environmentBlockViews(unterminated, variables);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_UNTERMINATED_RESULT, 0 <= invalidResult, "Result:%i.", invalidResult);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_UNTERMINATED_OUTPUT, variables.size() != countBeforeInvalid, "Variable count:%u, expected:%u.", variables.size(), countBeforeInvalid);

#ifdef LLC_WINDOWS
	::llc::sc_t modulePath[MAX_PATH] = {};
	cnst DWORD modulePathLength = GetModuleFileNameA(0, modulePath, ::llc::size(modulePath));
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_EXECUTE_RESULT, 0 == modulePathLength || ::llc::size(modulePath) == modulePathLength, "Executable path length:%u.", modulePathLength);
	::llc::string commandLine = {};
	cnst ::llc::err_t commandResult = ::llc::append_strings(commandLine, '"', ::llc::vcst_t{modulePath, modulePathLength}, "\" --process-child");
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_EXECUTE_RESULT, ::llc::failed(commandResult), "Command result:%i.", commandResult);
	::llc::au0_t processOutput = {};
	::llc::u2_t processExitCode = 0;
	cnst ::llc::err_t processResult = ::llc::SProcessManager{}.Execute
		( ::llc::vcst_t{modulePath, modulePathLength}
		, commandLine
		, {}
		, processOutput
		, processExitCode
		, {}
		);
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_EXECUTE_RESULT, ::llc::failed(processResult), "Result:%i.", processResult);
	stxp ::llc::sc_t expectedOutput[] = "llc process child";
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EXECUTE_OUTPUT, processOutput.size() != ::llc::size(expectedOutput) - 1 || 0 != memcmp(processOutput.begin(), expectedOutput, ::llc::size(expectedOutput) - 1), "Output size:%u, expected:%u.", processOutput.size(), ::llc::size(expectedOutput) - 1);
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EXECUTE_EXIT, EXIT_FAILURE != processExitCode, "Exit code:%u, expected:%u.", processExitCode, EXIT_FAILURE);
	llc_necs(::testProcessManager(errors, {modulePath, modulePathLength}, commandLine));

	processOutput.clear();
	cnst ::llc::err_t lookupResult = ::llc::SProcessManager{}.Execute({}, "cmd.exe /d /c exit 23", {}, processOutput, processExitCode, {});
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_LOOKUP, ::llc::failed(lookupResult) || 23 != processExitCode, "Result:%i, exit:%u.", lookupResult, processExitCode);

	::llc::au0_t processInput;
	llc_necs(processInput.resize(1024 * 1024));
	for(::llc::u2_t i = 0; i < processInput.size(); ++i)
		processInput[i] = (::llc::u0_t)i;
	processOutput.clear();
	cnst ::llc::err_t earlyExitResult = ::llc::SProcessManager{}.Execute({modulePath, modulePathLength}, commandLine, processInput, processOutput, processExitCode, {});
	LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_EARLY_EXIT, 0 <= earlyExitResult, "Result:%i.", earlyExitResult);

	commandLine.clear();
	llc_necs(::llc::append_strings(commandLine, '"', ::llc::vcst_t{modulePath, modulePathLength}, "\" --process-echo"));
	processOutput.clear();
	cnst ::llc::err_t duplexResult = ::llc::SProcessManager{}.Execute({modulePath, modulePathLength}, commandLine, processInput, processOutput, processExitCode, {});
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_DUPLEX, ::llc::failed(duplexResult) || 0 != processExitCode, "Result:%i, exit:%u (124 means timeout).", duplexResult, processExitCode);
	stxp ::llc::u2_t prefixSize = 16 * 4096;
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_DUPLEX, processOutput.size() != prefixSize + processInput.size(), "Output:%u, expected:%u.", processOutput.size(), prefixSize + processInput.size());
	bool prefixChanged = false;
	for(::llc::u2_t i = 0; i < prefixSize; ++i)
		prefixChanged |= 0 != processOutput[i];
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_DUPLEX, prefixChanged);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_DUPLEX, 0 != memcmp(processOutput.begin() + prefixSize, processInput.begin(), processInput.size()));
	llc_necs(::testProcessCommunicate(errors, {modulePath, modulePathLength}, commandLine, processInput, processOutput));
#elif defined(LLC_LINUX)
	cnst ::llc::vcst_t markerArgs = "llc_test_core --process-child";
	cnst ::llc::vcst_t echoArgs = "llc_test_core --process-echo";
	::llc::au0_t output, input;
	::llc::u2_t exitCode = 0;
	cnst ::llc::err_t markerResult = ::llc::SProcessManager{}.Execute("/proc/self/exe", markerArgs, {}, output, exitCode, {});
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_EXECUTE_RESULT, ::llc::failed(markerResult));
	cnst ::llc::vcst_t marker = "llc process child";
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_EXECUTE_OUTPUT, output.size() != marker.size() || memcmp(output.begin(), marker.begin(), marker.size()));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_EXECUTE_EXIT, EXIT_FAILURE != exitCode);
	llc_necs(::testProcessManager(errors, "/proc/self/exe", markerArgs));
	cnst ::llc::vcst_t signalArgs = "llc_test_core --process-signal";
	output.clear();
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_EXECUTE_EXIT, ::llc::failed(::llc::SProcessManager{}.Execute("/proc/self/exe", signalArgs, {}, output, exitCode, {})) || 128U + SIGTERM != exitCode);

	cnst ::llc::vcst_t lookupArgs = "true";
	output.clear();
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_LOOKUP, ::llc::failed(::llc::SProcessManager{}.Execute({}, lookupArgs, {}, output, exitCode, {})) || exitCode);
	cnst ::llc::vcst_t literalArgs = "printf %s|%s|%s \"two words\" \\\"quoted\\\" $(literal)";
	cnst ::llc::vcst_t expected = "two words|\"quoted\"|$(literal)";
	output.clear();
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_ARGUMENTS, ::llc::failed(::llc::SProcessManager{}.Execute({}, literalArgs, {}, output, exitCode, {})) || exitCode);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_ARGUMENTS, output.size() != expected.size() || memcmp(output.begin(), expected.begin(), expected.size()));
	cnst ::llc::kv<::llc::vcst_t> commandCases[] = {
		{R"cmd("printf" <%s> "" "two words" plain)cmd", "<><two words><plain>"},
		{R"cmd(pri"ntf" <%s> a"b c"d "a""b" "unterminated)cmd", "<ab cd><a\"b><unterminated>"},
		{R"cmd(printf <%s> "a\\" b\\ c\"d)cmd", R"cmd(<a\><b\\><c"d>)cmd"},
		{"printf\t<%s>\t\"tab\tinside\"\tend  ", "<tab\tinside><end>"},
		{R"cmd(printf <%s> $HOME $(literal) *.cpp 'single')cmd", "<$HOME><$(literal)><*.cpp><'single'>"},
	};
	for(cnst auto & commandCase : commandCases) {
		output.clear();
		cnst ::llc::err_t commandResult = ::llc::SProcessManager{}.Execute({}, commandCase.Key, {}, output, exitCode, {});
		LLC_TEST_CHECKF(errors, PROCESS_TEST_RESULT_ARGUMENTS, ::llc::failed(commandResult) || exitCode || output.size() != commandCase.Val.size() || memcmp(output.begin(), commandCase.Val.begin(), commandCase.Val.size()), "Command:%.*s; result:%i, exit:%u, bytes:%u, expected:%u.", LLC_VIEW_ARGS(commandCase.Key), commandResult, exitCode, output.size(), commandCase.Val.size());
	}
	::llc::SProcessManager<> manager;
	for(cnst ::llc::vcst_t invalidCommand : {::llc::vcst_t{" \t"}, ::llc::vcst_t{"\"\""}}) {
		cnst ::llc::err_t id = manager.Create("/bin/true", invalidCommand);
		LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::failed(id));
		LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::succeeded(manager.Launch(id)));
		LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::failed(manager.Close(id)));
	}
	cnst ::llc::err_t flagsId = manager.Create("/proc/self/exe", markerArgs, {}, 1);
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::failed(flagsId));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::succeeded(manager.Launch(flagsId)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::failed(manager.Close(flagsId)));

	cnst ::llc::vcst_t envArgs = "env";
	cnst ::llc::sc_t customEnvironment[] = "LLC_PROCESS_TEST=present\0";
	cnst ::llc::vcst_t expectedEnvironment = "LLC_PROCESS_TEST=present\n";
	output.clear();
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_ENVIRONMENT, ::llc::failed(::llc::SProcessManager{}.Execute({}, envArgs, {}, output, exitCode, {customEnvironment, sizeof(customEnvironment)})) || exitCode);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_ENVIRONMENT, output.size() != expectedEnvironment.size() || memcmp(output.begin(), expectedEnvironment.begin(), expectedEnvironment.size()));
	cnst ::llc::sc_t emptyEnvironment[] = {0};
	output.clear();
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_ENVIRONMENT, ::llc::failed(::llc::SProcessManager{}.Execute({}, envArgs, {}, output, exitCode, {emptyEnvironment, 1})) || exitCode || output.size());

	llc_necs(input.resize(1024 * 1024));
	for(::llc::u2_t i = 0; i < input.size(); ++i)
		input[i] = (::llc::u0_t)i;
	output.clear();
	cnst ::llc::err_t duplexResult = ::llc::SProcessManager{}.Execute("/proc/self/exe", echoArgs, input, output, exitCode, {});
	LLC_TEST_REQUIREF(errors, PROCESS_TEST_RESULT_DUPLEX, ::llc::failed(duplexResult) || exitCode, "Result:%i, exit:%u (142 means SIGALRM timeout).", duplexResult, exitCode);
	stxp ::llc::u2_t prefixSize = 16 * 4096;
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_DUPLEX, output.size() != prefixSize + input.size());
	bool prefixChanged = false;
	for(::llc::u2_t i = 0; i < prefixSize; ++i)
		prefixChanged |= 0 != output[i];
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_DUPLEX, prefixChanged || memcmp(output.begin() + prefixSize, input.begin(), input.size()));
	llc_necs(::testProcessCommunicate(errors, "/proc/self/exe", echoArgs, input, output));

	// Exercise SIGPIPE handling on this thread as well as on Execute's worker.
	struct sigaction before = {}, after = {};
	sigset_t maskBefore, maskAfter;
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_SIGNAL_POLICY, sigaction(SIGPIPE, 0, &before) || pthread_sigmask(SIG_SETMASK, 0, &maskBefore));
	cnst ::llc::err_t pipeId = manager.Create({}, "true");
	llc_necs(pipeId);
	llc_necs(manager.Redirect(pipeId));
	llc_necs(manager.Launch(pipeId));
	llc_necs(manager.Wait(pipeId, exitCode)); // Child has closed its read end.
	cnst ::llc::err_t brokenResult = manager.Write(pipeId, input);
	cnst ::llc::err_t closedPipes = manager.Close(pipeId);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_EARLY_EXIT, ::llc::succeeded(brokenResult) || ::llc::failed(closedPipes));
	LLC_TEST_REQUIRE(errors, PROCESS_TEST_RESULT_SIGNAL_POLICY, sigaction(SIGPIPE, 0, &after) || pthread_sigmask(SIG_SETMASK, 0, &maskAfter));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_SIGNAL_POLICY, before.sa_handler != after.sa_handler || sigismember(&maskBefore, SIGPIPE) != sigismember(&maskAfter, SIGPIPE));
	output.clear();
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_EARLY_EXIT, ::llc::succeeded(::llc::SProcessManager{}.Execute("/proc/self/exe", markerArgs, input, output, exitCode, {})));

	cnst ::llc::err_t missingId = manager.Create("/proc/self/llc-no-such-executable", markerArgs);
	llc_necs(missingId);
	llc_necs(manager.Redirect(missingId));
	cnst ::llc::err_t missingResult = manager.Launch(missingId);
	cnst bool hadChild = manager.KnownProcesses[missingId].State.Started;
	cnst ::llc::err_t closedMissing = manager.Close(missingId);
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::succeeded(missingResult) || hadChild || ::llc::failed(closedMissing));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_CREATE_FAILURE, ::llc::succeeded(manager.Launch(missingId)));
	LLC_TEST_CHECK(errors, PROCESS_TEST_RESULT_LAUNCH, ::llc::failed(::llc::SProcessManager{}.Launch({}, lookupArgs, {})));
#endif
	return 0;
}
