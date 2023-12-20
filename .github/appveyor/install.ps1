if ($env:PLATFORM -eq "x86") {
    $ARCH = "Win32"
} elseif ($env:PLATFORM -eq "x64") {
    $ARCH = "Win64"
} else {
    throw "Unknown platform: $env:PLATFORM"
}

# Download SQLite
mkdir C:\temp\$ARCH
mkdir C:\dev\SQLite-$ARCH
$htmlContent = Invoke-WebRequest -Uri "https://sqlite.org/download.html" | Select-Object -ExpandProperty Content
$regex = [regex]::new('PRODUCT,(\d+\.\d+\.\d+),(\d+/sqlite-amalgamation-\d+\.zip),\d+,(.+)')
$match = $regex.Match($htmlContent)
$relativeUrl = $match.Groups[2].Value
$downloadLink = "https://sqlite.org/$relativeUrl"
Invoke-WebRequest -Uri $downloadLink -OutFile C:\temp\$ARCH\sqlite.zip
Expand-Archive -Path C:\temp\$ARCH\sqlite.zip -DestinationPath C:\temp\$ARCH
Move-Item -Path C:\temp\$ARCH\sqlite-amalgamation-*\* C:\dev\SQLite-$ARCH
cd C:\dev\SQLite-$ARCH
cl sqlite3.c -DSQLITE_ENABLE_FTS5 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_FTS3_PARENTHESIS -DSQLITE_ENABLE_STAT4 -DSQLITE_SOUNDEX -DSQLITE_ENABLE_JSON1 -DSQLITE_ENABLE_GEOPOLY -DSQLITE_ENABLE_RTREE -DSQLITE_ENABLE_MATH_FUNCTIONS -DSQLITE_MAX_ATTACHED=125 -DSQLITE_API="__declspec(dllexport)" -link -dll -out:sqlite3.dll

# Install SQLite Extensions
cd $env:APPVEYOR_BUILD_FOLDER
cp .\src\extensions\extension-formats.c C:\dev\SQLite-$ARCH\
cp .\src\extensions\extension-formats.def C:\dev\SQLite-$ARCH\
cp .\src\extensions\extension-functions.c C:\dev\SQLite-$ARCH\
cp .\src\extensions\extension-functions.def C:\dev\SQLite-$ARCH\
cd C:\dev\SQLite-$ARCH\
cl /MD extension-formats.c -link -dll -def:extension-formats.def -out:formats.dll
cl /MD extension-functions.c -link -dll -def:extension-functions.def -out:math.dll
# FIXME: Disable building the 'fileio' extension for now (#3488)
# If this issue is resolved, be sure to delete the related patch for WiX
# curl -L -o fileio.c "https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk"
# curl -L -o test_windirent.c "https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk"
# curl -L -o test_windirent.h "https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk"
# cl /MD fileio.c test_windirent.c -link sqlite3.lib -dll -out:fileio.dll

# Install OpenSSL
if ($env:PLATFORM -eq "x86") {
    choco install -y openssl --version=1.1.1.4 --forcex86
    New-Item -ItemType SymbolicLink -Path C:\dev\OpenSSL-Win32 -Target "C:\OpenSSL-Win32"
} else {
    choco install -y openssl --version=1.1.1.4
    New-Item -ItemType SymbolicLink -Path C:\dev\OpenSSL-Win64 -Target "C:\OpenSSL-Win64"
}

# Install Magicsplat Tcl/Tk
choco install magicsplat-tcl-Tk
$env:Path += ";C:\Program Files\Tcl\bin"

# Install SQLCipher
cd C:\dev
git clone https://github.com/sqlcipher/sqlcipher
mv sqlcipher SQLCipher-$ARCH
cd SQLCipher-$ARCH
git reset --hard $(git describe --tags --abbrev=0)
nmake /f Makefile.msc sqlcipher.dll USE_AMALGAMATION=1 NO_TCL=1 SQLITE3DLL=sqlcipher.dll SQLITE3LIB=sqlcipher.lib SQLITE3EXE=sqlcipher.exe LTLINKOPTS="C:\dev\OpenSSL-$ARCH\lib\libcrypto.lib" OPT_FEATURE_FLAGS="-DSQLITE_TEMP_STORE=2 -DSQLITE_HAS_CODEC=1 -DSQLITE_ENABLE_FTS3=1 -DSQLITE_ENABLE_FTS5=1 -DSQLITE_ENABLE_FTS3_PARENTHESIS=1 -DSQLITE_ENABLE_STAT4=1 -DSQLITE_SOUNDEX=1 -DSQLITE_ENABLE_JSON1=1 -DSQLITE_ENABLE_GEOPOLY=1 -DSQLITE_ENABLE_RTREE=1 -DSQLCIPHER_CRYPTO_OPENSSL=1 -DSQLITE_MAX_ATTACHED=125 -IC:\dev\OpenSSL-$ARCH\include"
mkdir sqlcipher
copy sqlite3.h sqlcipher
