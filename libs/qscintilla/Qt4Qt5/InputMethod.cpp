// Copyright (c) 2015 Riverbank Computing Limited
// Copyright (c) 2011 Archaeopteryx Software, Inc.
// Copyright (c) 1990-2011, Scientific Toolworks, Inc.
//
// The License.txt file describes the conditions under which this software may
// be distributed.


#include <qglobal.h>

#include "Qsci/qsciscintillabase.h"

#include <QColor>
#include <QFont>
#include <QInputMethodEvent>
#include <QRect>
#include <QTextCharFormat>
#include <QTextFormat>
#include <QVariant>
#include <QVarLengthArray>

#include "ScintillaQt.h"


#define INDIC_INPUTMETHOD 24


void QsciScintillaBase::inputMethodEvent(QInputMethodEvent *event)
{
    // Do nothing if it appears to be a non-event.  This can sometimes happen
    // (but why?) on losing focus.
    if (event->commitString().isEmpty() && event->preeditString().isEmpty() && event->replacementLength() == 0)
        return;

    ScintillaBytes bytes;

    // Clear the current selection.
    sci->ClearSelection();
    if (preeditPos >= 0)
        sci->SetSelection(preeditPos, preeditPos);

    // Insert the commit string.
    if (!event->commitString().isEmpty() || event->replacementLength()) {
        // Select the text to be removed.
        int commitPos = SendScintilla(SCI_GETCURRENTPOS);
        int start = commitPos + event->replacementStart();
        int end = start + event->replacementLength();
        sci->SetSelection(start, end);

        // Replace the selection with the commit string.
        bytes = textAsBytes(event->commitString());
        sci->AddCharUTF(bytes.data(), bytes.length());
    }

    // Select the previous preedit string.
    int pos = SendScintilla(SCI_GETCURRENTPOS);
    sci->SetSelection(pos, pos + preeditNrBytes);

    // Replace the selection with the new preedit string.
    bool recording = sci->recordingMacro;
    sci->recordingMacro = false;
    SendScintilla(SCI_SETUNDOCOLLECTION, false);
    bytes = textAsBytes(event->preeditString());
    sci->AddCharUTF(bytes.data(), bytes.length());
    SendScintilla(SCI_SETUNDOCOLLECTION, true);
    sci->recordingMacro = recording;
    sci->SetSelection(pos, pos);

    // Store the state of the current preedit string.
    preeditString = event->preeditString();
    preeditNrBytes = bytes.length();
    preeditPos = preeditNrBytes ? SendScintilla(SCI_GETCURRENTPOS) : -1;

    if (preeditNrBytes) {
        // Apply attributes to the preedit string.
        int indicNum = 0;
        sci->ShowCaretAtCurrentPosition();
        foreach (QInputMethodEvent::Attribute a, event->attributes()) {
            QString prefix = preeditString.left(a.start);
            int prefixLength = textAsBytes(prefix).length();
            int caretPos = preeditPos + prefixLength;

            if (a.type == QInputMethodEvent::Cursor) {
                sci->SetSelection(caretPos, caretPos);
                if (!a.length)
                    sci->DropCaret();

            } else if (a.type == QInputMethodEvent::TextFormat) {
                Q_ASSERT(a.value.canConvert(QVariant::TextFormat));
                QTextFormat format = a.value.value<QTextFormat>();
                Q_ASSERT(format.isCharFormat());
                QTextCharFormat charFormat = format.toCharFormat();

                if (charFormat.underlineStyle() != QTextCharFormat::NoUnderline) {
                    QString sub = preeditString.mid(a.start, a.length);
                    int subLength = textAsBytes(sub).length();

                    // Set temporary indicator for underline style.
                    QColor uc = charFormat.underlineColor();
                    int style = INDIC_COMPOSITIONTHICK;
                    if (charFormat.underlineStyle() == QTextCharFormat::DashUnderline)
                        style = INDIC_DASH;

                    SendScintilla(SCI_INDICSETSTYLE, INDIC_INPUTMETHOD + indicNum, style);
                    SendScintilla(SCI_INDICSETFORE, INDIC_INPUTMETHOD + indicNum, uc.rgb());
                    SendScintilla(SCI_SETINDICATORCURRENT, INDIC_INPUTMETHOD + indicNum);
                    SendScintilla(SCI_INDICATORFILLRANGE, caretPos, subLength);
                    indicNum++;
                }
            }
        }
    }
}

QVariant QsciScintillaBase::inputMethodQuery(Qt::InputMethodQuery query) const
{
    int pos = SendScintilla(SCI_GETCURRENTPOS);
    int line = SendScintilla(SCI_LINEFROMPOSITION, pos);

    switch (query) {
#if QT_VERSION >= 0x050000
        case Qt::ImHints:
            return QWidget::inputMethodQuery(query);
#endif

        case Qt::ImMicroFocus:
        {
            int startPos = (preeditPos >= 0) ? preeditPos : pos;
            Point pt = sci->LocationFromPosition(startPos);
            int width = SendScintilla(SCI_GETCARETWIDTH);
            int height = SendScintilla(SCI_TEXTHEIGHT, line);
            return QRect(pt.x, pt.y, width, height);
        }

        case Qt::ImFont:
        {
            char fontName[64];
            int style = SendScintilla(SCI_GETSTYLEAT, pos);
            int len = SendScintilla(SCI_STYLEGETFONT, style, (sptr_t)fontName);
            int size = SendScintilla(SCI_STYLEGETSIZE, style);
            bool italic = SendScintilla(SCI_STYLEGETITALIC, style);
            int weight = SendScintilla(SCI_STYLEGETBOLD, style) ? QFont::Bold : -1;
            return QFont(QString::fromUtf8(fontName, len), size, weight, italic);
        }

        case Qt::ImCursorPosition:
        {
            int paraStart = sci->pdoc->ParaUp(pos);
            return pos - paraStart;
        }

        case Qt::ImSurroundingText:
        {
            int paraStart = sci->pdoc->ParaUp(pos);
            int paraEnd = sci->pdoc->ParaDown(pos);
            QVarLengthArray<char,1024> buffer(paraEnd - paraStart + 1);

            Sci_CharacterRange charRange;
            charRange.cpMin = paraStart;
            charRange.cpMax = paraEnd;

            Sci_TextRange textRange;
            textRange.chrg = charRange;
            textRange.lpstrText = buffer.data();

            SendScintilla(SCI_GETTEXTRANGE, 0, (sptr_t)&textRange);

            return bytesAsText(buffer.constData());
        }

        case Qt::ImCurrentSelection:
        {
            QVarLengthArray<char,1024> buffer(SendScintilla(SCI_GETSELTEXT));
            SendScintilla(SCI_GETSELTEXT, 0, (sptr_t)buffer.data());

            return bytesAsText(buffer.constData());
        }

        default:
            return QVariant();
    }
}
