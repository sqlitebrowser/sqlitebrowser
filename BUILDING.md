## BUILD INSTRUCTIONS AND REQUIREMENTS

### Generic Linux and FreeBSD

DB Browser for SQLite requires Trolltech's Qt, version 4.6 or newer. For more
information on Qt please consult http://www.qt.io. The GPL version of
Qt is available in almost all Linux distributions as a default package.

The only requirements for building this code are the presence of Qt and
sqlite3. Qt can be included as a static or shared library, depending on the
current Qt configuration on the building machine.

Provided you have Qt and cmake installed and configured, simply run:

    $ cmake .

followed by:

    $ make

in the main directory. This will generate the sqlitebrowser (or
`sqlitebrowser.exe`, or `sqlitebrowser.app`) application in the src subdirectory.
On some distributions you can then install this in the correct places by
running:

    $ sudo make install

The same process works for building the code in any platform supported by Qt
(including other Unix systems with X11.)

### Ubuntu Linux

**1**. Make sure the `build-essential`, `cmake`, `libqt4-dev` and `libsqlite3-dev`
       packages are installed.<br />
**2**. Download the DB Browser for SQLite source code.<br />
**3**. Open a terminal in the source code directory.<br />
**4**. Run these commands:<br />
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```
**5**. This should complete without errors, giving you a binary file called
       'sqlitebrowser'.

Done. :)

### CentOS / Fedora Linux

**1**. Make sure the `qt-devel`, `ant-antlr`, and `antlr-C++` packages are installed.<br />
```
$ sudo yum install qt-devel ant-antlr antlr-C++
```
**2**. Download the DB Browser for SQLite source code.<br />
**3**. Open a terminal in the source code directory.<br />
**4**. Run these commands:<br />
```bash
$ cmake . && make && sudo make install
```
**5**. This should complete without errors, and `sqlitebrowser` should now be launch-able from the command line.

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
It requires SQLite and Qt 4.x/5.x to be installed first.  These are the
[Homebrew](http://brew.sh) steps, though other package managers should work:

    $ brew install sqlite --with-functions --without-readline
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
    $ mv src/sqlitebrowser.app /Applications/

An icon for "sqlitebrowser" should now be in your main OSX Applications
list, ready to launch.

**Note 2** - There have been occasional [reports of compilation problems on OSX
10.9](https://github.com/sqlitebrowser/sqlitebrowser/issues/38), with the
'make' step complaining about no targets.  This seems to be solvable by
running:

    $ qmake -spec macx-g++

or:

    $ qmake -spec macx-llvm

(before the 'make' step)

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
