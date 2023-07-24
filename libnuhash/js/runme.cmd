@echo off
cd /d "%~dp0"
if "%NODE_HOME%"=="" (
	set "NODE_HOME=C:\Program Files\nodejs"
)
set "PATH=%NODE_HOME%;%PATH%"
if "%~1"=="" (
	call npm install
	call npm test
) else (
	call npm "%~1"
)
