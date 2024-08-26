// This module implements the QsciLexerYAML class.
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


#include "Qsci/qscilexeryaml.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerYAML::QsciLexerYAML(QObject *parent)
    : QsciLexer(parent), fold_comments(false)
{
}


// The dtor.
QsciLexerYAML::~QsciLexerYAML()
{
}


// Returns the language name.
const char *QsciLexerYAML::language() const
{
    return "YAML";
}


// Returns the lexer name.
const char *QsciLexerYAML::lexer() const
{
    return "yaml";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerYAML::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x00,0x00,0x00);

    case Comment:
        return QColor(0x00,0x88,0x00);

    case Identifier:
        return QColor(0x00,0x00,0x88);

    case Keyword:
        return QColor(0x88,0x00,0x88);

    case Number:
        return QColor(0x88,0x00,0x00);

    case Reference:
        return QColor(0x00,0x88,0x88);

    case DocumentDelimiter:
    case SyntaxErrorMarker:
        return QColor(0xff,0xff,0xff);

    case TextBlockMarker:
        return QColor(0x33,0x33,0x66);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerYAML::defaultEolFill(int style) const
{
    if (style == DocumentDelimiter || style == SyntaxErrorMarker)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerYAML::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Default:
    case TextBlockMarker:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        break;

    case Identifier:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case DocumentDelimiter:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
        f.setBold(true);
        break;

    case SyntaxErrorMarker:
#if defined(Q_OS_WIN)
        f = QFont("Times New Roman", 11);
#elif defined(Q_OS_MAC)
        f = QFont("Times New Roman", 12);
#else
        f = QFont("Bitstream Charter", 10);
#endif
        f.setBold(true);
        f.setItalic(true);
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerYAML::keywords(int set) const
{
    if (set == 1)
        return "true false yes no";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerYAML::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Identifier:
        return tr("Identifier");

    case Keyword:
        return tr("Keyword");

    case Number:
        return tr("Number");

    case Reference:
        return tr("Reference");

    case DocumentDelimiter:
        return tr("Document delimiter");

    case TextBlockMarker:
        return tr("Text block marker");

    case SyntaxErrorMarker:
        return tr("Syntax error marker");

    case Operator:
        return tr("Operator");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerYAML::defaultPaper(int style) const
{
    switch (style)
    {
    case DocumentDelimiter:
        return QColor(0x00,0x00,0x88);

    case SyntaxErrorMarker:
        return QColor(0xff,0x00,0x00);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerYAML::refreshProperties()
{
    setCommentProp();
}


// Read properties from the settings.
bool QsciLexerYAML::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerYAML::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);

    return rc;
}


// Return true if comments can be folded.
bool QsciLexerYAML::foldComments() const
{
    return fold_comments;
}


// Set if comments can be folded.
void QsciLexerYAML::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment.yaml" property.
void QsciLexerYAML::setCommentProp()
{
    emit propertyChanged("fold.comment.yaml",(fold_comments ? "1" : "0"));
}
