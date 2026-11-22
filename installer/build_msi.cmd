@echo off
setlocal

set ARCH=%1

set MSI=DB.Browser.for.SQLite-%ARCH%

set "WIX_BIN="
if defined WIX if exist "%WIX%\bin\candle.exe" set "WIX_BIN=%WIX%\bin"
if not defined WIX_BIN if exist "%ProgramFiles(x86)%\WiX Toolset v3.14\bin\candle.exe" set "WIX_BIN=%ProgramFiles(x86)%\WiX Toolset v3.14\bin"
if not defined WIX_BIN (
    echo ERROR: Could not find the WiX Toolset v3. Install WiX Toolset v3.14, or set WIX to its installation directory.
    exit /b 1
)

python ..\..\tools\collect_qt_translations.py --qt-translations "%QT_ROOT_DIR%\translations" --wxs translations.wxs
if errorlevel 1 exit /b 1

"%WIX_BIN%\candle.exe" -nologo -pedantic -arch %ARCH% product.wxs translations.wxs ui.wxs
if errorlevel 1 exit /b 1
"%WIX_BIN%\light.exe" -sval -nologo -pedantic -ext WixUIExtension -ext WixUtilExtension -cultures:en-us -loc strings.wxl product.wixobj translations.wixobj ui.wixobj -out %MSI%.msi
if errorlevel 1 exit /b 1

del product.wixobj
del translations.wixobj
del ui.wixobj
del %MSI%.wixpdb
