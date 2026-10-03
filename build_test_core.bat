@echo off
setlocal EnableExtensions

set "LLC_BUILD_DIRECTORY=%~1"
set "LLC_BUILD_TARGET=%~2"
set "LLC_PLATFORM=%~3"
set "LLC_CONFIGURATION=%~4"
if not defined LLC_PLATFORM set "LLC_PLATFORM=x64"
if not defined LLC_CONFIGURATION set "LLC_CONFIGURATION=Debug"
if not "%~5"=="" goto usage

:arguments_parsed

rem Normalize PATH casing inherited from environments that expose both Path and PATH.
set "LLC_BUILD_PATH=%PATH%"
set "Path="
set "PATH="
set "PATH=%LLC_BUILD_PATH%"
set "LLC_BUILD_PATH="

set "LLC_ROOT=%~dp0"
set "LLC_RUN_TESTS="
if defined LLC_BUILD_DIRECTORY goto select_build_directory
set "LLC_BUILD_ROOT=%LLC_ROOT%"
set "LLC_BUILD_TARGET=llc_test_core"
set "LLC_RUN_TESTS=1"
goto build_setup

:select_build_directory
if not defined LLC_BUILD_TARGET goto usage
call :resolve_build_root "%LLC_BUILD_DIRECTORY%"
if errorlevel 1 exit /b 1

:build_setup
call :find_solution "%LLC_BUILD_ROOT%"
if errorlevel 1 exit /b 1

set "LLC_VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%LLC_VSWHERE%" (
	echo Visual Studio locator not found: "%LLC_VSWHERE%"
	exit /b 1
)

set "LLC_MSBUILD="
for /f "usebackq delims=" %%I in (`"%LLC_VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\amd64\MSBuild.exe`) do set "LLC_MSBUILD=%%I"
if not defined LLC_MSBUILD (
	for /f "usebackq delims=" %%I in (`"%LLC_VSWHERE%" -latest -products * -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do if not defined LLC_MSBUILD set "LLC_MSBUILD=%%I"
)
if not defined LLC_MSBUILD (
	echo MSBuild was not found.
	exit /b 1
)

call :build
if errorlevel 1 exit /b %errorlevel%
if not defined LLC_RUN_TESTS exit /b 0

set "LLC_OUTPUT_PLATFORM=%LLC_PLATFORM%"
if /I "%LLC_OUTPUT_PLATFORM%"=="x86" set "LLC_OUTPUT_PLATFORM=Win32"
set "LLC_TEST_EXE=%LLC_ROOT%..\%LLC_OUTPUT_PLATFORM%.%LLC_CONFIGURATION%\%LLC_BUILD_TARGET%.exe"
if not exist "%LLC_TEST_EXE%" (
	echo Test executable not found: "%LLC_TEST_EXE%"
	exit /b 1
)

"%LLC_TEST_EXE%"
exit /b %errorlevel%

:resolve_build_root
if exist "%~f1\" (
	set "LLC_BUILD_ROOT=%~f1\"
	exit /b 0
)
if exist "%LLC_ROOT%..\%~1\" (
	for %%D in ("%LLC_ROOT%..\%~1") do set "LLC_BUILD_ROOT=%%~fD\"
	exit /b 0
)
echo Build directory not found: "%~1"
exit /b 1

:find_solution
set "LLC_BUILD_SOLUTION="
for %%S in ("%~1*.sln" "%~1*.slnx") do if exist "%%~fS" (
	if defined LLC_BUILD_SOLUTION (
		echo More than one solution found in "%~1".
		exit /b 1
	)
	set "LLC_BUILD_SOLUTION=%%~fS"
)
if not defined LLC_BUILD_SOLUTION (
	echo No solution found in "%~1".
	exit /b 1
)
exit /b 0

:build
echo Building %LLC_BUILD_TARGET% from %LLC_BUILD_SOLUTION% [%LLC_PLATFORM%^|%LLC_CONFIGURATION%]...
"%LLC_MSBUILD%" "%LLC_BUILD_SOLUTION%" /m /nologo /t:%LLC_BUILD_TARGET% /p:Configuration=%LLC_CONFIGURATION% /p:Platform=%LLC_PLATFORM%
exit /b %errorlevel%

:usage
echo Usage: %~nx0 [build-directory target [platform configuration]]
echo Examples:
echo   %~nx0
echo   %~nx0 llt lls
echo   %~nx0 llt lls-t x64 Release
echo   %~nx0 llc llc_test_core x64 Debug
exit /b 2
