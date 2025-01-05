@echo off

:: Output file name
set MSI=DB.Browser.for.SQLite-Extensions-%1

:: Set the ARCH based on the first parameter
if "%1"=="" (
    echo ERROR: You must select a build type, either "win64" or "win32"
    goto :eof
) else if "%1"=="win32" (
    set ARCH=x86
) else if "%1"=="win64" (
    set ARCH=x64    
) else (
    echo ERROR: Unknown build type="%1"
    goto :eof
)

:: Suppress some ICE checks
:: - 61 (major upgrade)
set ICE=-sice:ICE61

:: Compile & Link
"%WIX%\bin\candle.exe" -nologo -pedantic -arch %ARCH% extensions.wxs
"%WIX%\bin\light.exe" -nologo -pedantic %ICE% -ext WixUIExtension extensions.wixobj -out %MSI%.msi

:: Cleanup
del extensions.wixobj
del %MSI%.wixpdb
