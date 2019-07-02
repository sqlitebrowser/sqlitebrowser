# DB Browser for SQLite

[![Wiki][wiki-img]][wiki]
[![Build Status][travis-img]][travis]
[![Join the chat at https://gitter.im/sqlitebrowser/sqlitebrowser][gitter-img]][gitter]
[![Join the chat at https://join.slack.com/t/db4s/shared_invite/enQtMzc3MzY5OTU4NDgzLWRlYjk0ZmE5ZDEzYWVmNDQxYTYxNmJjNWVkMjI3ZmVjZTY2NDBjODY3YzNhNTNmZDVlNWI2ZGFjNTk5MjJkYmU][slack-img]][slack]
[![Download][download-img]][download]
[![Qt][qt-img]][qt]
[![Coverity][coverity-img]][coverity]
[![Patreon][patreon-img]][patreon]

![DB Browser for SQLite Screenshot](https://github.com/sqlitebrowser/sqlitebrowser/raw/master/images/sqlitebrowser.png "DB Browser for SQLite Screenshot")

## What it is

_DB Browser for SQLite_ (DB4S) is a high quality, visual, open source tool to
create, design, and edit database files compatible with SQLite.

DB4S is for users and developers who want to create, search, and edit
databases.  DB4S uses a familiar spreadsheet-like interface, and complicated SQL commands do not have to be learned.

Controls and wizards are available for users to:

* Create and compact database files
* Create, define, modify and delete tables
* Create, define, and delete indexes
* Browse, edit, add, and delete records
* Search records
* Import and export records as text
* Import and export tables from/to CSV files
* Import and export databases from/to SQL dump files
* Issue SQL queries and inspect the results
* Examine a log of all SQL commands issued by the application
* Plot simple graphs based on table or query data

## What it is not

This program is not a visual shell for the sqlite command line tool, and does
not require familiarity with SQL commands. It is a tool to be used by both
developers and end users, and must remain as simple to use as possible
in order to achieve these goals.

## Wiki

For user and developer documentation, check out our Wiki at:
https://github.com/sqlitebrowser/sqlitebrowser/wiki.

## Nightly builds

Download nightly builds for Windows and OSX here:

* https://nightlies.sqlitebrowser.org/latest

## Windows

Download Windows releases here:

* https://sqlitebrowser.org/dl/#windows

**Note** - If for some reason the standard Windows release does not work
(e.g. gives an error), try a nightly build.  Nightly builds often fix bugs
reported after the last release. :D

## MacOS X / macOS

DB Browser for SQLite works well on MacOS X / macOS.

* OSX 10.8 (Mountain Lion) - 10.13 (High Sierra) are tested and known to work

Download OSX releases here:

* https://sqlitebrowser.org/dl/#macos

The latest OSX binary can be installed via [Homebrew Cask](https://caskroom.github.io/ "Homebrew Cask"):

```
brew cask install db-browser-for-sqlite
```

## Linux

DB Browser for SQLite works well on Linux.

### Arch Linux

Arch Linux provides a package through pacman.

### Fedora

Install for Fedora (i386 and x86_64) by issuing the following command:

    $ sudo dnf install sqlitebrowser
    
### openSUSE

    $ sudo zypper install sqlitebrowser

### Debian

Note that Debian focuses more on stability rather than newest features. Therefore packages will typically contain some older (but well tested) version, compared to the latest release.

Update the cache using:

    sudo apt-get update

Install the package using:

    sudo apt-get install sqlitebrowser


### Ubuntu and Derivatives

#### Stable release

For Ubuntu and derivaties, [@deepsidhu1313](https://github.com/deepsidhu1313)
provides a PPA with the latest release here:

* https://launchpad.net/~linuxgndu/+archive/ubuntu/sqlitebrowser

To add this ppa just type in these commands in terminal:

    sudo add-apt-repository -y ppa:linuxgndu/sqlitebrowser

Then update the cache using:

    sudo apt-get update

Install the package using:

    sudo apt-get install sqlitebrowser

Ubuntu 14.04.X, 15.04.X, 15.10.X and 16.04.X are supported for now (until
Launchpad decides to discontinue building for any series).

Ubuntu Precise (12.04) and Utopic (14.10) are not supported:
* Precise does not have a new enough Qt package in its repository by default,
  which is a dependency
* Launchpad does not support Utopic any more, which has reached its End of
  Life

#### Nightly builds

Nightly builds are available here:

* https://launchpad.net/~linuxgndu/+archive/ubuntu/sqlitebrowser-testing

To add this ppa, type these commands into the terminal:

    sudo add-apt-repository -y ppa:linuxgndu/sqlitebrowser-testing

Then update the cache using:

    sudo apt-get update

Install the package using:

    sudo apt-get install sqlitebrowser

### Other Linux

On others, compile DB4S using the instructions
in [BUILDING.md](BUILDING.md).

## FreeBSD

DB Browser for SQLite works well on FreeBSD, and there is a port for it (thanks
to [lbartoletti](https://github.com/lbartoletti) :smile:).  DB4S can be installed
using either this command:

    # make -C /usr/ports/databases/sqlitebrowser install

or this command:

    # pkg install sqlitebrowser

## Snap packages

[![Get it from the Snap Store](https://snapcraft.io/static/images/badges/en/snap-store-black.svg)](https://snapcraft.io/sqlitebrowser)

#### Snap Nightlies

     snap install sqlitebrowser --devmode

#### Snap Stable

     snap install sqlitebrowser


## Compiling

Instructions for compiling on Windows, OSX, Linux, and FreeBSD are
in [BUILDING](BUILDING.md).

## Twitter

Follow us on Twitter: https://twitter.com/sqlitebrowser

## Website

* https://sqlitebrowser.org

## Old project page

* https://sourceforge.net/projects/sqlitebrowser

## Releases

* [Version 3.11.2 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.11.2) - 2019-04-03
* [Version 3.11.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.11.1) - 2019-02-18
* [Version 3.11.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.11.0) - 2019-02-07
* [Version 3.10.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.10.1) - 2017-09-20
* [Version 3.10.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.10.0) - 2017-08-20
* [Version 3.9.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.9.1) - 2016-10-03
* [Version 3.9.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.9.0) - 2016-08-24
* [Version 3.8.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.8.0) - 2015-12-25
* [Version 3.7.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.7.0) - 2015-06-14
* [Version 3.6.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.6.0) - 2015-04-27
* [Version 3.5.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.5.1) - 2015-02-08
* [Version 3.5.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.5.0) - 2015-01-31
* [Version 3.4.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.4.0) - 2014-10-29
* [Version 3.3.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.3.1) - 2014-08-31 - Project renamed from "SQLite Database Browser"
* [Version 3.3.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/v3.3.0) - 2014-08-24
* [Version 3.2.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.2.0) - 2014-07-06
* [Version 3.1.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.1.0) - 2014-05-17
* [Version 3.0.3 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.0.3) - 2014-04-28
* [Version 3.0.2 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.0.2) - 2014-02-12
* [Version 3.0.1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.0.1) - 2013-12-02
* [Version 3.0 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/sqlb-3.0) - 2013-09-15
* [Version 3.0rc1 released](https://github.com/sqlitebrowser/sqlitebrowser/releases/tag/rc1) - 2013-09-09 - Project now on GitHub
* Version 2.0b1 released - 2009-12-10 - Based on Qt4.6
* Version 1.2 released - 2005-04-05
* Version 1.1 released - 2004-07-20
* Version 1.01 released - 2003-10-02
* Version 1.0 released to public domain - 2003-08-19

## History

This program was developed originally by Mauricio Piacentini
([@piacentini](https://github.com/piacentini)) from Tabuleiro Producoes, as
the Arca Database Browser. The original version was used as a free companion
tool to the Arca Database Xtra, a commercial product that embeds SQLite
databases with some additional extensions to handle compressed and binary data.

The original code was trimmed and adjusted to be compatible with standard
SQLite 2.x databases. The resulting program was renamed SQLite Database
Browser, and released into the Public Domain by Mauricio. Icons were
contributed by [Raquel Ravanini](http://www.raquelravanini.com), also from
Tabuleiro. Jens Miltner ([@jmiltner](https://github.com/jmiltner)) contributed
the code to support SQLite 3.x databases for the 1.2 release.

Pete Morgan ([@daffodil](https://github.com/daffodil)) created an initial
project on GitHub with the code in 2012, where several contributors fixed and
improved pieces over the years. René Peinthor ([@rp-](https://github.com/rp-))
and Martin Kleusberg ([@MKleusberg](https://github.com/MKleusberg)) then
became involved, and have been the main driving force from that point.  Justin
Clift ([@justinclift](https://github.com/justinclift)) helps out with testing
on OSX, and started the new github.com/sqlitebrowser organisation on GitHub.

[John T. Haller](http://johnhaller.com), of
[PortableApps.com](http://portableapps.com) fame, created the new logo.  He
based it on the Tango icon set (public domain).

In August 2014, the project was renamed to "Database Browser for SQLite" at
the request of [Richard Hipp](http://www.hwaci.com/drh) (creator of
[SQLite](http://sqlite.org)), as the previous name was creating unintended
support issues.

In September 2014, the project was renamed to "DB Browser for SQLite", to
avoid confusion with an existing application called "Database Browser".

## Contributors

View the list by going to the [__Contributors__ tab](https://github.com/sqlitebrowser/sqlitebrowser/graphs/contributors).

## License

DB Browser for SQLite is bi-licensed under the Mozilla Public License
Version 2, as well as the GNU General Public License Version 3 or later.

Modification or redistribution is permitted under the conditions of these licenses.

  [travis-img]: https://travis-ci.org/sqlitebrowser/sqlitebrowser.svg?branch=master
  [travis]: https://travis-ci.org/sqlitebrowser/sqlitebrowser

  [gitter-img]: https://badges.gitter.im/sqlitebrowser/sqlitebrowser.svg
  [gitter]: https://gitter.im/sqlitebrowser/sqlitebrowser

  [slack-img]: https://img.shields.io/badge/chat-on%20slack-orange.svg
  [slack]: https://join.slack.com/t/db4s/shared_invite/enQtMzc3MzY5OTU4NDgzLWRlYjk0ZmE5ZDEzYWVmNDQxYTYxNmJjNWVkMjI3ZmVjZTY2NDBjODY3YzNhNTNmZDVlNWI2ZGFjNTk5MjJkYmU

  [download-img]: https://img.shields.io/github/downloads/sqlitebrowser/sqlitebrowser/total.svg
  [download]: https://github.com/sqlitebrowser/sqlitebrowser/releases

  [qt-img]: https://img.shields.io/badge/Qt-qmake-green.svg
  [qt]: https://www.qt.io

  [coverity-img]: https://img.shields.io/coverity/scan/11712.svg
  [coverity]: https://scan.coverity.com/projects/sqlitebrowser-sqlitebrowser

  [patreon-img]: https://img.shields.io/badge/donate-Patreon-coral.svg
  [patreon]: https://www.patreon.com/bePatron?u=11578749

  [wiki-img]: https://img.shields.io/badge/docs-Wiki-blue.svg
  [wiki]: https://github.com/sqlitebrowser/sqlitebrowser/wiki
