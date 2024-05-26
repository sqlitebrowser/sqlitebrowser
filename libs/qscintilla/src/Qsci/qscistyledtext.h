// This module defines interface to the QsciStyledText class.
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


#ifndef QSCISTYLEDTEXT_H
#define QSCISTYLEDTEXT_H

#include <qstring.h>

#include <Qsci/qsciglobal.h>


class QsciScintillaBase;
class QsciStyle;


//! \brief The QsciStyledText class is a container for a piece of text and the
//! style used to display the text.
class QSCINTILLA_EXPORT QsciStyledText
{
public:
    //! Constructs a QsciStyledText instance for text \a text and style number
    //! \a style.
    QsciStyledText(const QString &text, int style);

    //! Constructs a QsciStyledText instance for text \a text and style \a
    //! style.
    QsciStyledText(const QString &text, const QsciStyle &style);

    //! \internal Apply the style to a particular editor.
    void apply(QsciScintillaBase *sci) const;

    //! Returns a reference to the text.
    const QString &text() const {return styled_text;}

    //! Returns the number of the style.
    int style() const;

private:
    QString styled_text;
    int style_nr;
    const QsciStyle *explicit_style;
};

#endif
