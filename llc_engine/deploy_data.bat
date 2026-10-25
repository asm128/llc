@echo off
setlocal
if "%~1"=="" (
	echo Usage: deploy_data.bat output-directory 1>&2
	exit /b 1
)
pushd "%~dp0" || exit /b 1
if not exist "%~1\" (
	mkdir "%~1" || goto :failed
)
copy /Y "*.json" "%~1\" >nul
if errorlevel 1 goto :failed
popd
exit /b 0

:failed
popd
exit /b 1
