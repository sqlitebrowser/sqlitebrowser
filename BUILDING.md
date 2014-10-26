## BUILD INSTRUCTIONS AND REQUIREMENTS

### Generic Linux and FreeBSD

DB Browser for SQLite requires Trolltech's Qt, version 4.6 or newer. For more
information on Qt please consult http://www.qtsoftware.com. The GPL version of
Qt is available in almost all Linux distributions as a default package.

The only requirements for building this code are the presence of Qt and
sqlite3. Qt can be included as a static or shared library, depending on the
current Qt configuration on the building machine.

Provided you have Qt and cmake installed and configured, simply run:

    $ cmake .

followed by:

    $ make

in the main directory. This will generate the sqlitebrowser (or
sqlitebrowser.exe, or sqlitebrowser.app) application in the src subdirectory.
On some distributions you can then install this in the correct places by
running:

    $ sudo make install

The same process works for building the code in any platform supported by Qt
(including other Unix systems with X11.)

### Ubuntu Linux

**1**. Make sure the build-essential, cmake, libqt4-dev and libsqlite3-dev
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


## Building and running the Unit Tests

DB Browser for SQLite has unit tests in the "tests" subdirectory.

### Build the unit tests

Compile them like this:

```
$ cd tests
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Run the unit tests

Then run them like this:

```
$ ./sqlb-unittests
********* Start testing of TestTable *********
Config: Using QTest library 4.8.6, Qt 4.8.6
PASS   : TestTable::initTestCase()
PASS   : TestTable::sqlOutput()
PASS   : TestTable::autoincrement()
PASS   : TestTable::notnull()
PASS   : TestTable::withoutRowid()
PASS   : TestTable::parseSQL()
PASS   : TestTable::parseSQLdefaultexpr()
PASS   : TestTable::parseSQLMultiPk()
PASS   : TestTable::parseSQLForeignKey()
PASS   : TestTable::parseSQLSingleQuotes()
PASS   : TestTable::parseSQLKeywordInIdentifier()
PASS   : TestTable::parseSQLWithoutRowid()
PASS   : TestTable::parseNonASCIIChars()
PASS   : TestTable::createTableWithIn()
PASS   : TestTable::createTableWithNotLikeConstraint()
PASS   : TestTable::cleanupTestCase()
Totals: 16 passed, 0 failed, 0 skipped
********* Finished testing of TestTable *********
```

Everything should PASS, with no failures, and nothing skipped.
