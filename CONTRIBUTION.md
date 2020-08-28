# Contribute to DB4S

Thanks for your interest in our project.  
This document is intended to serve as lighthouse for contributors, not an executor.

## Contents
- [How to ask about DB4S](#how-to-ask-about-db4s)
- [How to contribute?](#how-to-contribute)
    - [Report bug](#report-bug)
    - [Suggest enhancement](#suggest-enhancement)
    - [Translation](#translation)
- [Styleguide](#styleguide)
    - [CPP source code](#cpp-source-code)
    - [Git commit message](#git-commit-message)

## How to ask about DB4S?
If you have any questions about our project, please check the following first.

- Look up related topics on the [wiki](https://github.com/sqlitebrowser/sqlitebrowser/wiki)
- Search for existing similar issues

If your question is still not resolved, you can create an issue and ask a question.  
A template from the GitHub issue will help you write a good bug report.

## How to contribute?
### Report bug
Please check the following before reporting a bug.

- Look up ['Known Issues'](https://github.com/sqlitebrowser/sqlitebrowser/issues/1688)
- Search for existing similar issues
- Try a [nightly build](https://nightlies.sqlitebrowser.org/latest)

If a bug has not yet been reported or has not been resolved, please follow these steps to help members identify the bug.

- Use a good title to identify the bug
    - Ex) `Crash DB4S when try to reset the settings`
- Detailed steps to reproduce the bug
    - Ex) `File -> Preferences -> General -> Restore Defaults`
- What you actually expected to do and why
    - Ex) `DB4S settings are initalized normally.`
- System environment, DB4S version, Loaded extensions
    - Ex) `macOS Catalina Version 10.15.6(19G2021), DB4S Version 3.12.0, No Extensions`
- Attach screenshots or screencasts related to the bug
    - You can add images can be pasted directly into the GitHub issue,  
    but screencast can be hosted on streamable.com and the link pasted into the issue.
- Crash log (If the crash log is known, **NOT** required)
    - If you have a long crash log, [Pastebin](https://pastebin.com/) can help (Make sure to set expiration to 'never')
- Project file or database file in use (**NOT** required)

And also a template from GitHub issue will help you write good bug report.

### Suggest enhancement
Please check the following before suggesting an enhancement.

- Search for existing similar issues

And please follow the below to help members identify the suggestion.

- Use a good title to identify the suggestion
- Describe the detailed steps for the enhancement you want to propose
- Attach a simple example of the proposal, already applied projects or screenshots

### Translation
Project translation can be done through the ['Qt Linguist'](http://doc.qt.io/qt-5/qtlinguist-index.html) program.
Languages that are already being translated can be translated by modifying the corresponding translation file,
and translations for new languages can be translated by adding a new entry to the TRANSLATION section of the src/src.pro file and then rebuilding the project.

#### DO NOT translate key names in shortcuts
Keyboard shortcuts get automatically localized by Qt, and translating key names may indeed break them. They show up on the translation file only for the rare occasions that one has to redirect a shortcut to another one in a specific language. But even then, you should use the English key names to get this working. Theoretically, one could follow Qt conventions for translated key names, and they should work, but the risk to stumble upon Qt bugs or moving conventions is high, so it is better to not translate keys and only remap the shortcut. If you want to preserve the original key sequence (recommended), just leave the translation empty and finished.

## Styleguide
Styleguides help keep the project's source code looking good and consistent.
This can be of great help to existing and new contributors.

### CPP source code
#### Indent
- 4 spaces(**NOT** tabs) for indentation

#### Declare variables
- Use lowerCamelCase
- Declare only one variable per line
- \* in pointer variable is attached to data type
    - Ex) `int* ptr`
- Avoid meaningless names (although these can be used for counter or temporary names)

#### Whitespace
- Do **NOT** use spaces after keywords
- Leave a space after each comma

#### Braces
- Place braces on the line after the keyword
- A Brace can be omitted if the execution statement in the conditional/repeated statement consists of only one line

### Git commit message
Keeping Git commit messages consistent can help keep track of code history.

- Limit the subject line to 72 character or less
- Capitalize the subject line
- Use the present tense
- Use the imperative mood
- Use the body to explain why
- Reference related issue number
    - Ex) `Add clearValue() method (#9999)`
