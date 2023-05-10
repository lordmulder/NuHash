@echo off
cd /d "%~dp0"

if "%MSYS2_DIR%"=="" (
	set "MSYS2_DIR=C:\msys64"
)

if not exist "%MSYS2_DIR%\msys2_shell.cmd" (
	echo MSYS2 shell not found. Please check MSYS2_DIR and try again!
	pause
	goto:eof
)

if exist "%~dp0..\..\..\out" (
	rmdir /S /Q "%~dp0..\..\..\out"
)

for %%m in (32,64) do (
	echo ========================================================================
	echo Clang%%m
	echo ========================================================================
	echo.
	call "%MSYS2_DIR%\msys2_shell.cmd" -clang%%m -no-start -defterm -where "%~dp0" -c "./mk-release.sh --no-remove"
	echo.
)

pause
