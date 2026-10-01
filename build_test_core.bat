@echo off
setlocal EnableExtensions

set "LLC_CONFIGURATION=%~1"
set "LLC_PLATFORM=%~2"
if not defined LLC_CONFIGURATION set "LLC_CONFIGURATION=Debug"
if not defined LLC_PLATFORM set "LLC_PLATFORM=x64"

rem Normalize PATH casing inherited from environments that expose both Path and PATH.
set "LLC_BUILD_PATH=%PATH%"
set "Path="
set "PATH="
set "PATH=%LLC_BUILD_PATH%"
set "LLC_BUILD_PATH="

set "LLC_ROOT=%~dp0"
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

call :build zlibvc
if errorlevel 1 exit /b %errorlevel%
call :build llc
if errorlevel 1 exit /b %errorlevel%
call :build llc_test_core
if errorlevel 1 exit /b %errorlevel%

set "LLC_OUTPUT_PLATFORM=%LLC_PLATFORM%"
if /I "%LLC_OUTPUT_PLATFORM%"=="x86" set "LLC_OUTPUT_PLATFORM=Win32"
set "LLC_TEST_EXE=%LLC_ROOT%..\%LLC_OUTPUT_PLATFORM%.%LLC_CONFIGURATION%\llc_test_core.exe"
if not exist "%LLC_TEST_EXE%" (
	echo Test executable not found: "%LLC_TEST_EXE%"
	exit /b 1
)

"%LLC_TEST_EXE%"
exit /b %errorlevel%

:build
echo Building %~1 [%LLC_CONFIGURATION%^|%LLC_PLATFORM%]...
"%LLC_MSBUILD%" "%LLC_ROOT%llc.sln" /m /nologo /t:%~1 /p:Configuration=%LLC_CONFIGURATION% /p:Platform=%LLC_PLATFORM%
exit /b %errorlevel%
