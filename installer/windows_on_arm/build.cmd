@echo off
:: Builds the ARM64 MSI from the files of this directory. Run it in this directory.
:: The steps are shared with :: installer\windows in ..\build_msi.cmd.

if "%1"=="" (
    echo ERROR: You must select a build type, which is "arm64"
    exit /b 1
)
if not "%1"=="arm64" (
    echo ERROR: Unknown build type="%1"
    exit /b 1
)

call ..\build_msi.cmd %1
exit /b %errorlevel%
