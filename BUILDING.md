## BUILD INSTRUCTIONS AND REQUIREMENTS ##

SQLite Database Browser requires Trolltech's Qt, version 4.6 or newer.
For more information on Qt please consult http://www.qtsoftware.com.
The GPL version of Qt is available in almost all Linux distributions
as a default package.

The only requirements for building this code are the presence of Qt
and sqlite3. Qt can be included as a static or shared library,
depending on the current Qt configuration on the building machine.

Provided you have Qt and cmake installed and configured, simply run

    $ cmake .

followed by

    $ make

in the main directory. This will generate the sqlitebrowser (or
sqlitebrowser.exe, or sqlitebrowser.app) application in the src
subdirectory.

The same process works for building the code in any platform
supported by Qt (including other Unix systems with X11.)

### Cross compiling for Windows ###

These are instructions to cross compile within a Linux system a
Windows binary and installer.

Requirements:

* mxe cross compile environment → http://mxe.cc
* cmake
* sqlitebrowser sources

Get the following mxe packages:

    $ make gcc sqlite qt nsis

After successful compilation go into your mxedir/usr/bin and
add 3 symlinks:

    $ ln -s i686-pc-mingw32-windres windres
    $ ln -s i686-pc-mingw32-makensis makensis
    $ ln -s /usr/bin/lrelease

Now cd into your sqlitebrowser source directory and create
a build directory for
the windows binary and create the correct makefiles:

    $ mkdir build-win
    $ cd build-win
    $ cmake -DCMAKE_TOOLCHAIN_FILE=/path to mxe/usr/i686-pc-mingw32/share/cmake/mxe-conf.cmake ..

Before compiling we have to add the mxe/usr/bin directory
to the PATH (so windres and makensis can be found):

    $ export PATH=/path to mxe/usr/bin:$PATH

Now compile:

    $ make

If you additionaly want an NSIS installer:

    $ make package

done.


## Compiling and running the Unit Tests ##

SQLite Browser has unit tests in the "tests" subdirectory.

### Compile the unit tests ###

Compile them like this:

```
$ cd tests
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### Run the unit tests ###

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

Everything should PASS, with no failures and nothing
skipped.
