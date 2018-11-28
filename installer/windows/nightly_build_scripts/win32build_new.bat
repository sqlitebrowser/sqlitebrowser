:: Destination path - specify where to move package after build
SET DEST_PATH=C:\\builds
MKDIR "%DEST_PATH%"

SET ZIP_EXE="C:\Program Files\7-Zip\7z.exe"
SET SQLITE_DIR=C:\\dev\\SQLite-Win32
SET SQLCIPHER_DIR=C:\\git_repos\\SQLCipher-Win32
SET SQLCIPHER_TAG=v3.4.2

:: You need to change the date format in Windows settings to YYYY-MM-DD
:: before this will work properly. ;)
set RUN_DATE=%DATE%

:: If no branch given on the command line, use master
IF "%1"=="" (SET BRANCH="master") ELSE (SET BRANCH="%1")

CD /d "C:\"
if exist "%SQLITE_DIR%" rd /q /s "%SQLITE_DIR%"
if exist "C:\\builds\\release-sqlite-win32" rd /q /s "C:\\builds\\release-sqlite-win32"
if exist "C:\\builds\\release-sqlcipher-win32" rd /q /s "C:\\builds\\release-sqlcipher-win32"

:: Unpack SQLite
CD C:\dev
%ZIP_EXE% e sqlite*zip "-o%SQLITE_DIR%"

:: Update repositories
::git clone -b v3.4.2 https://github.com/sqlcipher/sqlcipher.git SQLCipher-Win32
CD C:\\git_repos\\SQLCipher-Win32
git clean -dffx
git checkout -f HEAD
git checkout master
git pull
git checkout "%SQLCIPHER_TAG%"
git clean -dffx
git pull
git clean -dffx

::git clone -b %BRANCH% https://github.com/sqlitebrowser/sqlitebrowser.git "%DB4S_DIR%Win32"
CD C:\\git_repos\\sqlitebrowser
git clean -dffx
git checkout -f HEAD
git checkout master
git pull
git checkout %BRANCH%
git clean -dffx
git pull
git clean -dffx


:: WIN32 SQLITE BUILD PROCEDURE

:: Set path variables
CALL "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\vcvars32.bat"

:: Build SQLite x86
CD %SQLITE_DIR%
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_JSON1 -DSQLITE_API=__declspec(dllexport) -link -dll -out:sqlite3.dll

:: Run CMake for SQLite x86
CD C:\\builds
MKDIR "release-sqlite-win32"
CD "release-sqlite-win32"
cmake -G "Visual Studio 12 2013" -Wno-dev C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"


:: WIN32 SQLCIPHER BUILD PROCEDURE

:: Build SQLCipher x86
CD %SQLCIPHER_DIR%
nmake /f Makefile.msc sqlcipher.dll USE_AMALGAMATION=1 NO_TCL=1 SQLITE3DLL=sqlcipher.dll SQLITE3LIB=sqlcipher.lib SQLITE3EXE=sqlcipher.exe LTLINKOPTS="C:\dev\OpenSSL-Win32\lib\libeay32.lib" OPT_FEATURE_FLAGS="-DSQLITE_TEMP_STORE=2 -DSQLITE_HAS_CODEC=1 -DSQLITE_ENABLE_FTS5=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_JSON1=1 -DSQLCIPHER_CRYPTO_OPENSSL=1 -IC:\dev\OpenSSL-Win32\include"

:: Run CMake for SQLCipher x86
CD C:\\builds
MKDIR "release-sqlcipher-win32"
CD "release-sqlcipher-win32"
cmake -G "Visual Studio 12 2013" -Wno-dev -Dsqlcipher=1 C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"

:: Rename SQLCipher
CD "Release"
MOVE "DB Browser for SQLite.exe" "DB Browser for SQLCipher.exe"

:: Build MSI
CD C:\\git_repos\\sqlitebrowser\\installer\\windows
CALL build_new.cmd win32

:: Move package to DEST_PATH
MOVE /Y *.msi "%DEST_PATH%\DB.Browser.for.SQLite-%RUN_DATE%-win32.msi"

:: Create ZIP
CD %DEST_PATH%
msiexec /a "DB.Browser.for.SQLite-%RUN_DATE%-win32.msi" /q TARGETDIR=%CD%\zip
MOVE %CD%\zip\System\* "%CD%\zip\DB Browser for SQLite"
%ZIP_EXE% a "DB.Browser.for.SQLite-%RUN_DATE%-win32.zip" "%CD%\zip\DB Browser for SQLite"
RMDIR /S /Q %CD%\zip


:: Upload the packages to the nightlies server
pscp -q -p -i C:\dev\puttygen_private.ppk "%DEST_PATH%\DB*%RUN_DATE%*win32.*" nightlies@nightlies.sqlitebrowser.org:/nightlies/win32

:: Copy the new binaries to /latest directory on the nightlies server
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cd /nightlies/latest; rm -f *-win32.*"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win32/DB*SQLite-%RUN_DATE%-win32.msi /nightlies/latest/DB.Browser.for.SQLite-win32.msi"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win32/DB*SQLite-%RUN_DATE%-win32.zip /nightlies/latest/DB.Browser.for.SQLite-win32.zip"
