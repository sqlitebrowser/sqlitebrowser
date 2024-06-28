# CHANGELOG for DB4S

All notable changes to the **Database Browser for SQLite** -
DB4S - project are documented in this file.
The format is based on
[Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased - nightly] - soon to be [3.13.0] - 2024-07-xx

We are proud to announce DB4S 3.13.0 - a very large update over the previous 3.12.2 version released in 2021.
It incorporates literally hundreds of enhancements and bug fixes.
The list below highlights the major changes.
See the [commit history](https://github.com/sqlitebrowser/sqlitebrowser/commits/master/) for fine-grained details.

### Added
- **General**
	- Update to SQLite version 3.46.0
	- Integration with [dbhub.io](https://dbhub.io)
	- macOS ARM version
   	- Sign Windows builds with [SignPath.io](https://SignPath.io)

- **Appearance and GUI Enhancements**
	- Add .NET DateTime.Ticks to date display format 
	- Add a refresh button to the Database Structure tab 
	- Add an extra tab for editing check constraints to the Edit Table dialog 
	- Add an extra tab for editing foreign keys to the Edit Table dialog 
	- Add auto completion for math functions  
	- Add Ctrl+Enter as shortcut for executing query 
	- Add new menu item to import from CSV data in the system clipboard 
	- Add new menu item to show the row counts of all tables 
	- Allow users to export or import user-settings file in a GUI environment  
	- Add file, line number and function to Error Log 
	- Add GUID display format 
	- Add iif SQL function to the list of known functions in Execute SQL tab 
	- Add new option to freeze columns in the Browse Data tab 
	- Allow multiple TableBrowser tabs in the Browse Data tab 
	- Bar charts now display labels in x axis when NULL values present 
	- Scale large images in ImageViewer to fit the viewport by default 
	- Enable HiDPI on all platforms by default 
	- Change tabbed table browisng to use docks instead 
	- Change mouse cursor to the pointing hand when Ctrl+Shift are pressed 
	- Change to 'Preference' expression for consistent 
	- Data Browser: avoid showing BLOBs containing unprintable characters as text 
	- Data Browser: export query results as JSON 
	- Edit Database Cell: fixed potential for data loss and better feedback 
	- Edit Table: alternating row colors in tables for better readability 
	- Edit Table: Change the key icons from header to tooltips 
	- Fix indentation - always use 4 spaces
	- Plot Dock: use more precision for small numbers and less for big numbers 
	- Plot: fix selecting points to select corresponding line in table 
	- Plot: show x and y values on a tooltip when hovering over the plot 
	- Table Browser: "Clear Filters" button to clear the global filter 
	- Table Browser: give hint of table sorted or filtered state in the toolbar 
	- Table Browser: Support extended selections 
	- Add copy column name to table viewer 
	- Allow multiple -t/--table parameters on the command line 
	- Open recent files in read-only mode using a different shortcut
	- Allow selecting fixed format for big integer numbers 
	- Allow the user to change MaxRecentFiles  
	- Always load settings for Browse Data tabs 
	- Always raise new Data Browser tabs to the foreground 

- **SQL Processing**
	- parser: Add support for IS (NOT) DISTINCT FROM expressions 
	- Execute SQL: add Ctrl+Shift+T for "Open SQL file(s)" 
	- Fall back to asking SQLite when parsing a table schema has failed 
	- Update schema after attaching/detaching a database in an Execute SQL tab 
	- Also update schema when clicking the Refresh button in Browse Data tab 
	- Improvements for working in the SQL Execution area using keyboard 
	- When modifying a view use DROP VIEW IF EXISTS instead of just DROP VIEW 
	- Add basic support for new RETURNING keyword 
	- Change WHERE clauses of queries to use column names instead of indexes 
	- DB Schema: drag & drop SELECT queries 
	- Export query results as JSON 
	- Export SQL: fix quoting of BLOB values and non-printable strings 
	- Export SQL: option to keep original CREATE statements 
	- Improve support for BLOB columns as primary key 
	- Use LIMIT + OFFSET when generating SELECT statements instead 


- **Files**
	- Import: Add import-csv option to command line to import CSV files 
	- Move recent files items to new submenu and add clear recent file function  
	- CLI: Reuse `--table` as table name for a CSV Import 
	- CSV Export: binary BLOB to base64 
	- CSV Import: fix importing into existing table 
	- CSV Import: give option to use system locale conventions 
	- Import: allow CSV separator and quote to be passed from command line 
	- Import: announce support for CSV files in Desktop file 
	- Import: do not remove characters from CSV header fields 
	- Import: don't assume an empty file is a text file 
	- Import: support importing the single file argument as a CSV file 

- **Configuration**
	- New setting for configuring brace matching background 
	- New setting for having close button on tabs (default) or not 
	- New settings for changing the selection appearance in editors 
	- Add the ability to support custom config files  

### Changed

There have been too many changes since the last full release to list separately.
Important changes are listed in the **Added** section above.
 
### Fixed

There have been too many bug fixes since the last full release to list separately.
Please check out 3.13.0 and let us know if your issue still exists

### Removed

- _none_

### Translation
- Add Dutch, Indonesian, Swedish translation 
- Update Arabic, Brazilian Portuguese, Chinese, French, German, Italian, Japanese, Korean, Polish, Spanish translation 

## [3.12.2] - 2021-05-17

Release version of DB4S - DB Browser for SQLite.
The basic features of the program are described in the
[README.](https://github.com/sqlitebrowser/sqlitebrowser)

Download release versions from
[https://sqlitebrowser.org/dl/](https://sqlitebrowser.org/dl/)
