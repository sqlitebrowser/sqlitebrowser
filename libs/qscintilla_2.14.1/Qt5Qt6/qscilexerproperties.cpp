// This module implements the QsciLexerProperties class.
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


#include "Qsci/qscilexerproperties.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerProperties::QsciLexerProperties(QObject *parent)
    : QsciLexer(parent), fold_compact(true), initial_spaces(true)
{
}


// The dtor.
QsciLexerProperties::~QsciLexerProperties()
{
}


// Returns the language name.
const char *QsciLexerProperties::language() const
{
    return "Properties";
}


// Returns the lexer name.
const char *QsciLexerProperties::lexer() const
{
    return "props";
}


// Return the string of characters that comprise a word.
const char *QsciLexerProperties::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerProperties::defaultColor(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0x00,0x7f,0x7f);

    case Section:
        return QColor(0x7f,0x00,0x7f);

    case Assignment:
        return QColor(0xb0,0x60,0x00);

    case DefaultValue:
        return QColor(0x7f,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerProperties::defaultEolFill(int style) const
{
    if (style == Section)
        return true;

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerProperties::defaultFont(int style) const
{
    QFont f;

    if (style == Comment)
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS",9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif",9);
#endif
    else
        f = QsciLexer::defaultFont(style);

    return f;
}


// Returns the user name of a style.
QString QsciLexerProperties::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case Section:
        return tr("Section");

    case Assignment:
        return tr("Assignment");

    case DefaultValue:
        return tr("Default value");

    case Key:
        return tr("Key");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerProperties::defaultPaper(int style) const
{
    if (style == Section)
        return QColor(0xe0,0xf0,0xf0);

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerProperties::refreshProperties()
{
    setCompactProp();
    setInitialSpacesProp();
}


// Read properties from the settings.
bool QsciLexerProperties::readProperties(QSettings &qs,const QString &prefix)
{
    int rc = true;

    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    initial_spaces = qs.value(prefix + "initialspaces", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerProperties::writeProperties(QSettings &qs,const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcompact", fold_compact);
    qs.setValue(prefix + "initialspaces", initial_spaces);

    return rc;
}


// Set if folds are compact
void QsciLexerProperties::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerProperties::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Set if initial spaces are allowed.
void QsciLexerProperties::setInitialSpaces(bool enable)
{
    initial_spaces = enable;

    setInitialSpacesProp();
}


// Set the "lexer.props.allow.initial.spaces" property.
void QsciLexerProperties::setInitialSpacesProp()
{
    emit propertyChanged("lexer.props.allow.initial.spaces", (fold_compact ? "1" : "0"));
}
