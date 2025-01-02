@echo off

:: Output file name
set MSI=DB.Browser.for.SQLite-%1

:: Set the ARCH based on the first parameter
if "%1"=="" (
    echo ERROR: You must select a build type, either "win64" or "win32"
    goto :eof
) else if "%1"=="x86" (
    set ARCH=x86
) else if "%1"=="x64" (
    set ARCH=x64    
) else (
    echo ERROR: Unknown build type="%1"
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
"%WIX%\bin\candle.exe" -nologo -pedantic -arch %ARCH% product.wxs translations.wxs ui.wxs
"%WIX%\bin\light.exe" -sval -nologo -pedantic %LIGHT% %ICE% -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc strings.wxl product.wixobj translations.wixobj ui.wixobj -out %MSI%.msi

:: Cleanup
del product.wixobj
del translations.wixobj
del ui.wixobj
del %MSI%.wixpdb
