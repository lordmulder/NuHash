@echo off
cd /d "%~dp0"

if "%MSVC_PATH%"=="" (
	set "MSVC_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
)

if exist "%MSVC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" (
	call "%MSVC_PATH%\VC\Auxiliary\Build\vcvarsall.bat" x86
) else (
	echo vcvarsall.bat not found. Please check your MSVC_PATH variable!
	goto BuildError
)

if "%PANDODC_PATH%"=="" (
	set "PANDODC_PATH=C:\Program Files\Pandoc"
)

if not exist "%PANDODC_PATH%\pandoc.exe" (
	echo pandoc.exe not found. Please check your PANDODC_PATH variable!
	goto BuildError
)

REM ------------------------------------------------------------
REM CLEAN UP
REM ------------------------------------------------------------

if exist "%CD%\bin\" rmdir /S /Q "%CD%\bin"
if exist "%CD%\bin\" (
	echo Failed to clean up intermediate files!
	goto BuildError
)

if exist "%CD%\obj\" rmdir /S /Q "%CD%\obj"
if exist "%CD%\obj\" (
	echo Failed to clean up intermediate files!
	goto BuildError
)

REM ------------------------------------------------------------
REM BUILD EXECUTABLES
REM ------------------------------------------------------------

MSBuild.exe /property:Platform=x86   /property:Configuration=Release /target:rebuild "%CD%\nuhash.sln"
if not "%ERRORLEVEL%"=="0" goto BuildError

MSBuild.exe /property:Platform=x64   /property:Configuration=Release /target:rebuild "%CD%\nuhash.sln"
if not "%ERRORLEVEL%"=="0" goto BuildError

MSBuild.exe /property:Platform=ARM64 /property:Configuration=Release /target:rebuild "%CD%\nuhash.sln"
if not "%ERRORLEVEL%"=="0" goto BuildError

REM ------------------------------------------------------------
REM GENERATE DOCUMENTS
REM ------------------------------------------------------------

if not exist "%CD%\out\" mkdir "%CD%\out"

"%PANDODC_PATH%\pandoc.exe" --standalone --embed-resources --css etc\style\gh-pandoc.min.css --metadata title="NuHash README" -o "%~dp0\out\README.html" "%~dp0\README.md"

REM ------------------------------------------------------------
REM COPY FILES
REM ------------------------------------------------------------

if exist "%CD%\out\nuhash-x86.exe" del /F "%CD%\out\nuhash-x86.exe"
if exist "%CD%\out\nuhash-x64.exe" del /F "%CD%\out\nuhash-x64.exe"
if exist "%CD%\out\nuhash-a64.exe" del /F "%CD%\out\nuhash-a64.exe"

copy /Y /B "%CD%\bin\Win32\Release\nuhash.exe" "%CD%\out\nuhash-x86.exe"
copy /Y /B "%CD%\bin\x64\Release\nuhash.exe"   "%CD%\out\nuhash-x64.exe"
copy /Y /B "%CD%\bin\ARM64\Release\nuhash.exe" "%CD%\out\nuhash-a64.exe"

attrib +R "%CD%\out\nuhash-x86.exe"
attrib +R "%CD%\out\nuhash-x64.exe"
attrib +R "%CD%\out\nuhash-a64.exe"

REM ------------------------------------------------------------
REM COMPLETED
REM ------------------------------------------------------------

echo.
echo Build completed.
echo.
pause
goto:eof

REM ------------------------------------------------------------
REM BUILD ERROR
REM ------------------------------------------------------------

:BuildError
echo.
echo Build has failed !!!
echo.
pause
