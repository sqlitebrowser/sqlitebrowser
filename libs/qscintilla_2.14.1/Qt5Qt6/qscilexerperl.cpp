// This module implements the QsciLexerPerl class.
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


#include "Qsci/qscilexerperl.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerPerl::QsciLexerPerl(QObject *parent)
    : QsciLexer(parent),
      fold_atelse(false), fold_comments(false), fold_compact(true),
      fold_packages(true), fold_pod_blocks(true)
{
}


// The dtor.
QsciLexerPerl::~QsciLexerPerl()
{
}


// Returns the language name.
const char *QsciLexerPerl::language() const
{
    return "Perl";
}


// Returns the lexer name.
const char *QsciLexerPerl::lexer() const
{
    return "perl";
}


// Return the set of character sequences that can separate auto-completion
// words.
QStringList QsciLexerPerl::autoCompletionWordSeparators() const
{
    QStringList wl;

    wl << "::" << "->";

    return wl;
}


// Return the list of characters that can start a block.
const char *QsciLexerPerl::blockStart(int *style) const
{
    if (style)
        *style = Operator;

    return "{";
}


// Return the list of characters that can end a block.
const char *QsciLexerPerl::blockEnd(int *style) const
{
    if (style)
        *style = Operator;

    return "}";
}


// Return the style used for braces.
int QsciLexerPerl::braceStyle() const
{
    return Operator;
}


// Return the string of characters that comprise a word.
const char *QsciLexerPerl::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_$@%&";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerPerl::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Error:
    case Backticks:
    case QuotedStringQX:
        return QColor(0xff,0xff,0x00);

    case Comment:
        return QColor(0x00,0x7f,0x00);

    case POD:
    case PODVerbatim:
        return QColor(0x00,0x40,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case Keyword:
        return QColor(0x00,0x00,0x7f);

    case DoubleQuotedString:
    case SingleQuotedString:
    case SingleQuotedHereDocument:
    case DoubleQuotedHereDocument:
    case BacktickHereDocument:
    case QuotedStringQ:
    case QuotedStringQQ:
        return QColor(0x7f,0x00,0x7f);

    case Operator:
    case Identifier:
    case Scalar:
    case Array:
    case Hash:
    case SymbolTable:
    case Regex:
    case Substitution:
    case HereDocumentDelimiter:
    case QuotedStringQR:
    case QuotedStringQW:
    case SubroutinePrototype:
    case Translation:
        return QColor(0x00,0x00,0x00);

    case DataSection:
        return QColor(0x60,0x00,0x00);

    case FormatIdentifier:
    case FormatBody:
        return QColor(0xc0,0x00,0xc0);

    case DoubleQuotedStringVar:
    case RegexVar:
    case SubstitutionVar:
    case BackticksVar:
    case DoubleQuotedHereDocumentVar:
    case BacktickHereDocumentVar:
    case QuotedStringQQVar:
    case QuotedStringQXVar:
    case QuotedStringQRVar:
        return QColor(0xd0, 0x00, 0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerPerl::defaultEolFill(int style) const
{
    switch (style)
    {
    case POD:
    case DataSection:
    case SingleQuotedHereDocument:
    case DoubleQuotedHereDocument:
    case BacktickHereDocument:
    case PODVerbatim:
    case FormatBody:
    case DoubleQuotedHereDocumentVar:
    case BacktickHereDocumentVar:
        return true;
    }

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerPerl::defaultFont(int style) const
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

    case POD:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman",11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter",10);
#endif
        break;

    case Keyword:
    case Operator:
    case DoubleQuotedHereDocument:
    case FormatIdentifier:
    case RegexVar:
    case SubstitutionVar:
    case BackticksVar:
    case DoubleQuotedHereDocumentVar:
    case BacktickHereDocumentVar:
    case QuotedStringQXVar:
    case QuotedStringQRVar:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DoubleQuotedString:
    case SingleQuotedString:
    case QuotedStringQQ:
    case PODVerbatim:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        break;

    case BacktickHereDocument:
    case SubroutinePrototype:
        f = QsciLexer::defaultFont(style);
        f.setItalic(true);
        break;

    case DoubleQuotedStringVar:
    case QuotedStringQQVar:
#if defined(Q_OS_WIN)
        f = QFont("Courier New",10);
#elif defined(Q_OS_MAC)
        f = QFont("Courier", 12);
#else
        f = QFont("Bitstream Vera Sans Mono",9);
#endif
        f.setBold(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerPerl::keywords(int set) const
{
    if (set == 1)
        return
            "NULL __FILE__ __LINE__ __PACKAGE__ __DATA__ __END__ "
            "AUTOLOAD BEGIN CORE DESTROY END EQ GE GT INIT LE LT "
            "NE CHECK abs accept alarm and atan2 bind binmode "
            "bless caller chdir chmod chomp chop chown chr chroot "
            "close closedir cmp connect continue cos crypt "
            "dbmclose dbmopen defined delete die do dump each "
            "else elsif endgrent endhostent endnetent endprotoent "
            "endpwent endservent eof eq eval exec exists exit exp "
            "fcntl fileno flock for foreach fork format formline "
            "ge getc getgrent getgrgid getgrnam gethostbyaddr "
            "gethostbyname gethostent getlogin getnetbyaddr "
            "getnetbyname getnetent getpeername getpgrp getppid "
            "getpriority getprotobyname getprotobynumber "
            "getprotoent getpwent getpwnam getpwuid getservbyname "
            "getservbyport getservent getsockname getsockopt glob "
            "gmtime goto grep gt hex if index int ioctl join keys "
            "kill last lc lcfirst le length link listen local "
            "localtime lock log lstat lt m map mkdir msgctl "
            "msgget msgrcv msgsnd my ne next no not oct open "
            "opendir or ord our pack package pipe pop pos print "
            "printf prototype push q qq qr quotemeta qu qw qx "
            "rand read readdir readline readlink readpipe recv "
            "redo ref rename require reset return reverse "
            "rewinddir rindex rmdir s scalar seek seekdir select "
            "semctl semget semop send setgrent sethostent "
            "setnetent setpgrp setpriority setprotoent setpwent "
            "setservent setsockopt shift shmctl shmget shmread "
            "shmwrite shutdown sin sleep socket socketpair sort "
            "splice split sprintf sqrt srand stat study sub "
            "substr symlink syscall sysopen sysread sysseek "
            "system syswrite tell telldir tie tied time times tr "
            "truncate uc ucfirst umask undef unless unlink unpack "
            "unshift untie until use utime values vec wait "
            "waitpid wantarray warn while write x xor y";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerPerl::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Error:
        return tr("Error");

    case Comment:
        return tr("Comment");

    case POD:
        return tr("POD");

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

    case Array:
        return tr("Array");

    case Hash:
        return tr("Hash");

    case SymbolTable:
        return tr("Symbol table");

    case Regex:
        return tr("Regular expression");

    case Substitution:
        return tr("Substitution");

    case Backticks:
        return tr("Backticks");

    case DataSection:
        return tr("Data section");

    case HereDocumentDelimiter:
        return tr("Here document delimiter");

    case SingleQuotedHereDocument:
        return tr("Single-quoted here document");

    case DoubleQuotedHereDocument:
        return tr("Double-quoted here document");

    case BacktickHereDocument:
        return tr("Backtick here document");

    case QuotedStringQ:
        return tr("Quoted string (q)");

    case QuotedStringQQ:
        return tr("Quoted string (qq)");

    case QuotedStringQX:
        return tr("Quoted string (qx)");

    case QuotedStringQR:
        return tr("Quoted string (qr)");

    case QuotedStringQW:
        return tr("Quoted string (qw)");

    case PODVerbatim:
        return tr("POD verbatim");

    case SubroutinePrototype:
        return tr("Subroutine prototype");

    case FormatIdentifier:
        return tr("Format identifier");

    case FormatBody:
        return tr("Format body");

    case DoubleQuotedStringVar:
        return tr("Double-quoted string (interpolated variable)");

    case Translation:
        return tr("Translation");

    case RegexVar:
        return tr("Regular expression (interpolated variable)");

    case SubstitutionVar:
        return tr("Substitution (interpolated variable)");

    case BackticksVar:
        return tr("Backticks (interpolated variable)");

    case DoubleQuotedHereDocumentVar:
        return tr("Double-quoted here document (interpolated variable)");

    case BacktickHereDocumentVar:
        return tr("Backtick here document (interpolated variable)");

    case QuotedStringQQVar:
        return tr("Quoted string (qq, interpolated variable)");

    case QuotedStringQXVar:
        return tr("Quoted string (qx, interpolated variable)");

    case QuotedStringQRVar:
        return tr("Quoted string (qr, interpolated variable)");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerPerl::defaultPaper(int style) const
{
    switch (style)
    {
    case Error:
        return QColor(0xff,0x00,0x00);

    case POD:
        return QColor(0xe0,0xff,0xe0);

    case Scalar:
        return QColor(0xff,0xe0,0xe0);

    case Array:
        return QColor(0xff,0xff,0xe0);

    case Hash:
        return QColor(0xff,0xe0,0xff);

    case SymbolTable:
        return QColor(0xe0,0xe0,0xe0);

    case Regex:
        return QColor(0xa0,0xff,0xa0);

    case Substitution:
    case Translation:
        return QColor(0xf0,0xe0,0x80);

    case Backticks:
    case BackticksVar:
    case QuotedStringQXVar:
        return QColor(0xa0,0x80,0x80);

    case DataSection:
        return QColor(0xff,0xf0,0xd8);

    case HereDocumentDelimiter:
    case SingleQuotedHereDocument:
    case DoubleQuotedHereDocument:
    case BacktickHereDocument:
    case DoubleQuotedHereDocumentVar:
    case BacktickHereDocumentVar:
        return QColor(0xdd,0xd0,0xdd);

    case PODVerbatim:
        return QColor(0xc0,0xff,0xc0);

    case FormatBody:
        return QColor(0xff,0xf0,0xff);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerPerl::refreshProperties()
{
    setAtElseProp();
    setCommentProp();
    setCompactProp();
    setPackagesProp();
    setPODBlocksProp();
}


// Read properties from the settings.
bool QsciLexerPerl::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_atelse = qs.value(prefix + "foldatelse", false).toBool();
    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    fold_packages = qs.value(prefix + "foldpackages", true).toBool();
    fold_pod_blocks = qs.value(prefix + "foldpodblocks", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerPerl::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldatelse", fold_atelse);
    qs.setValue(prefix + "foldcomments", fold_comments);
    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "foldpackages", fold_packages);
    qs.setValue(prefix + "foldpodblocks", fold_pod_blocks);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerPerl::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerPerl::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerPerl::setCommentProp()
{
    emit propertyChanged("fold.comment",(fold_comments ? "1" : "0"));
}


// Return true if folds are compact.
bool QsciLexerPerl::foldCompact() const
{
    return fold_compact;
}


// Set if folds are compact
void QsciLexerPerl::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerPerl::setCompactProp()
{
    emit propertyChanged("fold.compact",(fold_compact ? "1" : "0"));
}


// Return true if packages can be folded.
bool QsciLexerPerl::foldPackages() const
{
    return fold_packages;
}


// Set if packages can be folded.
void QsciLexerPerl::setFoldPackages(bool fold)
{
    fold_packages = fold;

    setPackagesProp();
}


// Set the "fold.perl.package" property.
void QsciLexerPerl::setPackagesProp()
{
    emit propertyChanged("fold.perl.package",(fold_packages ? "1" : "0"));
}


// Return true if POD blocks can be folded.
bool QsciLexerPerl::foldPODBlocks() const
{
    return fold_pod_blocks;
}


// Set if POD blocks can be folded.
void QsciLexerPerl::setFoldPODBlocks(bool fold)
{
    fold_pod_blocks = fold;

    setPODBlocksProp();
}


// Set the "fold.perl.pod" property.
void QsciLexerPerl::setPODBlocksProp()
{
    emit propertyChanged("fold.perl.pod",(fold_pod_blocks ? "1" : "0"));
}


// Set if else can be folded.
void QsciLexerPerl::setFoldAtElse(bool fold)
{
    fold_atelse = fold;

    setAtElseProp();
}


// Set the "fold.perl.at.else" property.
void QsciLexerPerl::setAtElseProp()
{
    emit propertyChanged("fold.perl.at.else",(fold_atelse ? "1" : "0"));
}
