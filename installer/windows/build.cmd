@echo off

:: Check the ARCH in the first parameter
if "%1"=="" (
    echo ERROR: You must select a build type, either "x86" or "x64"
    exit /b 1
)
if not "%1"=="x86" if not "%1"=="x64" (
    echo ERROR: Unknown build type="%1"
    exit /b 1
)

call ..\build_msi.cmd %1
exit /b %errorlevel%
