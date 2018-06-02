@echo off

:: Set the output MSI and ARCH based on the calling parameter
if "%1"=="" (
    echo ERROR: You must select "Win64" or "Win32"
    goto :eof
) else if "%1"=="Win32" (
    set MSI=DB.Browser.for.SQLite-win32
    set ARCH=x86
) else if "%1"=="Win64" (
    set MSI=DB.Browser.for.SQLite-win64
    set ARCH=x64    
) else (
    echo ERROR: Unknown ARCH="%1"
    goto :eof
)

:: Suppress some ICE checks
:: - 61 (major upgrade)
:: - 03 & 82 (merge module)
:: - 43 & 57 (non-advertised shortcuts)
set ICE=-sice:ICE03 -sice:ICE82 -sice:ICE61 -sice:ICE43 -sice:ICE57

:: Suppress 'light.exe' warning
:: - 1104 (vcredist merge module installer version)
set LIGHT=-sw1104

:: Compile & Link
"%WIX%\bin\candle.exe" -nologo -pedantic -arch %ARCH% product.wxs
"%WIX%\bin\light.exe" -nologo -pedantic %LIGHT% %ICE% -ext WixUIExtension -ext WixUtilExtension product.wixobj -out %MSI%.msi

:: Cleanup
del product.wixobj
del %MSI%.wixpdb
