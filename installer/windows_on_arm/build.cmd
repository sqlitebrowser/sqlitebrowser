@echo off

set "WIX=C:\Program Files (x86)\WiX Toolset v3.14\bin"

:: Output file name
set MSI=DB.Browser.for.SQLite-%1

set ARCH=arm64

:: Suppress some ICE checks
:: - 61 (major upgrade)
:: - 03 & 82 (merge module)
:: - 38 & 43 & 57 (non-advertised shortcuts)
set ICE=-sice:ICE03 -sice:ICE82 -sice:ICE61 -sice:ICE38 -sice:ICE43 -sice:ICE57

:: Suppress 'light.exe' warning
:: - 1104 (vcredist merge module installer version)
set LIGHT=-sw1104

:: Compile & Link
"%WIX%\candle.exe" -nologo -pedantic -arch %ARCH% product.wxs translations.wxs ui.wxs
"%WIX%\light.exe" -sval -nologo -pedantic %LIGHT% %ICE% -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc strings.wxl product.wixobj translations.wixobj ui.wixobj -out %MSI%.msi

:: Cleanup
del product.wixobj
del translations.wixobj
del ui.wixobj
del %MSI%.wixpdb
