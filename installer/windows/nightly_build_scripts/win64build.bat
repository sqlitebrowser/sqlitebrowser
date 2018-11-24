:: Destination path - specify where to move package after build
SET DEST_PATH=C:\\builds
MKDIR "%DEST_PATH%"

SET SQLITE_DIR=C:\\dev\\SQLite-Win64
SET SQLCIPHER_DIR=C:\\git_repos\\SQLCipher-Win64
SET SQLCIPHER_TAG=v3.4.2

:: You need to change the date format in Windows settings to YYYY-MM-DD
:: before this will work properly. ;)
set RUN_DATE=%DATE%

:: If no branch given on the command line, use master
IF "%1"=="" (SET BRANCH="master") ELSE (SET BRANCH="%1")

CD /d "C:\"
if exist "%SQLITE_DIR%" rd /q /s "%SQLITE_DIR%"
if exist "C:\\builds\\release-sqlite-win64" rd /q /s "C:\\builds\\release-sqlite-win64"
if exist "C:\\builds\\release-sqlcipher-win64" rd /q /s "C:\\builds\\release-sqlcipher-win64"

:: Unpack SQLite
CD C:\dev
"C:\Program Files\7-Zip\7z.exe" e  sqlite*zip "-o%SQLITE_DIR%"

:: Update repositories
::git clone -b v3.4.2 https://github.com/sqlcipher/sqlcipher.git SQLCipher-Win64
CD C:\\git_repos\\SQLCipher-Win64
git clean -dffx
git checkout master
git pull
git checkout "%SQLCIPHER_TAG%"
git clean -dffx

::git clone -b %BRANCH% https://github.com/sqlitebrowser/sqlitebrowser.git "%DB4S_DIR%Win64"
CD C:\\git_repos\\sqlitebrowser
git clean -dffx
git checkout master
git pull
git checkout %BRANCH%
git clean -dffx


:: WIN64 SQLITE BUILD PROCEDURE

:: Set path variables
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars64.bat"

:: Build SQLite x64
CD %SQLITE_DIR%
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_JSON1 -DSQLITE_API=_declspec(dllexport) -link -dll -out:sqlite3.dll

:: Run CMake for SQLite x64
CD C:\\builds
MKDIR "release-sqlite-win64"
CD "release-sqlite-win64"
cmake -G "Visual Studio 15 2017 Win64" -Wno-dev C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"

:: Build MSI
MKDIR C:\\git_repos\\sqlitebrowser\\Release
MOVE C:\\builds\\release-sqlite-win64\\Release\\*.exe C:\\git_repos\\sqlitebrowser\\Release
CD C:\\git_repos\\sqlitebrowser\\installer\\windows
CALL build.cmd win64

:: Move package to DEST_PATH
MOVE /Y *msi "%DEST_PATH%\\DB Browser for SQLite-%RUN_DATE%-win64.msi"

:: Clean up
DEL /F "C:\git_repos\sqlitebrowser\Release\DB Browser for SQLite.exe"


:: WIN64 SQLCIPHER BUILD PROCEDURE

:: Build SQLCipher x64
CD %SQLCIPHER_DIR%
nmake /f Makefile.msc sqlcipher.dll USE_AMALGAMATION=1 NO_TCL=1 SQLITE3DLL=sqlcipher.dll SQLITE3LIB=sqlcipher.lib SQLITE3EXE=sqlcipher.exe LTLINKOPTS="C:\dev\OpenSSL-Win64\lib\libeay32.lib" OPT_FEATURE_FLAGS="-DSQLITE_TEMP_STORE=2 -DSQLITE_HAS_CODEC=1 -DSQLITE_ENABLE_FTS5=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_JSON1=1 -DSQLCIPHER_CRYPTO_OPENSSL=1 -IC:\dev\OpenSSL-Win64\include"

:: Run CMake for SQLCipher x64
CD C:\\builds
MKDIR "release-sqlcipher-win64"
CD "release-sqlcipher-win64"
cmake -G "Visual Studio 15 2017 Win64" -Wno-dev -Dsqlcipher=1 C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"

:: Build MSI
MOVE C:\\builds\\release-sqlcipher-win64\\Release\\*.exe C:\\git_repos\\sqlitebrowser\\Release
CD C:\\git_repos\\sqlitebrowser\\installer\\windows
CALL build.cmd win64 sqlcipher

:: Move package to DEST_PATH
MOVE /Y *msi "%DEST_PATH%\DB Browser for SQLite-sqlcipher-%RUN_DATE%-win64.msi"

:: Clean up
DEL /F "C:\git_repos\sqlitebrowser\Release\DB Browser for SQLite.exe"


:: Upload the packages to the nightlies server
pscp -q -p -i C:\dev\puttygen_private.ppk "%DEST_PATH%\DB*%RUN_DATE%*win64.msi" nightlies@nightlies.sqlitebrowser.org:/nightlies/win64

:: Copy the new binaries to /latest directory on the nightlies server
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cd /nightlies/latest; rm -f *-win64.msi"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win64/DB*SQLite-%RUN_DATE%-win64.msi /nightlies/latest/DB.Browser.for.SQLite-win64.msi"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win64/DB*sqlcipher-%RUN_DATE%-win64.msi /nightlies/latest/DB.Browser.for.SQLite-sqlcipher-win64.msi"
