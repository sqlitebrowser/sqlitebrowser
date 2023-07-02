## BUILD INSTRUCTIONS AND REQUIREMENTS

DB Browser for SQLite requires Qt as well as SQLite. For more information on Qt
please consult http://www.qt.io and for SQLite please see https://sqlite.org/.

Please note that all versions after 3.12.1 will require:
* A C++ compiler with support for C++14 or later

All versions after 3.9.1 will require:
* Qt 5.5 or later, however we advise you to use 5.7 or later
* A C++ compiler with support for C++11 or later

Without these or with older versions you won't be able to compile DB Browser for
Sqlite anymore. This applies to all platforms. However, most likely you won't
have to worry about these as most systems meet these requirements today.

If you have any chance, please use Qt 5.7 or any later version. Even though Qt
5.5 and 5.6 are supported by us, there might be glitches and minor problems
when using them.

----
## Table of Contents
- [Generic Linux and FreeBSD](#generic-linux-and-freebsd)
- [Ubuntu / Debian Linux](#ubuntu--debian-linux)
- [CentOS / Fedora Linux](#centos--fedora-linux)
- [OpenSUSE](#opensuse)
- [macOS](#macos)
    - [Building a single executable binary](#building-a-single-executable-binary)
    - [Building an .app bundle](#building-an-app-bundle)
- [Compiling on Windows with MSVC](#compiling-on-windows-with-msvc)
    - [Cross compiling for Windows](#cross-compiling-for-windows)
- [Build with SQLCipher support](#build-with-sqlcipher-support)
- [Building and running the Unit Tests](#building-and-running-the-unit-tests)
    - [Build the unit tests](#build-the-unit-tests)
    - [Run the unit tests](#run-the-unit-tests)
----

### Generic Linux and FreeBSD

The only requirements for building this code are the presence of Qt5 and
sqlite3. Qt can be included as a static or shared library, depending on the
current Qt configuration on the building machine.

Provided you have Qt and cmake installed and configured, simply run:

    $ cmake .

There is one potential problem... several Linux distributions provide a
QScintilla package compiled for (only) Qt4.  If it's present it can confuse
CMake, which will use it during compiling.  The resulting program just crashes
instead of running.  If you experience that kind of crash, try using this
cmake command instead when compiling:

    $ cmake -DFORCE_INTERNAL_QSCINTILLA=ON

That tells cmake to compile QScintilla itself, using the source code version
we bundle.

After the cmake line, run this:

    $ make

in the main directory. This will generate the sqlitebrowser (or
`sqlitebrowser.exe`, or `sqlitebrowser.app`) application in the src subdirectory.
On some distributions you can then install this in the correct places by
running:

    $ sudo make install

The same process works for building the code in any platform supported by Qt
(including other Unix systems with X11.)


### Ubuntu / Debian Linux

```bash
$ sudo apt install build-essential git cmake libsqlite3-dev qtchooser qt5-qmake qtbase5-dev-tools\
    qttools5-dev-tools libsqlcipher-dev qtbase5-dev libqt5scintilla2-dev libqcustomplot-dev\
    qttools5-dev
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
$ cd sqlitebrowser
$ mkdir build
$ cd build
$ cmake -Dsqlcipher=1 -Wno-dev ..
$ make
$ sudo make install
```

**Note** - Use `cmake -DFORCE_INTERNAL_QSCINTILLA=ON -Dsqlcipher=1 -Wno-dev ..` if you're using Debian and meet errors during compiling.

This should complete without errors, giving you a binary file called 'sqlitebrowser'.

Done. :)

### CentOS / Fedora Linux

**Note** - On CentOS or an older version of Fedora, you may need to use `yum` instead of `dnf`.

**Note 2** - On CentOS 7.x, you need to replace the `qwt-qt5-devel` package name with
`qt5-qtbase-devel` in the `dnf install` line below.


**Note 3** - On CentOS 8 (Stream), you need to replace the `qt-devel` package name with
`qt5-devel` in the `dnf install` line below. Make sure the `PowerTools` repo is enabled.

```
$ sudo dnf install cmake gcc-c++ git qt-devel qt5-linguist qwt-qt5-devel \
       sqlite-devel
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
$ cd sqlitebrowser
$ cmake -Wno-dev .
$ make
$ sudo make install
```

This should complete without errors, and `sqlitebrowser` should now be launch-able from the command line.

### OpenSUSE

```bash


$ zypper in -y build git-core, libQt5Core5,  libQt5Core5-32bit,  libqt5-qtbase,  libqt5-qtbase-devel,  libqt5-qttools,  libqt5-qttools-devel,  build,  gcc-c++,  gcc,  sqlite3-devel,  libsqlite3-0,  cmake,  sqlcipher-devel
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
$ cd sqlitebrowser
$ mkdir build
$ cd build
$ cmake -Dsqlcipher=1 -Wno-dev   -DFORCE_INTERNAL_QSCINTILLA=ON ..
$ make
$ sudo make install
```


### macOS

The application can be compiled to an .app bundle, suitable for placing in
/Applications.

### Building an .app bundle

Building an .app bundle version takes a bit more effort, but isn't too hard.
It requires SQLite and Qt 5.x to be installed first.  These are the
[Homebrew](http://brew.sh) steps, though other package managers should work:

    $ brew tap sqlitebrowser/sqlite3
    $ brew install sqlitefts5
    $ brew install qt@5
    $ brew install cmake
    $ brew link sqlitefts5 --force

Then it's just a matter of getting the source:

    $ git clone https://github.com/sqlitebrowser/sqlitebrowser.git

**Note** - Don't clone the repo to a directory with a quote character (') in
its name (eg ~/tmp/foo'), as compiling will error out.

And compiling it:

    $ cd sqlitebrowser
    $ mkdir build       # You can easily make a fresh build again by removing this directory
    $ cd build
    $ cmake -DCMAKE_PREFIX_PATH=/usr/local/opt/qt@5 ..
    $ cmake --build .
    $ brew unlink sqlitefts5
    $ mv sqlitebrowser.app /Applications/DB\ Browser\ for\ SQLite.app

An icon for "DB Browser for SQLite" should now be in your main OSX Applications
list, ready to launch.

### Building a universal binary
Building a universal binary is only possible on ARM64 architecture Macs.

#### Install dependencies
```bash
$ brew tap sqlitebrowser/ub
$ brew install db4sqtub@5 db4ssqlcipherub db4ssqliteftsub@5
```
> If you don't need SQLCipher support, you don't need to install `db4ssqlcipherub`.<br>
> No bottles are provided for Homebrew Formulae, so you have to build them from source, which can take a while.<br>
> Took 1 hour based on testing on a M2 Mac Mini.

#### Get the source code
```bash
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
or
$ git clone git@github.com/sqlitebrowser/sqlitebrowser
```

#### Change prefix for CMakelists.txt
```bash
$ sed -i '' 's|/opt/homebrew/opt/qt5|/opt/homebrew/opt/db4sqtub@5|g' CMakeLists.txt
$ sed -i '' 's|/opt/homebrew/opt/sqlitefts5|/opt/homebrew/opt/db4ssqliteftsub@5|g' CMakeLists.txt
```

#### Configure build and build
```bash
$ mkdir build && cd build
# For SQLite
$ cmake -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" ..
# For SQLCipher
$ cmake -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" -Dsqlcipher=1 ..
$ cmake --build .
```

#### Prepare for add the extension to the app bundle (Optional)
```bash
$ /opt/homebrew/opt/db4sqtub@5/bin/macdeployqt DB\ Browser\ for\ SQLite.app
```

#### Add the extension to the app bundle (Optional)
```bash
$ mkdir build/DB\ Browser\ for\ SQLite.app/Contents/Extensions
$ clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o formats-arm64.dylib
$ arch -x86_64 clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/extension-formats.c -o formats-x86_64.dylib
$ lipo -create -output build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib formats-arm64.dylib formats-x86_64.dylib
$ install_name_tool -id "@executable_path/../Extensions/formats.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib
$ ln -s formats.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/formats.dylib.dylib

$ clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o math-arm64.dylib
$ arch -x86_64 clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/extension-functions.c -o math-x86_64.dylib
$ lipo -create -output build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib math-arm64.dylib math-x86_64.dylib
$ install_name_tool -id "@executable_path/../Extensions/math.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib
$ ln -s math.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/math.dylib.dylib

$ curl -L -o src/extensions/fileio.c 'https://sqlite.org/src/raw?filename=ext/misc/fileio.c&ci=trunk'
$ curl -L -o src/extensions/test_windirent.c 'https://sqlite.org/src/raw?filename=src/test_windirent.c&ci=trunk'
$ curl -L -o src/extensions/test_windirent.h 'https://sqlite.org/src/raw?filename=src/test_windirent.h&ci=trunk'
$ clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirent.c -o fileio-arm64.dylib
$ arch -x86_64 clang -I /opt/homebrew/opt/db4ssqliteftsub@5/include -L /opt/homebrew/opt/db4ssqliteftsub@5/lib -fno-common -dynamiclib src/extensions/fileio.c src/extensions/test_windirent.c -o fileio-x86_64.dylib
$ lipo -create -output build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib fileio-arm64.dylib fileio-x86_64.dylib
$ install_name_tool -id "@executable_path/../Extensions/fileio.dylib" build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib
$ ln -s fileio.dylib build/DB\ Browser\ for\ SQLite.app/Contents/Extensions/fileio.dylib.dylib
```

#### Copy the license file and translations (Optional)
```bash
$ cp LICENSE LICENSE-PLUGINS build/DB\ Browser\ for\ SQLite.app/Contents/Resources/
$ mkdir -p build/DB\ Browser\ for\ SQLite.app/Contents/translations
$ for i in ar zh_CN zh_TW cs en fr de it ko pl pt pt_BR ru es uk; do
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qt_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qtbase_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qtmultimedia${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qtquick1_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qtscript_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
    cp -v /opt/homebrew/opt/db4sqtub@5/translations/qtxmlpatterns_${i}.qm build/DB\ Browser\ for\ SQLite.app/Contents/translations/
  done
```

#### Add the icon file (Optional)
```bash
$ cp installer/macos/macapp.icns build/DB\ Browser\ for\ SQLite.app/Contents/Resources/
$ /usr/libexec/PlistBuddy -c "Set :CFBundleIconFile macapp.icns" build/DB\ Browser\ for\ SQLite.app/Contents/Info.plist
```

### Compiling on Windows with MSVC

Complete setup, build, and packaging instructions with MSVC 2013 x64 are online
here:

&nbsp; &nbsp; https://github.com/sqlitebrowser/sqlitebrowser/wiki/Setting-up-a-Win64-development-environment-for-DB4S

### Cross compiling for Windows

These are instructions to cross compile within a Linux system a Windows binary
and installer.

Requirements:

* mxe cross compile environment → http://mxe.cc
* cmake
* sqlitebrowser sources

Get the following mxe packages:

    $ make gcc sqlite qt nsis

After successful compilation go into your mxedir/usr/bin and add 3 symlinks:

    $ ln -s i686-pc-mingw32-windres windres
    $ ln -s i686-pc-mingw32-makensis makensis
    $ ln -s /usr/bin/lrelease

Now cd into your sqlitebrowser source directory and create a build directory
for the windows binary and create the correct makefiles:

    $ mkdir build-win
    $ cd build-win
    $ cmake -DCMAKE_TOOLCHAIN_FILE=/path to mxe/usr/i686-pc-mingw32/share/cmake/mxe-conf.cmake ..

Before compiling we have to add the mxe/usr/bin directory to the PATH (so
windres and makensis can be found):

    $ export PATH=/path to mxe/usr/bin:$PATH

Now compile:

    $ make

If you additionally want an NSIS installer:

    $ make package

done.

## Build with SQLCipher support

When built with SQLCipher support, DB Browser for SQLite will allow you to open
and edit databases encrypted using SQLCipher as well as standard SQLite3
databases.

Before compiling make sure you have the necessary SQLCipher development files
installed. On Linux this can usually be accomplished by just installing the
correct package (e.g. 'libsqlcipher-dev' on Debian-based distributions). On
macOS the easiest way is to install it via Homebrew ('brew install
sqlcipher'). On Windows unfortunately it's a bit more difficult: You'll have
to download and compile the code as described on the
[SQLCipher website](https://www.zetetic.net/sqlcipher/) before you can proceed.

If SQLCipher is installed, simply follow the standard instructions for your
platform but enable the 'sqlcipher' build option by replacing any calls to
cmake like this:
```
If it says...			Change it to...
cmake				cmake -Dsqlcipher=1
cmake ..			cmake -Dsqlcipher=1 ..
```

## Building and running the Unit Tests

DB Browser for SQLite has unit tests in the "src/tests" subdirectory.

### Build the unit tests

The unit tests are enabled using the cmake variable `ENABLE_TESTING`;
it can be passed when running `cmake` to configure sqlitebrowser,
for example like this:

```bash
$ mkdir build
$ cd build
$ cmake -DENABLE_TESTING=ON ..
$ make
```

### Run the unit tests

Tests can be then run using `make test` or invoking `ctest` directly,
for example like this:

```
$ ctest -V
UpdateCTestConfiguration  from :SRCDIR/build/DartConfiguration.tcl
UpdateCTestConfiguration  from :SRCDIR/build/DartConfiguration.tcl
Test project SRCDIR/build
Constructing a list of tests
Done constructing a list of tests
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: test-sqlobjects

1: Test command: SRCDIR/build/src/tests/test-sqlobjects
1: Test timeout computed to be: 9.99988e+06
1: ********* Start testing of TestTable *********
1: Config: Using QTest library 4.8.6, Qt 4.8.6
1: PASS   : TestTable::initTestCase()
1: PASS   : TestTable::sqlOutput()
1: PASS   : TestTable::autoincrement()
1: PASS   : TestTable::notnull()
1: PASS   : TestTable::withoutRowid()
1: PASS   : TestTable::foreignKeys()
1: PASS   : TestTable::parseSQL()
1: PASS   : TestTable::parseSQLdefaultexpr()
1: PASS   : TestTable::parseSQLMultiPk()
1: PASS   : TestTable::parseSQLForeignKey()
1: PASS   : TestTable::parseSQLSingleQuotes()
1: PASS   : TestTable::parseSQLKeywordInIdentifier()
1: PASS   : TestTable::parseSQLWithoutRowid()
1: PASS   : TestTable::parseNonASCIIChars()
1: PASS   : TestTable::parseSQLEscapedQuotes()
1: PASS   : TestTable::parseSQLForeignKeys()
1: PASS   : TestTable::parseSQLCheckConstraint()
1: PASS   : TestTable::createTableWithIn()
1: PASS   : TestTable::createTableWithNotLikeConstraint()
1: PASS   : TestTable::cleanupTestCase()
1: Totals: 20 passed, 0 failed, 0 skipped
1: ********* Finished testing of TestTable *********
1/2 Test #1: test-sqlobjects ..................   Passed    0.02 sec
test 2
    Start 2: test-import

2: Test command: SRCDIR/build/src/tests/test-import
2: Test timeout computed to be: 9.99988e+06
2: ********* Start testing of TestImport *********
2: Config: Using QTest library 4.8.6, Qt 4.8.6
2: PASS   : TestImport::initTestCase()
2: PASS   : TestImport::csvImport()
2: PASS   : TestImport::cleanupTestCase()
2: Totals: 3 passed, 0 failed, 0 skipped
2: ********* Finished testing of TestImport *********
2/2 Test #2: test-import ......................   Passed    0.01 sec

100% tests passed, 0 tests failed out of 2

Total Test time (real) =   0.04 sec
```

Everything should PASS, with no failures, and nothing skipped.
