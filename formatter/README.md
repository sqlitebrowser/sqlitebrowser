# AUTOMATED CODE FORMATTING

## BACKGROUND

Code Formatting is considered "best practice" as it provides code quality assurance for a given project. Project code is consistently formatted in the same manner throughout the project tree and all development is directed to the same formatting standard. Linting and formatting source code files prior to committing to a CVS also removes extraneous whitespace characters. Differences in formatting style and extraneous whitespaces typically creep into a project over time where multiple developers are working on a project and each developer is using their preferred editing application. Problem is compounded when developers are using multiple platforms (i.e. Unix, Linux, Windows, et al)

All necessary configuration settings for automated code formatting as it applies to **DB Browser for SQLite** are stored in this folder.

## DEPENDENT APPLICATIONS

Application leverages Python 3 to execute other applications to perform automated code formatting. These dependent applications are available across may platforms Ensure these applications are installed prior to attempting to follow the **OPERATION** instructions below.

### Python 3

+ available for most platforms
+ Unix and Linux distributions may already have it installed.
+ see <https://www.python.org/downloads/> to install for your platform

### toml

+ `Tom's Obvious Markup Language` is used for format configuration storage.
+ Python does not have a _standard_ module for toml parsing.
+ install module from pypi using `pip install toml`
+ toml project - <https://github.com/toml-lang/toml>
+ wikipedia - <https://en.wikipedia.org/wiki/TOML>
+ pypi - <https://pypi.org/project/toml/>

### astyle

+ small, automatic formatter for C, C++, Objective-C, and Java languages
+ available for most platforms
+ project and documentation - <http://astyle.sourceforge.net/astyle.html>
+ Sourceforge - <https://sourceforge.net/projects/astyle/>
+ configuration - `astylerc`

### xmllint

+ basic XML file linting and formatting
+ available for most platforms
+ apart of the libxml2 library
+ project - <http://www.xmlsoft.org/>
+ configuration - environment variables and flags defined in settings.

### cmake-format

+ Python module containing QA tools for CMake files
+ install via `pip install cmake_format`
+ project - <https://github.com/cheshirekow/cmake_format>
+ pypi - <https://pypi.org/project/cmake-format/>
+ documentation - <https://cmake-format.readthedocs.io/en/latest/>
+ configuration - `cmake-format.json`

## OPERATION

For all platforms, invoke Python on the module `format_runner.py`:

```bash
[cd|DIR] [path to DB4S project root]
python3 formatter/format_runner.py
```

The Python module will test for each formatting application. If application is found, the script will execute instructions for that tool on **DB Browser for SQLite** files of type based on the stored settings.

Each formatting application will use the configuration settings stored in this folder. On script completion, examine any changes made by executing

```bash
git diff HEAD --
```

Ideally, code formatting should be done **prior to** committing a pull request.

## Configuration File `format_runner.toml`

Format tools configuration is stored in the `format_runner.toml` file in this directory. The basic layout of the configuration file breaks down to two major sections: `paths` and `tools`.

Each entry in the `paths` section starts with the name of the tool and is followed by a list of paths. Each path can be a specific file or a file-glob pattern. `format_runner` does not look for duplication.

The `tools` section is a list of entries where each entry defines arguments and other parameters for the operation of a format tool. Use the comments in the example below to better understand the layout of the configuration file.

It should be noted that how configuration arguments are defined is somewhat flexible. The long-form (e.g. `--something`) should be preferred versus short form (e.g. `-s`) for readability and reference. The use of dashes before an argument (`-` or `--`) is not mandatory. Formatting application will prepend dashes where necessary following the convention of a single dash for one character or two dashes for longer arguments.

```toml
# all paths are relative to the project root directory
[paths]
toolA = [
  "src/*.this_ext",
  "src/*.that_ext"
]
toolB = [
  "src/*.other_ext",
  "lib/*.another_ext"
]

[[tools]]

  [tools.name]
  # what appears on the console
  name_text = "toolA"
  # name of the executable
  name_exec = "toolA"
  # tri-state (absent, false, true)
  # force command arguments to be separated by an equal sign
  force_equal = false

  [tools.arg_version]
  # argument to invoke format tools version
  arg = "version"
  # (optional) Define a version limit of format tool.
  # Simple (uncompounded) logical expression only.
  # oper -> one of LE, LT, EQ, NE, GE, GT
  oper = "EQ"
  # version value to test against
  value = "1.0"

  [tools.arg_file]
  # command line switches for identifying input/output files.
  # if absent, format tool will be invoked to work on file 'in-place'
  # NOTE: an extra argument may be required for that (i.e. cmake-format `--in-place`)
  arg_input = "input"
  arg_output = "outfile-path"

  [tools.arg_conf]
  # argument to format tool to identify cofiguration file
  arg = "options"
  # name of configuration file to use - assumed to be relative to this folder.
  value = "astylerc"

[[tools]]

  [tools.name]
  # display name
  name_text = "toolB"
  # rather than redifining all paramters of the same tool, copy another instance.
  # copy source identified must exist and have been parsed.
  copy = "toolA"

  [tools.env]
  # environment flags that need to be set for proper operation of format tool.
  INDENT = "    "
  DIR = '/home/user'

  [tools.arg_extra]
  # extra args to place in the command to format tool.
  # key is name of flag (command line switch),
  # value is an additional paramter. If no value, use empty quotes.
  recursive = ""
  format = ""
```
