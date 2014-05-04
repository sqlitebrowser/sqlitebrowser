=======================
SQLite Database Browser
=======================

This is the official home of the SQLite Database Browser project, previously
housed on SourceForge.

Screenshot
----------

.. image:: https://github.com/sqlitebrowser/sqlitebrowser/raw/master/images/sqlitebrowser.png
   :height: 641px
   :width: 725px
   :scale: 100%
   :alt: SQLiteBrowser Screenshot
   :align: center

Old Project Pages
-----------------

- http://sqlitebrowser.sourceforge.net
- https://sourceforge.net/projects/sqlitebrowser/

What it is
----------

SQLite Database Browser is a free, open source visual tool used to create,
design and edit database files compatible with SQLite. It is meant to be used
by users and developers wanting to create databases, edit and search data
using a familiar spreadsheet-like interface, without the need to learn
complicated SQL commands.

What's been done since then
---------------------------

- Qt3Support was removed
- Recent files menu added
- Improved UI, making it more modern, replacing some dialogs etc.
- Syntax highlighting for SQL code
- Cleaned up the code, reducing the SLOC quite a bit
- Added basic support for triggers and views
- Added pragma editing
- Added BLOB support
- Added a new filter row for searching
- Improved performance when opening large tables
- Extended the SQL tab
- Added SQLite extension support
- Fixed a ton of bugs
- Probably more

All in all a fair amount of the code has been rewritten in order to regain
maintainability. Based on this quite a few bugs could be fixed and some
features added.

What's still to do
------------------

- Even more code cleanup
- Further improvement of the UI, adding more features and making it easier to
  use
- Feel free to add more issues at
  https://github.com/sqlitebrowser/sqlitebrowser/issues

Windows binaries
----------------
Windows binaries can be downloaded from here:

- https://github.com/sqlitebrowser/sqlitebrowser/releases

Nightly builds are also available at:

- http://rp.oldsch00l.com/sqlitebrowser/

MacOS X
-------

SQLite Database Browser works pretty well on MacOS X.

- OSX 10.7 (Lion) and 10.8 (Mountain Lion) are tested and known to work

Building on OSX is simple, but depends on SQLite and Qt to be installed
using Homebrew first::

  $ brew install sqlite --with-functions
  $ brew install qt --with-qt3support

Then it's just a matter of::

  $ git clone https://github.com/sqlitebrowser/sqlitebrowser.git
  $ cd sqlitebrowser
  $ qmake
  $ make
  $ mv src/sqlitebrowser.app /Applications/

An icon for "sqlitebrowser" should now be in your main OSX Applications
list, ready to launch.
