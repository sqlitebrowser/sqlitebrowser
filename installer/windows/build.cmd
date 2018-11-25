@echo off

:: Output file name
set MSI=DB.Browser.for.SQLite

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

:: Choose between building the application with SQLite (default) or SQLCipher library
if "%2"=="" (
    set SQLCIPHER=
    set MSI=%MSI%-%1
) else if "%2"=="sqlcipher" (
    set MSI=%MSI%-sqlcipher-%1
    set SQLCIPHER=-dSQLCipher=1
) else (
    echo ERROR: Second parameter can only be "sqlcipher" or nothing
    goto :eof
)

:: Suppress some ICE checks
:: - 61 (major upgrade)
:: - 03 & 82 (merge module)
:: - 38 & 43 & 57 (non-advertised shortcuts)
set ICE=-sice:ICE03 -sice:ICE82 -sice:ICE61 -sice:ICE38 -sice:ICE43 -sice:ICE57

:: Suppress 'light.exe' warning
:: - 1104 (vcredist merge module installer version)
set LIGHT=-sw1104

:: Compile & Link
"%WIX%\bin\candle.exe" -nologo -pedantic -arch %ARCH% %SQLCIPHER% product.wxs translations.wxs
"%WIX%\bin\light.exe" -sval -nologo -pedantic %LIGHT% %ICE% -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc strings.wxl product.wixobj translations.wixobj -out %MSI%.msi

:: Cleanup
del product.wixobj
del translations.wixobj
del %MSI%.wixpdb
