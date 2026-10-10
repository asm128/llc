@echo off
setlocal enabledelayedexpansion

for /r %%F in (gpk_*.cpp gpk_*.h) do (
    set "name=%%~nxF"
    ren "%%F" "llc_!name:~4!"
)