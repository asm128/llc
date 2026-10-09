@echo off
setlocal enabledelayedexpansion

for /r %%F in (llc_*.cpp llc_*.h) do (
    set "name=%%~nxF"
    ren "%%F" "gpk_!name:~4!"
)