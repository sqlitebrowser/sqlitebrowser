## BUILD INSTRUCTIONS AND REQUIREMENTS

DB Browser for SQLite requires Qt as well as SQLite. For more information on Qt
please consult http://www.qt.io and for SQLite please see https://sqlite.org/.

Please note that all versions after 3.9.1 will require:
* Qt 5.5 or later, however we advise you to use 5.7 or later
* A C++ compiler with support for C++11 or later

Without these or with older versions you won't be able to compile DB Browser for
Sqlite anymore. This applies to all platforms. However, most likely you won't
have to worry about these as most systems meet these requirements today.

If you have any chance, please use Qt 5.7 or any later version. Even though Qt
5.5 and 5.6 are supported by us, there might be glitches and minor problems
when using them.

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


### Ubuntu Linux

```bash
$ sudo apt install build-essential git cmake libsqlite3-dev qt5-default qttools5-dev-tools
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
$ cd sqlitebrowser
$ mkdir build
$ cd build
$ cmake -Wno-dev ..
$ make
$ sudo make install
```

This should complete without errors, giving you a binary file called 'sqlitebrowser'.

Done. :)

### CentOS / Fedora Linux

**Note** - On CentOS or an older version of Fedora, you may need to use `yum` instead of `dnf`

**Note 2** - On CentOS 7.x, you need to add `qt5-qtbase-devel` to the `dnf install` line below

```
$ sudo dnf install ant-antlr antlr-C++ cmake gcc-c++ git qt-devel qt5-linguist qwt-qt5-devel \
       sqlite-devel
$ git clone https://github.com/sqlitebrowser/sqlitebrowser
$ cd sqlitebrowser
$ cmake -Wno-dev .
$ make
$ sudo make install
```

This should complete without errors, and `sqlitebrowser` should now be launch-able from the command line.

### MacOS X

The application can be compiled to a single executable binary file, similar to
other command line utilities.  Or it can be compiled to a .app bundle, suitable
for placing in /Applications.

### Building a single executable binary

This is incredibly easy using [Homebrew](http://brew.sh).  Just run this command:

    $ brew install sqlitebrowser

And you're done.  A "sqlitebrowser" command should now be available in your PATH,
and can also be launched through Spotlight.

### Building an .app bundle

Building an .app bundle version takes a bit more effort, but isn't too hard.
It requires SQLite and Qt 5.x to be installed first.  These are the
[Homebrew](http://brew.sh) steps, though other package managers should work:

    $ brew install sqlite --with-functions --with-json1 --without-readline
    $ brew install qt
    $ brew link sqlite3 --force

Then it's just a matter of getting the source:

    $ git clone https://github.com/sqlitebrowser/sqlitebrowser.git

**Note** - Don't clone the repo to a directory with a quote character (') in
its name (eg ~/tmp/foo'), as compiling will error out.

And compiling it:

    $ cd sqlitebrowser
    $ qmake
    $ make
    $ brew unlink sqlite3
    $ mv src/DB\ Browser\ for\ SQLite.app /Applications/

An icon for "DB Browser for SQLite" should now be in your main OSX Applications
list, ready to launch.

**Note 2** - There have been occasional [reports of compilation problems on OSX
10.9](https://github.com/sqlitebrowser/sqlitebrowser/issues/38), with the
'make' step complaining about no targets.  This seems to be solvable by
running:

    $ qmake -spec macx-g++

or:

    $ qmake -spec macx-llvm

(before the 'make' step)

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

If you additionaly want an NSIS installer:

    $ make package

done.

## Build with SQLCipher support

When built with SQLCipher support, DB Browser for SQLite will allow you to open
and edit databases encrypted using SQLCipher as well as standard SQLite3
databases.

Before compiling make sure you have the necessary SQLCipher development files
installed. On Linux this can usually be accomplished by just installing the
correct package (e.g. 'libsqlcipher-dev' on Debian-based distributions). On
MacOS X the easiest way is to install it via Homebrew ('brew install
sqlcipher'). On Windows unfortunately it's a bit more difficult: You'll have
to download and compile the code as described on the
[SQLCipher website](https://www.zetetic.net/sqlcipher/) before you can proceed.

If SQLCipher is installed, simply follow the standard instructions for your
platform but enable the 'sqlcipher' build option by replacing any calls to
cmake and qmake like this:
```
If it says...			Change it to...
cmake				cmake -Dsqlcipher=1
cmake ..			cmake -Dsqlcipher=1 ..
qmake				qmake CONFIG+=sqlcipher
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
