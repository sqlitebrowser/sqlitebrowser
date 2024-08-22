// This module implements the QsciLexerTCL class.
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


#include "Qsci/qscilexertcl.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerTCL::QsciLexerTCL(QObject *parent)
    : QsciLexer(parent), fold_comments(false)
{
}


// The dtor.
QsciLexerTCL::~QsciLexerTCL()
{
}


// Returns the language name.
const char *QsciLexerTCL::language() const
{
    return "TCL";
}


// Returns the lexer name.
const char *QsciLexerTCL::lexer() const
{
    return "tcl";
}


// Return the style used for braces.
int QsciLexerTCL::braceStyle() const
{
    return Operator;
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerTCL::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x80,0x80,0x80);

    case Comment:
    case CommentLine:
    case CommentBox:
        return QColor(0x00,0x7f,0x00);

    case Number:
        return QColor(0x00,0x7f,0x7f);

    case QuotedKeyword:
    case QuotedString:
    case Modifier:
        return QColor(0x7f,0x00,0x7f);

    case Operator:
        return QColor(0x00,0x00,0x00);

    case Identifier:
    case ExpandKeyword:
    case TCLKeyword:
    case TkKeyword:
    case ITCLKeyword:
    case TkCommand:
    case KeywordSet6:
    case KeywordSet7:
    case KeywordSet8:
    case KeywordSet9:
        return QColor(0x00,0x00,0x7f);

    case Substitution:
    case SubstitutionBrace:
        return QColor(0x7f,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerTCL::defaultEolFill(int style) const
{
    switch (style)
    {
    case QuotedString:
    case CommentBox:
        return true;
    }

    return QsciLexer::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerTCL::defaultFont(int style) const
{
    QFont f;

    switch (style)
    {
    case Comment:
    case CommentLine:
    case CommentBox:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS", 9);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 12);
#else
        f = QFont("Bitstream Vera Serif", 9);
#endif
        break;

    case QuotedKeyword:
    case Operator:
    case ExpandKeyword:
    case TCLKeyword:
    case TkKeyword:
    case ITCLKeyword:
    case TkCommand:
        f = QsciLexer::defaultFont(style);
        f.setBold(true);
        break;

    case CommentBlock:
#if defined(Q_OS_WIN)
        f = QFont("Comic Sans MS", 8);
#elif defined(Q_OS_MAC)
        f = QFont("Comic Sans MS", 11);
#else
        f = QFont("Serif", 9);
#endif
        break;

    default:
        f = QsciLexer::defaultFont(style);
    }

    return f;
}


// Returns the set of keywords.
const char *QsciLexerTCL::keywords(int set) const
{
    if (set == 1)
        return
            "after append array auto_execok auto_import auto_load "
            "auto_load_index auto_qualify beep bgerror binary break case "
            "catch cd clock close concat continue dde default echo else "
            "elseif encoding eof error eval exec exit expr fblocked "
            "fconfigure fcopy file fileevent flush for foreach format gets "
            "glob global history http if incr info interp join lappend lindex "
            "linsert list llength load loadTk lrange lreplace lsearch lset "
            "lsort memory msgcat namespace open package pid pkg::create "
            "pkg_mkIndex Platform-specific proc puts pwd re_syntax read "
            "regexp registry regsub rename resource return scan seek set "
            "socket source split string subst switch tclLog tclMacPkgSearch "
            "tclPkgSetup tclPkgUnknown tell time trace unknown unset update "
            "uplevel upvar variable vwait while";

    if (set == 2)
        return
            "bell bind bindtags bitmap button canvas checkbutton clipboard "
            "colors console cursors destroy entry event focus font frame grab "
            "grid image Inter-client keysyms label labelframe listbox lower "
            "menu menubutton message option options pack panedwindow photo "
            "place radiobutton raise scale scrollbar selection send spinbox "
            "text tk tk_chooseColor tk_chooseDirectory tk_dialog tk_focusNext "
            "tk_getOpenFile tk_messageBox tk_optionMenu tk_popup "
            "tk_setPalette tkerror tkvars tkwait toplevel winfo wish wm";

    if (set == 3)
        return
            "@scope body class code common component configbody constructor "
            "define destructor hull import inherit itcl itk itk_component "
            "itk_initialize itk_interior itk_option iwidgets keep method "
            "private protected public";

    if (set == 4)
        return
            "tk_bisque tk_chooseColor tk_dialog tk_focusFollowsMouse "
            "tk_focusNext tk_focusPrev tk_getOpenFile tk_getSaveFile "
            "tk_messageBox tk_optionMenu tk_popup tk_setPalette tk_textCopy "
            "tk_textCut tk_textPaste tkButtonAutoInvoke tkButtonDown "
            "tkButtonEnter tkButtonInvoke tkButtonLeave tkButtonUp "
            "tkCancelRepeat tkCheckRadioDown tkCheckRadioEnter "
            "tkCheckRadioInvoke tkColorDialog tkColorDialog_BuildDialog "
            "tkColorDialog_CancelCmd tkColorDialog_Config "
            "tkColorDialog_CreateSelector tkColorDialog_DrawColorScale "
            "tkColorDialog_EnterColorBar tkColorDialog_HandleRGB Entry "
            "tkColorDialog_HandleSelEntry tkColorDialog_InitValues "
            "tkColorDialog_LeaveColorBar tkColorDialog_MoveSelector "
            "tkColorDialog_OkCmd tkColorDialog_RedrawColorBars "
            "tkColorDialog_RedrawFinalColor tkColorDialog_ReleaseMouse "
            "tkColorDialog_ResizeColorBars tkColorDialog_RgbToX "
            "tkColorDialog_SetRGBValue tkColorDialog_StartMove "
            "tkColorDialog_XToRgb tkConsoleAbout tkConsoleBind tkConsoleExit "
            "tkConsoleHistory tkConsoleInit tkConsoleInsert tkConsoleInvoke "
            "tkConsoleOutput tkConsolePrompt tkConsoleSource tkDarken "
            "tkEntryAutoScan tkEntryBackspace tkEntryButton1 "
            "tkEntryClosestGap tkEntryGetSelection tkEntryInsert "
            "tkEntryKeySelect tkEntryMouseSelect tkEntryNextWord tkEntryPaste "
            "tkEntryPreviousWord tkEntrySeeInsert tkEntrySetCursor "
            "tkEntryTranspose tkEventMotifBindings tkFDGetFileTypes "
            "tkFirstMenu tkFocusGroup_BindIn tkFocusGroup_BindOut "
            "tkFocusGroup_Create tkFocusGroup_Destroy tkFocusGroup_In "
            "tkFocusGroup_Out tkFocusOK tkGenerateMenuSelect tkIconList "
            "tkIconList_Add tkIconList_Arrange tkIconList_AutoScan "
            "tkIconList_Btn1 tkIconList_Config tkIconList_Create "
            "tkIconList_CtrlBtn1 tkIconList_Curselection tkIconList_DeleteAll "
            "tkIconList_Double1 tkIconList_DrawSelection tkIconList_FocusIn "
            "tkIconList_FocusOut tkIconList_Get tkIconList_Goto "
            "tkIconList_Index tkIconList_Invoke tkIconList_KeyPress "
            "tkIconList_Leave1 tkIconList_LeftRight tkIconList_Motion1 "
            "tkIconList_Reset tkIconList_ReturnKey tkIconList_See "
            "tkIconList_Select tkIconList_Selection tkIconList_ShiftBtn1 "
            "tkIconList_UpDown tkListbox tkListboxAutoScan "
            "tkListboxBeginExtend tkListboxBeginSelect tkListboxBeginToggle "
            "tkListboxCancel tkListboxDataExtend tkListboxExtendUpDown "
            "tkListboxKeyAccel_Goto tkListboxKeyAccel_Key "
            "tkListboxKeyAccel_Reset tkListboxKeyAccel_Set "
            "tkListboxKeyAccel_Unset tkListboxMotion tkListboxSelectAll "
            "tkListboxUpDown tkMbButtonUp tkMbEnter tkMbLeave tkMbMotion "
            "tkMbPost tkMenuButtonDown tkMenuDownArrow tkMenuDup tkMenuEscape "
            "tkMenuFind tkMenuFindName tkMenuFirstEntry tkMenuInvoke "
            "tkMenuLeave tkMenuLeftArrow tkMenuMotion tkMenuNextEntry "
            "tkMenuNextMenu tkMenuRightArrow tkMenuUnpost tkMenuUpArrow "
            "tkMessageBox tkMotifFDialog tkMotifFDialog_ActivateDList "
            "tkMotifFDialog_ActivateFEnt tkMotifFDialog_ActivateFList "
            "tkMotifFDialog_ActivateSEnt tkMotifFDialog_BrowseDList "
            "tkMotifFDialog_BrowseFList tkMotifFDialog_BuildUI "
            "tkMotifFDialog_CancelCmd tkMotifFDialog_Config "
            "tkMotifFDialog_Create tkMotifFDialog_FileTypes "
            "tkMotifFDialog_FilterCmd tkMotifFDialog_InterpFilter "
            "tkMotifFDialog_LoadFiles tkMotifFDialog_MakeSList "
            "tkMotifFDialog_OkCmd tkMotifFDialog_SetFilter "
            "tkMotifFDialog_SetListMode tkMotifFDialog_Update tkPostOverPoint "
            "tkRecolorTree tkRestoreOldGrab tkSaveGrabInfo tkScaleActivate "
            "tkScaleButton2Down tkScaleButtonDown tkScaleControlPress "
            "tkScaleDrag tkScaleEndDrag tkScaleIncrement tkScreenChanged "
            "tkScrollButton2Down tkScrollButtonDown tkScrollButtonDrag "
            "tkScrollButtonUp tkScrollByPages tkScrollByUnits tkScrollDrag "
            "tkScrollEndDrag tkScrollSelect tkScrollStartDrag "
            "tkScrollTopBottom tkScrollToPos tkTabToWindow tkTearOffMenu "
            "tkTextAutoScan tkTextButton1 tkTextClosestGap tkTextInsert "
            "tkTextKeyExtend tkTextKeySelect tkTextNextPara tkTextNextPos "
            "tkTextNextWord tkTextPaste tkTextPrevPara tkTextPrevPos "
            "tkTextPrevWord tkTextResetAnchor tkTextScrollPages "
            "tkTextSelectTo tkTextSetCursor tkTextTranspose tkTextUpDownLine "
            "tkTraverseToMenu tkTraverseWithinMenu";

    if (set == 5)
        return "expand";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerTCL::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Comment:
        return tr("Comment");

    case CommentLine:
        return tr("Comment line");

    case Number:
        return tr("Number");

    case QuotedKeyword:
        return tr("Quoted keyword");

    case QuotedString:
        return tr("Quoted string");

    case Operator:
        return tr("Operator");

    case Identifier:
        return tr("Identifier");

    case Substitution:
        return tr("Substitution");

    case SubstitutionBrace:
        return tr("Brace substitution");

    case Modifier:
        return tr("Modifier");

    case ExpandKeyword:
        return tr("Expand keyword");

    case TCLKeyword:
        return tr("TCL keyword");

    case TkKeyword:
        return tr("Tk keyword");

    case ITCLKeyword:
        return tr("iTCL keyword");

    case TkCommand:
        return tr("Tk command");

    case KeywordSet6:
        return tr("User defined 1");

    case KeywordSet7:
        return tr("User defined 2");

    case KeywordSet8:
        return tr("User defined 3");

    case KeywordSet9:
        return tr("User defined 4");

    case CommentBox:
        return tr("Comment box");

    case CommentBlock:
        return tr("Comment block");
    }

    return QString();
}


// Returns the background colour of the text for a style.
QColor QsciLexerTCL::defaultPaper(int style) const
{
    switch (style)
    {
    case Comment:
        return QColor(0xf0,0xff,0xe0);

    case QuotedKeyword:
    case QuotedString:
    case ITCLKeyword:
        return QColor(0xff,0xf0,0xf0);

    case Substitution:
        return QColor(0xef,0xff,0xf0);

    case ExpandKeyword:
        return QColor(0xff,0xff,0x80);

    case TkKeyword:
        return QColor(0xe0,0xff,0xf0);

    case TkCommand:
        return QColor(0xff,0xd0,0xd0);

    case CommentBox:
    case CommentBlock:
        return QColor(0xf0,0xff,0xf0);
    }

    return QsciLexer::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerTCL::refreshProperties()
{
    setCommentProp();
}


// Read properties from the settings.
bool QsciLexerTCL::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerTCL::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.setValue(prefix + "foldcomments", fold_comments);

    return rc;
}


// Set if comments can be folded.
void QsciLexerTCL::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerTCL::setCommentProp()
{
    emit propertyChanged("fold.comment", (fold_comments ? "1" : "0"));
}
