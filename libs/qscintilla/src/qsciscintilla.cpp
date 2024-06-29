// This module implements the "official" high-level API of the Qt port of
// Scintilla.  It is modelled on QTextEdit - a method of the same name should
// behave in the same way.
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


#include "Qsci/qsciscintilla.h"

#include <string.h>

#include <QAction>
#include <QApplication>
#include <QColor>
#include <QEvent>
#include <QImage>
#include <QIODevice>
#include <QKeyEvent>
#include <QKeySequence>
#include <QMenu>
#include <QPoint>

#include "Qsci/qsciabstractapis.h"
#include "Qsci/qscicommandset.h"
#include "Qsci/qscilexer.h"
#include "Qsci/qscistyle.h"
#include "Qsci/qscistyledtext.h"


// Make sure these match the values in Scintilla.h.  We don't #include that
// file because it just causes more clashes.
#define KEYWORDSET_MAX  8
#define MARKER_MAX      31

// The list separators for auto-completion and user lists.
const char acSeparator = '\x03';
const char userSeparator = '\x04';

// The default fold margin width.
static const int defaultFoldMarginWidth = 14;

// The default set of characters that make up a word.
static const char *defaultWordChars = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Forward declarations.
static QColor asQColor(long sci_colour);


// The ctor.
QsciScintilla::QsciScintilla(QWidget *parent)
    : QsciScintillaBase(parent),
      allocatedMarkers(0), allocatedIndicators(7), oldPos(-1), selText(false),
      fold(NoFoldStyle), foldmargin(2), autoInd(false),
      braceMode(NoBraceMatch), acSource(AcsNone), acThresh(-1),
      wchars(defaultWordChars), call_tips_position(CallTipsBelowText),
      call_tips_style(CallTipsNoContext), maxCallTips(-1),
      use_single(AcusNever), explicit_fillups(""), fillups_enabled(false)
{
    connect(this,SIGNAL(SCN_MODIFYATTEMPTRO()),
             SIGNAL(modificationAttempted()));

    connect(this,SIGNAL(SCN_MODIFIED(int,int,const char *,int,int,int,int,int,int,int)),
             SLOT(handleModified(int,int,const char *,int,int,int,int,int,int,int)));
    connect(this,SIGNAL(SCN_CALLTIPCLICK(int)),
             SLOT(handleCallTipClick(int)));
    connect(this,SIGNAL(SCN_CHARADDED(int)),
             SLOT(handleCharAdded(int)));
    connect(this,SIGNAL(SCN_INDICATORCLICK(int,int)),
             SLOT(handleIndicatorClick(int,int)));
    connect(this,SIGNAL(SCN_INDICATORRELEASE(int,int)),
             SLOT(handleIndicatorRelease(int,int)));
    connect(this,SIGNAL(SCN_MARGINCLICK(int,int,int)),
             SLOT(handleMarginClick(int,int,int)));
    connect(this,SIGNAL(SCN_MARGINRIGHTCLICK(int,int,int)),
             SLOT(handleMarginRightClick(int,int,int)));
    connect(this,SIGNAL(SCN_SAVEPOINTREACHED()),
             SLOT(handleSavePointReached()));
    connect(this,SIGNAL(SCN_SAVEPOINTLEFT()),
             SLOT(handleSavePointLeft()));
    connect(this,SIGNAL(SCN_UPDATEUI(int)),
             SLOT(handleUpdateUI(int)));
    connect(this,SIGNAL(QSCN_SELCHANGED(bool)),
             SLOT(handleSelectionChanged(bool)));
    connect(this,SIGNAL(SCN_AUTOCSELECTION(const char *,int)),
             SLOT(handleAutoCompletionSelection()));
    connect(this,SIGNAL(SCN_USERLISTSELECTION(const char *,int)),
             SLOT(handleUserListSelection(const char *,int)));

    // Set the default font.
    setFont(QApplication::font());

    // Set the default fore and background colours.
    QPalette pal = QApplication::palette();
    setColor(pal.text().color());
    setPaper(pal.base().color());
    setSelectionForegroundColor(pal.highlightedText().color());
    setSelectionBackgroundColor(pal.highlight().color());

#if defined(Q_OS_WIN)
    setEolMode(EolWindows);
#else
    // Note that EolMac is pre-OS/X.
    setEolMode(EolUnix);
#endif

    // Capturing the mouse seems to cause problems on multi-head systems. Qt
    // should do the right thing anyway.
    SendScintilla(SCI_SETMOUSEDOWNCAPTURES, 0UL);

    setMatchedBraceForegroundColor(Qt::blue);
    setUnmatchedBraceForegroundColor(Qt::red);

    setAnnotationDisplay(AnnotationStandard);
    setLexer();

    // Set the visible policy.  These are the same as SciTE's defaults
    // which, presumably, are sensible.
    SendScintilla(SCI_SETVISIBLEPOLICY, VISIBLE_STRICT | VISIBLE_SLOP, 4);

    // The default behaviour is unexpected.
    SendScintilla(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR,
            SC_CASEINSENSITIVEBEHAVIOUR_IGNORECASE);

    // Create the standard command set.
    stdCmds = new QsciCommandSet(this);

    doc.display(this,0);
}


// The dtor.
QsciScintilla::~QsciScintilla()
{
    // Detach any current lexer.
    detachLexer();

    doc.undisplay(this);
    delete stdCmds;
}


// Return the current text colour.
QColor QsciScintilla::color() const
{
    return nl_text_colour;
}


// Set the text colour.
void QsciScintilla::setColor(const QColor &c)
{
    if (lex.isNull())
    {
        // Assume style 0 applies to everything so that we don't need to use
        // SCI_STYLECLEARALL which clears everything.
        SendScintilla(SCI_STYLESETFORE, 0, c);
        nl_text_colour = c;
    }
}


// Return the overwrite mode.
bool QsciScintilla::overwriteMode() const
{
    return SendScintilla(SCI_GETOVERTYPE);
}


// Set the overwrite mode.
void QsciScintilla::setOverwriteMode(bool overwrite)
{
    SendScintilla(SCI_SETOVERTYPE, overwrite);
}


// Return the current paper colour.
QColor QsciScintilla::paper() const
{
    return nl_paper_colour;
}


// Set the paper colour.
void QsciScintilla::setPaper(const QColor &c)
{
    if (lex.isNull())
    {
        // Assume style 0 applies to everything so that we don't need to use
        // SCI_STYLECLEARALL which clears everything.  We still have to set the
        // default style as well for the background without any text.
        SendScintilla(SCI_STYLESETBACK, 0, c);
        SendScintilla(SCI_STYLESETBACK, STYLE_DEFAULT, c);
        nl_paper_colour = c;
    }
}


// Set the default font.
void QsciScintilla::setFont(const QFont &f)
{
    if (lex.isNull())
    {
        // Assume style 0 applies to everything so that we don't need to use
        // SCI_STYLECLEARALL which clears everything.
        setStylesFont(f, 0);
        QWidget::setFont(f);
    }
}


// Enable/disable auto-indent.
void QsciScintilla::setAutoIndent(bool autoindent)
{
    autoInd = autoindent;
}


// Set the brace matching mode.
void QsciScintilla::setBraceMatching(BraceMatch bm)
{
    braceMode = bm;
}


// Handle the addition of a character.
void QsciScintilla::handleCharAdded(int ch)
{
    // Ignore if there is a selection.
    long pos = SendScintilla(SCI_GETSELECTIONSTART);

    if (pos != SendScintilla(SCI_GETSELECTIONEND) || pos == 0)
        return;

    // If auto-completion is already active then see if this character is a
    // start character.  If it is then create a new list which will be a subset
    // of the current one.  The case where it isn't a start character seems to
    // be handled correctly elsewhere.
    if (isListActive() && isStartChar(ch))
    {
        cancelList();
        startAutoCompletion(acSource, false, use_single == AcusAlways);

        return;
    }

    // Handle call tips.
    if (call_tips_style != CallTipsNone && !lex.isNull() && strchr("(),", ch) != NULL)
        callTip();

    // Handle auto-indentation.
    if (autoInd)
    {
        if (lex.isNull() || (lex->autoIndentStyle() & AiMaintain))
            maintainIndentation(ch, pos);
        else
            autoIndentation(ch, pos);
    }

    // See if we might want to start auto-completion.
    if (!isCallTipActive() && acSource != AcsNone)
    {
        if (isStartChar(ch))
            startAutoCompletion(acSource, false, use_single == AcusAlways);
        else if (acThresh >= 1 && isWordCharacter(ch))
            startAutoCompletion(acSource, true, use_single == AcusAlways);
    }
}


// See if a call tip is active.
bool QsciScintilla::isCallTipActive() const
{
    return SendScintilla(SCI_CALLTIPACTIVE);
}


// Handle a possible change to any current call tip.
void QsciScintilla::callTip()
{
    QsciAbstractAPIs *apis = lex->apis();

    if (!apis)
        return;

    int pos, commas = 0;
    bool found = false;
    char ch;

    pos = SendScintilla(SCI_GETCURRENTPOS);

    // Move backwards through the line looking for the start of the current
    // call tip and working out which argument it is.
    while ((ch = getCharacter(pos)) != '\0')
    {
        if (ch == ',')
            ++commas;
        else if (ch == ')')
        {
            int depth = 1;

            // Ignore everything back to the start of the corresponding
            // parenthesis.
            while ((ch = getCharacter(pos)) != '\0')
            {
                if (ch == ')')
                    ++depth;
                else if (ch == '(' && --depth == 0)
                    break;
            }
        }
        else if (ch == '(')
        {
            found = true;
            break;
        }
    }

    // Cancel any existing call tip.
    SendScintilla(SCI_CALLTIPCANCEL);

    // Done if there is no new call tip to set.
    if (!found)
        return;

    QStringList context = apiContext(pos, pos, ctPos);

    if (context.isEmpty())
        return;

    // The last word is complete, not partial.
    context << QString();

    ct_cursor = 0;
    ct_shifts.clear();
    ct_entries = apis->callTips(context, commas, call_tips_style, ct_shifts);

    int nr_entries = ct_entries.count();

    if (nr_entries == 0)
        return;

    if (maxCallTips > 0 && maxCallTips < nr_entries)
    {
        ct_entries = ct_entries.mid(0, maxCallTips);
        nr_entries = maxCallTips;
    }

    int shift;
    QString ct;

    int nr_shifts = ct_shifts.count();

    if (maxCallTips < 0 && nr_entries > 1)
    {
        shift = (nr_shifts > 0 ? ct_shifts.first() : 0);
        ct = ct_entries[0];
        ct.prepend('\002');
    }
    else
    {
        if (nr_shifts > nr_entries)
            nr_shifts = nr_entries;

        // Find the biggest shift.
        shift = 0;

        for (int i = 0; i < nr_shifts; ++i)
        {
            int sh = ct_shifts[i];

            if (shift < sh)
                shift = sh;
        }

        ct = ct_entries.join("\n");
    }

    QByteArray ct_bytes = textAsBytes(ct);
    const char *cts = ct_bytes.constData();

    SendScintilla(SCI_CALLTIPSHOW, adjustedCallTipPosition(shift), cts);

    // Done if there is more than one call tip.
    if (nr_entries > 1)
        return;

    // Highlight the current argument.
    const char *astart;

    if (commas == 0)
        astart = strchr(cts, '(');
    else
        for (astart = strchr(cts, ','); astart && --commas > 0; astart = strchr(astart + 1, ','))
            ;

    if (!astart || !*++astart)
        return;

    // The end is at the next comma or unmatched closing parenthesis.
    const char *aend;
    int depth = 0;

    for (aend = astart; *aend; ++aend)
    {
        char ch = *aend;

        if (ch == ',' && depth == 0)
            break;
        else if (ch == '(')
            ++depth;
        else if (ch == ')')
        {
            if (depth == 0)
                break;

            --depth;
        }
    }

    if (astart != aend)
        SendScintilla(SCI_CALLTIPSETHLT, astart - cts, aend - cts);
}


// Handle a call tip click.
void QsciScintilla::handleCallTipClick(int dir)
{
    int nr_entries = ct_entries.count();

    // Move the cursor while bounds checking.
    if (dir == 1)
    {
        if (ct_cursor - 1 < 0)
            return;

        --ct_cursor;
    }
    else if (dir == 2)
    {
        if (ct_cursor + 1 >= nr_entries)
            return;

        ++ct_cursor;
    }
    else
        return;

    int shift = (ct_shifts.count() > ct_cursor ? ct_shifts[ct_cursor] : 0);
    QString ct = ct_entries[ct_cursor];

    // Add the arrows.
    if (ct_cursor < nr_entries - 1)
        ct.prepend('\002');

    if (ct_cursor > 0)
        ct.prepend('\001');

    SendScintilla(SCI_CALLTIPSHOW, adjustedCallTipPosition(shift),
            textAsBytes(ct).constData());
}


// Shift the position of the call tip (to take any context into account) but
// don't go before the start of the line.
int QsciScintilla::adjustedCallTipPosition(int ctshift) const
{
    int ct = ctPos;

    if (ctshift)
    {
        int ctmin = SendScintilla(SCI_POSITIONFROMLINE, SendScintilla(SCI_LINEFROMPOSITION, ct));

        if (ct - ctshift < ctmin)
            ct = ctmin;
    }

    return ct;
}


// Return the list of words that make up the context preceding the given
// position.  The list will only have more than one element if there is a lexer
// set and it defines start strings.  If so, then the last element might be
// empty if a start string has just been typed.  On return pos is at the start
// of the context.
QStringList QsciScintilla::apiContext(int pos, int &context_start,
        int &last_word_start)
{
    enum {
        Either,
        Separator,
        Word
    };

    QStringList words;
    int good_pos = pos, expecting = Either;

    last_word_start = -1;

    while (pos > 0)
    {
        if (getSeparator(pos))
        {
            if (expecting == Either)
                words.prepend(QString());
            else if (expecting == Word)
                break;

            good_pos = pos;
            expecting = Word;
        }
        else
        {
            QString word = getWord(pos);

            if (word.isEmpty() || expecting == Separator)
                break;

            words.prepend(word);
            good_pos = pos;
            expecting = Separator;

            // Return the position of the start of the last word if required.
            if (last_word_start < 0)
                last_word_start = pos;
        }

        // Strip any preceding spaces (mainly around operators).
        char ch;

        while ((ch = getCharacter(pos)) != '\0')
        {
            // This is the same definition of space that Scintilla uses.
            if (ch != ' ' && (ch < 0x09 || ch > 0x0d))
            {
                ++pos;
                break;
            }
        }
    }

    // A valid sequence always starts with a word and so should be expecting a
    // separator.
    if (expecting != Separator)
        words.clear();

    context_start = good_pos;

    return words;
}


// Try and get a lexer's word separator from the text before the current
// position.  Return true if one was found and adjust the position accordingly.
bool QsciScintilla::getSeparator(int &pos) const
{
    int opos = pos;

    // Go through each separator.
    for (int i = 0; i < wseps.count(); ++i)
    {
        const QString &ws = wseps[i];

        // Work backwards.
        uint l;

        for (l = ws.length(); l; --l)
        {
            char ch = getCharacter(pos);

            if (ch == '\0' || ws.at(l - 1) != ch)
                break;
        }

        if (!l)
            return true;

        // Reset for the next separator.
        pos = opos;
    }

    return false;
}


// Try and get a word from the text before the current position.  Return the
// word if one was found and adjust the position accordingly.
QString QsciScintilla::getWord(int &pos) const
{
    QString word;
    bool numeric = true;
    char ch;

    while ((ch = getCharacter(pos)) != '\0')
    {
        if (!isWordCharacter(ch))
        {
            ++pos;
            break;
        }

        if (ch < '0' || ch > '9')
            numeric = false;

        word.prepend(ch);
    }

    // We don't auto-complete numbers.
    if (numeric)
        word.truncate(0);

    return word;
}


// Get the "next" character (ie. the one before the current position) in the
// current line.  The character will be '\0' if there are no more.
char QsciScintilla::getCharacter(int &pos) const
{
    if (pos <= 0)
        return '\0';

    char ch = SendScintilla(SCI_GETCHARAT, --pos);

    // Don't go past the end of the previous line.
    if (ch == '\n' || ch == '\r')
    {
        ++pos;
        return '\0';
    }

    return ch;
}


// See if a character is an auto-completion start character, ie. the last
// character of a word separator.
bool QsciScintilla::isStartChar(char ch) const
{
    QString s = QChar(ch);

    for (int i = 0; i < wseps.count(); ++i)
        if (wseps[i].endsWith(s))
            return true;

    return false;
}


// Possibly start auto-completion.
void QsciScintilla::startAutoCompletion(AutoCompletionSource acs,
        bool checkThresh, bool choose_single)
{
    int start, ignore;
    QStringList context = apiContext(SendScintilla(SCI_GETCURRENTPOS), start,
            ignore);

    if (context.isEmpty())
        return;

    // Get the last word's raw data and length.
    QByteArray s = textAsBytes(context.last());
    const char *last_data = s.constData();
    int last_len = s.length();

    if (checkThresh && last_len < acThresh)
        return;

    // Generate the string representing the valid words to select from.
    QStringList wlist;

    if ((acs == AcsAll || acs == AcsAPIs) && !lex.isNull())
    {
        QsciAbstractAPIs *apis = lex->apis();

        if (apis)
            apis->updateAutoCompletionList(context, wlist);
    }

    if (acs == AcsAll || acs == AcsDocument)
    {
        int sflags = SCFIND_WORDSTART;

        if (!SendScintilla(SCI_AUTOCGETIGNORECASE))
            sflags |= SCFIND_MATCHCASE;

        SendScintilla(SCI_SETSEARCHFLAGS, sflags);

        int pos = 0;
        int dlen = SendScintilla(SCI_GETLENGTH);
        int caret = SendScintilla(SCI_GETCURRENTPOS);
        int clen = caret - start;
        char *orig_context = new char[clen + 1];

        SendScintilla(SCI_GETTEXTRANGE, start, caret, orig_context);

        for (;;)
        {
            int fstart;

            SendScintilla(SCI_SETTARGETSTART, pos);
            SendScintilla(SCI_SETTARGETEND, dlen);

            if ((fstart = SendScintilla(SCI_SEARCHINTARGET, clen, orig_context)) < 0)
                break;

            // Move past the root part.
            pos = fstart + clen;

            // Skip if this is the context we are auto-completing.
            if (pos == caret)
                continue;

            // Get the rest of this word.
            QString w = last_data;

            while (pos < dlen)
            {
                char ch = SendScintilla(SCI_GETCHARAT, pos);

                if (!isWordCharacter(ch))
                    break;

                w += ch;
                ++pos;
            }

            // Add the word if it isn't already there.
            if (!w.isEmpty())
            {
                bool keep;

                // If there are APIs then check if the word is already present
                // as an API word (i.e. with a trailing space).
                if (acs == AcsAll)
                {
                    QString api_w = w;
                    api_w.append(' ');

                    keep = !wlist.contains(api_w);
                }
                else
                {
                    keep = true;
                }

                if (keep && !wlist.contains(w))
                    wlist.append(w);
            }
        }

        delete []orig_context;
    }

    if (wlist.isEmpty())
        return;

    wlist.sort();

    SendScintilla(SCI_AUTOCSETCHOOSESINGLE, choose_single);
    SendScintilla(SCI_AUTOCSETSEPARATOR, acSeparator);

    SendScintilla(SCI_AUTOCSHOW, last_len,
            textAsBytes(wlist.join(QChar(acSeparator))).constData());
}


// Maintain the indentation of the previous line.
void QsciScintilla::maintainIndentation(char ch, long pos)
{
    if (ch != '\r' && ch != '\n')
        return;

    int curr_line = SendScintilla(SCI_LINEFROMPOSITION, pos);

    // Get the indentation of the preceding non-zero length line.
    int ind = 0;

    for (int line = curr_line - 1; line >= 0; --line)
    {
        if (SendScintilla(SCI_GETLINEENDPOSITION, line) >
            SendScintilla(SCI_POSITIONFROMLINE, line))
        {
            ind = indentation(line);
            break;
        }
    }

    if (ind > 0)
        autoIndentLine(pos, curr_line, ind);
}


// Implement auto-indentation.
void QsciScintilla::autoIndentation(char ch, long pos)
{
    int curr_line = SendScintilla(SCI_LINEFROMPOSITION, pos);
    int ind_width = indentWidth();
    long curr_line_start = SendScintilla(SCI_POSITIONFROMLINE, curr_line);

    const char *block_start = lex->blockStart();
    bool start_single = (block_start && qstrlen(block_start) == 1);

    const char *block_end = lex->blockEnd();
    bool end_single = (block_end && qstrlen(block_end) == 1);

    if (end_single && block_end[0] == ch)
    {
        if (!(lex->autoIndentStyle() & AiClosing) && rangeIsWhitespace(curr_line_start, pos - 1))
            autoIndentLine(pos, curr_line, blockIndent(curr_line - 1) - ind_width);
    }
    else if (start_single && block_start[0] == ch)
    {
        // De-indent if we have already indented because the previous line was
        // a start of block keyword.
        if (!(lex->autoIndentStyle() & AiOpening) && curr_line > 0 && getIndentState(curr_line - 1) == isKeywordStart && rangeIsWhitespace(curr_line_start, pos - 1))
            autoIndentLine(pos, curr_line, blockIndent(curr_line - 1) - ind_width);
    }
    else if (ch == '\r' || ch == '\n')
    {
        // Don't auto-indent the line (ie. preserve its existing indentation)
        // if we have inserted a new line above by pressing return at the start
        // of this line - in other words, if the previous line is empty.
        long prev_line_length = SendScintilla(SCI_GETLINEENDPOSITION, curr_line - 1) - SendScintilla(SCI_POSITIONFROMLINE, curr_line - 1);

        if (prev_line_length != 0)
            autoIndentLine(pos, curr_line, blockIndent(curr_line - 1));
    }
}


// Set the indentation for a line.
void QsciScintilla::autoIndentLine(long pos, int line, int indent)
{
    if (indent < 0)
        return;

    long pos_before = SendScintilla(SCI_GETLINEINDENTPOSITION, line);
    SendScintilla(SCI_SETLINEINDENTATION, line, indent);
    long pos_after = SendScintilla(SCI_GETLINEINDENTPOSITION, line);
    long new_pos = -1;

    if (pos_after > pos_before)
    {
        new_pos = pos + (pos_after - pos_before);
    }
    else if (pos_after < pos_before && pos >= pos_after)
    {
        if (pos >= pos_before)
            new_pos = pos + (pos_after - pos_before);
        else
            new_pos = pos_after;
    }

    if (new_pos >= 0)
        SendScintilla(SCI_SETSEL, new_pos, new_pos);
}


// Return the indentation of the block defined by the given line (or something
// significant before).
int QsciScintilla::blockIndent(int line)
{
    if (line < 0)
        return 0;

    // Handle the trvial case.
    if (!lex->blockStartKeyword() && !lex->blockStart() && !lex->blockEnd())
        return indentation(line);

    int line_limit = line - lex->blockLookback();

    if (line_limit < 0)
        line_limit = 0;

    for (int l = line; l >= line_limit; --l)
    {
        IndentState istate = getIndentState(l);

        if (istate != isNone)
        {
            int ind_width = indentWidth();
            int ind = indentation(l);

            if (istate == isBlockStart)
            {
                if (!(lex -> autoIndentStyle() & AiOpening))
                    ind += ind_width;
            }
            else if (istate == isBlockEnd)
            {
                if (lex -> autoIndentStyle() & AiClosing)
                    ind -= ind_width;

                if (ind < 0)
                    ind = 0;
            }
            else if (line == l)
                ind += ind_width;

            return ind;
        }
    }

    return indentation(line);
}


// Return true if all characters starting at spos up to, but not including
// epos, are spaces or tabs.
bool QsciScintilla::rangeIsWhitespace(long spos, long epos)
{
    while (spos < epos)
    {
        char ch = SendScintilla(SCI_GETCHARAT, spos);

        if (ch != ' ' && ch != '\t')
            return false;

        ++spos;
    }

    return true;
}


// Returns the indentation state of a line.
QsciScintilla::IndentState QsciScintilla::getIndentState(int line)
{
    IndentState istate;

    // Get the styled text.
    long spos = SendScintilla(SCI_POSITIONFROMLINE, line);
    long epos = SendScintilla(SCI_POSITIONFROMLINE, line + 1);

    char *text = new char[(epos - spos + 1) * 2];

    SendScintilla(SCI_GETSTYLEDTEXT, spos, epos, text);

    int style, bstart_off, bend_off;

    // Block start/end takes precedence over keywords.
    const char *bstart_words = lex->blockStart(&style);
    bstart_off = findStyledWord(text, style, bstart_words);

    const char *bend_words = lex->blockEnd(&style);
    bend_off = findStyledWord(text, style, bend_words);

    // If there is a block start but no block end characters then ignore it
    // unless the block start is the last significant thing on the line, ie.
    // assume Python-like blocking.
    if (bstart_off >= 0 && !bend_words)
        for (int i = bstart_off * 2; text[i] != '\0'; i += 2)
            if (!QChar(text[i]).isSpace())
                return isNone;

    if (bstart_off > bend_off)
        istate = isBlockStart;
    else if (bend_off > bstart_off)
        istate = isBlockEnd;
    else
    {
        const char *words = lex->blockStartKeyword(&style);

        istate = (findStyledWord(text,style,words) >= 0) ? isKeywordStart : isNone;
    }

    delete[] text;

    return istate;
}


// text is a pointer to some styled text (ie. a character byte followed by a
// style byte).  style is a style number.  words is a space separated list of
// words.  Returns the position in the text immediately after the last one of
// the words with the style.  The reason we are after the last, and not the
// first, occurance is that we are looking for words that start and end a block
// where the latest one is the most significant.
int QsciScintilla::findStyledWord(const char *text, int style,
        const char *words)
{
    if (!words)
        return -1;

    // Find the range of text with the style we are looking for.
    const char *stext;

    for (stext = text; stext[1] != style; stext += 2)
        if (stext[0] == '\0')
            return -1;

    // Move to the last character.
    const char *etext = stext;

    while (etext[2] != '\0')
        etext += 2;

    // Backtrack until we find the style.  There will be one.
    while (etext[1] != style)
        etext -= 2;

    // Look for each word in turn.
    while (words[0] != '\0')
    {
        // Find the end of the word.
        const char *eword = words;

        while (eword[1] != ' ' && eword[1] != '\0')
            ++eword;

        // Now search the text backwards.
        const char *wp = eword;

        for (const char *tp = etext; tp >= stext; tp -= 2)
        {
            if (tp[0] != wp[0] || tp[1] != style)
            {
                // Reset the search.
                wp = eword;
                continue;
            }

            // See if all the word has matched.
            if (wp-- == words)
                return ((tp - text) / 2) + (eword - words) + 1;
        }

        // Move to the start of the next word if there is one.
        words = eword + 1;

        if (words[0] == ' ')
            ++words;
    }

    return -1;
}


// Return true if the code page is UTF8.
bool QsciScintilla::isUtf8() const
{
    return (SendScintilla(SCI_GETCODEPAGE) == SC_CP_UTF8);
}


// Set the code page.
void QsciScintilla::setUtf8(bool cp)
{
    SendScintilla(SCI_SETCODEPAGE, (cp ? SC_CP_UTF8 : 0));
}


// Return the end-of-line mode.
QsciScintilla::EolMode QsciScintilla::eolMode() const
{
    return (EolMode)SendScintilla(SCI_GETEOLMODE);
}


// Set the end-of-line mode.
void QsciScintilla::setEolMode(EolMode mode)
{
    SendScintilla(SCI_SETEOLMODE, mode);
}


// Convert the end-of-lines to a particular mode.
void QsciScintilla::convertEols(EolMode mode)
{
    SendScintilla(SCI_CONVERTEOLS, mode);
}


// Add an edge column.
void QsciScintilla::addEdgeColumn(int colnr, const QColor &col)
{
    SendScintilla(SCI_MULTIEDGEADDLINE, colnr, col);
}


// Clear all multi-edge columns.
void QsciScintilla::clearEdgeColumns()
{
    SendScintilla(SCI_MULTIEDGECLEARALL);
}


// Return the edge colour.
QColor QsciScintilla::edgeColor() const
{
    return asQColor(SendScintilla(SCI_GETEDGECOLOUR));
}


// Set the edge colour.
void QsciScintilla::setEdgeColor(const QColor &col)
{
    SendScintilla(SCI_SETEDGECOLOUR, col);
}


// Return the edge column.
int QsciScintilla::edgeColumn() const
{
    return SendScintilla(SCI_GETEDGECOLUMN);
}


// Set the edge column.
void QsciScintilla::setEdgeColumn(int colnr)
{
    SendScintilla(SCI_SETEDGECOLUMN, colnr);
}


// Return the edge mode.
QsciScintilla::EdgeMode QsciScintilla::edgeMode() const
{
    return (EdgeMode)SendScintilla(SCI_GETEDGEMODE);
}


// Set the edge mode.
void QsciScintilla::setEdgeMode(EdgeMode mode)
{
    SendScintilla(SCI_SETEDGEMODE, mode);
}


// Return the end-of-line visibility.
bool QsciScintilla::eolVisibility() const
{
    return SendScintilla(SCI_GETVIEWEOL);
}


// Set the end-of-line visibility.
void QsciScintilla::setEolVisibility(bool visible)
{
    SendScintilla(SCI_SETVIEWEOL, visible);
}


// Return the extra ascent.
int QsciScintilla::extraAscent() const
{
    return SendScintilla(SCI_GETEXTRAASCENT);
}


// Set the extra ascent.
void QsciScintilla::setExtraAscent(int extra)
{
    SendScintilla(SCI_SETEXTRAASCENT, extra);
}


// Return the extra descent.
int QsciScintilla::extraDescent() const
{
    return SendScintilla(SCI_GETEXTRADESCENT);
}


// Set the extra descent.
void QsciScintilla::setExtraDescent(int extra)
{
    SendScintilla(SCI_SETEXTRADESCENT, extra);
}


// Return the whitespace size.
int QsciScintilla::whitespaceSize() const
{
    return SendScintilla(SCI_GETWHITESPACESIZE);
}


// Set the whitespace size.
void QsciScintilla::setWhitespaceSize(int size)
{
    SendScintilla(SCI_SETWHITESPACESIZE, size);
}


// Set the whitespace background colour.
void QsciScintilla::setWhitespaceBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_SETWHITESPACEBACK, col.isValid(), col);
}


// Set the whitespace foreground colour.
void QsciScintilla::setWhitespaceForegroundColor(const QColor &col)
{
    SendScintilla(SCI_SETWHITESPACEFORE, col.isValid(), col);
}


// Return the whitespace visibility.
QsciScintilla::WhitespaceVisibility QsciScintilla::whitespaceVisibility() const
{
    return (WhitespaceVisibility)SendScintilla(SCI_GETVIEWWS);
}


// Set the whitespace visibility.
void QsciScintilla::setWhitespaceVisibility(WhitespaceVisibility mode)
{
    SendScintilla(SCI_SETVIEWWS, mode);
}


// Return the tab draw mode.
QsciScintilla::TabDrawMode QsciScintilla::tabDrawMode() const
{
    return (TabDrawMode)SendScintilla(SCI_GETTABDRAWMODE);
}


// Set the tab draw mode.
void QsciScintilla::setTabDrawMode(TabDrawMode mode)
{
    SendScintilla(SCI_SETTABDRAWMODE, mode);
}


// Return the line wrap mode.
QsciScintilla::WrapMode QsciScintilla::wrapMode() const
{
    return (WrapMode)SendScintilla(SCI_GETWRAPMODE);
}


// Set the line wrap mode.
void QsciScintilla::setWrapMode(WrapMode mode)
{
    SendScintilla(SCI_SETLAYOUTCACHE,
            (mode == WrapNone ? SC_CACHE_CARET : SC_CACHE_DOCUMENT));
    SendScintilla(SCI_SETWRAPMODE, mode);
}


// Return the line wrap indent mode.
QsciScintilla::WrapIndentMode QsciScintilla::wrapIndentMode() const
{
    return (WrapIndentMode)SendScintilla(SCI_GETWRAPINDENTMODE);
}


// Set the line wrap indent mode.
void QsciScintilla::setWrapIndentMode(WrapIndentMode mode)
{
    SendScintilla(SCI_SETWRAPINDENTMODE, mode);
}


// Set the line wrap visual flags.
void QsciScintilla::setWrapVisualFlags(WrapVisualFlag endFlag,
        WrapVisualFlag startFlag, int indent)
{
    int flags = SC_WRAPVISUALFLAG_NONE;
    int loc = SC_WRAPVISUALFLAGLOC_DEFAULT;

    switch (endFlag)
    {
    case WrapFlagNone:
        break;

    case WrapFlagByText:
        flags |= SC_WRAPVISUALFLAG_END;
        loc |= SC_WRAPVISUALFLAGLOC_END_BY_TEXT;
        break;

    case WrapFlagByBorder:
        flags |= SC_WRAPVISUALFLAG_END;
        break;

    case WrapFlagInMargin:
        flags |= SC_WRAPVISUALFLAG_MARGIN;
        break;
    }

    switch (startFlag)
    {
    case WrapFlagNone:
        break;

    case WrapFlagByText:
        flags |= SC_WRAPVISUALFLAG_START;
        loc |= SC_WRAPVISUALFLAGLOC_START_BY_TEXT;
        break;

    case WrapFlagByBorder:
        flags |= SC_WRAPVISUALFLAG_START;
        break;

    case WrapFlagInMargin:
        flags |= SC_WRAPVISUALFLAG_MARGIN;
        break;
    }

    SendScintilla(SCI_SETWRAPVISUALFLAGS, flags);
    SendScintilla(SCI_SETWRAPVISUALFLAGSLOCATION, loc);
    SendScintilla(SCI_SETWRAPSTARTINDENT, indent);
}


// Set the folding style.
void QsciScintilla::setFolding(FoldStyle folding, int margin)
{
    fold = folding;
    foldmargin = margin;

    if (folding == NoFoldStyle)
    {
        SendScintilla(SCI_SETMARGINWIDTHN, margin, 0L);
        return;
    }

    int mask = SendScintilla(SCI_GETMODEVENTMASK);
    SendScintilla(SCI_SETMODEVENTMASK, mask | SC_MOD_CHANGEFOLD);

    SendScintilla(SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SendScintilla(SCI_SETMARGINTYPEN, margin, (long)SC_MARGIN_SYMBOL);
    SendScintilla(SCI_SETMARGINMASKN, margin, SC_MASK_FOLDERS);
    SendScintilla(SCI_SETMARGINSENSITIVEN, margin, 1);

    // Set the marker symbols to use.
    switch (folding)
    {
    case NoFoldStyle:
        break;

    case PlainFoldStyle:
        setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
        setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS);
        setFoldMarker(SC_MARKNUM_FOLDERSUB);
        setFoldMarker(SC_MARKNUM_FOLDERTAIL);
        setFoldMarker(SC_MARKNUM_FOLDEREND);
        setFoldMarker(SC_MARKNUM_FOLDEROPENMID);
        setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL);
        break;

    case CircledFoldStyle:
        setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS);
        setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS);
        setFoldMarker(SC_MARKNUM_FOLDERSUB);
        setFoldMarker(SC_MARKNUM_FOLDERTAIL);
        setFoldMarker(SC_MARKNUM_FOLDEREND);
        setFoldMarker(SC_MARKNUM_FOLDEROPENMID);
        setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL);
        break;

    case BoxedFoldStyle:
        setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
        setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
        setFoldMarker(SC_MARKNUM_FOLDERSUB);
        setFoldMarker(SC_MARKNUM_FOLDERTAIL);
        setFoldMarker(SC_MARKNUM_FOLDEREND);
        setFoldMarker(SC_MARKNUM_FOLDEROPENMID);
        setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL);
        break;

    case CircledTreeFoldStyle:
        setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS);
        setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS);
        setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
        setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE);
        setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED);
        setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED);
        setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE);
        break;

    case BoxedTreeFoldStyle:
        setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);
        setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
        setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
        setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
        setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
        setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
        setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
        break;
    }

    SendScintilla(SCI_SETMARGINWIDTHN, margin, defaultFoldMarginWidth);
}


// Clear all current folds.
void QsciScintilla::clearFolds()
{
    recolor();

    int maxLine = SendScintilla(SCI_GETLINECOUNT);

    for (int line = 0; line < maxLine; line++)
    {
        int level = SendScintilla(SCI_GETFOLDLEVEL, line);

        if (level & SC_FOLDLEVELHEADERFLAG)
        {
            SendScintilla(SCI_SETFOLDEXPANDED, line, 1);
            foldExpand(line, true, false, 0, level);
            line--;
        }
    }
}


// Set up a folder marker.
void QsciScintilla::setFoldMarker(int marknr, int mark)
{
    SendScintilla(SCI_MARKERDEFINE, marknr, mark);

    if (mark != SC_MARK_EMPTY)
    {
        SendScintilla(SCI_MARKERSETFORE, marknr, QColor(Qt::white));
        SendScintilla(SCI_MARKERSETBACK, marknr, QColor(Qt::black));
    }
}


// Handle a click in the fold margin.  This is mostly taken from SciTE.
void QsciScintilla::foldClick(int lineClick, int bstate)
{
    bool shift = bstate & Qt::ShiftModifier;
    bool ctrl = bstate & Qt::ControlModifier;

    if (shift && ctrl)
    {
        foldAll();
        return;
    }

    int levelClick = SendScintilla(SCI_GETFOLDLEVEL, lineClick);

    if (levelClick & SC_FOLDLEVELHEADERFLAG)
    {
        if (shift)
        {
            // Ensure all children are visible.
            SendScintilla(SCI_SETFOLDEXPANDED, lineClick, 1);
            foldExpand(lineClick, true, true, 100, levelClick);
        }
        else if (ctrl)
        {
            if (SendScintilla(SCI_GETFOLDEXPANDED, lineClick))
            {
                // Contract this line and all its children.
                SendScintilla(SCI_SETFOLDEXPANDED, lineClick, 0L);
                foldExpand(lineClick, false, true, 0, levelClick);
            }
            else
            {
                // Expand this line and all its children.
                SendScintilla(SCI_SETFOLDEXPANDED, lineClick, 1);
                foldExpand(lineClick, true, true, 100, levelClick);
            }
        }
        else
        {
            // Toggle this line.
            SendScintilla(SCI_TOGGLEFOLD, lineClick);
        }
    }
}


// Do the hard work of hiding and showing lines.  This is mostly taken from
// SciTE.
void QsciScintilla::foldExpand(int &line, bool doExpand, bool force,
        int visLevels, int level)
{
    int lineMaxSubord = SendScintilla(SCI_GETLASTCHILD, line,
            level & SC_FOLDLEVELNUMBERMASK);

    line++;

    while (line <= lineMaxSubord)
    {
        if (force)
        {
            if (visLevels > 0)
                SendScintilla(SCI_SHOWLINES, line, line);
            else
                SendScintilla(SCI_HIDELINES, line, line);
        }
        else if (doExpand)
            SendScintilla(SCI_SHOWLINES, line, line);

        int levelLine = level;

        if (levelLine == -1)
            levelLine = SendScintilla(SCI_GETFOLDLEVEL, line);

        if (levelLine & SC_FOLDLEVELHEADERFLAG)
        {
            if (force)
            {
                if (visLevels > 1)
                    SendScintilla(SCI_SETFOLDEXPANDED, line, 1);
                else
                    SendScintilla(SCI_SETFOLDEXPANDED, line, 0L);

                foldExpand(line, doExpand, force, visLevels - 1);
            }
            else if (doExpand)
            {
                if (!SendScintilla(SCI_GETFOLDEXPANDED, line))
                    SendScintilla(SCI_SETFOLDEXPANDED, line, 1);

                foldExpand(line, true, force, visLevels - 1);
            }
            else
                foldExpand(line, false, force, visLevels - 1);
        }
        else
            line++;
    }
}


// Fully expand (if there is any line currently folded) all text.  Otherwise,
// fold all text.  This is mostly taken from SciTE.
void QsciScintilla::foldAll(bool children)
{
    recolor();

    int maxLine = SendScintilla(SCI_GETLINECOUNT);
    bool expanding = true;

    for (int lineSeek = 0; lineSeek < maxLine; lineSeek++)
    {
        if (SendScintilla(SCI_GETFOLDLEVEL,lineSeek) & SC_FOLDLEVELHEADERFLAG)
        {
            expanding = !SendScintilla(SCI_GETFOLDEXPANDED, lineSeek);
            break;
        }
    }

    for (int line = 0; line < maxLine; line++)
    {
        int level = SendScintilla(SCI_GETFOLDLEVEL, line);

        if (!(level & SC_FOLDLEVELHEADERFLAG))
            continue;

        if (children ||
            (SC_FOLDLEVELBASE == (level & SC_FOLDLEVELNUMBERMASK)))
        {
            if (expanding)
            {
                SendScintilla(SCI_SETFOLDEXPANDED, line, 1);
                foldExpand(line, true, false, 0, level);
                line--;
            }
            else
            {
                int lineMaxSubord = SendScintilla(SCI_GETLASTCHILD, line, -1);

                SendScintilla(SCI_SETFOLDEXPANDED, line, 0L);

                if (lineMaxSubord > line)
                    SendScintilla(SCI_HIDELINES, line + 1, lineMaxSubord);
            }
        }
    }
}


// Handle a fold change.  This is mostly taken from SciTE.
void QsciScintilla::foldChanged(int line,int levelNow,int levelPrev)
{
    if (levelNow & SC_FOLDLEVELHEADERFLAG)
    {
        if (!(levelPrev & SC_FOLDLEVELHEADERFLAG))
            SendScintilla(SCI_SETFOLDEXPANDED, line, 1);
    }
    else if (levelPrev & SC_FOLDLEVELHEADERFLAG)
    {
        if (!SendScintilla(SCI_GETFOLDEXPANDED, line))
        {
            // Removing the fold from one that has been contracted so should
            // expand.  Otherwise lines are left invisible with no way to make
            // them visible.
            foldExpand(line, true, false, 0, levelPrev);
        }
    }
}


// Toggle the fold for a line if it contains a fold marker.
void QsciScintilla::foldLine(int line)
{
    SendScintilla(SCI_TOGGLEFOLD, line);
}


// Return the list of folded lines.
QList<int> QsciScintilla::contractedFolds() const
{
    QList<int> folds;
    int linenr = 0, fold_line;

    while ((fold_line = SendScintilla(SCI_CONTRACTEDFOLDNEXT, linenr)) >= 0)
    {
        folds.append(fold_line);
        linenr = fold_line + 1;
    }

    return folds;
}


// Set the fold state from a list.
void QsciScintilla::setContractedFolds(const QList<int> &folds)
{
    for (int i = 0; i < folds.count(); ++i)
    {
        int line = folds[i];
        int last_line = SendScintilla(SCI_GETLASTCHILD, line, -1);

        SendScintilla(SCI_SETFOLDEXPANDED, line, 0L);
        SendScintilla(SCI_HIDELINES, line + 1, last_line);
    }
}


// Handle the SCN_MODIFIED notification.
void QsciScintilla::handleModified(int pos, int mtype, const char *text,
        int len, int added, int line, int foldNow, int foldPrev, int token,
        int annotationLinesAdded)
{
    Q_UNUSED(pos);
    Q_UNUSED(text);
    Q_UNUSED(len);
    Q_UNUSED(token);
    Q_UNUSED(annotationLinesAdded);

    if (mtype & SC_MOD_CHANGEFOLD)
    {
        if (fold)
            foldChanged(line, foldNow, foldPrev);
    }

    if (mtype & (SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT))
    {
        emit textChanged();

        if (added != 0)
            emit linesChanged();
    }
}


// Zoom in a number of points.
void QsciScintilla::zoomIn(int range)
{
    zoomTo(SendScintilla(SCI_GETZOOM) + range);
}


// Zoom in a single point.
void QsciScintilla::zoomIn()
{
    SendScintilla(SCI_ZOOMIN);
}


// Zoom out a number of points.
void QsciScintilla::zoomOut(int range)
{
    zoomTo(SendScintilla(SCI_GETZOOM) - range);
}


// Zoom out a single point.
void QsciScintilla::zoomOut()
{
    SendScintilla(SCI_ZOOMOUT);
}


// Set the zoom to a number of points.
void QsciScintilla::zoomTo(int size)
{
    if (size < -10)
        size = -10;
    else if (size > 20)
        size = 20;

    SendScintilla(SCI_SETZOOM, size);
}


// Find the first occurrence of a string.
bool QsciScintilla::findFirst(const QString &expr, bool re, bool cs, bool wo,
        bool wrap, bool forward, int line, int index, bool show, bool posix,
        bool cxx11)
{
    if (expr.isEmpty())
    {
        findState.status = FindState::Idle;
        return false;
    }

    findState.status = FindState::Finding;
    findState.expr = expr;
    findState.wrap = wrap;
    findState.forward = forward;

    findState.flags =
        (cs ? SCFIND_MATCHCASE : 0) |
        (wo ? SCFIND_WHOLEWORD : 0) |
        (re ? SCFIND_REGEXP : 0) |
        (posix ? SCFIND_POSIX : 0) |
        (cxx11 ? SCFIND_CXX11REGEX : 0);

    if (line < 0 || index < 0)
        findState.startpos = SendScintilla(SCI_GETCURRENTPOS);
    else
        findState.startpos = positionFromLineIndex(line, index);

    if (forward)
        findState.endpos = SendScintilla(SCI_GETLENGTH);
    else
        findState.endpos = 0;

    findState.show = show;

    return doFind();
}


// Find the first occurrence of a string in the current selection.
bool QsciScintilla::findFirstInSelection(const QString &expr, bool re, bool cs,
        bool wo, bool forward, bool show, bool posix, bool cxx11)
{
    if (expr.isEmpty())
    {
        findState.status = FindState::Idle;
        return false;
    }

    findState.status = FindState::FindingInSelection;
    findState.expr = expr;
    findState.wrap = false;
    findState.forward = forward;

    findState.flags =
        (cs ? SCFIND_MATCHCASE : 0) |
        (wo ? SCFIND_WHOLEWORD : 0) |
        (re ? SCFIND_REGEXP : 0) |
        (posix ? SCFIND_POSIX : 0) |
        (cxx11 ? SCFIND_CXX11REGEX : 0);

    findState.startpos_orig = SendScintilla(SCI_GETSELECTIONSTART);
    findState.endpos_orig = SendScintilla(SCI_GETSELECTIONEND);

    if (forward)
    {
        findState.startpos = findState.startpos_orig;
        findState.endpos = findState.endpos_orig;
    }
    else
    {
        findState.startpos = findState.endpos_orig;
        findState.endpos = findState.startpos_orig;
    }

    findState.show = show;

    return doFind();
}


// Cancel any current search.
void QsciScintilla::cancelFind()
{
    findState.status = FindState::Idle;
}


// Find the next occurrence of a string.
bool QsciScintilla::findNext()
{
    if (findState.status == FindState::Idle)
        return false;

    return doFind();
}


// Do the hard work of the find methods.
bool QsciScintilla::doFind()
{
    SendScintilla(SCI_SETSEARCHFLAGS, findState.flags);

    int pos = simpleFind();

    // See if it was found.  If not and wraparound is wanted, try again.
    if (pos == -1 && findState.wrap)
    {
        if (findState.forward)
        {
            findState.startpos = 0;
            findState.endpos = SendScintilla(SCI_GETLENGTH);
        }
        else
        {
            findState.startpos = SendScintilla(SCI_GETLENGTH);
            findState.endpos = 0;
        }

        pos = simpleFind();
    }

    if (pos == -1)
    {
        // Restore the original selection.
        if (findState.status == FindState::FindingInSelection)
            SendScintilla(SCI_SETSEL, findState.startpos_orig,
                    findState.endpos_orig);

        findState.status = FindState::Idle;
        return false;
    }

    // It was found.
    long targstart = SendScintilla(SCI_GETTARGETSTART);
    long targend = SendScintilla(SCI_GETTARGETEND);

    // Ensure the text found is visible if required.
    if (findState.show)
    {
        int startLine = SendScintilla(SCI_LINEFROMPOSITION, targstart);
        int endLine = SendScintilla(SCI_LINEFROMPOSITION, targend);

        for (int i = startLine; i <= endLine; ++i)
            SendScintilla(SCI_ENSUREVISIBLEENFORCEPOLICY, i);
    }

    // Now set the selection.
    SendScintilla(SCI_SETSEL, targstart, targend);

    // Finally adjust the start position so that we don't find the same one
    // again.
    if (findState.forward)
        findState.startpos = targend;
    else if ((findState.startpos = targstart - 1) < 0)
        findState.startpos = 0;

    return true;
}


// Do a simple find between the start and end positions.
int QsciScintilla::simpleFind()
{
    if (findState.startpos == findState.endpos)
        return -1;

    SendScintilla(SCI_SETTARGETSTART, findState.startpos);
    SendScintilla(SCI_SETTARGETEND, findState.endpos);

    QByteArray s = textAsBytes(findState.expr);

    return SendScintilla(SCI_SEARCHINTARGET, s.length(), s.constData());
}


// Replace the text found with the previous find method.
void QsciScintilla::replace(const QString &replaceStr)
{
    if (findState.status == FindState::Idle)
        return;

    long start = SendScintilla(SCI_GETSELECTIONSTART);
    long orig_len = SendScintilla(SCI_GETSELECTIONEND) - start;

    SendScintilla(SCI_TARGETFROMSELECTION);

    int cmd = (findState.flags & SCFIND_REGEXP) ? SCI_REPLACETARGETRE : SCI_REPLACETARGET;

    long len = SendScintilla(cmd, -1, textAsBytes(replaceStr).constData());

    // Reset the selection.
    SendScintilla(SCI_SETSELECTIONSTART, start);
    SendScintilla(SCI_SETSELECTIONEND, start + len);

    // Fix the original selection.
    findState.endpos_orig += (len - orig_len);

    if (findState.forward)
    {
        findState.startpos = start + len;
        findState.endpos += (len - orig_len);
    }
}


// Query the modified state.
bool QsciScintilla::isModified() const
{
    return doc.isModified();
}


// Set the modified state.
void QsciScintilla::setModified(bool m)
{
    if (!m)
        SendScintilla(SCI_SETSAVEPOINT);
}


// Handle the SCN_INDICATORCLICK notification.
void QsciScintilla::handleIndicatorClick(int pos, int modifiers)
{
    int state = mapModifiers(modifiers);
    int line, index;

    lineIndexFromPosition(pos, &line, &index);

    emit indicatorClicked(line, index, Qt::KeyboardModifiers(state));
}


// Handle the SCN_INDICATORRELEASE notification.
void QsciScintilla::handleIndicatorRelease(int pos, int modifiers)
{
    int state = mapModifiers(modifiers);
    int line, index;

    lineIndexFromPosition(pos, &line, &index);

    emit indicatorReleased(line, index, Qt::KeyboardModifiers(state));
}


// Handle the SCN_MARGINCLICK notification.
void QsciScintilla::handleMarginClick(int pos, int modifiers, int margin)
{
    int state = mapModifiers(modifiers);
    int line = SendScintilla(SCI_LINEFROMPOSITION, pos);

    if (fold && margin == foldmargin)
        foldClick(line, state);
    else
        emit marginClicked(margin, line, Qt::KeyboardModifiers(state));
}


// Handle the SCN_MARGINRIGHTCLICK notification.
void QsciScintilla::handleMarginRightClick(int pos, int modifiers, int margin)
{
    int state = mapModifiers(modifiers);
    int line = SendScintilla(SCI_LINEFROMPOSITION, pos);

    emit marginRightClicked(margin, line, Qt::KeyboardModifiers(state));
}


// Handle the SCN_SAVEPOINTREACHED notification.
void QsciScintilla::handleSavePointReached()
{
    doc.setModified(false);
    emit modificationChanged(false);
}


// Handle the SCN_SAVEPOINTLEFT notification.
void QsciScintilla::handleSavePointLeft()
{
    doc.setModified(true);
    emit modificationChanged(true);
}


// Handle the QSCN_SELCHANGED signal.
void QsciScintilla::handleSelectionChanged(bool yes)
{
    selText = yes;

    emit copyAvailable(yes);
    emit selectionChanged();
}


// Get the current selection.
void QsciScintilla::getSelection(int *lineFrom, int *indexFrom, int *lineTo,
        int *indexTo) const
{
    if (selText)
    {
        lineIndexFromPosition(SendScintilla(SCI_GETSELECTIONSTART), lineFrom,
                indexFrom);
        lineIndexFromPosition(SendScintilla(SCI_GETSELECTIONEND), lineTo,
                indexTo);
    }
    else
        *lineFrom = *indexFrom = *lineTo = *indexTo = -1;
}


// Sets the current selection.
void QsciScintilla::setSelection(int lineFrom, int indexFrom, int lineTo,
        int indexTo)
{
    SendScintilla(SCI_SETSEL, positionFromLineIndex(lineFrom, indexFrom),
            positionFromLineIndex(lineTo, indexTo));
}


// Set the background colour of selected text.
void QsciScintilla::setSelectionBackgroundColor(const QColor &col)
{
    int alpha = col.alpha();

    if (alpha == 255)
        alpha = SC_ALPHA_NOALPHA;

    SendScintilla(SCI_SETSELBACK, 1, col);
    SendScintilla(SCI_SETSELALPHA, alpha);
}


// Set the foreground colour of selected text.
void QsciScintilla::setSelectionForegroundColor(const QColor &col)
{
    SendScintilla(SCI_SETSELFORE, 1, col);
}


// Reset the background colour of selected text to the default.
void QsciScintilla::resetSelectionBackgroundColor()
{
    SendScintilla(SCI_SETSELALPHA, SC_ALPHA_NOALPHA);
    SendScintilla(SCI_SETSELBACK, 0UL);
}


// Reset the foreground colour of selected text to the default.
void QsciScintilla::resetSelectionForegroundColor()
{
    SendScintilla(SCI_SETSELFORE, 0UL);
}


// Set the fill to the end-of-line for the selection.
void QsciScintilla::setSelectionToEol(bool filled)
{
    SendScintilla(SCI_SETSELEOLFILLED, filled);
}


// Return the fill to the end-of-line for the selection.
bool QsciScintilla::selectionToEol() const
{
    return SendScintilla(SCI_GETSELEOLFILLED);
}


// Set the width of the caret.
void QsciScintilla::setCaretWidth(int width)
{
    SendScintilla(SCI_SETCARETWIDTH, width);
}


// Set the width of the frame of the line containing the caret.
void QsciScintilla::setCaretLineFrameWidth(int width)
{
    SendScintilla(SCI_SETCARETLINEFRAME, width);
}


// Set the foreground colour of the caret.
void QsciScintilla::setCaretForegroundColor(const QColor &col)
{
    SendScintilla(SCI_SETCARETFORE, col);
}


// Set the background colour of the line containing the caret.
void QsciScintilla::setCaretLineBackgroundColor(const QColor &col)
{
    int alpha = col.alpha();

    if (alpha == 255)
        alpha = SC_ALPHA_NOALPHA;

    SendScintilla(SCI_SETCARETLINEBACK, col);
    SendScintilla(SCI_SETCARETLINEBACKALPHA, alpha);
}


// Set the state of the background colour of the line containing the caret.
void QsciScintilla::setCaretLineVisible(bool enable)
{
    SendScintilla(SCI_SETCARETLINEVISIBLE, enable);
}


// Set the background colour of a hotspot area.
void QsciScintilla::setHotspotBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_SETHOTSPOTACTIVEBACK, 1, col);
}


// Set the foreground colour of a hotspot area.
void QsciScintilla::setHotspotForegroundColor(const QColor &col)
{
    SendScintilla(SCI_SETHOTSPOTACTIVEFORE, 1, col);
}


// Reset the background colour of a hotspot area to the default.
void QsciScintilla::resetHotspotBackgroundColor()
{
    SendScintilla(SCI_SETHOTSPOTACTIVEBACK, 0UL);
}


// Reset the foreground colour of a hotspot area to the default.
void QsciScintilla::resetHotspotForegroundColor()
{
    SendScintilla(SCI_SETHOTSPOTACTIVEFORE, 0UL);
}


// Set the underline of a hotspot area.
void QsciScintilla::setHotspotUnderline(bool enable)
{
    SendScintilla(SCI_SETHOTSPOTACTIVEUNDERLINE, enable);
}


// Set the wrapping of a hotspot area.
void QsciScintilla::setHotspotWrap(bool enable)
{
    SendScintilla(SCI_SETHOTSPOTSINGLELINE, !enable);
}


// Query the read-only state.
bool QsciScintilla::isReadOnly() const
{
    return SendScintilla(SCI_GETREADONLY);
}


// Set the read-only state.
void QsciScintilla::setReadOnly(bool ro)
{
    setAttribute(Qt::WA_InputMethodEnabled, !ro);
    SendScintilla(SCI_SETREADONLY, ro);
}


// Append the given text.
void QsciScintilla::append(const QString &text)
{
    bool ro = ensureRW();

    QByteArray s = textAsBytes(text);
    SendScintilla(SCI_APPENDTEXT, s.length(), s.constData());

    SendScintilla(SCI_EMPTYUNDOBUFFER);

    setReadOnly(ro);
}


// Insert the given text at the current position.
void QsciScintilla::insert(const QString &text)
{
    insertAtPos(text, -1);
}


// Insert the given text at the given line and offset.
void QsciScintilla::insertAt(const QString &text, int line, int index)
{
    insertAtPos(text, positionFromLineIndex(line, index));
}


// Insert the given text at the given position.
void QsciScintilla::insertAtPos(const QString &text, int pos)
{
    bool ro = ensureRW();

    SendScintilla(SCI_BEGINUNDOACTION);
    SendScintilla(SCI_INSERTTEXT, pos, textAsBytes(text).constData());
    SendScintilla(SCI_ENDUNDOACTION);

    setReadOnly(ro);
}


// Begin a sequence of undoable actions.
void QsciScintilla::beginUndoAction()
{
    SendScintilla(SCI_BEGINUNDOACTION);
}


// End a sequence of undoable actions.
void QsciScintilla::endUndoAction()
{
    SendScintilla(SCI_ENDUNDOACTION);
}


// Redo a sequence of actions.
void QsciScintilla::redo()
{
    SendScintilla(SCI_REDO);
}


// Undo a sequence of actions.
void QsciScintilla::undo()
{
    SendScintilla(SCI_UNDO);
}


// See if there is something to redo.
bool QsciScintilla::isRedoAvailable() const
{
    return SendScintilla(SCI_CANREDO);
}


// See if there is something to undo.
bool QsciScintilla::isUndoAvailable() const
{
    return SendScintilla(SCI_CANUNDO);
}


// Return the number of lines.
int QsciScintilla::lines() const
{
    return SendScintilla(SCI_GETLINECOUNT);
}


// Return the line at a position.
int QsciScintilla::lineAt(const QPoint &pos) const
{
    long chpos = SendScintilla(SCI_POSITIONFROMPOINTCLOSE, pos.x(), pos.y());

    if (chpos < 0)
        return -1;

    return SendScintilla(SCI_LINEFROMPOSITION, chpos);
}


// Return the length of a line.
int QsciScintilla::lineLength(int line) const
{
    if (line < 0 || line >= SendScintilla(SCI_GETLINECOUNT))
        return -1;

    return SendScintilla(SCI_LINELENGTH, line);
}


// Return the length of the current text.
int QsciScintilla::length() const
{
    return SendScintilla(SCI_GETTEXTLENGTH);
}


// Remove any selected text.
void QsciScintilla::removeSelectedText()
{
    SendScintilla(SCI_REPLACESEL, "");
}


// Replace any selected text.
void QsciScintilla::replaceSelectedText(const QString &text)
{
    SendScintilla(SCI_REPLACESEL, textAsBytes(text).constData());
}


// Return the current selected text.
QString QsciScintilla::selectedText() const
{
    if (!selText)
        return QString();

    int size = SendScintilla(SCI_GETSELECTIONEND) - SendScintilla(SCI_GETSELECTIONSTART);
    char *buf = new char[size + 1];

    SendScintilla(SCI_GETSELTEXT, buf);

    QString qs = bytesAsText(buf, size);
    delete[] buf;

    return qs;
}


// Return the current text.
QString QsciScintilla::text() const
{
    int size = length();
    char *buf = new char[size + 1];

    // Note that the docs seem to be wrong and we need to ask for the length
    // plus the '\0'.
    SendScintilla(SCI_GETTEXT, size + 1, buf);

    QString qs = bytesAsText(buf, size);
    delete[] buf;

    return qs;
}


// Return the text of a line.
QString QsciScintilla::text(int line) const
{
    int size = lineLength(line);

    if (size < 1)
        return QString();

    char *buf = new char[size];

    SendScintilla(SCI_GETLINE, line, buf);

    QString qs = bytesAsText(buf, size);
    delete[] buf;

    return qs;
}


// Return the text between two positions.
QString QsciScintilla::text(int start, int end) const
{
    int size = end - start;
    char *buf = new char[size + 1];
    SendScintilla(SCI_GETTEXTRANGE, start, end, buf);
    QString text = bytesAsText(buf, size);
    delete[] buf;

    return text;
}


// Return the text as encoded bytes between two positions.
QByteArray QsciScintilla::bytes(int start, int end) const
{
    QByteArray bytes(end - start + 1, '\0');

    SendScintilla(SCI_GETTEXTRANGE, start, end, bytes.data());

    return bytes;
}


// Set the given text.
void QsciScintilla::setText(const QString &text)
{
    bool ro = ensureRW();

    SendScintilla(SCI_CLEARALL);
    QByteArray bytes = textAsBytes(text);
    SendScintilla(SCI_ADDTEXT, bytes.size(), bytes.constData());
    SendScintilla(SCI_EMPTYUNDOBUFFER);

    setReadOnly(ro);
}


// Get the cursor position
void QsciScintilla::getCursorPosition(int *line, int *index) const
{
    lineIndexFromPosition(SendScintilla(SCI_GETCURRENTPOS), line, index);
}


// Set the cursor position
void QsciScintilla::setCursorPosition(int line, int index)
{
    SendScintilla(SCI_GOTOPOS, positionFromLineIndex(line, index));
}


// Ensure the cursor is visible.
void QsciScintilla::ensureCursorVisible()
{
    SendScintilla(SCI_SCROLLCARET);
}


// Ensure a line is visible.
void QsciScintilla::ensureLineVisible(int line)
{
    SendScintilla(SCI_ENSUREVISIBLEENFORCEPOLICY, line);
}


// Copy text to the clipboard.
void QsciScintilla::copy()
{
    SendScintilla(SCI_COPY);
}


// Cut text to the clipboard.
void QsciScintilla::cut()
{
    SendScintilla(SCI_CUT);
}


// Paste text from the clipboard.
void QsciScintilla::paste()
{
    SendScintilla(SCI_PASTE);
}


// Select all text, or deselect any selected text.
void QsciScintilla::selectAll(bool select)
{
    if (select)
        SendScintilla(SCI_SELECTALL);
    else
        SendScintilla(SCI_SETANCHOR, SendScintilla(SCI_GETCURRENTPOS));
}


// Delete all text.
void QsciScintilla::clear()
{
    bool ro = ensureRW();

    SendScintilla(SCI_CLEARALL);
    SendScintilla(SCI_EMPTYUNDOBUFFER);

    setReadOnly(ro);
}


// Return the indentation of a line.
int QsciScintilla::indentation(int line) const
{
    return SendScintilla(SCI_GETLINEINDENTATION, line);
}


// Set the indentation of a line.
void QsciScintilla::setIndentation(int line, int indentation)
{
    SendScintilla(SCI_BEGINUNDOACTION);
    SendScintilla(SCI_SETLINEINDENTATION, line, indentation);
    SendScintilla(SCI_ENDUNDOACTION);
}


// Indent a line.
void QsciScintilla::indent(int line)
{
    setIndentation(line, indentation(line) + indentWidth());
}


// Unindent a line.
void QsciScintilla::unindent(int line)
{
    int newIndent = indentation(line) - indentWidth();

    if (newIndent < 0)
        newIndent = 0;

    setIndentation(line, newIndent);
}


// Return the indentation of the current line.
int QsciScintilla::currentIndent() const
{
    return indentation(SendScintilla(SCI_LINEFROMPOSITION,
                SendScintilla(SCI_GETCURRENTPOS)));
}


// Return the current indentation width.
int QsciScintilla::indentWidth() const
{
    int w = indentationWidth();

    if (w == 0)
        w = tabWidth();

    return w;
}


// Return the state of indentation guides.
bool QsciScintilla::indentationGuides() const
{
    return (SendScintilla(SCI_GETINDENTATIONGUIDES) != SC_IV_NONE);
}


// Enable and disable indentation guides.
void QsciScintilla::setIndentationGuides(bool enable)
{
    int iv;

    if (!enable)
        iv = SC_IV_NONE;
    else if (lex.isNull())
        iv = SC_IV_REAL;
    else
        iv = lex->indentationGuideView();

    SendScintilla(SCI_SETINDENTATIONGUIDES, iv);
}


// Set the background colour of indentation guides.
void QsciScintilla::setIndentationGuidesBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETBACK, STYLE_INDENTGUIDE, col);
}


// Set the foreground colour of indentation guides.
void QsciScintilla::setIndentationGuidesForegroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETFORE, STYLE_INDENTGUIDE, col);
}


// Return the indentation width.
int QsciScintilla::indentationWidth() const
{
    return SendScintilla(SCI_GETINDENT);
}


// Set the indentation width.
void QsciScintilla::setIndentationWidth(int width)
{
    SendScintilla(SCI_SETINDENT, width);
}


// Return the tab width.
int QsciScintilla::tabWidth() const
{
    return SendScintilla(SCI_GETTABWIDTH);
}


// Set the tab width.
void QsciScintilla::setTabWidth(int width)
{
    SendScintilla(SCI_SETTABWIDTH, width);
}


// Return the effect of the backspace key.
bool QsciScintilla::backspaceUnindents() const
{
    return SendScintilla(SCI_GETBACKSPACEUNINDENTS);
}


// Set the effect of the backspace key.
void QsciScintilla::setBackspaceUnindents(bool unindents)
{
    SendScintilla(SCI_SETBACKSPACEUNINDENTS, unindents);
}


// Return the effect of the tab key.
bool QsciScintilla::tabIndents() const
{
    return SendScintilla(SCI_GETTABINDENTS);
}


// Set the effect of the tab key.
void QsciScintilla::setTabIndents(bool indents)
{
    SendScintilla(SCI_SETTABINDENTS, indents);
}


// Return the indentation use of tabs.
bool QsciScintilla::indentationsUseTabs() const
{
    return SendScintilla(SCI_GETUSETABS);
}


// Set the indentation use of tabs.
void QsciScintilla::setIndentationsUseTabs(bool tabs)
{
    SendScintilla(SCI_SETUSETABS, tabs);
}


// Return the number of margins.
int QsciScintilla::margins() const
{
    return SendScintilla(SCI_GETMARGINS);
}


// Set the number of margins.
void QsciScintilla::setMargins(int margins)
{
    SendScintilla(SCI_SETMARGINS, margins);
}


// Return the margin background colour.
QColor QsciScintilla::marginBackgroundColor(int margin) const
{
    return asQColor(SendScintilla(SCI_GETMARGINBACKN, margin));
}


// Set the margin background colour.
void QsciScintilla::setMarginBackgroundColor(int margin, const QColor &col)
{
    SendScintilla(SCI_SETMARGINBACKN, margin, col);
}


// Return the margin options.
int QsciScintilla::marginOptions() const
{
    return SendScintilla(SCI_GETMARGINOPTIONS);
}


// Set the margin options.
void QsciScintilla::setMarginOptions(int options)
{
    SendScintilla(SCI_SETMARGINOPTIONS, options);
}


// Return the margin type.
QsciScintilla::MarginType QsciScintilla::marginType(int margin) const
{
    return (MarginType)SendScintilla(SCI_GETMARGINTYPEN, margin);
}


// Set the margin type.
void QsciScintilla::setMarginType(int margin, QsciScintilla::MarginType type)
{
    SendScintilla(SCI_SETMARGINTYPEN, margin, type);
}


// Clear margin text.
void QsciScintilla::clearMarginText(int line)
{
    if (line < 0)
        SendScintilla(SCI_MARGINTEXTCLEARALL);
    else
        SendScintilla(SCI_MARGINSETTEXT, line, (const char *)0);
}


// Annotate a line.
void QsciScintilla::setMarginText(int line, const QString &text, int style)
{
    int style_offset = SendScintilla(SCI_MARGINGETSTYLEOFFSET);

    SendScintilla(SCI_MARGINSETTEXT, line, textAsBytes(text).constData());
    SendScintilla(SCI_MARGINSETSTYLE, line, style - style_offset);
}


// Annotate a line.
void QsciScintilla::setMarginText(int line, const QString &text, const QsciStyle &style)
{
    style.apply(this);

    setMarginText(line, text, style.style());
}


// Annotate a line.
void QsciScintilla::setMarginText(int line, const QsciStyledText &text)
{
    text.apply(this);

    setMarginText(line, text.text(), text.style());
}


// Annotate a line.
void QsciScintilla::setMarginText(int line, const QList<QsciStyledText> &text)
{
    char *styles;
    QByteArray styled_text = styleText(text, &styles,
            SendScintilla(SCI_MARGINGETSTYLEOFFSET));

    SendScintilla(SCI_MARGINSETTEXT, line, styled_text.constData());
    SendScintilla(SCI_MARGINSETSTYLES, line, styles);

    delete[] styles;
}


// Return the state of line numbers in a margin.
bool QsciScintilla::marginLineNumbers(int margin) const
{
    return SendScintilla(SCI_GETMARGINTYPEN, margin);
}


// Enable and disable line numbers in a margin.
void QsciScintilla::setMarginLineNumbers(int margin, bool lnrs)
{
    SendScintilla(SCI_SETMARGINTYPEN, margin,
            lnrs ? SC_MARGIN_NUMBER : SC_MARGIN_SYMBOL);
}


// Return the marker mask of a margin.
int QsciScintilla::marginMarkerMask(int margin) const
{
    return SendScintilla(SCI_GETMARGINMASKN, margin);
}


// Set the marker mask of a margin.
void QsciScintilla::setMarginMarkerMask(int margin,int mask)
{
    SendScintilla(SCI_SETMARGINMASKN, margin, mask);
}


// Return the state of a margin's sensitivity.
bool QsciScintilla::marginSensitivity(int margin) const
{
    return SendScintilla(SCI_GETMARGINSENSITIVEN, margin);
}


// Enable and disable a margin's sensitivity.
void QsciScintilla::setMarginSensitivity(int margin,bool sens)
{
    SendScintilla(SCI_SETMARGINSENSITIVEN, margin, sens);
}


// Return the width of a margin.
int QsciScintilla::marginWidth(int margin) const
{
    return SendScintilla(SCI_GETMARGINWIDTHN, margin);
}


// Set the width of a margin.
void QsciScintilla::setMarginWidth(int margin, int width)
{
    SendScintilla(SCI_SETMARGINWIDTHN, margin, width);
}


// Set the width of a margin to the width of some text.
void QsciScintilla::setMarginWidth(int margin, const QString &s)
{
    int width = SendScintilla(SCI_TEXTWIDTH, STYLE_LINENUMBER,
            textAsBytes(s).constData());

    setMarginWidth(margin, width);
}


// Set the background colour of all margins.
void QsciScintilla::setMarginsBackgroundColor(const QColor &col)
{
    handleStylePaperChange(col, STYLE_LINENUMBER);
}


// Set the foreground colour of all margins.
void QsciScintilla::setMarginsForegroundColor(const QColor &col)
{
    handleStyleColorChange(col, STYLE_LINENUMBER);
}


// Set the font of all margins.
void QsciScintilla::setMarginsFont(const QFont &f)
{
    setStylesFont(f, STYLE_LINENUMBER);
}


// Define an indicator.
int QsciScintilla::indicatorDefine(IndicatorStyle style, int indicatorNumber)
{
    checkIndicator(indicatorNumber);

    if (indicatorNumber >= 0)
        SendScintilla(SCI_INDICSETSTYLE, indicatorNumber,
                static_cast<long>(style));

    return indicatorNumber;
}


// Return the state of an indicator being drawn under the text.
bool QsciScintilla::indicatorDrawUnder(int indicatorNumber) const
{
    if (indicatorNumber < 0 || indicatorNumber >= INDIC_IME)
        return false;

    return SendScintilla(SCI_INDICGETUNDER, indicatorNumber);
}


// Set the state of indicators being drawn under the text.
void QsciScintilla::setIndicatorDrawUnder(bool under, int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
                SendScintilla(SCI_INDICSETUNDER, i, under);
        }
        else
        {
            SendScintilla(SCI_INDICSETUNDER, indicatorNumber, under);
        }
    }
}


// Set the indicator foreground colour.
void QsciScintilla::setIndicatorForegroundColor(const QColor &col,
        int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        int alpha = col.alpha();

        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
            {
                SendScintilla(SCI_INDICSETFORE, i, col);
                SendScintilla(SCI_INDICSETALPHA, i, alpha);
            }
        }
        else
        {
            SendScintilla(SCI_INDICSETFORE, indicatorNumber, col);
            SendScintilla(SCI_INDICSETALPHA, indicatorNumber, alpha);
        }
    }
}


// Set the indicator hover foreground colour.
void QsciScintilla::setIndicatorHoverForegroundColor(const QColor &col,
        int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
                SendScintilla(SCI_INDICSETHOVERFORE, i, col);
        }
        else
        {
            SendScintilla(SCI_INDICSETHOVERFORE, indicatorNumber, col);
        }
    }
}


// Set the indicator hover style.
void QsciScintilla::setIndicatorHoverStyle(IndicatorStyle style,
        int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
                SendScintilla(SCI_INDICSETHOVERSTYLE, i,
                        static_cast<long>(style));
        }
        else
        {
            SendScintilla(SCI_INDICSETHOVERSTYLE, indicatorNumber,
                    static_cast<long>(style));
        }
    }
}


// Set the indicator outline colour.
void QsciScintilla::setIndicatorOutlineColor(const QColor &col, int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        int alpha = col.alpha();

        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
                SendScintilla(SCI_INDICSETOUTLINEALPHA, i, alpha);
        }
        else
        {
            SendScintilla(SCI_INDICSETOUTLINEALPHA, indicatorNumber, alpha);
        }
    }
}


// Fill a range with an indicator.
void QsciScintilla::fillIndicatorRange(int lineFrom, int indexFrom,
        int lineTo, int indexTo, int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        int start = positionFromLineIndex(lineFrom, indexFrom);
        int finish = positionFromLineIndex(lineTo, indexTo);

        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
            {
                SendScintilla(SCI_SETINDICATORCURRENT, i);
                SendScintilla(SCI_INDICATORFILLRANGE, start, finish - start);
            }
        }
        else
        {
            SendScintilla(SCI_SETINDICATORCURRENT, indicatorNumber);
            SendScintilla(SCI_INDICATORFILLRANGE, start, finish - start);
        }
    }
}


// Clear a range with an indicator.
void QsciScintilla::clearIndicatorRange(int lineFrom, int indexFrom,
        int lineTo, int indexTo, int indicatorNumber)
{
    if (indicatorNumber < INDIC_IME)
    {
        int start = positionFromLineIndex(lineFrom, indexFrom);
        int finish = positionFromLineIndex(lineTo, indexTo);

        // We ignore allocatedIndicators to allow any indicators defined
        // elsewhere (e.g. in lexers) to be set.
        if (indicatorNumber < 0)
        {
            for (int i = 0; i < INDIC_IME; ++i)
            {
                SendScintilla(SCI_SETINDICATORCURRENT, i);
                SendScintilla(SCI_INDICATORCLEARRANGE, start, finish - start);
            }
        }
        else
        {
            SendScintilla(SCI_SETINDICATORCURRENT, indicatorNumber);
            SendScintilla(SCI_INDICATORCLEARRANGE, start, finish - start);
        }
    }
}


// Define a marker based on a symbol.
int QsciScintilla::markerDefine(MarkerSymbol sym, int markerNumber)
{
    checkMarker(markerNumber);

    if (markerNumber >= 0)
        SendScintilla(SCI_MARKERDEFINE, markerNumber, static_cast<long>(sym));

    return markerNumber;
}


// Define a marker based on a character.
int QsciScintilla::markerDefine(char ch, int markerNumber)
{
    checkMarker(markerNumber);

    if (markerNumber >= 0)
        SendScintilla(SCI_MARKERDEFINE, markerNumber,
                static_cast<long>(SC_MARK_CHARACTER) + ch);

    return markerNumber;
}


// Define a marker based on a QPixmap.
int QsciScintilla::markerDefine(const QPixmap &pm, int markerNumber)
{
    checkMarker(markerNumber);

    if (markerNumber >= 0)
        SendScintilla(SCI_MARKERDEFINEPIXMAP, markerNumber, pm);

    return markerNumber;
}


// Define a marker based on a QImage.
int QsciScintilla::markerDefine(const QImage &im, int markerNumber)
{
    checkMarker(markerNumber);

    if (markerNumber >= 0)
    {
        SendScintilla(SCI_RGBAIMAGESETHEIGHT, im.height());
        SendScintilla(SCI_RGBAIMAGESETWIDTH, im.width());
        SendScintilla(SCI_MARKERDEFINERGBAIMAGE, markerNumber, im);
    }

    return markerNumber;
}


// Add a marker to a line.
int QsciScintilla::markerAdd(int linenr, int markerNumber)
{
    if (markerNumber < 0 || markerNumber > MARKER_MAX || (allocatedMarkers & (1 << markerNumber)) == 0)
        return -1;

    return SendScintilla(SCI_MARKERADD, linenr, markerNumber);
}


// Get the marker mask for a line.
unsigned QsciScintilla::markersAtLine(int linenr) const
{
    return SendScintilla(SCI_MARKERGET, linenr);
}


// Delete a marker from a line.
void QsciScintilla::markerDelete(int linenr, int markerNumber)
{
    if (markerNumber <= MARKER_MAX)
    {
        if (markerNumber < 0)
        {
            unsigned am = allocatedMarkers;

            for (int m = 0; m <= MARKER_MAX; ++m)
            {
                if (am & 1)
                    SendScintilla(SCI_MARKERDELETE, linenr, m);

                am >>= 1;
            }
        }
        else if (allocatedMarkers & (1 << markerNumber))
            SendScintilla(SCI_MARKERDELETE, linenr, markerNumber);
    }
}


// Delete a marker from the text.
void QsciScintilla::markerDeleteAll(int markerNumber)
{
    if (markerNumber <= MARKER_MAX)
    {
        if (markerNumber < 0)
            SendScintilla(SCI_MARKERDELETEALL, -1);
        else if (allocatedMarkers & (1 << markerNumber))
            SendScintilla(SCI_MARKERDELETEALL, markerNumber);
    }
}


// Delete a marker handle from the text.
void QsciScintilla::markerDeleteHandle(int mhandle)
{
    SendScintilla(SCI_MARKERDELETEHANDLE, mhandle);
}


// Return the line containing a marker instance.
int QsciScintilla::markerLine(int mhandle) const
{
    return SendScintilla(SCI_MARKERLINEFROMHANDLE, mhandle);
}


// Search forwards for a marker.
int QsciScintilla::markerFindNext(int linenr, unsigned mask) const
{
    return SendScintilla(SCI_MARKERNEXT, linenr, mask);
}


// Search backwards for a marker.
int QsciScintilla::markerFindPrevious(int linenr, unsigned mask) const
{
    return SendScintilla(SCI_MARKERPREVIOUS, linenr, mask);
}


// Set the marker background colour.
void QsciScintilla::setMarkerBackgroundColor(const QColor &col, int markerNumber)
{
    if (markerNumber <= MARKER_MAX)
    {
        int alpha = col.alpha();

        // An opaque background would make the text invisible.
        if (alpha == 255)
            alpha = SC_ALPHA_NOALPHA;

        if (markerNumber < 0)
        {
            unsigned am = allocatedMarkers;

            for (int m = 0; m <= MARKER_MAX; ++m)
            {
                if (am & 1)
                {
                    SendScintilla(SCI_MARKERSETBACK, m, col);
                    SendScintilla(SCI_MARKERSETALPHA, m, alpha);
                }

                am >>= 1;
            }
        }
        else if (allocatedMarkers & (1 << markerNumber))
        {
            SendScintilla(SCI_MARKERSETBACK, markerNumber, col);
            SendScintilla(SCI_MARKERSETALPHA, markerNumber, alpha);
        }
    }
}


// Set the marker foreground colour.
void QsciScintilla::setMarkerForegroundColor(const QColor &col, int markerNumber)
{
    if (markerNumber <= MARKER_MAX)
    {
        if (markerNumber < 0)
        {
            unsigned am = allocatedMarkers;

            for (int m = 0; m <= MARKER_MAX; ++m)
            {
                if (am & 1)
                    SendScintilla(SCI_MARKERSETFORE, m, col);

                am >>= 1;
            }
        }
        else if (allocatedMarkers & (1 << markerNumber))
        {
            SendScintilla(SCI_MARKERSETFORE, markerNumber, col);
        }
    }
}


// Check a marker, allocating a marker number if necessary.
void QsciScintilla::checkMarker(int &markerNumber)
{
    allocateId(markerNumber, allocatedMarkers, 0, MARKER_MAX);
}


// Check an indicator, allocating an indicator number if necessary.
void QsciScintilla::checkIndicator(int &indicatorNumber)
{
    allocateId(indicatorNumber, allocatedIndicators, INDIC_CONTAINER,
            INDIC_IME - 1);
}


// Make sure an identifier is valid and allocate it if necessary.
void QsciScintilla::allocateId(int &id, unsigned &allocated, int min, int max)
{
    if (id >= 0)
    {
        // Note that we allow existing identifiers to be explicitly redefined.
        if (id > max)
            id = -1;
    }
    else
    {
        unsigned aids = allocated >> min;

        // Find the smallest unallocated identifier.
        for (id = min; id <= max; ++id)
        {
            if ((aids & 1) == 0)
                break;

            aids >>= 1;
        }
    }

    // Allocate the identifier if it is valid.
    if (id >= 0)
        allocated |= (1 << id);
}


// Reset the fold margin colours.
void QsciScintilla::resetFoldMarginColors()
{
    SendScintilla(SCI_SETFOLDMARGINHICOLOUR, 0, 0L);
    SendScintilla(SCI_SETFOLDMARGINCOLOUR, 0, 0L);
}


// Set the fold margin colours.
void QsciScintilla::setFoldMarginColors(const QColor &fore, const QColor &back)
{
    SendScintilla(SCI_SETFOLDMARGINHICOLOUR, 1, fore);
    SendScintilla(SCI_SETFOLDMARGINCOLOUR, 1, back);
}


// Set the call tips background colour.
void QsciScintilla::setCallTipsBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_CALLTIPSETBACK, col);
}


// Set the call tips foreground colour.
void QsciScintilla::setCallTipsForegroundColor(const QColor &col)
{
    SendScintilla(SCI_CALLTIPSETFORE, col);
}


// Set the call tips highlight colour.
void QsciScintilla::setCallTipsHighlightColor(const QColor &col)
{
    SendScintilla(SCI_CALLTIPSETFOREHLT, col);
}


// Set the matched brace background colour.
void QsciScintilla::setMatchedBraceBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETBACK, STYLE_BRACELIGHT, col);
}


// Set the matched brace foreground colour.
void QsciScintilla::setMatchedBraceForegroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETFORE, STYLE_BRACELIGHT, col);
}


// Set the matched brace indicator.
void QsciScintilla::setMatchedBraceIndicator(int indicatorNumber)
{
    SendScintilla(SCI_BRACEHIGHLIGHTINDICATOR, 1, indicatorNumber);
}


// Reset the matched brace indicator.
void QsciScintilla::resetMatchedBraceIndicator()
{
    SendScintilla(SCI_BRACEHIGHLIGHTINDICATOR, 0, 0L);
}


// Set the unmatched brace background colour.
void QsciScintilla::setUnmatchedBraceBackgroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETBACK, STYLE_BRACEBAD, col);
}


// Set the unmatched brace foreground colour.
void QsciScintilla::setUnmatchedBraceForegroundColor(const QColor &col)
{
    SendScintilla(SCI_STYLESETFORE, STYLE_BRACEBAD, col);
}


// Set the unmatched brace indicator.
void QsciScintilla::setUnmatchedBraceIndicator(int indicatorNumber)
{
    SendScintilla(SCI_BRACEBADLIGHTINDICATOR, 1, indicatorNumber);
}


// Reset the unmatched brace indicator.
void QsciScintilla::resetUnmatchedBraceIndicator()
{
    SendScintilla(SCI_BRACEBADLIGHTINDICATOR, 0, 0L);
}


// Detach any lexer.
void QsciScintilla::detachLexer()
{
    if (!lex.isNull())
    {
        lex->setEditor(0);
        lex->disconnect(this);

        SendScintilla(SCI_STYLERESETDEFAULT);
        SendScintilla(SCI_STYLECLEARALL);
    }
}


// Set the lexer.
void QsciScintilla::setLexer(QsciLexer *lexer)
{
    // Detach any current lexer.
    detachLexer();

    // Connect up the new lexer.
    lex = lexer;

    if (lex)
    {
        SendScintilla(SCI_CLEARDOCUMENTSTYLE);

        if (lex->lexer())
            SendScintilla(SCI_SETLEXERLANGUAGE, lex->lexer());
        else
            SendScintilla(SCI_SETLEXER, lex->lexerId());

        lex->setEditor(this);

        connect(lex,SIGNAL(colorChanged(const QColor &, int)),
                SLOT(handleStyleColorChange(const QColor &, int)));
        connect(lex,SIGNAL(eolFillChanged(bool, int)),
                SLOT(handleStyleEolFillChange(bool, int)));
        connect(lex,SIGNAL(fontChanged(const QFont &, int)),
                SLOT(handleStyleFontChange(const QFont &, int)));
        connect(lex,SIGNAL(paperChanged(const QColor &, int)),
                SLOT(handleStylePaperChange(const QColor &, int)));
        connect(lex,SIGNAL(propertyChanged(const char *, const char *)),
                SLOT(handlePropertyChange(const char *, const char *)));

        SendScintilla(SCI_SETPROPERTY, "fold", "1");
        SendScintilla(SCI_SETPROPERTY, "fold.html", "1");

        // Set the keywords.  Scintilla allows for sets numbered 0 to
        // KEYWORDSET_MAX (although the lexers only seem to exploit 0 to
        // KEYWORDSET_MAX - 1).  We number from 1 in line with SciTE's property
        // files.
        for (int k = 0; k <= KEYWORDSET_MAX; ++k)
        {
            const char *kw = lex -> keywords(k + 1);

            if (!kw)
                kw = "";

            SendScintilla(SCI_SETKEYWORDS, k, kw);
        }

        // Initialise each style.  Do the default first so its (possibly
        // incorrect) font setting gets reset when style 0 is set.
        setLexerStyle(STYLE_DEFAULT);

        for (int s = 0; s <= STYLE_MAX; ++s)
            if (!lex->description(s).isEmpty())
                setLexerStyle(s);

        // Initialise the properties.
        lex->refreshProperties();

        // Set the auto-completion fillups and word separators.
        setAutoCompletionFillupsEnabled(fillups_enabled);
        wseps = lex->autoCompletionWordSeparators();

        wchars = lex->wordCharacters();

        if (!wchars)
            wchars = defaultWordChars;

        SendScintilla(SCI_AUTOCSETIGNORECASE, !lex->caseSensitive());

        recolor();
    }
    else
    {
        SendScintilla(SCI_SETLEXER, SCLEX_CONTAINER);

        setColor(nl_text_colour);
        setPaper(nl_paper_colour);

        SendScintilla(SCI_AUTOCSETFILLUPS, "");
        SendScintilla(SCI_AUTOCSETIGNORECASE, false);
        wseps.clear();
        wchars = defaultWordChars;
    }
}


// Set a particular style of the current lexer.
void QsciScintilla::setLexerStyle(int style)
{
    handleStyleColorChange(lex->color(style), style);
    handleStyleEolFillChange(lex->eolFill(style), style);
    handleStyleFontChange(lex->font(style), style);
    handleStylePaperChange(lex->paper(style), style);
}


// Get the current lexer.
QsciLexer *QsciScintilla::lexer() const
{
    return lex;
}


// Handle a change in lexer style foreground colour.
void QsciScintilla::handleStyleColorChange(const QColor &c, int style)
{
    SendScintilla(SCI_STYLESETFORE, style, c);
}


// Handle a change in lexer style end-of-line fill.
void QsciScintilla::handleStyleEolFillChange(bool eolfill, int style)
{
    SendScintilla(SCI_STYLESETEOLFILLED, style, eolfill);
}


// Handle a change in lexer style font.
void QsciScintilla::handleStyleFontChange(const QFont &f, int style)
{
    setStylesFont(f, style);

    if (style == lex->braceStyle())
    {
        setStylesFont(f, STYLE_BRACELIGHT);
        setStylesFont(f, STYLE_BRACEBAD);
    }
}


// Set the font for a style.
void QsciScintilla::setStylesFont(const QFont &f, int style)
{
    SendScintilla(SCI_STYLESETFONT, style, f.family().toLatin1().data());
    SendScintilla(SCI_STYLESETSIZEFRACTIONAL, style,
            long(f.pointSizeF() * SC_FONT_SIZE_MULTIPLIER));

    // Pass the Qt weight via the back door.
    SendScintilla(SCI_STYLESETWEIGHT, style, -f.weight());

    SendScintilla(SCI_STYLESETITALIC, style, f.italic());
    SendScintilla(SCI_STYLESETUNDERLINE, style, f.underline());

    // Tie the font settings of the default style to that of style 0 (the style
    // conventionally used for whitespace by lexers).  This is needed so that
    // fold marks, indentations, edge columns etc are set properly.
    if (style == 0)
        setStylesFont(f, STYLE_DEFAULT);
}


// Handle a change in lexer style background colour.
void QsciScintilla::handleStylePaperChange(const QColor &c, int style)
{
    SendScintilla(SCI_STYLESETBACK, style, c);
}


// Handle a change in lexer property.
void QsciScintilla::handlePropertyChange(const char *prop, const char *val)
{
    SendScintilla(SCI_SETPROPERTY, prop, val);
}


// Handle a change to the user visible user interface.
void QsciScintilla::handleUpdateUI(int)
{
    int newPos = SendScintilla(SCI_GETCURRENTPOS);

    if (newPos != oldPos)
    {
        oldPos = newPos;

        int line = SendScintilla(SCI_LINEFROMPOSITION, newPos);
        int col = SendScintilla(SCI_GETCOLUMN, newPos);

        emit cursorPositionChanged(line, col);
    }

    if (braceMode != NoBraceMatch)
        braceMatch();
}


// Handle brace matching.
void QsciScintilla::braceMatch()
{
    long braceAtCaret, braceOpposite;

    findMatchingBrace(braceAtCaret, braceOpposite, braceMode);

    if (braceAtCaret >= 0 && braceOpposite < 0)
    {
        SendScintilla(SCI_BRACEBADLIGHT, braceAtCaret);
        SendScintilla(SCI_SETHIGHLIGHTGUIDE, 0UL);
    }
    else
    {
        char chBrace = SendScintilla(SCI_GETCHARAT, braceAtCaret);

        SendScintilla(SCI_BRACEHIGHLIGHT, braceAtCaret, braceOpposite);

        long columnAtCaret = SendScintilla(SCI_GETCOLUMN, braceAtCaret);
        long columnOpposite = SendScintilla(SCI_GETCOLUMN, braceOpposite);

        if (chBrace == ':')
        {
            long lineStart = SendScintilla(SCI_LINEFROMPOSITION, braceAtCaret);
            long indentPos = SendScintilla(SCI_GETLINEINDENTPOSITION,
                    lineStart);
            long indentPosNext = SendScintilla(SCI_GETLINEINDENTPOSITION,
                    lineStart + 1);

            columnAtCaret = SendScintilla(SCI_GETCOLUMN, indentPos);

            long columnAtCaretNext = SendScintilla(SCI_GETCOLUMN,
                    indentPosNext);
            long indentSize = SendScintilla(SCI_GETINDENT);

            if (columnAtCaretNext - indentSize > 1)
                columnAtCaret = columnAtCaretNext - indentSize;

            if (columnOpposite == 0)
                columnOpposite = columnAtCaret;
        }

        long column = columnAtCaret;

        if (column > columnOpposite)
            column = columnOpposite;

        SendScintilla(SCI_SETHIGHLIGHTGUIDE, column);
    }
}


// Check if the character at a position is a brace.
long QsciScintilla::checkBrace(long pos, int brace_style, bool &colonMode)
{
    long brace_pos = -1;
    char ch = SendScintilla(SCI_GETCHARAT, pos);

    if (ch == ':')
    {
        // A bit of a hack, we should really use a virtual.
        if (!lex.isNull() && qstrcmp(lex->lexer(), "python") == 0)
        {
            brace_pos = pos;
            colonMode = true;
        }
    }
    else if (ch && strchr("[](){}<>", ch))
    {
        if (brace_style < 0)
            brace_pos = pos;
        else
        {
            int style = SendScintilla(SCI_GETSTYLEAT, pos) & 0x1f;

            if (style == brace_style)
                brace_pos = pos;
        }
    }

    return brace_pos;
}


// Find a brace and it's match.  Return true if the current position is inside
// a pair of braces.
bool QsciScintilla::findMatchingBrace(long &brace, long &other, BraceMatch mode)
{
    bool colonMode = false;
    int brace_style = (lex.isNull() ? -1 : lex->braceStyle());

    brace = -1;
    other = -1;

    long caretPos = SendScintilla(SCI_GETCURRENTPOS);

    if (caretPos > 0)
        brace = checkBrace(caretPos - 1, brace_style, colonMode);

    bool isInside = false;

    if (brace < 0 && mode == SloppyBraceMatch)
    {
        brace = checkBrace(caretPos, brace_style, colonMode);

        if (brace >= 0 && !colonMode)
            isInside = true;
    }

    if (brace >= 0)
    {
        if (colonMode)
        {
            // Find the end of the Python indented block.
            long lineStart = SendScintilla(SCI_LINEFROMPOSITION, brace);
            long lineMaxSubord = SendScintilla(SCI_GETLASTCHILD, lineStart, -1);

            other = SendScintilla(SCI_GETLINEENDPOSITION, lineMaxSubord);
        }
        else
            other = SendScintilla(SCI_BRACEMATCH, brace, 0L);

        if (other > brace)
            isInside = !isInside;
    }

    return isInside;
}


// Move to the matching brace.
void QsciScintilla::moveToMatchingBrace()
{
    gotoMatchingBrace(false);
}


// Select to the matching brace.
void QsciScintilla::selectToMatchingBrace()
{
    gotoMatchingBrace(true);
}


// Move to the matching brace and optionally select the text.
void QsciScintilla::gotoMatchingBrace(bool select)
{
    long braceAtCaret;
    long braceOpposite;

    bool isInside = findMatchingBrace(braceAtCaret, braceOpposite,
            SloppyBraceMatch);

    if (braceOpposite >= 0)
    {
        // Convert the character positions into caret positions based on
        // whether the caret position was inside or outside the braces.
        if (isInside)
        {
            if (braceOpposite > braceAtCaret)
                braceAtCaret++;
            else
                braceOpposite++;
        }
        else
        {
            if (braceOpposite > braceAtCaret)
                braceOpposite++;
            else
                braceAtCaret++;
        }

        ensureLineVisible(SendScintilla(SCI_LINEFROMPOSITION, braceOpposite));

        if (select)
            SendScintilla(SCI_SETSEL, braceAtCaret, braceOpposite);
        else
            SendScintilla(SCI_SETSEL, braceOpposite, braceOpposite);
    }
}


// Return a position from a line number and an index within the line.
int QsciScintilla::positionFromLineIndex(int line, int index) const
{
    int pos = SendScintilla(SCI_POSITIONFROMLINE, line);

    // Allow for multi-byte characters.
    for(int i = 0; i < index; i++)
        pos = SendScintilla(SCI_POSITIONAFTER, pos);

    return pos;
}


// Return a line number and an index within the line from a position.
void QsciScintilla::lineIndexFromPosition(int position, int *line, int *index) const
{
    int lin = SendScintilla(SCI_LINEFROMPOSITION, position);
    int linpos = SendScintilla(SCI_POSITIONFROMLINE, lin);
    int indx = 0;

    // Allow for multi-byte characters.
    while (linpos < position)
    {
        int new_linpos = SendScintilla(SCI_POSITIONAFTER, linpos);

        // If the position hasn't moved then we must be at the end of the text
        // (which implies that the position passed was beyond the end of the
        // text).
        if (new_linpos == linpos)
            break;

        linpos = new_linpos;
        ++indx;
    }

    *line = lin;
    *index = indx;
}


// Set the source of the automatic auto-completion list.
void QsciScintilla::setAutoCompletionSource(AutoCompletionSource source)
{
    acSource = source;
}


// Set the threshold for automatic auto-completion.
void QsciScintilla::setAutoCompletionThreshold(int thresh)
{
    acThresh = thresh;
}


// Set the auto-completion word separators if there is no current lexer.
void QsciScintilla::setAutoCompletionWordSeparators(const QStringList &separators)
{
    if (lex.isNull())
        wseps = separators;
}


// Explicitly auto-complete from all sources.
void QsciScintilla::autoCompleteFromAll()
{
    startAutoCompletion(AcsAll, false, use_single != AcusNever);
}


// Explicitly auto-complete from the APIs.
void QsciScintilla::autoCompleteFromAPIs()
{
    startAutoCompletion(AcsAPIs, false, use_single != AcusNever);
}


// Explicitly auto-complete from the document.
void QsciScintilla::autoCompleteFromDocument()
{
    startAutoCompletion(AcsDocument, false, use_single != AcusNever);
}


// Check if a character can be in a word.
bool QsciScintilla::isWordCharacter(char ch) const
{
    return (strchr(wchars, ch) != NULL);
}


// Return the set of valid word characters.
const char *QsciScintilla::wordCharacters() const
{
    return wchars;
}


// Recolour the document.
void QsciScintilla::recolor(int start, int end)
{
    SendScintilla(SCI_COLOURISE, start, end);
}


// Registered a QPixmap image.
void QsciScintilla::registerImage(int id, const QPixmap &pm)
{
    SendScintilla(SCI_REGISTERIMAGE, id, pm);
}


// Registered a QImage image.
void QsciScintilla::registerImage(int id, const QImage &im)
{
    SendScintilla(SCI_RGBAIMAGESETHEIGHT, im.height());
    SendScintilla(SCI_RGBAIMAGESETWIDTH, im.width());
    SendScintilla(SCI_REGISTERRGBAIMAGE, id, im);
}


// Clear all registered images.
void QsciScintilla::clearRegisteredImages()
{
    SendScintilla(SCI_CLEARREGISTEREDIMAGES);
}


// Enable auto-completion fill-ups.
void QsciScintilla::setAutoCompletionFillupsEnabled(bool enable)
{
    const char *fillups;

    if (!enable)
        fillups = "";
    else if (!lex.isNull())
        fillups = lex->autoCompletionFillups();
    else
        fillups = explicit_fillups.data();

    SendScintilla(SCI_AUTOCSETFILLUPS, fillups);

    fillups_enabled = enable;
}


// See if auto-completion fill-ups are enabled.
bool QsciScintilla::autoCompletionFillupsEnabled() const
{
    return fillups_enabled;
}


// Set the fill-up characters for auto-completion if there is no current lexer.
void QsciScintilla::setAutoCompletionFillups(const char *fillups)
{
    explicit_fillups = fillups;
    setAutoCompletionFillupsEnabled(fillups_enabled);
}


// Set the case sensitivity for auto-completion.
void QsciScintilla::setAutoCompletionCaseSensitivity(bool cs)
{
    SendScintilla(SCI_AUTOCSETIGNORECASE, !cs);
}


// Return the case sensitivity for auto-completion.
bool QsciScintilla::autoCompletionCaseSensitivity() const
{
    return !SendScintilla(SCI_AUTOCGETIGNORECASE);
}


// Set the replace word mode for auto-completion.
void QsciScintilla::setAutoCompletionReplaceWord(bool replace)
{
    SendScintilla(SCI_AUTOCSETDROPRESTOFWORD, replace);
}


// Return the replace word mode for auto-completion.
bool QsciScintilla::autoCompletionReplaceWord() const
{
    return SendScintilla(SCI_AUTOCGETDROPRESTOFWORD);
}


// Set the single item mode for auto-completion.
void QsciScintilla::setAutoCompletionUseSingle(AutoCompletionUseSingle single)
{
    use_single = single;
}


// Return the single item mode for auto-completion.
QsciScintilla::AutoCompletionUseSingle QsciScintilla::autoCompletionUseSingle() const
{
    return use_single;
}


// Set the single item mode for auto-completion (deprecated).
void QsciScintilla::setAutoCompletionShowSingle(bool single)
{
    use_single = (single ? AcusExplicit : AcusNever);
}


// Return the single item mode for auto-completion (deprecated).
bool QsciScintilla::autoCompletionShowSingle() const
{
    return (use_single != AcusNever);
}


// Set current call tip position.
void QsciScintilla::setCallTipsPosition(CallTipsPosition position)
{
    SendScintilla(SCI_CALLTIPSETPOSITION, (position == CallTipsAboveText));
    call_tips_position = position;
}


// Set current call tip style.
void QsciScintilla::setCallTipsStyle(CallTipsStyle style)
{
    call_tips_style = style;
}


// Set maximum number of call tips displayed.
void QsciScintilla::setCallTipsVisible(int nr)
{
    maxCallTips = nr;
}


// Set the document to display.
void QsciScintilla::setDocument(const QsciDocument &document)
{
    if (doc.pdoc != document.pdoc)
    {
        doc.undisplay(this);
        doc.attach(document);
        doc.display(this,&document);
    }
}


// Ensure the document is read-write and return true if was was read-only.
bool QsciScintilla::ensureRW()
{
    bool ro = isReadOnly();

    if (ro)
        setReadOnly(false);

    return ro;
}


// Return the number of the first visible line.
int QsciScintilla::firstVisibleLine() const
{
    return SendScintilla(SCI_GETFIRSTVISIBLELINE);
}


// Set the number of the first visible line.
void QsciScintilla::setFirstVisibleLine(int linenr)
{
    SendScintilla(SCI_SETFIRSTVISIBLELINE, linenr);
}


// Return the height in pixels of the text in a particular line.
int QsciScintilla::textHeight(int linenr) const
{
    return SendScintilla(SCI_TEXTHEIGHT, linenr);
}


// See if auto-completion or user list is active.
bool QsciScintilla::isListActive() const
{
    return SendScintilla(SCI_AUTOCACTIVE);
}


// Cancel any current auto-completion or user list.
void QsciScintilla::cancelList()
{
    SendScintilla(SCI_AUTOCCANCEL);
}


// Handle a selection from the auto-completion list.
void QsciScintilla::handleAutoCompletionSelection()
{
    if (!lex.isNull())
    {
        QsciAbstractAPIs *apis = lex->apis();

        if (apis)
            apis->autoCompletionSelected(acSelection);
    }
}


// Display a user list.
void QsciScintilla::showUserList(int id, const QStringList &list)
{
    // Sanity check to make sure auto-completion doesn't get confused.
    if (id <= 0)
        return;

    SendScintilla(SCI_AUTOCSETSEPARATOR, userSeparator);

    SendScintilla(SCI_USERLISTSHOW, id,
            textAsBytes(list.join(QChar(userSeparator))).constData());
}


// Translate the SCN_USERLISTSELECTION notification into something more useful.
void QsciScintilla::handleUserListSelection(const char *text, int id)
{
    emit userListActivated(id, QString(text));

    // Make sure the editor hasn't been deactivated as a side effect.
    activateWindow();
}


// Return the case sensitivity of any lexer.
bool QsciScintilla::caseSensitive() const
{
    return lex.isNull() ? true : lex->caseSensitive();
}


// Return true if the current list is an auto-completion list rather than a
// user list.
bool QsciScintilla::isAutoCompletionList() const
{
    return (SendScintilla(SCI_AUTOCGETSEPARATOR) == acSeparator);
}


// Read the text from a QIODevice.
bool QsciScintilla::read(QIODevice *io)
{
    const int min_size = 1024 * 8;

    int buf_size = min_size;
    char *buf = new char[buf_size];

    int data_len = 0;
    bool ok = true;

    qint64 part;

    // Read the whole lot in so we don't have to worry about character
    // boundaries.
    do
    {
        // Make sure there is a minimum amount of room.
        if (buf_size - data_len < min_size)
        {
            buf_size *= 2;
            char *new_buf = new char[buf_size * 2];

            memcpy(new_buf, buf, data_len);
            delete[] buf;
            buf = new_buf;
        }

        part = io->read(buf + data_len, buf_size - data_len - 1);
        data_len += part;
    }
    while (part > 0);

    if (part < 0)
        ok = false;
    else
    {
        buf[data_len] = '\0';

        bool ro = ensureRW();

        SendScintilla(SCI_SETTEXT, buf);
        SendScintilla(SCI_EMPTYUNDOBUFFER);

        setReadOnly(ro);
    }

    delete[] buf;

    return ok;
}


// Write the text to a QIODevice.
bool QsciScintilla::write(QIODevice *io) const
{
    const char *buf = reinterpret_cast<const char *>(SendScintillaPtrResult(SCI_GETCHARACTERPOINTER));

    const char *bp = buf;
    uint buflen = qstrlen(buf);

    while (buflen > 0)
    {
        qint64 part = io->write(bp, buflen);

        if (part < 0)
            return false;

        bp += part;
        buflen -= part;
    }

    return true;
}


// Return the word at the given coordinates.
QString QsciScintilla::wordAtLineIndex(int line, int index) const
{
    return wordAtPosition(positionFromLineIndex(line, index));
}


// Return the word at the given coordinates.
QString QsciScintilla::wordAtPoint(const QPoint &point) const
{
    long close_pos = SendScintilla(SCI_POSITIONFROMPOINTCLOSE, point.x(),
            point.y());

    return wordAtPosition(close_pos);
}


// Return the word at the given position.
QString QsciScintilla::wordAtPosition(int position) const
{
    if (position < 0)
        return QString();

    long start_pos = SendScintilla(SCI_WORDSTARTPOSITION, position, true);
    long end_pos = SendScintilla(SCI_WORDENDPOSITION, position, true);

    if (start_pos >= end_pos)
        return QString();

    return text(start_pos, end_pos);
}


// Return the display style for annotations.
QsciScintilla::AnnotationDisplay QsciScintilla::annotationDisplay() const
{
    return (AnnotationDisplay)SendScintilla(SCI_ANNOTATIONGETVISIBLE);
}


// Set the display style for annotations.
void QsciScintilla::setAnnotationDisplay(QsciScintilla::AnnotationDisplay display)
{
    SendScintilla(SCI_ANNOTATIONSETVISIBLE, display);
    setScrollBars();
}


// Clear annotations.
void QsciScintilla::clearAnnotations(int line)
{
    if (line >= 0)
        SendScintilla(SCI_ANNOTATIONSETTEXT, line, (const char *)0);
    else
        SendScintilla(SCI_ANNOTATIONCLEARALL);

    setScrollBars();
}


// Annotate a line.
void QsciScintilla::annotate(int line, const QString &text, int style)
{
    int style_offset = SendScintilla(SCI_ANNOTATIONGETSTYLEOFFSET);

    SendScintilla(SCI_ANNOTATIONSETTEXT, line, textAsBytes(text).constData());
    SendScintilla(SCI_ANNOTATIONSETSTYLE, line, style - style_offset);

    setScrollBars();
}


// Annotate a line.
void QsciScintilla::annotate(int line, const QString &text, const QsciStyle &style)
{
    style.apply(this);

    annotate(line, text, style.style());
}


// Annotate a line.
void QsciScintilla::annotate(int line, const QsciStyledText &text)
{
    text.apply(this);

    annotate(line, text.text(), text.style());
}


// Annotate a line.
void QsciScintilla::annotate(int line, const QList<QsciStyledText> &text)
{
    char *styles;
    QByteArray styled_text = styleText(text, &styles,
            SendScintilla(SCI_ANNOTATIONGETSTYLEOFFSET));

    SendScintilla(SCI_ANNOTATIONSETTEXT, line, styled_text.constData());
    SendScintilla(SCI_ANNOTATIONSETSTYLES, line, styles);

    delete[] styles;
}


// Get the annotation for a line, if any.
QString QsciScintilla::annotation(int line) const
{
    int size = SendScintilla(SCI_ANNOTATIONGETTEXT, line, (const char *)0);
    char *buf = new char[size + 1];

    QString qs = bytesAsText(buf, size);
    delete[] buf;

    return qs;
}


// Convert a list of styled text to the low-level arrays.
QByteArray QsciScintilla::styleText(const QList<QsciStyledText> &styled_text,
        char **styles, int style_offset)
{
    QString text;
    int i;

    // Build the full text.
    for (i = 0; i < styled_text.count(); ++i)
    {
        const QsciStyledText &st = styled_text[i];

        st.apply(this);

        text.append(st.text());
    }

    QByteArray s = textAsBytes(text);

    // There is a style byte for every byte.
    char *sp = *styles = new char[s.length()];

    for (i = 0; i < styled_text.count(); ++i)
    {
        const QsciStyledText &st = styled_text[i];
        QByteArray part = textAsBytes(st.text());
        int part_length = part.length();

        for (int c = 0; c < part_length; ++c)
            *sp++ = (char)(st.style() - style_offset);
    }

    return s;
}


// Convert Scintilla modifiers to the Qt equivalent.
int QsciScintilla::mapModifiers(int modifiers)
{
    int state = 0;

    if (modifiers & SCMOD_SHIFT)
        state |= Qt::ShiftModifier;

    if (modifiers & SCMOD_CTRL)
        state |= Qt::ControlModifier;

    if (modifiers & SCMOD_ALT)
        state |= Qt::AltModifier;

    if (modifiers & (SCMOD_SUPER | SCMOD_META))
        state |= Qt::MetaModifier;

    return state;
}


// Re-implemented to handle shortcut overrides.
bool QsciScintilla::event(QEvent *e)
{
    if (e->type() == QEvent::ShortcutOverride && !isReadOnly())
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);

        if (ke->key())
        {
            // We want ordinary characters.
            if ((ke->modifiers() == Qt::NoModifier || ke->modifiers() == Qt::ShiftModifier || ke->modifiers() == Qt::KeypadModifier) && ke->key() < Qt::Key_Escape)
            {
                ke->accept();
                return true;
            }

            // We want any key that is bound.
            QsciCommand *cmd = stdCmds->boundTo(ke->key() | (ke->modifiers() & ~Qt::KeypadModifier));

            if (cmd)
            {
                ke->accept();
                return true;
            }
        }
    }

    return QsciScintillaBase::event(e);
}


// Re-implemented to zoom when the Control modifier is pressed.
void QsciScintilla::wheelEvent(QWheelEvent *e)
{
#if defined(Q_OS_MAC)
    const Qt::KeyboardModifier zoom_modifier = Qt::MetaModifier;
#else
    const Qt::KeyboardModifier zoom_modifier = Qt::ControlModifier;
#endif

   if ((e->modifiers() & zoom_modifier) != 0)
   {
       QPoint ad = e->angleDelta();
       int delta = (qAbs(ad.x()) > qAbs(ad.y())) ? ad.x() : ad.y();

       if (delta > 0)
           zoomIn();
       else
           zoomOut();
   }
   else 
   {
       QsciScintillaBase::wheelEvent(e);
   }
}


// Re-implemented to handle chenges to the enabled state.
void QsciScintilla::changeEvent(QEvent *e)
{
    QsciScintillaBase::changeEvent(e);

    if (e->type() != QEvent::EnabledChange)
        return;

    if (isEnabled())
        SendScintilla(SCI_SETCARETSTYLE, CARETSTYLE_LINE);
    else
        SendScintilla(SCI_SETCARETSTYLE, CARETSTYLE_INVISIBLE);

    QColor fore = palette().color(QPalette::Disabled, QPalette::Text);
    QColor back = palette().color(QPalette::Disabled, QPalette::Base);

    if (lex.isNull())
    {
        if (isEnabled())
        {
            fore = nl_text_colour;
            back = nl_paper_colour;
        }

        SendScintilla(SCI_STYLESETFORE, 0, fore);

        // Assume style 0 applies to everything so that we don't need to use
        // SCI_STYLECLEARALL which clears everything.  We still have to set the
        // default style as well for the background without any text.
        SendScintilla(SCI_STYLESETBACK, 0, back);
        SendScintilla(SCI_STYLESETBACK, STYLE_DEFAULT, back);
    }
    else
    {
        setEnabledColors(STYLE_DEFAULT, fore, back);

        for (int s = 0; s <= STYLE_MAX; ++s)
            if (!lex->description(s).isNull())
                setEnabledColors(s, fore, back);
    }
}


// Set the foreground and background colours for a style.
void QsciScintilla::setEnabledColors(int style, QColor &fore, QColor &back)
{
    if (isEnabled())
    {
        fore = lex->color(style);
        back = lex->paper(style);
    }

    handleStyleColorChange(fore, style);
    handleStylePaperChange(back, style);
}


// Re-implemented to implement a more Qt-like context menu.
void QsciScintilla::contextMenuEvent(QContextMenuEvent *e)
{
    if (contextMenuNeeded(e->x(), e->y()))
    {
        QMenu *menu = createStandardContextMenu();

        if (menu)
        {
            menu->setAttribute(Qt::WA_DeleteOnClose);
            menu->popup(e->globalPos());
        }
    }
}


// Create an instance of the standard context menu.
QMenu *QsciScintilla::createStandardContextMenu()
{
    bool read_only = isReadOnly();
    bool has_selection = hasSelectedText();
    QMenu *menu = new QMenu(this);
    QAction *action;

    if (!read_only)
    {
        action = menu->addAction(tr("&Undo"), this, SLOT(undo()));
        set_shortcut(action, QsciCommand::Undo);
        action->setEnabled(isUndoAvailable());

        action = menu->addAction(tr("&Redo"), this, SLOT(redo()));
        set_shortcut(action, QsciCommand::Redo);
        action->setEnabled(isRedoAvailable());

        menu->addSeparator();

        action = menu->addAction(tr("Cu&t"), this, SLOT(cut()));
        set_shortcut(action, QsciCommand::SelectionCut);
        action->setEnabled(has_selection);
    }

    action = menu->addAction(tr("&Copy"), this, SLOT(copy()));
    set_shortcut(action, QsciCommand::SelectionCopy);
    action->setEnabled(has_selection);

    if (!read_only)
    {
        action = menu->addAction(tr("&Paste"), this, SLOT(paste()));
        set_shortcut(action, QsciCommand::Paste);
        action->setEnabled(SendScintilla(SCI_CANPASTE));

        action = menu->addAction(tr("Delete"), this, SLOT(delete_selection()));
        action->setEnabled(has_selection);
    }

    if (!menu->isEmpty())
        menu->addSeparator();

    action = menu->addAction(tr("Select All"), this, SLOT(selectAll()));
    set_shortcut(action, QsciCommand::SelectAll);
    action->setEnabled(length() != 0);

    return menu;
}


// Set the shortcut for an action using any current key binding.
void QsciScintilla::set_shortcut(QAction *action, QsciCommand::Command cmd_id) const
{
    QsciCommand *cmd = stdCmds->find(cmd_id);

    if (cmd && cmd->key())
        action->setShortcut(QKeySequence(cmd->key()));
}


// Delete the current selection.
void QsciScintilla::delete_selection()
{
    SendScintilla(SCI_CLEAR);
}


// Convert a Scintilla colour to a QColor.
static QColor asQColor(long sci_colour)
{
    return QColor(
            ((int)sci_colour) & 0x00ff,
            ((int)(sci_colour >> 8)) & 0x00ff,
            ((int)(sci_colour >> 16)) & 0x00ff);
}


// Set the scroll width.
void QsciScintilla::setScrollWidth(int pixelWidth)
{
    SendScintilla(SCI_SETSCROLLWIDTH, pixelWidth);
}

// Get the scroll width.
int QsciScintilla::scrollWidth() const
{
    return SendScintilla(SCI_GETSCROLLWIDTH);
}


// Set scroll width tracking.
void QsciScintilla::setScrollWidthTracking(bool enabled)
{
    SendScintilla(SCI_SETSCROLLWIDTHTRACKING, enabled);
}


// Get scroll width tracking.
bool QsciScintilla::scrollWidthTracking() const
{
    return SendScintilla(SCI_GETSCROLLWIDTHTRACKING);
}
