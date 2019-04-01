// Scintilla source code edit control
/** @file Document.h
 ** Text document that handles notifications, DBCS, styling, words and end of line.
 **/
// Copyright 1998-2011 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef DOCUMENT_H
#define DOCUMENT_H

namespace Scintilla {

class DocWatcher;
class DocModification;
class Document;
class LineMarkers;
class LineLevels;
class LineState;
class LineAnnotation;

enum EncodingFamily { efEightBit, efUnicode, efDBCS };

/**
 * The range class represents a range of text in a document.
 * The two values are not sorted as one end may be more significant than the other
 * as is the case for the selection where the end position is the position of the caret.
 * If either position is invalidPosition then the range is invalid and most operations will fail.
 */
class Range {
public:
	Sci::Position start;
	Sci::Position end;

	explicit Range(Sci::Position pos=0) noexcept :
		start(pos), end(pos) {
	}
	Range(Sci::Position start_, Sci::Position end_) noexcept :
		start(start_), end(end_) {
	}

	bool operator==(const Range &other) const noexcept {
		return (start == other.start) && (end == other.end);
	}

	bool Valid() const noexcept {
		return (start != Sci::invalidPosition) && (end != Sci::invalidPosition);
	}

	Sci::Position First() const noexcept {
		return (start <= end) ? start : end;
	}

	Sci::Position Last() const noexcept {
		return (start > end) ? start : end;
	}

	// Is the position within the range?
	bool Contains(Sci::Position pos) const noexcept {
		if (start < end) {
			return (pos >= start && pos <= end);
		} else {
			return (pos <= start && pos >= end);
		}
	}

	// Is the character after pos within the range?
	bool ContainsCharacter(Sci::Position pos) const noexcept {
		if (start < end) {
			return (pos >= start && pos < end);
		} else {
			return (pos < start && pos >= end);
		}
	}

	bool Contains(Range other) const noexcept {
		return Contains(other.start) && Contains(other.end);
	}

	bool Overlaps(Range other) const noexcept {
		return
		Contains(other.start) ||
		Contains(other.end) ||
		other.Contains(start) ||
		other.Contains(end);
	}
};

/**
 * Interface class for regular expression searching
 */
class RegexSearchBase {
public:
	virtual ~RegexSearchBase() {}

	virtual Sci::Position FindText(Document *doc, Sci::Position minPos, Sci::Position maxPos, const char *s,
                        bool caseSensitive, bool word, bool wordStart, int flags, Sci::Position *length) = 0;

	///@return String with the substitutions, must remain valid until the next call or destruction
	virtual const char *SubstituteByPosition(Document *doc, const char *text, Sci::Position *length) = 0;
};

/// Factory function for RegexSearchBase
extern RegexSearchBase *CreateRegexSearch(CharClassify *charClassTable);

struct StyledText {
	size_t length;
	const char *text;
	bool multipleStyles;
	size_t style;
	const unsigned char *styles;
	StyledText(size_t length_, const char *text_, bool multipleStyles_, int style_, const unsigned char *styles_) noexcept :
		length(length_), text(text_), multipleStyles(multipleStyles_), style(style_), styles(styles_) {
	}
	// Return number of bytes from start to before '\n' or end of text.
	// Return 1 when start is outside text
	size_t LineLength(size_t start) const noexcept {
		size_t cur = start;
		while ((cur < length) && (text[cur] != '\n'))
			cur++;
		return cur-start;
	}
	size_t StyleAt(size_t i) const noexcept {
		return multipleStyles ? styles[i] : style;
	}
};

class HighlightDelimiter {
public:
	HighlightDelimiter() : isEnabled(false) {
		Clear();
	}

	void Clear() {
		beginFoldBlock = -1;
		endFoldBlock = -1;
		firstChangeableLineBefore = -1;
		firstChangeableLineAfter = -1;
	}

	bool NeedsDrawing(Sci::Line line) const {
		return isEnabled && (line <= firstChangeableLineBefore || line >= firstChangeableLineAfter);
	}

	bool IsFoldBlockHighlighted(Sci::Line line) const {
		return isEnabled && beginFoldBlock != -1 && beginFoldBlock <= line && line <= endFoldBlock;
	}

	bool IsHeadOfFoldBlock(Sci::Line line) const {
		return beginFoldBlock == line && line < endFoldBlock;
	}

	bool IsBodyOfFoldBlock(Sci::Line line) const {
		return beginFoldBlock != -1 && beginFoldBlock < line && line < endFoldBlock;
	}

	bool IsTailOfFoldBlock(Sci::Line line) const {
		return beginFoldBlock != -1 && beginFoldBlock < line && line == endFoldBlock;
	}

	Sci::Line beginFoldBlock;	// Begin of current fold block
	Sci::Line endFoldBlock;	// End of current fold block
	Sci::Line firstChangeableLineBefore;	// First line that triggers repaint before starting line that determined current fold block
	Sci::Line firstChangeableLineAfter;	// First line that triggers repaint after starting line that determined current fold block
	bool isEnabled;
};

inline int LevelNumber(int level) noexcept {
	return level & SC_FOLDLEVELNUMBERMASK;
}

class LexInterface {
protected:
	Document *pdoc;
	ILexer *instance;
	bool performingStyle;	///< Prevent reentrance
public:
	explicit LexInterface(Document *pdoc_) : pdoc(pdoc_), instance(nullptr), performingStyle(false) {
	}
	virtual ~LexInterface() {
	}
	void Colourise(Sci::Position start, Sci::Position end);
	virtual int LineEndTypesSupported();
	bool UseContainerLexing() const {
		return instance == nullptr;
	}
};

struct RegexError : public std::runtime_error {
	RegexError() : std::runtime_error("regex failure") {}
};

/**
 * The ActionDuration class stores the average time taken for some action such as styling or
 * wrapping a line. It is used to decide how many repetitions of that action can be performed
 * on idle to maximize efficiency without affecting application responsiveness.
 * The duration changes if the time for the action changes. For example, if a simple lexer is
 * changed to a complex lexer. Changes are damped and clamped to avoid short periods of easy
 * or difficult processing moving the value too far leading to inefficiency or poor user
 * experience.
 */

class ActionDuration {
	double duration;
	const double minDuration;
	const double maxDuration;
public:
	ActionDuration(double duration_, double minDuration_, double maxDuration_) noexcept;
	void AddSample(size_t numberActions, double durationOfActions) noexcept;
	double Duration() const noexcept;
};

/**
 */
class Document : PerLine, public IDocumentWithLineEnd, public ILoader {

public:
	/** Used to pair watcher pointer with user data. */
	struct WatcherWithUserData {
		DocWatcher *watcher;
		void *userData;
		WatcherWithUserData(DocWatcher *watcher_=nullptr, void *userData_=nullptr) noexcept :
			watcher(watcher_), userData(userData_) {
		}
		bool operator==(const WatcherWithUserData &other) const noexcept {
			return (watcher == other.watcher) && (userData == other.userData);
		}
	};

private:
	int refCount;
	CellBuffer cb;
	CharClassify charClass;
	std::unique_ptr<CaseFolder> pcf;
	Sci::Position endStyled;
	int styleClock;
	int enteredModification;
	int enteredStyling;
	int enteredReadOnlyCount;

	bool insertionSet;
	std::string insertion;

	std::vector<WatcherWithUserData> watchers;

	// ldSize is not real data - it is for dimensions and loops
	enum lineData { ldMarkers, ldLevels, ldState, ldMargin, ldAnnotation, ldSize };
	std::unique_ptr<PerLine> perLineData[ldSize];
	LineMarkers *Markers() const;
	LineLevels *Levels() const;
	LineState *States() const;
	LineAnnotation *Margins() const;
	LineAnnotation *Annotations() const;

	bool matchesValid;
	std::unique_ptr<RegexSearchBase> regex;
	std::unique_ptr<LexInterface> pli;

public:

	struct CharacterExtracted {
		unsigned int character;
		unsigned int widthBytes;
		CharacterExtracted(unsigned int character_, unsigned int widthBytes_) noexcept :
			character(character_), widthBytes(widthBytes_) {
		}
		// For DBCS characters turn 2 bytes into an int
		static CharacterExtracted DBCS(unsigned char lead, unsigned char trail) noexcept {
			return CharacterExtracted((lead << 8) | trail, 2);
		}
	};

	int eolMode;
	/// Can also be SC_CP_UTF8 to enable UTF-8 mode
	int dbcsCodePage;
	int lineEndBitSet;
	int tabInChars;
	int indentInChars;
	int actualIndentInChars;
	bool useTabs;
	bool tabIndents;
	bool backspaceUnindents;
	ActionDuration durationStyleOneLine;

	std::unique_ptr<IDecorationList> decorations;

	Document(int options);
	// Deleted so Document objects can not be copied.
	Document(const Document &) = delete;
	Document(Document &&) = delete;
	void operator=(const Document &) = delete;
	Document &operator=(Document &&) = delete;
	~Document() override;

	int AddRef();
	int SCI_METHOD Release() override;

	// From PerLine
	void Init() override;
	void InsertLine(Sci::Line line) override;
	void RemoveLine(Sci::Line line) override;

	int LineEndTypesSupported() const;
	bool SetDBCSCodePage(int dbcsCodePage_);
	int GetLineEndTypesAllowed() const { return cb.GetLineEndTypes(); }
	bool SetLineEndTypesAllowed(int lineEndBitSet_);
	int GetLineEndTypesActive() const { return cb.GetLineEndTypes(); }

	int SCI_METHOD Version() const override {
		return dvLineEnd;
	}

	void SCI_METHOD SetErrorStatus(int status) override;

	Sci_Position SCI_METHOD LineFromPosition(Sci_Position pos) const override;
	Sci::Line SciLineFromPosition(Sci::Position pos) const noexcept;	// Avoids casting LineFromPosition
	Sci::Position ClampPositionIntoDocument(Sci::Position pos) const;
	bool ContainsLineEnd(const char *s, Sci::Position length) const { return cb.ContainsLineEnd(s, length); }
	bool IsCrLf(Sci::Position pos) const;
	int LenChar(Sci::Position pos);
	bool InGoodUTF8(Sci::Position pos, Sci::Position &start, Sci::Position &end) const noexcept;
	Sci::Position MovePositionOutsideChar(Sci::Position pos, Sci::Position moveDir, bool checkLineEnd=true) const;
	Sci::Position NextPosition(Sci::Position pos, int moveDir) const noexcept;
	bool NextCharacter(Sci::Position &pos, int moveDir) const noexcept;	// Returns true if pos changed
	Document::CharacterExtracted CharacterAfter(Sci::Position position) const;
	Document::CharacterExtracted CharacterBefore(Sci::Position position) const;
	Sci_Position SCI_METHOD GetRelativePosition(Sci_Position positionStart, Sci_Position characterOffset) const override;
	Sci::Position GetRelativePositionUTF16(Sci::Position positionStart, Sci::Position characterOffset) const;
	int SCI_METHOD GetCharacterAndWidth(Sci_Position position, Sci_Position *pWidth) const override;
	int SCI_METHOD CodePage() const override;
	bool SCI_METHOD IsDBCSLeadByte(char ch) const override;
	bool IsDBCSLeadByteNoExcept(char ch) const noexcept;
	bool IsDBCSLeadByteInvalid(char ch) const noexcept;
	bool IsDBCSTrailByteInvalid(char ch) const noexcept;
	int DBCSDrawBytes(const char *text, int len) const noexcept;
	int SafeSegment(const char *text, int length, int lengthSegment) const noexcept;
	EncodingFamily CodePageFamily() const noexcept;

	// Gateways to modifying document
	void ModifiedAt(Sci::Position pos) noexcept;
	void CheckReadOnly();
	bool DeleteChars(Sci::Position pos, Sci::Position len);
	Sci::Position InsertString(Sci::Position position, const char *s, Sci::Position insertLength);
	void ChangeInsertion(const char *s, Sci::Position length);
	int SCI_METHOD AddData(const char *data, Sci_Position length) override;
	void * SCI_METHOD ConvertToDocument() override;
	Sci::Position Undo();
	Sci::Position Redo();
	bool CanUndo() const { return cb.CanUndo(); }
	bool CanRedo() const { return cb.CanRedo(); }
	void DeleteUndoHistory() { cb.DeleteUndoHistory(); }
	bool SetUndoCollection(bool collectUndo) {
		return cb.SetUndoCollection(collectUndo);
	}
	bool IsCollectingUndo() const { return cb.IsCollectingUndo(); }
	void BeginUndoAction() { cb.BeginUndoAction(); }
	void EndUndoAction() { cb.EndUndoAction(); }
	void AddUndoAction(Sci::Position token, bool mayCoalesce) { cb.AddUndoAction(token, mayCoalesce); }
	void SetSavePoint();
	bool IsSavePoint() const { return cb.IsSavePoint(); }

	void TentativeStart() { cb.TentativeStart(); }
	void TentativeCommit() { cb.TentativeCommit(); }
	void TentativeUndo();
	bool TentativeActive() const { return cb.TentativeActive(); }

	const char * SCI_METHOD BufferPointer() override { return cb.BufferPointer(); }
	const char *RangePointer(Sci::Position position, Sci::Position rangeLength) { return cb.RangePointer(position, rangeLength); }
	Sci::Position GapPosition() const { return cb.GapPosition(); }

	int SCI_METHOD GetLineIndentation(Sci_Position line) override;
	Sci::Position SetLineIndentation(Sci::Line line, Sci::Position indent);
	Sci::Position GetLineIndentPosition(Sci::Line line) const;
	Sci::Position GetColumn(Sci::Position pos);
	Sci::Position CountCharacters(Sci::Position startPos, Sci::Position endPos) const;
	Sci::Position CountUTF16(Sci::Position startPos, Sci::Position endPos) const;
	Sci::Position FindColumn(Sci::Line line, Sci::Position column);
	void Indent(bool forwards, Sci::Line lineBottom, Sci::Line lineTop);
	static std::string TransformLineEnds(const char *s, size_t len, int eolModeWanted);
	void ConvertLineEnds(int eolModeSet);
	void SetReadOnly(bool set) { cb.SetReadOnly(set); }
	bool IsReadOnly() const { return cb.IsReadOnly(); }
	bool IsLarge() const { return cb.IsLarge(); }
	int Options() const;

	void DelChar(Sci::Position pos);
	void DelCharBack(Sci::Position pos);

	char CharAt(Sci::Position position) const noexcept { return cb.CharAt(position); }
	void SCI_METHOD GetCharRange(char *buffer, Sci_Position position, Sci_Position lengthRetrieve) const override {
		cb.GetCharRange(buffer, position, lengthRetrieve);
	}
	char SCI_METHOD StyleAt(Sci_Position position) const override { return cb.StyleAt(position); }
	int StyleIndexAt(Sci_Position position) const noexcept { return static_cast<unsigned char>(cb.StyleAt(position)); }
	void GetStyleRange(unsigned char *buffer, Sci::Position position, Sci::Position lengthRetrieve) const {
		cb.GetStyleRange(buffer, position, lengthRetrieve);
	}
	int GetMark(Sci::Line line) const;
	Sci::Line MarkerNext(Sci::Line lineStart, int mask) const;
	int AddMark(Sci::Line line, int markerNum);
	void AddMarkSet(Sci::Line line, int valueSet);
	void DeleteMark(Sci::Line line, int markerNum);
	void DeleteMarkFromHandle(int markerHandle);
	void DeleteAllMarks(int markerNum);
	Sci::Line LineFromHandle(int markerHandle) const;
	Sci_Position SCI_METHOD LineStart(Sci_Position line) const override;
	bool IsLineStartPosition(Sci::Position position) const;
	Sci_Position SCI_METHOD LineEnd(Sci_Position line) const override;
	Sci::Position LineEndPosition(Sci::Position position) const;
	bool IsLineEndPosition(Sci::Position position) const;
	bool IsPositionInLineEnd(Sci::Position position) const;
	Sci::Position VCHomePosition(Sci::Position position) const;
	Sci::Position IndexLineStart(Sci::Line line, int lineCharacterIndex) const;
	Sci::Line LineFromPositionIndex(Sci::Position pos, int lineCharacterIndex) const;

	int SCI_METHOD SetLevel(Sci_Position line, int level) override;
	int SCI_METHOD GetLevel(Sci_Position line) const override;
	void ClearLevels();
	Sci::Line GetLastChild(Sci::Line lineParent, int level=-1, Sci::Line lastLine=-1);
	Sci::Line GetFoldParent(Sci::Line line) const;
	void GetHighlightDelimiters(HighlightDelimiter &highlightDelimiter, Sci::Line line, Sci::Line lastLine);

	Sci::Position ExtendWordSelect(Sci::Position pos, int delta, bool onlyWordCharacters=false) const;
	Sci::Position NextWordStart(Sci::Position pos, int delta) const;
	Sci::Position NextWordEnd(Sci::Position pos, int delta) const;
	Sci_Position SCI_METHOD Length() const override { return cb.Length(); }
	void Allocate(Sci::Position newSize) { cb.Allocate(newSize); }

	CharacterExtracted ExtractCharacter(Sci::Position position) const noexcept;

	bool IsWordStartAt(Sci::Position pos) const;
	bool IsWordEndAt(Sci::Position pos) const;
	bool IsWordAt(Sci::Position start, Sci::Position end) const;

	bool MatchesWordOptions(bool word, bool wordStart, Sci::Position pos, Sci::Position length) const;
	bool HasCaseFolder() const noexcept;
	void SetCaseFolder(CaseFolder *pcf_);
	Sci::Position FindText(Sci::Position minPos, Sci::Position maxPos, const char *search, int flags, Sci::Position *length);
	const char *SubstituteByPosition(const char *text, Sci::Position *length);
	int LineCharacterIndex() const;
	void AllocateLineCharacterIndex(int lineCharacterIndex);
	void ReleaseLineCharacterIndex(int lineCharacterIndex);
	Sci::Line LinesTotal() const noexcept;

	void SetDefaultCharClasses(bool includeWordClass);
	void SetCharClasses(const unsigned char *chars, CharClassify::cc newCharClass);
	int GetCharsOfClass(CharClassify::cc characterClass, unsigned char *buffer) const;
	void SCI_METHOD StartStyling(Sci_Position position, char mask) override;
	bool SCI_METHOD SetStyleFor(Sci_Position length, char style) override;
	bool SCI_METHOD SetStyles(Sci_Position length, const char *styles) override;
	Sci::Position GetEndStyled() const noexcept { return endStyled; }
	void EnsureStyledTo(Sci::Position pos);
	void StyleToAdjustingLineDuration(Sci::Position pos);
	void LexerChanged();
	int GetStyleClock() const noexcept { return styleClock; }
	void IncrementStyleClock() noexcept;
	void SCI_METHOD DecorationSetCurrentIndicator(int indicator) override;
	void SCI_METHOD DecorationFillRange(Sci_Position position, int value, Sci_Position fillLength) override;
	LexInterface *GetLexInterface() const;
	void SetLexInterface(LexInterface *pLexInterface);

	int SCI_METHOD SetLineState(Sci_Position line, int state) override;
	int SCI_METHOD GetLineState(Sci_Position line) const override;
	Sci::Line GetMaxLineState() const;
	void SCI_METHOD ChangeLexerState(Sci_Position start, Sci_Position end) override;

	StyledText MarginStyledText(Sci::Line line) const;
	void MarginSetStyle(Sci::Line line, int style);
	void MarginSetStyles(Sci::Line line, const unsigned char *styles);
	void MarginSetText(Sci::Line line, const char *text);
	void MarginClearAll();

	StyledText AnnotationStyledText(Sci::Line line) const;
	void AnnotationSetText(Sci::Line line, const char *text);
	void AnnotationSetStyle(Sci::Line line, int style);
	void AnnotationSetStyles(Sci::Line line, const unsigned char *styles);
	int AnnotationLines(Sci::Line line) const;
	void AnnotationClearAll();

	bool AddWatcher(DocWatcher *watcher, void *userData);
	bool RemoveWatcher(DocWatcher *watcher, void *userData);

	bool IsASCIIWordByte(unsigned char ch) const;
	CharClassify::cc WordCharacterClass(unsigned int ch) const;
	bool IsWordPartSeparator(unsigned int ch) const;
	Sci::Position WordPartLeft(Sci::Position pos) const;
	Sci::Position WordPartRight(Sci::Position pos) const;
	Sci::Position ExtendStyleRange(Sci::Position pos, int delta, bool singleLine = false);
	bool IsWhiteLine(Sci::Line line) const;
	Sci::Position ParaUp(Sci::Position pos) const;
	Sci::Position ParaDown(Sci::Position pos) const;
	int IndentSize() const noexcept { return actualIndentInChars; }
	Sci::Position BraceMatch(Sci::Position position, Sci::Position maxReStyle);

private:
	void NotifyModifyAttempt();
	void NotifySavePoint(bool atSavePoint);
	void NotifyModified(DocModification mh);
};

class UndoGroup {
	Document *pdoc;
	bool groupNeeded;
public:
	UndoGroup(Document *pdoc_, bool groupNeeded_=true) :
		pdoc(pdoc_), groupNeeded(groupNeeded_) {
		if (groupNeeded) {
			pdoc->BeginUndoAction();
		}
	}
	~UndoGroup() {
		if (groupNeeded) {
			pdoc->EndUndoAction();
		}
	}
	bool Needed() const noexcept {
		return groupNeeded;
	}
};


/**
 * To optimise processing of document modifications by DocWatchers, a hint is passed indicating the
 * scope of the change.
 * If the DocWatcher is a document view then this can be used to optimise screen updating.
 */
class DocModification {
public:
	int modificationType;
	Sci::Position position;
	Sci::Position length;
	Sci::Line linesAdded;	/**< Negative if lines deleted. */
	const char *text;	/**< Only valid for changes to text, not for changes to style. */
	Sci::Line line;
	int foldLevelNow;
	int foldLevelPrev;
	Sci::Line annotationLinesAdded;
	Sci::Position token;

	DocModification(int modificationType_, Sci::Position position_=0, Sci::Position length_=0,
		Sci::Line linesAdded_=0, const char *text_=nullptr, Sci::Line line_=0) noexcept :
		modificationType(modificationType_),
		position(position_),
		length(length_),
		linesAdded(linesAdded_),
		text(text_),
		line(line_),
		foldLevelNow(0),
		foldLevelPrev(0),
		annotationLinesAdded(0),
		token(0) {}

	DocModification(int modificationType_, const Action &act, Sci::Line linesAdded_=0) noexcept :
		modificationType(modificationType_),
		position(act.position),
		length(act.lenData),
		linesAdded(linesAdded_),
		text(act.data.get()),
		line(0),
		foldLevelNow(0),
		foldLevelPrev(0),
		annotationLinesAdded(0),
		token(0) {}
};

/**
 * A class that wants to receive notifications from a Document must be derived from DocWatcher
 * and implement the notification methods. It can then be added to the watcher list with AddWatcher.
 */
class DocWatcher {
public:
	virtual ~DocWatcher() {}

	virtual void NotifyModifyAttempt(Document *doc, void *userData) = 0;
	virtual void NotifySavePoint(Document *doc, void *userData, bool atSavePoint) = 0;
	virtual void NotifyModified(Document *doc, DocModification mh, void *userData) = 0;
	virtual void NotifyDeleted(Document *doc, void *userData) = 0;
	virtual void NotifyStyleNeeded(Document *doc, void *userData, Sci::Position endPos) = 0;
	virtual void NotifyLexerChanged(Document *doc, void *userData) = 0;
	virtual void NotifyErrorOccurred(Document *doc, void *userData, int status) = 0;
};

}

#endif
