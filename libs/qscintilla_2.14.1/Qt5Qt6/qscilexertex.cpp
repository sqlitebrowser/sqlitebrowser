// This module implements the QsciLexerTeX class.
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


#include "Qsci/qscilexertex.h"

#include <qcolor.h>
#include <qfont.h>
#include <qsettings.h>


// The ctor.
QsciLexerTeX::QsciLexerTeX(QObject *parent)
    : QsciLexer(parent),
      fold_comments(false), fold_compact(true), process_comments(false),
      process_if(true)
{
}


// The dtor.
QsciLexerTeX::~QsciLexerTeX()
{
}


// Returns the language name.
const char *QsciLexerTeX::language() const
{
    return "TeX";
}


// Returns the lexer name.
const char *QsciLexerTeX::lexer() const
{
    return "tex";
}


// Return the string of characters that comprise a word.
const char *QsciLexerTeX::wordCharacters() const
{
    return "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ\\@";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerTeX::defaultColor(int style) const
{
    switch (style)
    {
    case Default:
        return QColor(0x3f,0x3f,0x3f);

    case Special:
        return QColor(0x00,0x7f,0x7f);

    case Group:
        return QColor(0x7f,0x00,0x00);

    case Symbol:
        return QColor(0x7f,0x7f,0x00);

    case Command:
        return QColor(0x00,0x7f,0x00);
    }

    return QsciLexer::defaultColor(style);
}


// Returns the set of keywords.
const char *QsciLexerTeX::keywords(int set) const
{
    if (set == 1)
        return
            "above abovedisplayshortskip abovedisplayskip "
            "abovewithdelims accent adjdemerits advance "
            "afterassignment aftergroup atop atopwithdelims "
            "badness baselineskip batchmode begingroup "
            "belowdisplayshortskip belowdisplayskip binoppenalty "
            "botmark box boxmaxdepth brokenpenalty catcode char "
            "chardef cleaders closein closeout clubpenalty copy "
            "count countdef cr crcr csname day deadcycles def "
            "defaulthyphenchar defaultskewchar delcode delimiter "
            "delimiterfactor delimeters delimitershortfall "
            "delimeters dimen dimendef discretionary "
            "displayindent displaylimits displaystyle "
            "displaywidowpenalty displaywidth divide "
            "doublehyphendemerits dp dump edef else "
            "emergencystretch end endcsname endgroup endinput "
            "endlinechar eqno errhelp errmessage "
            "errorcontextlines errorstopmode escapechar everycr "
            "everydisplay everyhbox everyjob everymath everypar "
            "everyvbox exhyphenpenalty expandafter fam fi "
            "finalhyphendemerits firstmark floatingpenalty font "
            "fontdimen fontname futurelet gdef global group "
            "globaldefs halign hangafter hangindent hbadness "
            "hbox hfil horizontal hfill horizontal hfilneg hfuzz "
            "hoffset holdinginserts hrule hsize hskip hss "
            "horizontal ht hyphenation hyphenchar hyphenpenalty "
            "hyphen if ifcase ifcat ifdim ifeof iffalse ifhbox "
            "ifhmode ifinner ifmmode ifnum ifodd iftrue ifvbox "
            "ifvmode ifvoid ifx ignorespaces immediate indent "
            "input inputlineno input insert insertpenalties "
            "interlinepenalty jobname kern language lastbox "
            "lastkern lastpenalty lastskip lccode leaders left "
            "lefthyphenmin leftskip leqno let limits linepenalty "
            "line lineskip lineskiplimit long looseness lower "
            "lowercase mag mark mathaccent mathbin mathchar "
            "mathchardef mathchoice mathclose mathcode mathinner "
            "mathop mathopen mathord mathpunct mathrel "
            "mathsurround maxdeadcycles maxdepth meaning "
            "medmuskip message mkern month moveleft moveright "
            "mskip multiply muskip muskipdef newlinechar noalign "
            "noboundary noexpand noindent nolimits nonscript "
            "scriptscript nonstopmode nulldelimiterspace "
            "nullfont number omit openin openout or outer output "
            "outputpenalty over overfullrule overline "
            "overwithdelims pagedepth pagefilllstretch "
            "pagefillstretch pagefilstretch pagegoal pageshrink "
            "pagestretch pagetotal par parfillskip parindent "
            "parshape parskip patterns pausing penalty "
            "postdisplaypenalty predisplaypenalty predisplaysize "
            "pretolerance prevdepth prevgraf radical raise read "
            "relax relpenalty right righthyphenmin rightskip "
            "romannumeral scriptfont scriptscriptfont "
            "scriptscriptstyle scriptspace scriptstyle "
            "scrollmode setbox setlanguage sfcode shipout show "
            "showbox showboxbreadth showboxdepth showlists "
            "showthe skewchar skip skipdef spacefactor spaceskip "
            "span special splitbotmark splitfirstmark "
            "splitmaxdepth splittopskip string tabskip textfont "
            "textstyle the thickmuskip thinmuskip time toks "
            "toksdef tolerance topmark topskip tracingcommands "
            "tracinglostchars tracingmacros tracingonline "
            "tracingoutput tracingpages tracingparagraphs "
            "tracingrestores tracingstats uccode uchyph "
            "underline unhbox unhcopy unkern unpenalty unskip "
            "unvbox unvcopy uppercase vadjust valign vbadness "
            "vbox vcenter vfil vfill vfilneg vfuzz voffset vrule "
            "vsize vskip vsplit vss vtop wd widowpenalty write "
            "xdef xleaders xspaceskip year "
            "TeX bgroup egroup endgraf space empty null newcount "
            "newdimen newskip newmuskip newbox newtoks newhelp "
            "newread newwrite newfam newlanguage newinsert newif "
            "maxdimen magstephalf magstep frenchspacing "
            "nonfrenchspacing normalbaselines obeylines "
            "obeyspaces raggedr ight ttraggedright thinspace "
            "negthinspace enspace enskip quad qquad smallskip "
            "medskip bigskip removelastskip topglue vglue hglue "
            "break nobreak allowbreak filbreak goodbreak "
            "smallbreak medbreak bigbreak line leftline "
            "rightline centerline rlap llap underbar strutbox "
            "strut cases matrix pmatrix bordermatrix eqalign "
            "displaylines eqalignno leqalignno pageno folio "
            "tracingall showhyphens fmtname fmtversion hphantom "
            "vphantom phantom smash";

    return 0;
}


// Returns the user name of a style.
QString QsciLexerTeX::description(int style) const
{
    switch (style)
    {
    case Default:
        return tr("Default");

    case Special:
        return tr("Special");

    case Group:
        return tr("Group");

    case Symbol:
        return tr("Symbol");

    case Command:
        return tr("Command");

    case Text:
        return tr("Text");
    }

    return QString();
}


// Refresh all properties.
void QsciLexerTeX::refreshProperties()
{
    setCommentProp();
    setCompactProp();
    setProcessCommentsProp();
    setAutoIfProp();
}


// Read properties from the settings.
bool QsciLexerTeX::readProperties(QSettings &qs, const QString &prefix)
{
    int rc = true;

    fold_comments = qs.value(prefix + "foldcomments", false).toBool();
    fold_compact = qs.value(prefix + "foldcompact", true).toBool();
    process_comments = qs.value(prefix + "processcomments", false).toBool();
    process_if = qs.value(prefix + "processif", true).toBool();

    return rc;
}


// Write properties to the settings.
bool QsciLexerTeX::writeProperties(QSettings &qs, const QString &prefix) const
{
    int rc = true;

    qs.value(prefix + "foldcomments", fold_comments);
    qs.value(prefix + "foldcompact", fold_compact);
    qs.value(prefix + "processcomments", process_comments);
    qs.value(prefix + "processif", process_if);

    return rc;
}


// Set if comments can be folded.
void QsciLexerTeX::setFoldComments(bool fold)
{
    fold_comments = fold;

    setCommentProp();
}


// Set the "fold.comment" property.
void QsciLexerTeX::setCommentProp()
{
    emit propertyChanged("fold.comment", (fold_comments ? "1" : "0"));
}


// Set if folds are compact.
void QsciLexerTeX::setFoldCompact(bool fold)
{
    fold_compact = fold;

    setCompactProp();
}


// Set the "fold.compact" property.
void QsciLexerTeX::setCompactProp()
{
    emit propertyChanged("fold.compact", (fold_compact ? "1" : "0"));
}


// Set if comments are processed
void QsciLexerTeX::setProcessComments(bool enable)
{
    process_comments = enable;

    setProcessCommentsProp();
}


// Set the "lexer.tex.comment.process" property.
void QsciLexerTeX::setProcessCommentsProp()
{
    emit propertyChanged("lexer.tex.comment.process", (process_comments ? "1" : "0"));
}


// Set if \if<unknown> is processed
void QsciLexerTeX::setProcessIf(bool enable)
{
    process_if = enable;

    setAutoIfProp();
}


// Set the "lexer.tex.auto.if" property.
void QsciLexerTeX::setAutoIfProp()
{
    emit propertyChanged("lexer.tex.auto.if", (process_if ? "1" : "0"));
}
