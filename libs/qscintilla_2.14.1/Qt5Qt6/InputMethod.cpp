// Copyright (c) 2021 Riverbank Computing Limited
// Copyright (c) 2011 Archaeopteryx Software, Inc.
// Copyright (c) 1990-2011, Scientific Toolworks, Inc.
//
// The License.txt file describes the conditions under which this software may
// be distributed.


#include <qglobal.h>

#include <QColor>
#include <QFont>
#include <QInputMethodEvent>
#include <QRect>
#include <QTextCharFormat>
#include <QTextFormat>
#include <QVariant>
#include <QVarLengthArray>

#include "Qsci/qsciscintillabase.h"
#include "ScintillaQt.h"


#define INDIC_INPUTMETHOD 24

#define MAXLENINPUTIME 200
#define SC_INDICATOR_INPUT INDIC_IME
#define SC_INDICATOR_TARGET INDIC_IME+1
#define SC_INDICATOR_CONVERTED INDIC_IME+2
#define SC_INDICATOR_UNKNOWN INDIC_IME_MAX


static bool IsHangul(const QChar qchar)
{
    int unicode = (int)qchar.unicode();
    // Korean character ranges used for preedit chars.
    // http://www.programminginkorean.com/programming/hangul-in-unicode/
    const bool HangulJamo = (0x1100 <= unicode && unicode <= 0x11FF);
    const bool HangulCompatibleJamo = (0x3130 <= unicode && unicode <= 0x318F);
    const bool HangulJamoExtendedA = (0xA960 <= unicode && unicode <= 0xA97F);
    const bool HangulJamoExtendedB = (0xD7B0 <= unicode && unicode <= 0xD7FF);
    const bool HangulSyllable = (0xAC00 <= unicode && unicode <= 0xD7A3);
    return HangulJamo || HangulCompatibleJamo  || HangulSyllable ||
                HangulJamoExtendedA || HangulJamoExtendedB;
}

static void MoveImeCarets(QsciScintillaQt *sqt, int offset)
{
    // Move carets relatively by bytes
    for (size_t r=0; r < sqt->sel.Count(); r++) {
        int positionInsert = sqt->sel.Range(r).Start().Position();
        sqt->sel.Range(r).caret.SetPosition(positionInsert + offset);
        sqt->sel.Range(r).anchor.SetPosition(positionInsert + offset);
    }
}

static void DrawImeIndicator(QsciScintillaQt *sqt, int indicator, int len)
{
    // Emulate the visual style of IME characters with indicators.
    // Draw an indicator on the character before caret by the character bytes of len
    // so it should be called after AddCharUTF().
    // It does not affect caret positions.
    if (indicator < 8 || indicator > INDIC_MAX) {
        return;
    }
    sqt->pdoc->DecorationSetCurrentIndicator(indicator);
    for (size_t r=0; r< sqt-> sel.Count(); r++) {
        int positionInsert = sqt->sel.Range(r).Start().Position();
        sqt->pdoc->DecorationFillRange(positionInsert - len, 1, len);
    }
}

static int GetImeCaretPos(QInputMethodEvent *event)
{
    foreach (QInputMethodEvent::Attribute attr, event->attributes()) {
        if (attr.type == QInputMethodEvent::Cursor)
            return attr.start;
    }
    return 0;
}

static std::vector<int> MapImeIndicators(QInputMethodEvent *event)
{
    std::vector<int> imeIndicator(event->preeditString().size(), SC_INDICATOR_UNKNOWN);
    foreach (QInputMethodEvent::Attribute attr, event->attributes()) {
        if (attr.type == QInputMethodEvent::TextFormat) {
            QTextFormat format = attr.value.value<QTextFormat>();
            QTextCharFormat charFormat = format.toCharFormat();

            int indicator = SC_INDICATOR_UNKNOWN;
            switch (charFormat.underlineStyle()) {
                case QTextCharFormat::NoUnderline: // win32, linux
                    indicator = SC_INDICATOR_TARGET;
                    break;
                case QTextCharFormat::SingleUnderline: // osx
                case QTextCharFormat::DashUnderline: // win32, linux
                    indicator = SC_INDICATOR_INPUT;
                    break;
                case QTextCharFormat::DotLine:
                case QTextCharFormat::DashDotLine:
                case QTextCharFormat::WaveUnderline:
                case QTextCharFormat::SpellCheckUnderline:
                    indicator = SC_INDICATOR_CONVERTED;
                    break;

                default:
                    indicator = SC_INDICATOR_UNKNOWN;
            }

            if (format.hasProperty(QTextFormat::BackgroundBrush)) // win32, linux
                indicator = SC_INDICATOR_TARGET;

#ifdef Q_OS_OSX
            if (charFormat.underlineStyle() == QTextCharFormat::SingleUnderline) {
                QColor uc = charFormat.underlineColor();
                if (uc.lightness() < 2) { // osx
                    indicator = SC_INDICATOR_TARGET;
                }
            }
#endif

            for (int i = attr.start; i < attr.start+attr.length; i++) {
                imeIndicator[i] = indicator;
            }
        }
    }
    return imeIndicator;
}

void QsciScintillaBase::inputMethodEvent(QInputMethodEvent *event)
{
    // Copy & paste by johnsonj with a lot of helps of Neil
    // Great thanks for my forerunners, jiniya and BLUEnLIVE

    if (sci->pdoc->IsReadOnly() || sci->SelectionContainsProtected()) {
        // Here, a canceling and/or completing composition function is needed.
        return;
    }

    bool initialCompose = false;
    if (sci->pdoc->TentativeActive()) {
        sci->pdoc->TentativeUndo();
    } else {
        // No tentative undo means start of this composition so
        // Fill in any virtual spaces.
        initialCompose = true;
    }

    sci->view.imeCaretBlockOverride = false;

    if (!event->commitString().isEmpty()) {
        const QString commitStr = event->commitString();
        const int commitStrLen = commitStr.length();

        for (int i = 0; i < commitStrLen;) {
            const int ucWidth = commitStr.at(i).isHighSurrogate() ? 2 : 1;
            const QString oneCharUTF16 = commitStr.mid(i, ucWidth);
            const QByteArray oneChar = textAsBytes(oneCharUTF16);
            const int oneCharLen = oneChar.length();

            sci->AddCharUTF(oneChar.data(), oneChar.length());
            i += ucWidth;
        }

    } else if (!event->preeditString().isEmpty()) {
        const QString preeditStr = event->preeditString();
        const int preeditStrLen = preeditStr.length();
        if (preeditStrLen == 0) {
            sci->ShowCaretAtCurrentPosition();
            return;
        }

        if (initialCompose)
            sci->ClearBeforeTentativeStart();
        sci->pdoc->TentativeStart(); // TentativeActive() from now on.

        std::vector<int> imeIndicator = MapImeIndicators(event);

        for (unsigned int i = 0; i < preeditStrLen;) {
            const unsigned int ucWidth = preeditStr.at(i).isHighSurrogate() ? 2 : 1;
            const QString oneCharUTF16 = preeditStr.mid(i, ucWidth);
            const QByteArray oneChar = textAsBytes(oneCharUTF16);
            const int oneCharLen = oneChar.length();

            sci->AddCharUTF(oneChar.data(), oneCharLen);

            DrawImeIndicator(sci, imeIndicator[i], oneCharLen);
            i += ucWidth;
        }

        // Move IME carets.
        int imeCaretPos = GetImeCaretPos(event);
        int imeEndToImeCaretU16 = imeCaretPos - preeditStrLen;
        int imeCaretPosDoc = sci->pdoc->GetRelativePositionUTF16(sci->CurrentPosition(), imeEndToImeCaretU16);

        MoveImeCarets(sci, - sci->CurrentPosition() + imeCaretPosDoc);

        if (IsHangul(preeditStr.at(0))) {
#ifndef Q_OS_WIN
            if (imeCaretPos > 0) {
                int oneCharBefore = sci->pdoc->GetRelativePosition(sci->CurrentPosition(), -1);
                MoveImeCarets(sci, - sci->CurrentPosition() + oneCharBefore);
            }
#endif
            sci->view.imeCaretBlockOverride = true;
        }

        // Set candidate box position for Qt::ImCursorRectangle.
        preeditPos = sci->CurrentPosition();
        sci->EnsureCaretVisible();
        updateMicroFocus();
    }
    sci->ShowCaretAtCurrentPosition();
}

QVariant QsciScintillaBase::inputMethodQuery(Qt::InputMethodQuery query) const
{
    int pos = SendScintilla(SCI_GETCURRENTPOS);
    int line = SendScintilla(SCI_LINEFROMPOSITION, pos);

    switch (query) {
        case Qt::ImHints:
            return QWidget::inputMethodQuery(query);

        case Qt::ImCursorRectangle:
        {
            int startPos = (preeditPos >= 0) ? preeditPos : pos;
            Scintilla::Point pt = sci->LocationFromPosition(startPos);
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

            SendScintilla(SCI_GETTEXTRANGE, paraStart, paraEnd, buffer.data());

            return bytesAsText(buffer.constData(), buffer.size());
        }

        case Qt::ImCurrentSelection:
        {
            QVarLengthArray<char,1024> buffer(SendScintilla(SCI_GETSELTEXT) + 1);
            SendScintilla(SCI_GETSELTEXT, 0, (sptr_t)buffer.data());

            return bytesAsText(buffer.constData(), buffer.size() - 1);
        }

        default:
            return QVariant();
    }
}
