// This module implements the QsciLexerBash class.
//
// Copyright (c) 2023 Riverbank Computing Limited <info@riverbankcomputing.com>
// 
// This file is part of QScintilla.
// 
// This file may be used under the terms of the GNU General Public License
// version 3.0 as published by the Free Software Foundation and appearing in
// the file LICENSE included in the packaging of this file.  Please review the
// following information to ensure the GNU General Public License version 3.0
// requirements will be met: http://www.gnu.org/copyleft/gpl.html.
// 
// If you do not wish to use this file under the terms of the GPL version 3.0
// then you may purchase a commercial license.  For more information contact
// info@riverbankcomputing.com.
// 
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#include "Qsci/qscilexerbash.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerBash::QsciLexerBash(QObject *parent)
    : QsciLexer(parent), fold_comments(false), fold_compact(true)
{
}


// The dtor.
QsciLexerBash::~QsciLexerBash()
{
}


// Returns the language name.
const char *QsciLexerBash::language() const
{
    return "Bash";
}


// Returns the lexer name.
const char *QsciLexerBash::lexer() const
{
    return "bash";
}


// Return the style used for braces.
int QsciLexerBash::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerBash::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_$@%&";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerBash::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Error:
    case Backticks:
        return QColor(0xff,0xff,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
    case SingleQuotedHereDocument:
        return QColor(0x7f,0x00,0x7f);

    case Operator:
    case Identifier:
    case Scalar:
    case ParameterExpansion:
    case HereDocumentDelimiter:
        return QColor(0x00,0x00,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerBash::defaultEolFill(int style) const
{
    switch (style)
    {
    case SingleQuotedHereDocument:
        return true;
    }

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerBash::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        break;

    case Keyword:
    case Operator:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DoubleQuotedString:
    case SingleQuotedString:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerBash::keywords(int set) const
{
    if (set == 1)
        return
            "alias ar asa awk banner basename bash bc bdiff break "
            "bunzip2 bzip2 cal calendar case cat cc cd chmod "
            "cksum clear cmp col comm compress continue cp cpio "
            "crypt csplit ctags cut date dc dd declare deroff dev "
            "df diff diff3 dircmp dirname do done du echo ed "
            "egrep elif else env esac eval ex exec exit expand "
            "export expr false fc fgrep fi file find fmt fold for "
            "function functions getconf getopt getopts grep gres "
            "hash head help history iconv id if in integer jobs "
            "join kill local lc let line ln logname look ls m4 "
            "mail mailx make man mkdir more mt mv newgrp nl nm "
            "nohup ntps od pack paste patch pathchk pax pcat perl "
            "pg pr print printf ps pwd read readonly red return "
            "rev rm rmdir sed select set sh shift size sleep sort "
            "spell split start stop strings strip stty sum "
            "suspend sync tail tar tee test then time times touch "
            "tr trap true tsort tty type typeset ulimit umask "
            "unalias uname uncompress unexpand uniq unpack unset "
            "until uudecode uuencode vi vim vpax wait wc whence "
            "which while who wpaste wstart xargs zcat "

            "chgrp chown chroot dir dircolors factor groups "
            "hostid install link md5sum mkfifo mknod nice pinky "
            "printenv ptx readlink seq sha1sum shred stat su tac "
            "unlink users vdir whoami yes";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerBash::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Error:
        return tr("Error");

    case Comment:
        return tr("Comment");

    case Number:
        return tr("Number");

    case Keyword:
        return tr("Keyword");

    case DoubleQuotedString:
        return tr("Double-quoted string");

    case SingleQuotedString:
        return tr("Single-quoted string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case Scalar:
        return tr("Scalar");

    case ParameterExpansion:
        return tr("Parameter expansion");

    case Backticks:
        return tr("Backticks");

    case HereDocumentDelimiter:
        return tr("Here document delimiter");

    case SingleQuotedHereDocument:
        return tr("Single-quoted here document");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerBash::defaultPaper(int style) const
{
    switch (style)
    {
    case Error:
        return QColor(0xff,0x00,0x00);

    case Scalar:
        return QColor(0xff,0xe0,0xe0);

    case ParameterExpansion:
        return QColor(0xff,0xff,0xe0);

    case Backticks:
        return QColor(0xa0,0x80,0x80);

    case HereDocumentDelimiter:
    case SingleQuotedHereDocument:
        return QColor(0xdd,0xd0,0xdd);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerBash::refreshProperties()
{
    setCommentProp();
    setCompactProp();
}


// Read properties from the settings.
bool QsciLexerBash::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerBash::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerBash::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerBash::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerBash::setCommentProp()
{
    emit propertyChanged("fold.comment", (fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerBash::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerBash::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerBash::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}
