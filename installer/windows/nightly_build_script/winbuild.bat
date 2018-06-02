:: Destination path - specify where to move package after build
set DEST_PATH=C:\\builds
MKDIR "%DEST_PATH%"

set SQLITE_DIR=C:\\dev\\SQLite-
set SQLCIPHER_DIR=C:\\git_repos\\SQLCipher-

:: You need to change the date format in Windows settings to YYYY-MM-DD
:: before this will work properly. ;)
set RUN_DATE=%DATE%

:: If no branch given on the command line, use master
IF "%1"=="" (SET BRANCH="master") ELSE (SET BRANCH="%1")

cd /d "C:\"
if exist "git_repos" rd /q /s "git_repos"
if exist "%SQLITE_DIR%Win32" rd /q /s "%SQLITE_DIR%Win32"
if exist "%SQLITE_DIR%Win64" rd /q /s "%SQLITE_DIR%Win64"
if exist "%SQLCIPHER_DIR%Win32" rd /q /s "%SQLCIPHER_DIR%Win32"
if exist "%SQLCIPHER_DIR%Win64" rd /q /s "%SQLCIPHER_DIR%Win64"

:: Unpack SQLite
cd C:\dev
"C:\Program Files\7-Zip\7z.exe" e  sqlite*zip "-o%SQLITE_DIR%Win32"
"C:\Program Files\7-Zip\7z.exe" e  sqlite*zip "-o%SQLITE_DIR%Win64"

:: Clone repositories
MKDIR "C:\\git_repos"
cd C:\\git_repos
git clone -b msvc2013_win32 --depth 1 https://github.com/justinclift/sqlcipher.git SQLCipher-Win32
git clone -b msvc2013_win64 --depth 1 https://github.com/justinclift/sqlcipher.git SQLCipher-Win64
git clone -b %BRANCH% https://github.com/sqlitebrowser/sqlitebrowser.git

:: WIN32 SQLITE BUILD PROCEDURE

:: Set path variables
call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\vcvars32.bat"

:: Build SQLite x86
cd %SQLITE_DIR%Win32
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_JSON1 -DSQLITE_API=_declspec(dllexport) -link -dll -out:sqlite3.dll

:: Run CMake for SQLite x86
cd C:\\git_repos\\sqlitebrowser
MKDIR "release-sqlite-win32"
cd "release-sqlite-win32"
cmake -G "Visual Studio 12 2013" -Wno-dev ..

:: Build package
FOR %%I IN (*.sln) DO devenv /Build Release "%%I" /project "PACKAGE"

:: Move package to DEST_PATH
MOVE /Y *exe "%DEST_PATH%\DB Browser for SQLite-%RUN_DATE%-win32.exe"

:: WIN32 SQLCIPHER BUILD PROCEDURE

:: Build SQLCipher x86
cd %SQLCIPHER_DIR%Win32
nmake /f Makefile.msc

:: Run CMake for SQLCipher x86
cd C:\\git_repos\\sqlitebrowser
MKDIR "release-sqlcipher-win32"
cd "release-sqlcipher-win32"
cmake -G "Visual Studio 12 2013" -Wno-dev -Dsqlcipher=1 ..

:: Build package
FOR %%I IN (*.sln) DO devenv /Build Release "%%I" /project "PACKAGE"

:: Move package to DEST_PATH
MOVE /Y *exe "%DEST_PATH%\DB Browser for SQLite-sqlcipher-%RUN_DATE%-win32.exe"

:: WIN64 SQLITE BUILD PROCEDURE

:: Set path variables
call "C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\bin\\amd64\\vcvars64.bat"

:: Build SQLite x64
cd %SQLITE_DIR%Win64
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_JSON1 -DSQLITE_API=_declspec(dllexport) -link -dll -out:sqlite3.dll

:: Run CMake for SQLite x64
cd C:\\git_repos\\sqlitebrowser
MKDIR "release-sqlite-win64"
cd "release-sqlite-win64"
cmake -G "Visual Studio 12 2013 Win64" -Wno-dev ..

:: Build package
FOR %%I IN (*.sln) DO devenv /Build Release "%%I" /project "PACKAGE"

:: Move package to DEST_PATH
MOVE /Y *exe "%DEST_PATH%\DB Browser for SQLite-%RUN_DATE%-win64.exe"

:: WIN64 SQLCIPHER BUILD PROCEDURE

:: Build SQLCipher x64
cd %SQLCIPHER_DIR%Win64
nmake /f Makefile.msc

:: Run CMake for SQLCipher x64
cd C:\\git_repos\\sqlitebrowser
MKDIR "release-sqlcipher-win64"
cd "release-sqlcipher-win64"
cmake -G "Visual Studio 12 2013 Win64" -Wno-dev -Dsqlcipher=1 ..

:: Build package
FOR %%I IN (*.sln) DO devenv /Build Release "%%I" /project "PACKAGE"

:: Move package to DEST_PATH
MOVE /Y *exe "%DEST_PATH%\DB Browser for SQLite-sqlcipher-%RUN_DATE%-win64.exe"

:: Upload the packages to the nightlies server
pscp -q -p -i C:\dev\puttygen_private.ppk "%DEST_PATH%\DB*%RUN_DATE%*win32.exe" nightlies@nightlies.sqlitebrowser.org:/nightlies/win32
pscp -q -p -i C:\dev\puttygen_private.ppk "%DEST_PATH%\DB*%RUN_DATE%*win64.exe" nightlies@nightlies.sqlitebrowser.org:/nightlies/win64

:: Copy the new binaries to /latest directory on the nightlies server
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cd /nightlies/latest; rm -f *.exe"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win32/DB*SQLite-%RUN_DATE%-win32.exe /nightlies/latest/DB.Browser.for.SQLite-win32.exe"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win32/DB*sqlcipher-%RUN_DATE%-win32.exe /nightlies/latest/DB.Browser.for.SQLite-sqlcipher-win32.exe"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win64/DB*SQLite-%RUN_DATE%-win64.exe /nightlies/latest/DB.Browser.for.SQLite-win64.exe"
plink -i C:\dev\puttygen_private.ppk nightlies@nightlies.sqlitebrowser.org "cp /nightlies/win64/DB*sqlcipher-%RUN_DATE%-win64.exe /nightlies/latest/DB.Browser.for.SQLite-sqlcipher-win64.exe"

:: Wipe working dir
cd /d C:\
rd /q /s "C:\\git_repos"
