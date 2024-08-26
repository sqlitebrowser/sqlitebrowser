// This defines the interface to the QsciCommand class.
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


#ifndef QSCICOMMAND_H
#define QSCICOMMAND_H

#include <qstring.h>

#include <Qsci/qsciglobal.h>
#include <Qsci/qsciscintillabase.h>


class QsciScintilla;


//! \brief The QsciCommand class represents an internal editor command that may
//! have one or two keys bound to it.
//!
//! Methods are provided to change the keys bound to the command and to remove
//! a key binding.  Each command has a user friendly description of the command
//! for use in key mapping dialogs.
class QSCINTILLA_EXPORT QsciCommand
{
public:
    //! This enum defines the different commands that can be assigned to a key.
    enum Command {
        //! Move down one line.
        LineDown = QsciScintillaBase::SCI_LINEDOWN,

        //! Extend the selection down one line.
        LineDownExtend = QsciScintillaBase::SCI_LINEDOWNEXTEND,

        //! Extend the rectangular selection down one line.
        LineDownRectExtend = QsciScintillaBase::SCI_LINEDOWNRECTEXTEND,

        //! Scroll the view down one line.
        LineScrollDown = QsciScintillaBase::SCI_LINESCROLLDOWN,

        //! Move up one line.
        LineUp = QsciScintillaBase::SCI_LINEUP,

        //! Extend the selection up one line.
        LineUpExtend = QsciScintillaBase::SCI_LINEUPEXTEND,

        //! Extend the rectangular selection up one line.
        LineUpRectExtend = QsciScintillaBase::SCI_LINEUPRECTEXTEND,

        //! Scroll the view up one line.
        LineScrollUp = QsciScintillaBase::SCI_LINESCROLLUP,

        //! Scroll to the start of the document.
        ScrollToStart = QsciScintillaBase::SCI_SCROLLTOSTART,

        //! Scroll to the end of the document.
        ScrollToEnd = QsciScintillaBase::SCI_SCROLLTOEND,

        //! Scroll vertically to centre the current line.
        VerticalCentreCaret = QsciScintillaBase::SCI_VERTICALCENTRECARET,

        //! Move down one paragraph.
        ParaDown = QsciScintillaBase::SCI_PARADOWN,

        //! Extend the selection down one paragraph.
        ParaDownExtend = QsciScintillaBase::SCI_PARADOWNEXTEND,

        //! Move up one paragraph.
        ParaUp = QsciScintillaBase::SCI_PARAUP,

        //! Extend the selection up one paragraph.
        ParaUpExtend = QsciScintillaBase::SCI_PARAUPEXTEND,

        //! Move left one character.
        CharLeft = QsciScintillaBase::SCI_CHARLEFT,

        //! Extend the selection left one character.
        CharLeftExtend = QsciScintillaBase::SCI_CHARLEFTEXTEND,

        //! Extend the rectangular selection left one character.
        CharLeftRectExtend = QsciScintillaBase::SCI_CHARLEFTRECTEXTEND,

        //! Move right one character.
        CharRight = QsciScintillaBase::SCI_CHARRIGHT,

        //! Extend the selection right one character.
        CharRightExtend = QsciScintillaBase::SCI_CHARRIGHTEXTEND,

        //! Extend the rectangular selection right one character.
        CharRightRectExtend = QsciScintillaBase::SCI_CHARRIGHTRECTEXTEND,

        //! Move left one word.
        WordLeft = QsciScintillaBase::SCI_WORDLEFT,

        //! Extend the selection left one word.
        WordLeftExtend = QsciScintillaBase::SCI_WORDLEFTEXTEND,

        //! Move right one word.
        WordRight = QsciScintillaBase::SCI_WORDRIGHT,

        //! Extend the selection right one word.
        WordRightExtend = QsciScintillaBase::SCI_WORDRIGHTEXTEND,

        //! Move to the end of the previous word.
        WordLeftEnd = QsciScintillaBase::SCI_WORDLEFTEND,

        //! Extend the selection to the end of the previous word.
        WordLeftEndExtend = QsciScintillaBase::SCI_WORDLEFTENDEXTEND,

        //! Move to the end of the next word.
        WordRightEnd = QsciScintillaBase::SCI_WORDRIGHTEND,

        //! Extend the selection to the end of the next word.
        WordRightEndExtend = QsciScintillaBase::SCI_WORDRIGHTENDEXTEND,

        //! Move left one word part.
        WordPartLeft = QsciScintillaBase::SCI_WORDPARTLEFT,

        //! Extend the selection left one word part.
        WordPartLeftExtend = QsciScintillaBase::SCI_WORDPARTLEFTEXTEND,

        //! Move right one word part.
        WordPartRight = QsciScintillaBase::SCI_WORDPARTRIGHT,

        //! Extend the selection right one word part.
        WordPartRightExtend = QsciScintillaBase::SCI_WORDPARTRIGHTEXTEND,

        //! Move to the start of the document line.
        Home = QsciScintillaBase::SCI_HOME,

        //! Extend the selection to the start of the document line.
        HomeExtend = QsciScintillaBase::SCI_HOMEEXTEND,

        //! Extend the rectangular selection to the start of the document line.
        HomeRectExtend = QsciScintillaBase::SCI_HOMERECTEXTEND,

        //! Move to the start of the displayed line.
        HomeDisplay = QsciScintillaBase::SCI_HOMEDISPLAY,

        //! Extend the selection to the start of the displayed line.
        HomeDisplayExtend = QsciScintillaBase::SCI_HOMEDISPLAYEXTEND,

        //! Move to the start of the displayed or document line.
        HomeWrap = QsciScintillaBase::SCI_HOMEWRAP,

        //! Extend the selection to the start of the displayed or document
        //! line.
        HomeWrapExtend = QsciScintillaBase::SCI_HOMEWRAPEXTEND,

        //! Move to the first visible character in the document line.
        VCHome = QsciScintillaBase::SCI_VCHOME,

        //! Extend the selection to the first visible character in the document
        //! line.
        VCHomeExtend = QsciScintillaBase::SCI_VCHOMEEXTEND,

        //! Extend the rectangular selection to the first visible character in
        //! the document line.
        VCHomeRectExtend = QsciScintillaBase::SCI_VCHOMERECTEXTEND,

        //! Move to the first visible character of the displayed or document
        //! line.
        VCHomeWrap = QsciScintillaBase::SCI_VCHOMEWRAP,

        //! Extend the selection to the first visible character of the
        //! displayed or document line.
        VCHomeWrapExtend = QsciScintillaBase::SCI_VCHOMEWRAPEXTEND,

        //! Move to the end of the document line.
        LineEnd = QsciScintillaBase::SCI_LINEEND,

        //! Extend the selection to the end of the document line.
        LineEndExtend = QsciScintillaBase::SCI_LINEENDEXTEND,

        //! Extend the rectangular selection to the end of the document line.
        LineEndRectExtend = QsciScintillaBase::SCI_LINEENDRECTEXTEND,

        //! Move to the end of the displayed line.
        LineEndDisplay = QsciScintillaBase::SCI_LINEENDDISPLAY,

        //! Extend the selection to the end of the displayed line.
        LineEndDisplayExtend = QsciScintillaBase::SCI_LINEENDDISPLAYEXTEND,

        //! Move to the end of the displayed or document line.
        LineEndWrap = QsciScintillaBase::SCI_LINEENDWRAP,

        //! Extend the selection to the end of the displayed or document line.
        LineEndWrapExtend = QsciScintillaBase::SCI_LINEENDWRAPEXTEND,

        //! Move to the start of the document.
        DocumentStart = QsciScintillaBase::SCI_DOCUMENTSTART,

        //! Extend the selection to the start of the document.
        DocumentStartExtend = QsciScintillaBase::SCI_DOCUMENTSTARTEXTEND,

        //! Move to the end of the document.
        DocumentEnd = QsciScintillaBase::SCI_DOCUMENTEND,

        //! Extend the selection to the end of the document.
        DocumentEndExtend = QsciScintillaBase::SCI_DOCUMENTENDEXTEND,

        //! Move up one page.
        PageUp = QsciScintillaBase::SCI_PAGEUP,

        //! Extend the selection up one page.
        PageUpExtend = QsciScintillaBase::SCI_PAGEUPEXTEND,

        //! Extend the rectangular selection up one page.
        PageUpRectExtend = QsciScintillaBase::SCI_PAGEUPRECTEXTEND,

        //! Move down one page.
        PageDown = QsciScintillaBase::SCI_PAGEDOWN,

        //! Extend the selection down one page.
        PageDownExtend = QsciScintillaBase::SCI_PAGEDOWNEXTEND,

        //! Extend the rectangular selection down one page.
        PageDownRectExtend = QsciScintillaBase::SCI_PAGEDOWNRECTEXTEND,

        //! Stuttered move up one page.
        StutteredPageUp = QsciScintillaBase::SCI_STUTTEREDPAGEUP,

        //! Stuttered extend the selection up one page.
        StutteredPageUpExtend = QsciScintillaBase::SCI_STUTTEREDPAGEUPEXTEND,

        //! Stuttered move down one page.
        StutteredPageDown = QsciScintillaBase::SCI_STUTTEREDPAGEDOWN,

        //! Stuttered extend the selection down one page.
        StutteredPageDownExtend = QsciScintillaBase::SCI_STUTTEREDPAGEDOWNEXTEND,

        //! Delete the current character.
        Delete = QsciScintillaBase::SCI_CLEAR,

        //! Delete the previous character.
        DeleteBack = QsciScintillaBase::SCI_DELETEBACK,

        //! Delete the previous character if not at start of line.
        DeleteBackNotLine = QsciScintillaBase::SCI_DELETEBACKNOTLINE,

        //! Delete the word to the left.
        DeleteWordLeft = QsciScintillaBase::SCI_DELWORDLEFT,

        //! Delete the word to the right.
        DeleteWordRight = QsciScintillaBase::SCI_DELWORDRIGHT,

        //! Delete right to the end of the next word.
        DeleteWordRightEnd = QsciScintillaBase::SCI_DELWORDRIGHTEND,

        //! Delete the line to the left.
        DeleteLineLeft = QsciScintillaBase::SCI_DELLINELEFT,

        //! Delete the line to the right.
        DeleteLineRight = QsciScintillaBase::SCI_DELLINERIGHT,

        //! Delete the current line.
        LineDelete = QsciScintillaBase::SCI_LINEDELETE,

        //! Cut the current line to the clipboard.
        LineCut = QsciScintillaBase::SCI_LINECUT,

        //! Copy the current line to the clipboard.
        LineCopy = QsciScintillaBase::SCI_LINECOPY,

        //! Transpose the current and previous lines.
        LineTranspose = QsciScintillaBase::SCI_LINETRANSPOSE,

        //! Duplicate the current line.
        LineDuplicate = QsciScintillaBase::SCI_LINEDUPLICATE,

        //! Select the whole document.
        SelectAll = QsciScintillaBase::SCI_SELECTALL,

        //! Move the selected lines up one line.
        MoveSelectedLinesUp = QsciScintillaBase::SCI_MOVESELECTEDLINESUP,

        //! Move the selected lines down one line.
        MoveSelectedLinesDown = QsciScintillaBase::SCI_MOVESELECTEDLINESDOWN,

        //! Duplicate the selection.
        SelectionDuplicate = QsciScintillaBase::SCI_SELECTIONDUPLICATE,

        //! Convert the selection to lower case.
        SelectionLowerCase = QsciScintillaBase::SCI_LOWERCASE,

        //! Convert the selection to upper case.
        SelectionUpperCase = QsciScintillaBase::SCI_UPPERCASE,

        //! Cut the selection to the clipboard.
        SelectionCut = QsciScintillaBase::SCI_CUT,

        //! Copy the selection to the clipboard.
        SelectionCopy = QsciScintillaBase::SCI_COPY,

        //! Paste from the clipboard.
        Paste = QsciScintillaBase::SCI_PASTE,

        //! Toggle insert/overtype.
        EditToggleOvertype = QsciScintillaBase::SCI_EDITTOGGLEOVERTYPE,

        //! Insert a platform dependent newline.
        Newline = QsciScintillaBase::SCI_NEWLINE,

        //! Insert a formfeed.
        Formfeed = QsciScintillaBase::SCI_FORMFEED,

        //! Indent one level.
        Tab = QsciScintillaBase::SCI_TAB,

        //! De-indent one level.
        Backtab = QsciScintillaBase::SCI_BACKTAB,

        //! Cancel any current operation.
        Cancel = QsciScintillaBase::SCI_CANCEL,

        //! Undo the last command.
        Undo = QsciScintillaBase::SCI_UNDO,

        //! Redo the last command.
        Redo = QsciScintillaBase::SCI_REDO,

        //! Zoom in.
        ZoomIn = QsciScintillaBase::SCI_ZOOMIN,

        //! Zoom out.
        ZoomOut = QsciScintillaBase::SCI_ZOOMOUT,

        //! Reverse the selected lines.
        ReverseLines = QsciScintillaBase::SCI_LINEREVERSE,
    };

    //! Return the command that will be executed by this instance.
    Command command() const {return scicmd;}

    //! Execute the command.
    void execute();

    //! Binds the key \a key to the command.  If \a key is 0 then the key
    //! binding is removed.  If \a key is invalid then the key binding is
    //! unchanged.  Valid keys are any visible or control character or any
    //! of \c Qt::Key_Down, \c Qt::Key_Up, \c Qt::Key_Left, \c Qt::Key_Right,
    //! \c Qt::Key_Home, \c Qt::Key_End, \c Qt::Key_PageUp,
    //! \c Qt::Key_PageDown, \c Qt::Key_Delete, \c Qt::Key_Insert,
    //! \c Qt::Key_Escape, \c Qt::Key_Backspace, \c Qt::Key_Tab,
    //! \c Qt::Key_Backtab, \c Qt::Key_Return, \c Qt::Key_Enter,
    //! \c Qt::Key_Super_L, \c Qt::Key_Super_R or \c Qt::Key_Menu.  Keys may be
    //! modified with any combination of \c Qt::ShiftModifier,
    //! \c Qt::ControlModifier, \c Qt::AltModifier and \c Qt::MetaModifier.
    //!
    //! \sa key(), setAlternateKey(), validKey()
    void setKey(int key);

    //! Binds the alternate key \a altkey to the command.  If \a key is 0
    //! then the alternate key binding is removed.
    //!
    //! \sa alternateKey(), setKey(), validKey()
    void setAlternateKey(int altkey);

    //! The key that is currently bound to the command is returned.
    //!
    //! \sa setKey(), alternateKey()
    int key() const {return qkey;}

    //! The alternate key that is currently bound to the command is
    //! returned.
    //!
    //! \sa setAlternateKey(), key()
    int alternateKey() const {return qaltkey;}

    //! If the key \a key is valid then true is returned.
    static bool validKey(int key);

    //! The user friendly description of the command is returned.
    QString description() const;

private:
    friend class QsciCommandSet;

    QsciCommand(QsciScintilla *qs, Command cmd, int key, int altkey,
            const char *desc);

    void bindKey(int key,int &qk,int &scik);

    QsciScintilla *qsCmd;
    Command scicmd;
    int qkey, scikey, qaltkey, scialtkey;
    const char *descCmd;

    QsciCommand(const QsciCommand &);
    QsciCommand &operator=(const QsciCommand &);
};

#endif
