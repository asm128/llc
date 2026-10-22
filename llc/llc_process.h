#include "llc_array_obj.h"
#include "llc_keyval.h"
#include "llc_string.h"
#include "llc_n2.h"

//#ifdef LLC_WINDOWS
//#	if !defined(WIN32_LEAN_AND_MEAN)
//#		define WIN32_LEAN_AND_MEAN
//#	endif
//#	include <Windows.h>
//#elif defined(LLC_LINUX)
//#	include <sys/types.h>
//#endif

#ifndef LLC_PROCESS_H_23627
#define LLC_PROCESS_H_23627

namespace llc
{
	typedef size_t					TWindowsHandle;
	typedef sc_t					* ntsc_t;
	stxp	cnst TWindowsHandle		INVALID_WINDOWS_HANDLE	 = (size_t)-1;
	stct SWindowsProcessInfo {
		TWindowsHandle	hProcess				= {};
		TWindowsHandle	hThread					= {};
		u2_t			Id						= {};
		u2_t			Thread					= {};
	};
//#pragma pack(push, 1)
	stct SWindowsStartupInfo {
		u2_t			SizeInBytes				= {};
		ntsc_t			Reserved				= {};
		ntsc_t			Desktop					= {};
		ntsc_t			Title					= {};
		n2u2_t			WindowPosition			= {};
		n2u2_t			WindowSize				= {};
		n2u2_t			TextModeSize			= {};
		u2_t			TextModeFillAttribute	= {};
		u2_t			Flags					= {}; // ?? CreationFlags? ExecutionMode? What the fuck flags means? Why did't they call it "number" instead, or just plain "Bits". ... ... ... Retards.
		u1_t			ShowWindow				= {};
		u1_t			Reserved2ByteCount		= {};
		u0_t			* Reserved2				= {};
		TWindowsHandle	HandleStdInput			= {};
		TWindowsHandle	HandleStdOutput			= {};
		TWindowsHandle	HandleStdError			= {};
	};
//#pragma pack(pop)
#ifdef LLC_WINDOWS
	static_assert(sizeof(SWindowsProcessInfo) == sizeof(PROCESS_INFORMATION));
	static_assert(sizeof(SWindowsStartupInfo) == sizeof(STARTUPINFOA));
#endif
#pragma pack(push, 1)
	stct SProcessWin32 {
		SWindowsProcessInfo	Info		= {};
		SWindowsStartupInfo	Startup		= {sizeof(SWindowsStartupInfo)};
		TWindowsHandle		StdinWrite	= INVALID_WINDOWS_HANDLE;
		TWindowsHandle		StdoutRead	= INVALID_WINDOWS_HANDLE;
	};
#pragma pack(pop)
	stct SProcessLinux {
		s2_t				PID			= -1;
		s2_t				StdinRead	= -1;
		s2_t				StdinWrite	= -1;
		s2_t				StdoutRead	= -1;
		s2_t				StdoutWrite	= -1;
	};

	stct SProcess {
		s2_t			ThreadId;
	};

	tpl_t
	stct SProcessManager {
		tydf			_t				T, TProcess;

		apod<T>			PlatformState	= {};
		apod<SProcess>	KnownProcesses	= {};

		err_t			Create			(vcst_t application, vcst_t commandLine, vcs0_t environment = {}, u2_t creationFlags = 0);
		err_t			Execute			(u2_t processId); // (vcst_t application, vcst_t commandLine, vcu0_t input, au0_t & output, u2_t & exitCode, vcs0_t environment = {});
		err_t			Launch			(u2_t processId); // (vcst_t application, vcst_t commandLine, vcs0_t environment = {});
		err_t			Redirect		(u2_t processId);

		err_t			Write			(u2_t processId, vcu0_t input);
		err_t			Read			(u2_t processId, au0_t & output);
		
		err_t			Wait			(u2_t processId, u2_t & exitCode);// Linux: signal termination is 128 + signal; wait consumes the child id.
		err_t			sClose			(u2_t processId);	// Linux: closes pipes and transfers a running child to a detached waiter, never kills it. If the waiter cannot be created, reaps synchronously and reports failure.
	};
} // namespace

#endif // LLC_PROCESS_H_23627
