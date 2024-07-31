// This module implements the QsciCommandSet class.
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


#include "Qsci/qscicommandset.h"

#include <QSettings>

#include "Qsci/qscicommand.h"
#include "Qsci/qsciscintilla.h"
#include "Qsci/qsciscintillabase.h"


// Starting with QScintilla v2.7 the standard OS/X keyboard shortcuts are used
// where possible.  In order to restore the behaviour of earlier versions then
// #define DONT_USE_OSX_KEYS here or add it to the qmake project (.pro) file.
#if defined(Q_OS_MAC) && !defined(DONT_USE_OSX_KEYS)
#define USING_OSX_KEYS
#else
#undef  USING_OSX_KEYS
#endif


// The ctor.
QsciCommandSet::QsciCommandSet(QsciScintilla *qs) : qsci(qs)
{
    struct sci_cmd {
        QsciCommand::Command cmd;
        int key;
        int altkey;
        const char *desc;
    };

    static struct sci_cmd cmd_table[] = {
        {
            QsciCommand::LineDown,
            Qt::Key_Down,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Move down one line")
        },
        {
            QsciCommand::LineDownExtend,
            Qt::Key_Down | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection down one line")
        },
        {
            QsciCommand::LineDownRectExtend,
            Qt::Key_Down | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_N | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection down one line")
        },
        {
            QsciCommand::LineScrollDown,
            Qt::Key_Down | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Scroll view down one line")
        },
        {
            QsciCommand::LineUp,
            Qt::Key_Up,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Move up one line")
        },
        {
            QsciCommand::LineUpExtend,
            Qt::Key_Up | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection up one line")
        },
        {
            QsciCommand::LineUpRectExtend,
            Qt::Key_Up | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_P | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection up one line")
        },
        {
            QsciCommand::LineScrollUp,
            Qt::Key_Up | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Scroll view up one line")
        },
        {
            QsciCommand::ScrollToStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Home,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Scroll to start of document")
        },
        {
            QsciCommand::ScrollToEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_End,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Scroll to end of document")
        },
        {
            QsciCommand::VerticalCentreCaret,
#if defined(USING_OSX_KEYS)
            Qt::Key_L | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Scroll vertically to centre current line")
        },
        {
            QsciCommand::ParaDown,
            Qt::Key_BracketRight | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move down one paragraph")
        },
        {
            QsciCommand::ParaDownExtend,
            Qt::Key_BracketRight | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection down one paragraph")
        },
        {
            QsciCommand::ParaUp,
            Qt::Key_BracketLeft | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move up one paragraph")
        },
        {
            QsciCommand::ParaUpExtend,
            Qt::Key_BracketLeft | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection up one paragraph")
        },
        {
            QsciCommand::CharLeft,
            Qt::Key_Left,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Move left one character")
        },
        {
            QsciCommand::CharLeftExtend,
            Qt::Key_Left | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection left one character")
        },
        {
            QsciCommand::CharLeftRectExtend,
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_B | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection left one character")
        },
        {
            QsciCommand::CharRight,
            Qt::Key_Right,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Move right one character")
        },
        {
            QsciCommand::CharRightExtend,
            Qt::Key_Right | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection right one character")
        },
        {
            QsciCommand::CharRightRectExtend,
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_F | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection right one character")
        },
        {
            QsciCommand::WordLeft,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT,
#else
            Qt::Key_Left | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move left one word")
        },
        {
            QsciCommand::WordLeftExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection left one word")
        },
        {
            QsciCommand::WordRight,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Right | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move right one word")
        },
        {
            QsciCommand::WordRightExtend,
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection right one word")
        },
        {
            QsciCommand::WordLeftEnd,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to end of previous word")
        },
        {
            QsciCommand::WordLeftEndExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of previous word")
        },
        {
            QsciCommand::WordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to end of next word")
        },
        {
            QsciCommand::WordRightEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of next word")
        },
        {
            QsciCommand::WordPartLeft,
            Qt::Key_Slash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move left one word part")
        },
        {
            QsciCommand::WordPartLeftExtend,
            Qt::Key_Slash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection left one word part")
        },
        {
            QsciCommand::WordPartRight,
            Qt::Key_Backslash | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move right one word part")
        },
        {
            QsciCommand::WordPartRightExtend,
            Qt::Key_Backslash | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection right one word part")
        },
        {
            QsciCommand::Home,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to start of document line")
        },
        {
            QsciCommand::HomeExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to start of document line")
        },
        {
            QsciCommand::HomeRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_A | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection to start of document line")
        },
        {
            QsciCommand::HomeDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL,
#else
            Qt::Key_Home | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to start of display line")
        },
        {
            QsciCommand::HomeDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Left | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to start of display line")
        },
        {
            QsciCommand::HomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Move to start of display or document line")
        },
        {
            QsciCommand::HomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to start of display or document line")
        },
        {
            QsciCommand::VCHome,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                "Move to first visible character in document line")
        },
        {
            QsciCommand::VCHomeExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                "Extend selection to first visible character in document line")
        },
        {
            QsciCommand::VCHomeRectExtend,
#if defined(USING_OSX_KEYS)
            0,
#else
            Qt::Key_Home | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                "Extend rectangular selection to first visible character in document line")
        },
        {
            QsciCommand::VCHomeWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Move to first visible character of display in document line")
        },
        {
            QsciCommand::VCHomeWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to first visible character in display or document line")
        },
        {
            QsciCommand::LineEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META,
#else
            Qt::Key_End,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to end of document line")
        },
        {
            QsciCommand::LineEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::SHIFT,
#else
            Qt::Key_End | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of document line")
        },
        {
            QsciCommand::LineEndRectExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_E | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            Qt::Key_End | Qt::ALT | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection to end of document line")
        },
        {
            QsciCommand::LineEndDisplay,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL,
#else
            Qt::Key_End | Qt::ALT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to end of display line")
        },
        {
            QsciCommand::LineEndDisplayExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Right | Qt::CTRL | Qt::SHIFT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of display line")
        },
        {
            QsciCommand::LineEndWrap,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Move to end of display or document line")
        },
        {
            QsciCommand::LineEndWrapExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of display or document line")
        },
        {
            QsciCommand::DocumentStart,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL,
#else
            Qt::Key_Home | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to start of document")
        },
        {
            QsciCommand::DocumentStartExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Up | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Home | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to start of document")
        },
        {
            QsciCommand::DocumentEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL,
#else
            Qt::Key_End | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move to end of document")
        },
        {
            QsciCommand::DocumentEndExtend,
#if defined(USING_OSX_KEYS)
            Qt::Key_Down | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_End | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend selection to end of document")
        },
        {
            QsciCommand::PageUp,
            Qt::Key_PageUp,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move up one page")
        },
        {
            QsciCommand::PageUpExtend,
            Qt::Key_PageUp | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection up one page")
        },
        {
            QsciCommand::PageUpRectExtend,
            Qt::Key_PageUp | Qt::ALT | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection up one page")
        },
        {
            QsciCommand::PageDown,
            Qt::Key_PageDown,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Move down one page")
        },
        {
            QsciCommand::PageDownExtend,
            Qt::Key_PageDown | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Extend selection down one page")
        },
        {
            QsciCommand::PageDownRectExtend,
            Qt::Key_PageDown | Qt::ALT | Qt::SHIFT,
#if defined(USING_OSX_KEYS)
            Qt::Key_V | Qt::META | Qt::ALT | Qt::SHIFT,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Extend rectangular selection down one page")
        },
        {
            QsciCommand::StutteredPageUp,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Stuttered move up one page")
        },
        {
            QsciCommand::StutteredPageUpExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Stuttered extend selection up one page")
        },
        {
            QsciCommand::StutteredPageDown,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Stuttered move down one page")
        },
        {
            QsciCommand::StutteredPageDownExtend,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Stuttered extend selection down one page")
        },
        {
            QsciCommand::Delete,
            Qt::Key_Delete,
#if defined(USING_OSX_KEYS)
            Qt::Key_D | Qt::META,
#else
            0,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Delete current character")
        },
        {
            QsciCommand::DeleteBack,
            Qt::Key_Backspace,
#if defined(USING_OSX_KEYS)
            Qt::Key_H | Qt::META,
#else
            Qt::Key_Backspace | Qt::SHIFT,
#endif
            QT_TRANSLATE_NOOP("QsciCommand", "Delete previous character")
        },
        {
            QsciCommand::DeleteBackNotLine,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                "Delete previous character if not at start of line")
        },
        {
            QsciCommand::DeleteWordLeft,
            Qt::Key_Backspace | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Delete word to left")
        },
        {
            QsciCommand::DeleteWordRight,
            Qt::Key_Delete | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Delete word to right")
        },
        {
            QsciCommand::DeleteWordRightEnd,
#if defined(USING_OSX_KEYS)
            Qt::Key_Delete | Qt::ALT,
#else
            0,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Delete right to end of next word")
        },
        {
            QsciCommand::DeleteLineLeft,
            Qt::Key_Backspace | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Delete line to left")
        },
        {
            QsciCommand::DeleteLineRight,
#if defined(USING_OSX_KEYS)
            Qt::Key_K | Qt::META,
#else
            Qt::Key_Delete | Qt::CTRL | Qt::SHIFT,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Delete line to right")
        },
        {
            QsciCommand::LineDelete,
            Qt::Key_L | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Delete current line")
        },
        {
            QsciCommand::LineCut,
            Qt::Key_L | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Cut current line")
        },
        {
            QsciCommand::LineCopy,
            Qt::Key_T | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Copy current line")
        },
        {
            QsciCommand::LineTranspose,
            Qt::Key_T | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Transpose current and previous lines")
        },
        {
            QsciCommand::LineDuplicate,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Duplicate the current line")
        },
        {
            QsciCommand::SelectAll,
            Qt::Key_A | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Select all")
        },
        {
            QsciCommand::MoveSelectedLinesUp,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Move selected lines up one line")
        },
        {
            QsciCommand::MoveSelectedLinesDown,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand",
                    "Move selected lines down one line")
        },
        {
            QsciCommand::SelectionDuplicate,
            Qt::Key_D | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Duplicate selection")
        },
        {
            QsciCommand::SelectionLowerCase,
            Qt::Key_U | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Convert selection to lower case")
        },
        {
            QsciCommand::SelectionUpperCase,
            Qt::Key_U | Qt::CTRL | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Convert selection to upper case")
        },
        {
            QsciCommand::SelectionCut,
            Qt::Key_X | Qt::CTRL,
            Qt::Key_Delete | Qt::SHIFT,
            QT_TRANSLATE_NOOP("QsciCommand", "Cut selection")
        },
        {
            QsciCommand::SelectionCopy,
            Qt::Key_C | Qt::CTRL,
            Qt::Key_Insert | Qt::CTRL,
            QT_TRANSLATE_NOOP("QsciCommand", "Copy selection")
        },
        {
            QsciCommand::Paste,
            Qt::Key_V | Qt::CTRL,
            Qt::Key_Insert | Qt::SHIFT,
            QT_TRANSLATE_NOOP("QsciCommand", "Paste")
        },
        {
            QsciCommand::EditToggleOvertype,
            Qt::Key_Insert,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Toggle insert/overtype")
        },
        {
            QsciCommand::Newline,
            Qt::Key_Return,
            Qt::Key_Return | Qt::SHIFT,
            QT_TRANSLATE_NOOP("QsciCommand", "Insert newline")
        },
        {
            QsciCommand::Formfeed,
            0,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Formfeed")
        },
        {
            QsciCommand::Tab,
            Qt::Key_Tab,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Indent one level")
        },
        {
            QsciCommand::Backtab,
            Qt::Key_Tab | Qt::SHIFT,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "De-indent one level")
        },
        {
            QsciCommand::Cancel,
            Qt::Key_Escape,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Cancel")
        },
        {
            QsciCommand::Undo,
            Qt::Key_Z | Qt::CTRL,
            Qt::Key_Backspace | Qt::ALT,
            QT_TRANSLATE_NOOP("QsciCommand", "Undo last command")
        },
        {
            QsciCommand::Redo,
#if defined(USING_OSX_KEYS)
            Qt::Key_Z | Qt::CTRL | Qt::SHIFT,
#else
            Qt::Key_Y | Qt::CTRL,
#endif
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Redo last command")
        },
        {
            QsciCommand::ZoomIn,
            Qt::Key_Plus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Zoom in")
        },
        {
            QsciCommand::ZoomOut,
            Qt::Key_Minus | Qt::CTRL,
            0,
            QT_TRANSLATE_NOOP("QsciCommand", "Zoom out")
        },
    };

    // Clear the default map.
    qsci->SendScintilla(QsciScintillaBase::SCI_CLEARALLCMDKEYS);

    // By default control characters don't do anything (rather than insert the
    // control character into the text).
    for (int k = 'A'; k <= 'Z'; ++k)
        qsci->SendScintilla(QsciScintillaBase::SCI_ASSIGNCMDKEY,
                k + (QsciScintillaBase::SCMOD_CTRL << 16),
                QsciScintillaBase::SCI_NULL);

    for (int i = 0; i < sizeof (cmd_table) / sizeof (cmd_table[0]); ++i)
        cmds.append(
                new QsciCommand(qsci, cmd_table[i].cmd, cmd_table[i].key,
                        cmd_table[i].altkey, cmd_table[i].desc));
}


// The dtor.
QsciCommandSet::~QsciCommandSet()
{
    for (int i = 0; i < cmds.count(); ++i)
        delete cmds.at(i);
}


// Read the command set from settings.
bool QsciCommandSet::readSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;

    for (int i = 0; i < cmds.count(); ++i)
    {
        QsciCommand *cmd = cmds.at(i);

        QString skey = QString("%1/keymap/c%2/").arg(prefix).arg(static_cast<int>(cmd->command()));

        int key;
        bool ok;

        // Read the key.
        ok = qs.contains(skey + "key");
        key = qs.value(skey + "key", 0).toInt();

        if (ok)
            cmd->setKey(key);
        else
            rc = false;

        // Read the alternate key.
        ok = qs.contains(skey + "alt");
        key = qs.value(skey + "alt", 0).toInt();

        if (ok)
            cmd->setAlternateKey(key);
        else
            rc = false;
    }

    return rc;
}


// Write the command set to settings.
bool QsciCommandSet::writeSettings(QSettings &qs, const char *prefix)
{
    bool rc = true;

    for (int i = 0; i < cmds.count(); ++i)
    {
        QsciCommand *cmd = cmds.at(i);

        QString skey = QString("%1/keymap/c%2/").arg(prefix).arg(static_cast<int>(cmd->command()));

        // Write the key.
        qs.setValue(skey + "key", cmd->key());

        // Write the alternate key.
        qs.setValue(skey + "alt", cmd->alternateKey());
    }

    return rc;
}


// Clear the key bindings.
void QsciCommandSet::clearKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setKey(0);
}


// Clear the alternate key bindings.
void QsciCommandSet::clearAlternateKeys()
{
    for (int i = 0; i < cmds.count(); ++i)
        cmds.at(i)->setAlternateKey(0);
}


// Find the command bound to a key.
QsciCommand *QsciCommandSet::boundTo(int key) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        QsciCommand *cmd = cmds.at(i);

        if (cmd->key() == key || cmd->alternateKey() == key)
            return cmd;
    }

    return 0;
}


// Find a command.
QsciCommand *QsciCommandSet::find(QsciCommand::Command command) const
{
    for (int i = 0; i < cmds.count(); ++i)
    {
        QsciCommand *cmd = cmds.at(i);

        if (cmd->command() == command)
            return cmd;
    }

    // This should never happen.
    return 0;
}
