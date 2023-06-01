:: Destination path - specify where to move package after build
SET DEST_PATH=C:\\builds
MKDIR "%DEST_PATH%"

SET ZIP_EXE="C:\Program Files\7-Zip\7z.exe"
SET SQLITE_DIR=C:\\dev\\SQLite-Win32
SET SQLCIPHER_ROOT_DIR=C:\\git_repos\\SQLCipher-Win32
SET SQLCIPHER_TAG=v4.5.4

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


:: Get the current commit hash
FOR /F %%A IN ('git rev-parse --verify HEAD') DO SET CURRENT_COMMIT=%%A

:: Get the last build commit hash from the server
curl -f -L -o commit.txt "https://nightlies.sqlitebrowser.org/win32/commit.txt"

:: Save the hash to a variable for comparison
IF EXIST "commit.txt" SET /P LAST_COMMIT=<commit.txt

:: Do not continue if there are no changes
IF "%CURRENT_COMMIT%"=="%LAST_COMMIT%" EXIT


:: WIN32 SQLITE BUILD PROCEDURE

:: Set path variables
CALL "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvars32.bat"

:: Build SQLite x86
CD %SQLITE_DIR%
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_STAT4 -DSQLITE_SOUNDEX -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_GEOPOLY -DSQLITE_ENABLE_RTREE -DSQLITE_ENABLE_MATH_FUNCTIONS -DSQLITE_MAX_ATTACHED=125 -DSQLITE_API=__declspec(dllexport) -link -dll -out:sqlite3.dll

:: Build extensions
COPY C:\git_repos\sqlitebrowser\src\extensions\extension-functions.c
COPY C:\git_repos\sqlitebrowser\src\extensions\extension-functions.def
cl /MD extension-functions.c -link -dll -def:extension-functions.def -out:math.dll
COPY C:\git_repos\sqlitebrowser\src\extensions\extension-formats.c
COPY C:\git_repos\sqlitebrowser\src\extensions\extension-formats.def
cl /MD extension-formats.c -link -dll -def:extension-formats.def -out:formats.dll
curl -L -o fileio.c "https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk"
curl -L -o test_windirent.c "https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk"
curl -L -o test_windirent.h "https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk"
cl /MD fileio.c test_windirent.c -link sqlite3.lib -dll -out:fileio.dll

:: Run CMake for SQLite x86
CD C:\\builds
MKDIR "release-sqlite-win32"
CD "release-sqlite-win32"
cmake -G "Visual Studio 15 2017" -Wno-dev C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"


:: WIN32 SQLCIPHER BUILD PROCEDURE

:: Build SQLCipher x86
CD %SQLCIPHER_ROOT_DIR%
nmake /f Makefile.msc sqlcipher.dll USE_AMALGAMATION=1 NO_TCL=1 SQLITE3DLL=sqlcipher.dll SQLITE3LIB=sqlcipher.lib SQLITE3EXE=sqlcipher.exe LTLINKOPTS="C:\dev\OpenSSL-Win32\lib\libcrypto.lib" OPT_FEATURE_FLAGS="-DSQLITE_TEMP_STORE=2 -DSQLITE_HAS_CODEC=1 -DSQLITE_ENABLE_FTS3=1 -DSQLITE_ENABLE_FTS5=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_STAT4=1 -DSQLITE_SOUNDEX=1 -DSQLITE_ENABLE_JSON1=1 -DSQLITE_ENABLE_GEOPOLY=1 -DSQLITE_ENABLE_RTREE=1 -DSQLCIPHER_CRYPTO_OPENSSL=1 -DSQLITE_MAX_ATTACHED=125 -IC:\dev\OpenSSL-Win32\include"
MKDIR "sqlcipher"
COPY "sqlite3.h" "sqlcipher"

:: Run CMake for SQLCipher x86
CD C:\\builds
MKDIR "release-sqlcipher-win32"
CD "release-sqlcipher-win32"
cmake -G "Visual Studio 15 2017" -Wno-dev -Dsqlcipher=1 C:\\git_repos\\sqlitebrowser

:: Build package
devenv /Build Release sqlitebrowser.sln /project "ALL_BUILD"

:: Rename SQLCipher
CD "Release"
MOVE "DB Browser for SQLite.exe" "DB Browser for SQLCipher.exe"

:: Build MSI
CD C:\\git_repos\\sqlitebrowser\\installer\\windows
CALL build.cmd win32

:: Move package to DEST_PATH
MOVE /Y *.msi "%DEST_PATH%\DB.Browser.for.SQLite-%RUN_DATE%-win32.msi"

:: Create ZIP
CD %DEST_PATH%
msiexec /a "DB.Browser.for.SQLite-%RUN_DATE%-win32.msi" /q TARGETDIR=%CD%\zip
MOVE %CD%\zip\System\* "%CD%\zip\DB Browser for SQLite"
%ZIP_EXE% a "DB.Browser.for.SQLite-%RUN_DATE%-win32.zip" "%CD%\zip\DB Browser for SQLite"
RMDIR /S /Q %CD%\zip


:: Save the last commit hash to 'commit.txt' and upload it to the nightlies server
CD C:\\git_repos\\sqlitebrowser
git rev-parse --verify HEAD 1>C:\\builds\\commit.txt
pscp -q -p -load nightlies "%DEST_PATH%\commit.txt" nightlies@nightlies.sqlitebrowser.org:/nightlies/win32

:: Upload the packages to the nightlies server
pscp -q -p -load nightlies "%DEST_PATH%\DB*%RUN_DATE%*win32.*" nightlies@nightlies.sqlitebrowser.org:/nightlies/win32

:: Copy the new binaries to /latest directory on the nightlies server
plink -load nightlies -batch "cd /nightlies/latest; rm -f *-win32.*"
plink -load nightlies -batch "cp /nightlies/win32/DB*SQLite-%RUN_DATE%-win32.msi /nightlies/latest/DB.Browser.for.SQLite-win32.msi"
plink -load nightlies -batch "cp /nightlies/win32/DB*SQLite-%RUN_DATE%-win32.zip /nightlies/latest/DB.Browser.for.SQLite-win32.zip"
