// Scintilla source code edit control
/** @file LexCPP.cxx
 ** Lexer for C++, C, Java, and JavaScript.
 ** Further folding features and configuration properties added by "Udo Lechner" <dlchnr(at)gmx(dot)net>
 **/
// Copyright 1998-2005 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include <utility>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "ILexer.h"
#include "Scintilla.h"
#include "SciLexer.h"

#include "StringCopy.h"
#include "WordList.h"
#include "LexAccessor.h"
#include "Accessor.h"
#include "StyleContext.h"
#include "CharacterSet.h"
#include "LexerModule.h"
#include "OptionSet.h"
#include "SparseState.h"
#include "SubStyles.h"

using namespace Scintilla;

namespace {
	// Use an unnamed namespace to protect the functions and classes from name conflicts

bool IsSpaceEquiv(int state) noexcept {
	return (state <= SCE_C_COMMENTDOC) ||
		// including SCE_C_DEFAULT, SCE_C_COMMENT, SCE_C_COMMENTLINE
		(state == SCE_C_COMMENTLINEDOC) || (state == SCE_C_COMMENTDOCKEYWORD) ||
		(state == SCE_C_COMMENTDOCKEYWORDERROR);
}

// Preconditions: sc.currentPos points to a character after '+' or '-'.
// The test for pos reaching 0 should be redundant,
// and is in only for safety measures.
// Limitation: this code will give the incorrect answer for code like
// a = b+++/ptn/...
// Putting a space between the '++' post-inc operator and the '+' binary op
// fixes this, and is highly recommended for readability anyway.
bool FollowsPostfixOperator(const StyleContext &sc, LexAccessor &styler) {
	Sci_Position pos = sc.currentPos;
	while (--pos > 0) {
		const char ch = styler[pos];
		if (ch == '+' || ch == '-') {
			return styler[pos - 1] == ch;
		}
	}
	return false;
}

bool followsReturnKeyword(const StyleContext &sc, LexAccessor &styler) {
	// Don't look at styles, so no need to flush.
	Sci_Position pos = sc.currentPos;
	const Sci_Position currentLine = styler.GetLine(pos);
	const Sci_Position lineStartPos = styler.LineStart(currentLine);
	while (--pos > lineStartPos) {
		const char ch = styler.SafeGetCharAt(pos);
		if (ch != ' ' && ch != '\t') {
			break;
		}
	}
	const char *retBack = "nruter";
	const char *s = retBack;
	while (*s
		&& pos >= lineStartPos
		&& styler.SafeGetCharAt(pos) == *s) {
		s++;
		pos--;
	}
	return !*s;
}

bool IsSpaceOrTab(int ch) noexcept {
	return ch == ' ' || ch == '\t';
}

bool OnlySpaceOrTab(const std::string &s) noexcept {
	for (const char ch : s) {
		if (!IsSpaceOrTab(ch))
			return false;
	}
	return true;
}

std::vector<std::string> StringSplit(const std::string &text, int separator) {
	std::vector<std::string> vs(text.empty() ? 0 : 1);
	for (const char ch : text) {
		if (ch == separator) {
			vs.emplace_back();
		} else {
			vs.back() += ch;
		}
	}
	return vs;
}

struct BracketPair {
	std::vector<std::string>::iterator itBracket;
	std::vector<std::string>::iterator itEndBracket;
};

BracketPair FindBracketPair(std::vector<std::string> &tokens) {
	BracketPair bp;
	std::vector<std::string>::iterator itTok = std::find(tokens.begin(), tokens.end(), "(");
	bp.itBracket = tokens.end();
	bp.itEndBracket = tokens.end();
	if (itTok != tokens.end()) {
		bp.itBracket = itTok;
		size_t nest = 0;
		while (itTok != tokens.end()) {
			if (*itTok == "(") {
				nest++;
			} else if (*itTok == ")") {
				nest--;
				if (nest == 0) {
					bp.itEndBracket = itTok;
					return bp;
				}
			}
			++itTok;
		}
	}
	bp.itBracket = tokens.end();
	return bp;
}

void highlightTaskMarker(StyleContext &sc, LexAccessor &styler,
		int activity, const WordList &markerList, bool caseSensitive){
	if ((isoperator(sc.chPrev) || IsASpace(sc.chPrev)) && markerList.Length()) {
		const int lengthMarker = 50;
		char marker[lengthMarker+1] = "";
		const Sci_Position currPos = static_cast<Sci_Position>(sc.currentPos);
		int i = 0;
		while (i < lengthMarker) {
			const char ch = styler.SafeGetCharAt(currPos + i);
			if (IsASpace(ch) || isoperator(ch)) {
				break;
			}
			if (caseSensitive)
				marker[i] = ch;
			else
				marker[i] = MakeLowerCase(ch);
			i++;
		}
		marker[i] = '\0';
		if (markerList.InList(marker)) {
			sc.SetState(SCE_C_TASKMARKER|activity);
		}
	}
}

struct EscapeSequence {
	int digitsLeft;
	CharacterSet setHexDigits;
	CharacterSet setOctDigits;
	CharacterSet setNoneNumeric;
	CharacterSet *escapeSetValid;
	EscapeSequence() {
		digitsLeft = 0;
		escapeSetValid = 0;
		setHexDigits = CharacterSet(CharacterSet::setDigits, "ABCDEFabcdef");
		setOctDigits = CharacterSet(CharacterSet::setNone, "01234567");
	}
	void resetEscapeState(int nextChar) {
		digitsLeft = 0;
		escapeSetValid = &setNoneNumeric;
		if (nextChar == 'U') {
			digitsLeft = 9;
			escapeSetValid = &setHexDigits;
		} else if (nextChar == 'u') {
			digitsLeft = 5;
			escapeSetValid = &setHexDigits;
		} else if (nextChar == 'x') {
			digitsLeft = 5;
			escapeSetValid = &setHexDigits;
		} else if (setOctDigits.Contains(nextChar)) {
			digitsLeft = 3;
			escapeSetValid = &setOctDigits;
		}
	}
	bool atEscapeEnd(int currChar) const {
		return (digitsLeft <= 0) || !escapeSetValid->Contains(currChar);
	}
};

std::string GetRestOfLine(LexAccessor &styler, Sci_Position start, bool allowSpace) {
	std::string restOfLine;
	Sci_Position i =0;
	char ch = styler.SafeGetCharAt(start, '\n');
	const Sci_Position endLine = styler.LineEnd(styler.GetLine(start));
	while (((start+i) < endLine) && (ch != '\r')) {
		const char chNext = styler.SafeGetCharAt(start + i + 1, '\n');
		if (ch == '/' && (chNext == '/' || chNext == '*'))
			break;
		if (allowSpace || (ch != ' '))
			restOfLine += ch;
		i++;
		ch = chNext;
	}
	return restOfLine;
}

bool IsStreamCommentStyle(int style) noexcept {
	return style == SCE_C_COMMENT ||
		style == SCE_C_COMMENTDOC ||
		style == SCE_C_COMMENTDOCKEYWORD ||
		style == SCE_C_COMMENTDOCKEYWORDERROR;
}

struct PPDefinition {
	Sci_Position line;
	std::string key;
	std::string value;
	bool isUndef;
	std::string arguments;
	PPDefinition(Sci_Position line_, const std::string &key_, const std::string &value_, bool isUndef_ = false, const std::string &arguments_="") :
		line(line_), key(key_), value(value_), isUndef(isUndef_), arguments(arguments_) {
	}
};

class LinePPState {
	int state;
	int ifTaken;
	int level;
	bool ValidLevel() const noexcept {
		return level >= 0 && level < 32;
	}
	int maskLevel() const noexcept {
		if (level >= 0) {
			return 1 << level;
		} else {
			return 1;
		}
	}
public:
	LinePPState() : state(0), ifTaken(0), level(-1) {
	}
	bool IsInactive() const noexcept {
		return state != 0;
	}
	bool CurrentIfTaken() const noexcept {
		return (ifTaken & maskLevel()) != 0;
	}
	void StartSection(bool on) noexcept {
		level++;
		if (ValidLevel()) {
			if (on) {
				state &= ~maskLevel();
				ifTaken |= maskLevel();
			} else {
				state |= maskLevel();
				ifTaken &= ~maskLevel();
			}
		}
	}
	void EndSection() noexcept {
		if (ValidLevel()) {
			state &= ~maskLevel();
			ifTaken &= ~maskLevel();
		}
		level--;
	}
	void InvertCurrentLevel() noexcept {
		if (ValidLevel()) {
			state ^= maskLevel();
			ifTaken |= maskLevel();
		}
	}
};

// Hold the preprocessor state for each line seen.
// Currently one entry per line but could become sparse with just one entry per preprocessor line.
class PPStates {
	std::vector<LinePPState> vlls;
public:
	LinePPState ForLine(Sci_Position line) const {
		if ((line > 0) && (vlls.size() > static_cast<size_t>(line))) {
			return vlls[line];
		} else {
			return LinePPState();
		}
	}
	void Add(Sci_Position line, LinePPState lls) {
		vlls.resize(line+1);
		vlls[line] = lls;
	}
};

// An individual named option for use in an OptionSet

// Options used for LexerCPP
struct OptionsCPP {
	bool stylingWithinPreprocessor;
	bool identifiersAllowDollars;
	bool trackPreprocessor;
	bool updatePreprocessor;
	bool verbatimStringsAllowEscapes;
	bool triplequotedStrings;
	bool hashquotedStrings;
	bool backQuotedStrings;
	bool escapeSequence;
	bool fold;
	bool foldSyntaxBased;
	bool foldComment;
	bool foldCommentMultiline;
	bool foldCommentExplicit;
	std::string foldExplicitStart;
	std::string foldExplicitEnd;
	bool foldExplicitAnywhere;
	bool foldPreprocessor;
	bool foldPreprocessorAtElse;
	bool foldCompact;
	bool foldAtElse;
	OptionsCPP() {
		stylingWithinPreprocessor = false;
		identifiersAllowDollars = true;
		trackPreprocessor = true;
		updatePreprocessor = true;
		verbatimStringsAllowEscapes = false;
		triplequotedStrings = false;
		hashquotedStrings = false;
		backQuotedStrings = false;
		escapeSequence = false;
		fold = false;
		foldSyntaxBased = true;
		foldComment = false;
		foldCommentMultiline = true;
		foldCommentExplicit = true;
		foldExplicitStart = "";
		foldExplicitEnd = "";
		foldExplicitAnywhere = false;
		foldPreprocessor = false;
		foldPreprocessorAtElse = false;
		foldCompact = false;
		foldAtElse = false;
	}
};

const char *const cppWordLists[] = {
            "Primary keywords and identifiers",
            "Secondary keywords and identifiers",
            "Documentation comment keywords",
            "Global classes and typedefs",
            "Preprocessor definitions",
            "Task marker and error marker keywords",
            0,
};

struct OptionSetCPP : public OptionSet<OptionsCPP> {
	OptionSetCPP() {
		DefineProperty("styling.within.preprocessor", &OptionsCPP::stylingWithinPreprocessor,
			"For C++ code, determines whether all preprocessor code is styled in the "
			"preprocessor style (0, the default) or only from the initial # to the end "
			"of the command word(1).");

		DefineProperty("lexer.cpp.allow.dollars", &OptionsCPP::identifiersAllowDollars,
			"Set to 0 to disallow the '$' character in identifiers with the cpp lexer.");

		DefineProperty("lexer.cpp.track.preprocessor", &OptionsCPP::trackPreprocessor,
			"Set to 1 to interpret #if/#else/#endif to grey out code that is not active.");

		DefineProperty("lexer.cpp.update.preprocessor", &OptionsCPP::updatePreprocessor,
			"Set to 1 to update preprocessor definitions when #define found.");

		DefineProperty("lexer.cpp.verbatim.strings.allow.escapes", &OptionsCPP::verbatimStringsAllowEscapes,
			"Set to 1 to allow verbatim strings to contain escape sequences.");

		DefineProperty("lexer.cpp.triplequoted.strings", &OptionsCPP::triplequotedStrings,
			"Set to 1 to enable highlighting of triple-quoted strings.");

		DefineProperty("lexer.cpp.hashquoted.strings", &OptionsCPP::hashquotedStrings,
			"Set to 1 to enable highlighting of hash-quoted strings.");

		DefineProperty("lexer.cpp.backquoted.strings", &OptionsCPP::backQuotedStrings,
			"Set to 1 to enable highlighting of back-quoted raw strings .");

		DefineProperty("lexer.cpp.escape.sequence", &OptionsCPP::escapeSequence,
			"Set to 1 to enable highlighting of escape sequences in strings");

		DefineProperty("fold", &OptionsCPP::fold);

		DefineProperty("fold.cpp.syntax.based", &OptionsCPP::foldSyntaxBased,
			"Set this property to 0 to disable syntax based folding.");

		DefineProperty("fold.comment", &OptionsCPP::foldComment,
			"This option enables folding multi-line comments and explicit fold points when using the C++ lexer. "
			"Explicit fold points allows adding extra folding by placing a //{ comment at the start and a //} "
			"at the end of a section that should fold.");

		DefineProperty("fold.cpp.comment.multiline", &OptionsCPP::foldCommentMultiline,
			"Set this property to 0 to disable folding multi-line comments when fold.comment=1.");

		DefineProperty("fold.cpp.comment.explicit", &OptionsCPP::foldCommentExplicit,
			"Set this property to 0 to disable folding explicit fold points when fold.comment=1.");

		DefineProperty("fold.cpp.explicit.start", &OptionsCPP::foldExplicitStart,
			"The string to use for explicit fold start points, replacing the standard //{.");

		DefineProperty("fold.cpp.explicit.end", &OptionsCPP::foldExplicitEnd,
			"The string to use for explicit fold end points, replacing the standard //}.");

		DefineProperty("fold.cpp.explicit.anywhere", &OptionsCPP::foldExplicitAnywhere,
			"Set this property to 1 to enable explicit fold points anywhere, not just in line comments.");

		DefineProperty("fold.cpp.preprocessor.at.else", &OptionsCPP::foldPreprocessorAtElse,
			"This option enables folding on a preprocessor #else or #endif line of an #if statement.");

		DefineProperty("fold.preprocessor", &OptionsCPP::foldPreprocessor,
			"This option enables folding preprocessor directives when using the C++ lexer. "
			"Includes C#'s explicit #region and #endregion folding directives.");

		DefineProperty("fold.compact", &OptionsCPP::foldCompact);

		DefineProperty("fold.at.else", &OptionsCPP::foldAtElse,
			"This option enables C++ folding on a \"} else {\" line of an if statement.");

		DefineWordListSets(cppWordLists);
	}
};

const char styleSubable[] = {SCE_C_IDENTIFIER, SCE_C_COMMENTDOCKEYWORD, 0};

LexicalClass lexicalClasses[] = {
	// Lexer Cpp SCLEX_CPP SCE_C_:
	0, "SCE_C_DEFAULT", "default", "White space",
	1, "SCE_C_COMMENT", "comment", "Comment: /* */.",
	2, "SCE_C_COMMENTLINE", "comment line", "Line Comment: //.",
	3, "SCE_C_COMMENTDOC", "comment documentation", "Doc comment: block comments beginning with /** or /*!",
	4, "SCE_C_NUMBER", "literal numeric", "Number",
	5, "SCE_C_WORD", "keyword", "Keyword",
	6, "SCE_C_STRING", "literal string", "Double quoted string",
	7, "SCE_C_CHARACTER", "literal string character", "Single quoted string",
	8, "SCE_C_UUID", "literal uuid", "UUIDs (only in IDL)",
	9, "SCE_C_PREPROCESSOR", "preprocessor", "Preprocessor",
	10, "SCE_C_OPERATOR", "operator", "Operators",
	11, "SCE_C_IDENTIFIER", "identifier", "Identifiers",
	12, "SCE_C_STRINGEOL", "error literal string", "End of line where string is not closed",
	13, "SCE_C_VERBATIM", "literal string multiline raw", "Verbatim strings for C#",
	14, "SCE_C_REGEX", "literal regex", "Regular expressions for JavaScript",
	15, "SCE_C_COMMENTLINEDOC", "comment documentation line", "Doc Comment Line: line comments beginning with /// or //!.",
	16, "SCE_C_WORD2", "identifier", "Keywords2",
	17, "SCE_C_COMMENTDOCKEYWORD", "comment documentation keyword", "Comment keyword",
	18, "SCE_C_COMMENTDOCKEYWORDERROR", "error comment documentation keyword", "Comment keyword error",
	19, "SCE_C_GLOBALCLASS", "identifier", "Global class",
	20, "SCE_C_STRINGRAW", "literal string multiline raw", "Raw strings for C++0x",
	21, "SCE_C_TRIPLEVERBATIM", "literal string multiline raw", "Triple-quoted strings for Vala",
	22, "SCE_C_HASHQUOTEDSTRING", "literal string", "Hash-quoted strings for Pike",
	23, "SCE_C_PREPROCESSORCOMMENT", "comment preprocessor", "Preprocessor stream comment",
	24, "SCE_C_PREPROCESSORCOMMENTDOC", "comment preprocessor documentation", "Preprocessor stream doc comment",
	25, "SCE_C_USERLITERAL", "literal", "User defined literals",
	26, "SCE_C_TASKMARKER", "comment taskmarker", "Task Marker",
	27, "SCE_C_ESCAPESEQUENCE", "literal string escapesequence", "Escape sequence",
};

}

class LexerCPP : public ILexerWithMetaData {
	bool caseSensitive;
	CharacterSet setWord;
	CharacterSet setNegationOp;
	CharacterSet setArithmethicOp;
	CharacterSet setRelOp;
	CharacterSet setLogicalOp;
	CharacterSet setWordStart;
	PPStates vlls;
	std::vector<PPDefinition> ppDefineHistory;
	WordList keywords;
	WordList keywords2;
	WordList keywords3;
	WordList keywords4;
	WordList ppDefinitions;
	WordList markerList;
	struct SymbolValue {
		std::string value;
		std::string arguments;
		SymbolValue(const std::string &value_="", const std::string &arguments_="") : value(value_), arguments(arguments_) {
		}
		SymbolValue &operator = (const std::string &value_) {
			value = value_;
			arguments.clear();
			return *this;
		}
		bool IsMacro() const noexcept {
			return !arguments.empty();
		}
	};
	typedef std::map<std::string, SymbolValue> SymbolTable;
	SymbolTable preprocessorDefinitionsStart;
	OptionsCPP options;
	OptionSetCPP osCPP;
	EscapeSequence escapeSeq;
	SparseState<std::string> rawStringTerminators;
	enum { activeFlag = 0x40 };
	enum { ssIdentifier, ssDocKeyword };
	SubStyles subStyles;
	std::string returnBuffer;
public:
	explicit LexerCPP(bool caseSensitive_) :
		caseSensitive(caseSensitive_),
		setWord(CharacterSet::setAlphaNum, "._", 0x80, true),
		setNegationOp(CharacterSet::setNone, "!"),
		setArithmethicOp(CharacterSet::setNone, "+-/*%"),
		setRelOp(CharacterSet::setNone, "=!<>"),
		setLogicalOp(CharacterSet::setNone, "|&"),
		subStyles(styleSubable, 0x80, 0x40, activeFlag) {
	}
	virtual ~LexerCPP() {
	}
	void SCI_METHOD Release() override {
		delete this;
	}
	int SCI_METHOD Version() const override {
		return lvMetaData;
	}
	const char * SCI_METHOD PropertyNames() override {
		return osCPP.PropertyNames();
	}
	int SCI_METHOD PropertyType(const char *name) override {
		return osCPP.PropertyType(name);
	}
	const char * SCI_METHOD DescribeProperty(const char *name) override {
		return osCPP.DescribeProperty(name);
	}
	Sci_Position SCI_METHOD PropertySet(const char *key, const char *val) override;
	const char * SCI_METHOD DescribeWordListSets() override {
		return osCPP.DescribeWordListSets();
	}
	Sci_Position SCI_METHOD WordListSet(int n, const char *wl) override;
	void SCI_METHOD Lex(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) override;
	void SCI_METHOD Fold(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) override;

	void * SCI_METHOD PrivateCall(int, void *) override {
		return 0;
	}

	int SCI_METHOD LineEndTypesSupported() override {
		return SC_LINE_END_TYPE_UNICODE;
	}

	int SCI_METHOD AllocateSubStyles(int styleBase, int numberStyles) override {
		return subStyles.Allocate(styleBase, numberStyles);
	}
	int SCI_METHOD SubStylesStart(int styleBase) override {
		return subStyles.Start(styleBase);
	}
	int SCI_METHOD SubStylesLength(int styleBase) override {
		return subStyles.Length(styleBase);
	}
	int SCI_METHOD StyleFromSubStyle(int subStyle) override {
		const int styleBase = subStyles.BaseStyle(MaskActive(subStyle));
		const int active = subStyle & activeFlag;
		return styleBase | active;
	}
	int SCI_METHOD PrimaryStyleFromStyle(int style) override {
		return MaskActive(style);
	}
	void SCI_METHOD FreeSubStyles() override {
		subStyles.Free();
	}
	void SCI_METHOD SetIdentifiers(int style, const char *identifiers) override {
		subStyles.SetIdentifiers(style, identifiers);
	}
	int SCI_METHOD DistanceToSecondaryStyles() override {
		return activeFlag;
	}
	const char * SCI_METHOD GetSubStyleBases() override {
		return styleSubable;
	}
	int SCI_METHOD NamedStyles() override {
		return std::max(subStyles.LastAllocated() + 1,
			static_cast<int>(ELEMENTS(lexicalClasses))) +
			activeFlag;
	}
	const char * SCI_METHOD NameOfStyle(int style) override {
		if (style >= NamedStyles())
			return "";
		if (style < static_cast<int>(ELEMENTS(lexicalClasses)))
			return lexicalClasses[style].name;
		// TODO: inactive and substyles
		return "";
	}
	const char * SCI_METHOD TagsOfStyle(int style) override {
		if (style >= NamedStyles())
			return "Excess";
		returnBuffer.clear();
		const int firstSubStyle = subStyles.FirstAllocated();
		if (firstSubStyle >= 0) {
			const int lastSubStyle = subStyles.LastAllocated();
			if (((style >= firstSubStyle) && (style <= (lastSubStyle))) ||
				((style >= firstSubStyle + activeFlag) && (style <= (lastSubStyle + activeFlag)))) {
				int styleActive = style;
				if (style > lastSubStyle) {
					returnBuffer = "inactive ";
					styleActive -= activeFlag;
				}
				const int styleMain = StyleFromSubStyle(styleActive);
				returnBuffer += lexicalClasses[styleMain].tags;
				return returnBuffer.c_str();
			}
		}
		if (style < static_cast<int>(ELEMENTS(lexicalClasses)))
			return lexicalClasses[style].tags;
		if (style >= activeFlag) {
			returnBuffer = "inactive ";
			const int styleActive = style - activeFlag;
			if (styleActive < static_cast<int>(ELEMENTS(lexicalClasses)))
				returnBuffer += lexicalClasses[styleActive].tags;
			else
				returnBuffer = "";
			return returnBuffer.c_str();
		}
		return "";
	}
	const char * SCI_METHOD DescriptionOfStyle(int style) override {
		if (style >= NamedStyles())
			return "";
		if (style < static_cast<int>(ELEMENTS(lexicalClasses)))
			return lexicalClasses[style].description;
		// TODO: inactive and substyles
		return "";
	}

	static ILexer *LexerFactoryCPP() {
		return new LexerCPP(true);
	}
	static ILexer *LexerFactoryCPPInsensitive() {
		return new LexerCPP(false);
	}
	static int MaskActive(int style) noexcept {
		return style & ~activeFlag;
	}
	void EvaluateTokens(std::vector<std::string> &tokens, const SymbolTable &preprocessorDefinitions);
	std::vector<std::string> Tokenize(const std::string &expr) const;
	bool EvaluateExpression(const std::string &expr, const SymbolTable &preprocessorDefinitions);
};

Sci_Position SCI_METHOD LexerCPP::PropertySet(const char *key, const char *val) {
	if (osCPP.PropertySet(&options, key, val)) {
		if (strcmp(key, "lexer.cpp.allow.dollars") == 0) {
			setWord = CharacterSet(CharacterSet::setAlphaNum, "._", 0x80, true);
			if (options.identifiersAllowDollars) {
				setWord.Add('$');
			}
		}
		return 0;
	}
	return -1;
}

Sci_Position SCI_METHOD LexerCPP::WordListSet(int n, const char *wl) {
	WordList *wordListN = 0;
	switch (n) {
	case 0:
		wordListN = &keywords;
		break;
	case 1:
		wordListN = &keywords2;
		break;
	case 2:
		wordListN = &keywords3;
		break;
	case 3:
		wordListN = &keywords4;
		break;
	case 4:
		wordListN = &ppDefinitions;
		break;
	case 5:
		wordListN = &markerList;
		break;
	}
	Sci_Position firstModification = -1;
	if (wordListN) {
		WordList wlNew;
		wlNew.Set(wl);
		if (*wordListN != wlNew) {
			wordListN->Set(wl);
			firstModification = 0;
			if (n == 4) {
				// Rebuild preprocessorDefinitions
				preprocessorDefinitionsStart.clear();
				for (int nDefinition = 0; nDefinition < ppDefinitions.Length(); nDefinition++) {
					const char *cpDefinition = ppDefinitions.WordAt(nDefinition);
					const char *cpEquals = strchr(cpDefinition, '=');
					if (cpEquals) {
						std::string name(cpDefinition, cpEquals - cpDefinition);
						std::string val(cpEquals+1);
						const size_t bracket = name.find('(');
						const size_t bracketEnd = name.find(')');
						if ((bracket != std::string::npos) && (bracketEnd != std::string::npos)) {
							// Macro
							std::string args = name.substr(bracket + 1, bracketEnd - bracket - 1);
							name = name.substr(0, bracket);
							preprocessorDefinitionsStart[name] = SymbolValue(val, args);
						} else {
							preprocessorDefinitionsStart[name] = val;
						}
					} else {
						std::string name(cpDefinition);
						std::string val("1");
						preprocessorDefinitionsStart[name] = val;
					}
				}
			}
		}
	}
	return firstModification;
}

void SCI_METHOD LexerCPP::Lex(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) {
	LexAccessor styler(pAccess);

	CharacterSet setOKBeforeRE(CharacterSet::setNone, "([{=,:;!%^&*|?~+-");
	CharacterSet setCouldBePostOp(CharacterSet::setNone, "+-");

	CharacterSet setDoxygen(CharacterSet::setAlpha, "$@\\&<>#{}[]");

	setWordStart = CharacterSet(CharacterSet::setAlpha, "_", 0x80, true);

	CharacterSet setInvalidRawFirst(CharacterSet::setNone, " )\\\t\v\f\n");

	if (options.identifiersAllowDollars) {
		setWordStart.Add('$');
	}

	int chPrevNonWhite = ' ';
	int visibleChars = 0;
	bool lastWordWasUUID = false;
	int styleBeforeDCKeyword = SCE_C_DEFAULT;
	int styleBeforeTaskMarker = SCE_C_DEFAULT;
	bool continuationLine = false;
	bool isIncludePreprocessor = false;
	bool isStringInPreprocessor = false;
	bool inRERange = false;
	bool seenDocKeyBrace = false;

	Sci_Position lineCurrent = styler.GetLine(startPos);
	if ((MaskActive(initStyle) == SCE_C_PREPROCESSOR) ||
      (MaskActive(initStyle) == SCE_C_COMMENTLINE) ||
      (MaskActive(initStyle) == SCE_C_COMMENTLINEDOC)) {
		// Set continuationLine if last character of previous line is '\'
		if (lineCurrent > 0) {
			const Sci_Position endLinePrevious = styler.LineEnd(lineCurrent - 1);
			if (endLinePrevious > 0) {
				continuationLine = styler.SafeGetCharAt(endLinePrevious-1) == '\\';
			}
		}
	}

	// look back to set chPrevNonWhite properly for better regex colouring
	if (startPos > 0) {
		Sci_Position back = startPos;
		while (--back && IsSpaceEquiv(MaskActive(styler.StyleAt(back))))
			;
		if (MaskActive(styler.StyleAt(back)) == SCE_C_OPERATOR) {
			chPrevNonWhite = styler.SafeGetCharAt(back);
		}
	}

	StyleContext sc(startPos, length, initStyle, styler);
	LinePPState preproc = vlls.ForLine(lineCurrent);

	bool definitionsChanged = false;

	// Truncate ppDefineHistory before current line

	if (!options.updatePreprocessor)
		ppDefineHistory.clear();

	std::vector<PPDefinition>::iterator itInvalid = std::find_if(ppDefineHistory.begin(), ppDefineHistory.end(),
		[lineCurrent](const PPDefinition &p) { return p.line >= lineCurrent; });
	if (itInvalid != ppDefineHistory.end()) {
		ppDefineHistory.erase(itInvalid, ppDefineHistory.end());
		definitionsChanged = true;
	}

	SymbolTable preprocessorDefinitions = preprocessorDefinitionsStart;
	for (const PPDefinition &ppDef : ppDefineHistory) {
		if (ppDef.isUndef)
			preprocessorDefinitions.erase(ppDef.key);
		else
			preprocessorDefinitions[ppDef.key] = SymbolValue(ppDef.value, ppDef.arguments);
	}

	std::string rawStringTerminator = rawStringTerminators.ValueAt(lineCurrent-1);
	SparseState<std::string> rawSTNew(lineCurrent);

	int activitySet = preproc.IsInactive() ? activeFlag : 0;

	const WordClassifier &classifierIdentifiers = subStyles.Classifier(SCE_C_IDENTIFIER);
	const WordClassifier &classifierDocKeyWords = subStyles.Classifier(SCE_C_COMMENTDOCKEYWORD);

	Sci_Position lineEndNext = styler.LineEnd(lineCurrent);

	for (; sc.More();) {

		if (sc.atLineStart) {
			// Using MaskActive() is not needed in the following statement.
			// Inside inactive preprocessor declaration, state will be reset anyway at the end of this block.
			if ((sc.state == SCE_C_STRING) || (sc.state == SCE_C_CHARACTER)) {
				// Prevent SCE_C_STRINGEOL from leaking back to previous line which
				// ends with a line continuation by locking in the state up to this position.
				sc.SetState(sc.state);
			}
			if ((MaskActive(sc.state) == SCE_C_PREPROCESSOR) && (!continuationLine)) {
				sc.SetState(SCE_C_DEFAULT|activitySet);
			}
			// Reset states to beginning of colourise so no surprises
			// if different sets of lines lexed.
			visibleChars = 0;
			lastWordWasUUID = false;
			isIncludePreprocessor = false;
			inRERange = false;
			if (preproc.IsInactive()) {
				activitySet = activeFlag;
				sc.SetState(sc.state | activitySet);
			}
		}

		if (sc.atLineEnd) {
			lineCurrent++;
			lineEndNext = styler.LineEnd(lineCurrent);
			vlls.Add(lineCurrent, preproc);
			if (rawStringTerminator != "") {
				rawSTNew.Set(lineCurrent-1, rawStringTerminator);
			}
		}

		// Handle line continuation generically.
		if (sc.ch == '\\') {
			if (static_cast<Sci_Position>((sc.currentPos+1)) >= lineEndNext) {
				lineCurrent++;
				lineEndNext = styler.LineEnd(lineCurrent);
				vlls.Add(lineCurrent, preproc);
				if (rawStringTerminator != "") {
					rawSTNew.Set(lineCurrent-1, rawStringTerminator);
				}
				sc.Forward();
				if (sc.ch == '\r' && sc.chNext == '\n') {
					// Even in UTF-8, \r and \n are separate
					sc.Forward();
				}
				continuationLine = true;
				sc.Forward();
				continue;
			}
		}

		const bool atLineEndBeforeSwitch = sc.atLineEnd;

		// Determine if the current state should terminate.
		switch (MaskActive(sc.state)) {
			case SCE_C_OPERATOR:
				sc.SetState(SCE_C_DEFAULT|activitySet);
				break;
			case SCE_C_NUMBER:
				// We accept almost anything because of hex. and number suffixes
				if (sc.ch == '_') {
					sc.ChangeState(SCE_C_USERLITERAL|activitySet);
				} else if (!(setWord.Contains(sc.ch)
				   || (sc.ch == '\'')
				   || ((sc.ch == '+' || sc.ch == '-') && (sc.chPrev == 'e' || sc.chPrev == 'E' ||
				                                          sc.chPrev == 'p' || sc.chPrev == 'P')))) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				}
				break;
			case SCE_C_USERLITERAL:
				if (!(setWord.Contains(sc.ch)))
					sc.SetState(SCE_C_DEFAULT|activitySet);
				break;
			case SCE_C_IDENTIFIER:
				if (sc.atLineStart || sc.atLineEnd || !setWord.Contains(sc.ch) || (sc.ch == '.')) {
					char s[1000];
					if (caseSensitive) {
						sc.GetCurrent(s, sizeof(s));
					} else {
						sc.GetCurrentLowered(s, sizeof(s));
					}
					if (keywords.InList(s)) {
						lastWordWasUUID = strcmp(s, "uuid") == 0;
						sc.ChangeState(SCE_C_WORD|activitySet);
					} else if (keywords2.InList(s)) {
						sc.ChangeState(SCE_C_WORD2|activitySet);
					} else if (keywords4.InList(s)) {
						sc.ChangeState(SCE_C_GLOBALCLASS|activitySet);
					} else {
						int subStyle = classifierIdentifiers.ValueFor(s);
						if (subStyle >= 0) {
							sc.ChangeState(subStyle|activitySet);
						}
					}
					const bool literalString = sc.ch == '\"';
					if (literalString || sc.ch == '\'') {
						size_t lenS = strlen(s);
						const bool raw = literalString && sc.chPrev == 'R' && !setInvalidRawFirst.Contains(sc.chNext);
						if (raw)
							s[lenS--] = '\0';
						const bool valid =
							(lenS == 0) ||
							((lenS == 1) && ((s[0] == 'L') || (s[0] == 'u') || (s[0] == 'U'))) ||
							((lenS == 2) && literalString && (s[0] == 'u') && (s[1] == '8'));
						if (valid) {
							if (literalString) {
								if (raw) {
									// Set the style of the string prefix to SCE_C_STRINGRAW but then change to
									// SCE_C_DEFAULT as that allows the raw string start code to run.
									sc.ChangeState(SCE_C_STRINGRAW|activitySet);
									sc.SetState(SCE_C_DEFAULT|activitySet);
								} else {
									sc.ChangeState(SCE_C_STRING|activitySet);
								}
							} else {
								sc.ChangeState(SCE_C_CHARACTER|activitySet);
							}
						} else {
							sc.SetState(SCE_C_DEFAULT | activitySet);
						}
					} else {
						sc.SetState(SCE_C_DEFAULT|activitySet);
					}
				}
				break;
			case SCE_C_PREPROCESSOR:
				if (options.stylingWithinPreprocessor) {
					if (IsASpace(sc.ch)) {
						sc.SetState(SCE_C_DEFAULT|activitySet);
					}
				} else if (isStringInPreprocessor && (sc.Match('>') || sc.Match('\"') || sc.atLineEnd)) {
					isStringInPreprocessor = false;
				} else if (!isStringInPreprocessor) {
					if ((isIncludePreprocessor && sc.Match('<')) || sc.Match('\"')) {
						isStringInPreprocessor = true;
					} else if (sc.Match('/', '*')) {
						if (sc.Match("/**") || sc.Match("/*!")) {
							sc.SetState(SCE_C_PREPROCESSORCOMMENTDOC|activitySet);
						} else {
							sc.SetState(SCE_C_PREPROCESSORCOMMENT|activitySet);
						}
						sc.Forward();	// Eat the *
					} else if (sc.Match('/', '/')) {
						sc.SetState(SCE_C_DEFAULT|activitySet);
					}
				}
				break;
			case SCE_C_PREPROCESSORCOMMENT:
			case SCE_C_PREPROCESSORCOMMENTDOC:
				if (sc.Match('*', '/')) {
					sc.Forward();
					sc.ForwardSetState(SCE_C_PREPROCESSOR|activitySet);
					continue;	// Without advancing in case of '\'.
				}
				break;
			case SCE_C_COMMENT:
				if (sc.Match('*', '/')) {
					sc.Forward();
					sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
				} else {
					styleBeforeTaskMarker = SCE_C_COMMENT;
					highlightTaskMarker(sc, styler, activitySet, markerList, caseSensitive);
				}
				break;
			case SCE_C_COMMENTDOC:
				if (sc.Match('*', '/')) {
					sc.Forward();
					sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
				} else if (sc.ch == '@' || sc.ch == '\\') { // JavaDoc and Doxygen support
					// Verify that we have the conditions to mark a comment-doc-keyword
					if ((IsASpace(sc.chPrev) || sc.chPrev == '*') && (!IsASpace(sc.chNext))) {
						styleBeforeDCKeyword = SCE_C_COMMENTDOC;
						sc.SetState(SCE_C_COMMENTDOCKEYWORD|activitySet);
					}
				}
				break;
			case SCE_C_COMMENTLINE:
				if (sc.atLineStart && !continuationLine) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				} else {
					styleBeforeTaskMarker = SCE_C_COMMENTLINE;
					highlightTaskMarker(sc, styler, activitySet, markerList, caseSensitive);
				}
				break;
			case SCE_C_COMMENTLINEDOC:
				if (sc.atLineStart && !continuationLine) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				} else if (sc.ch == '@' || sc.ch == '\\') { // JavaDoc and Doxygen support
					// Verify that we have the conditions to mark a comment-doc-keyword
					if ((IsASpace(sc.chPrev) || sc.chPrev == '/' || sc.chPrev == '!') && (!IsASpace(sc.chNext))) {
						styleBeforeDCKeyword = SCE_C_COMMENTLINEDOC;
						sc.SetState(SCE_C_COMMENTDOCKEYWORD|activitySet);
					}
				}
				break;
			case SCE_C_COMMENTDOCKEYWORD:
				if ((styleBeforeDCKeyword == SCE_C_COMMENTDOC) && sc.Match('*', '/')) {
					sc.ChangeState(SCE_C_COMMENTDOCKEYWORDERROR);
					sc.Forward();
					sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
					seenDocKeyBrace = false;
				} else if (sc.ch == '[' || sc.ch == '{') {
					seenDocKeyBrace = true;
				} else if (!setDoxygen.Contains(sc.ch)
				           && !(seenDocKeyBrace && (sc.ch == ',' || sc.ch == '.'))) {
					char s[100];
					if (caseSensitive) {
						sc.GetCurrent(s, sizeof(s));
					} else {
						sc.GetCurrentLowered(s, sizeof(s));
					}
					if (!(IsASpace(sc.ch) || (sc.ch == 0))) {
						sc.ChangeState(SCE_C_COMMENTDOCKEYWORDERROR|activitySet);
					} else if (!keywords3.InList(s + 1)) {
						int subStyleCDKW = classifierDocKeyWords.ValueFor(s+1);
						if (subStyleCDKW >= 0) {
							sc.ChangeState(subStyleCDKW|activitySet);
						} else {
							sc.ChangeState(SCE_C_COMMENTDOCKEYWORDERROR|activitySet);
						}
					}
					sc.SetState(styleBeforeDCKeyword|activitySet);
					seenDocKeyBrace = false;
				}
				break;
			case SCE_C_STRING:
				if (sc.atLineEnd) {
					sc.ChangeState(SCE_C_STRINGEOL|activitySet);
				} else if (isIncludePreprocessor) {
					if (sc.ch == '>') {
						sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
						isIncludePreprocessor = false;
					}
				} else if (sc.ch == '\\') {
					if (options.escapeSequence) {
						sc.SetState(SCE_C_ESCAPESEQUENCE|activitySet);
						escapeSeq.resetEscapeState(sc.chNext);
					}
					sc.Forward(); // Skip all characters after the backslash
				} else if (sc.ch == '\"') {
					if (sc.chNext == '_') {
						sc.ChangeState(SCE_C_USERLITERAL|activitySet);
					} else {
						sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
					}
				}
				break;
			case SCE_C_ESCAPESEQUENCE:
				escapeSeq.digitsLeft--;
				if (!escapeSeq.atEscapeEnd(sc.ch)) {
					break;
				}
				if (sc.ch == '"') {
					sc.SetState(SCE_C_STRING|activitySet);
					sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
				} else if (sc.ch == '\\') {
					escapeSeq.resetEscapeState(sc.chNext);
					sc.Forward();
				} else {
					sc.SetState(SCE_C_STRING|activitySet);
					if (sc.atLineEnd) {
						sc.ChangeState(SCE_C_STRINGEOL|activitySet);
					}
				}
				break;
			case SCE_C_HASHQUOTEDSTRING:
				if (sc.ch == '\\') {
					if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
						sc.Forward();
					}
				} else if (sc.ch == '\"') {
					sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
				}
				break;
			case SCE_C_STRINGRAW:
				if (sc.Match(rawStringTerminator.c_str())) {
					for (size_t termPos=rawStringTerminator.size(); termPos; termPos--)
						sc.Forward();
					sc.SetState(SCE_C_DEFAULT|activitySet);
					rawStringTerminator = "";
				}
				break;
			case SCE_C_CHARACTER:
				if (sc.atLineEnd) {
					sc.ChangeState(SCE_C_STRINGEOL|activitySet);
				} else if (sc.ch == '\\') {
					if (sc.chNext == '\"' || sc.chNext == '\'' || sc.chNext == '\\') {
						sc.Forward();
					}
				} else if (sc.ch == '\'') {
					if (sc.chNext == '_') {
						sc.ChangeState(SCE_C_USERLITERAL|activitySet);
					} else {
						sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
					}
				}
				break;
			case SCE_C_REGEX:
				if (sc.atLineStart) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				} else if (! inRERange && sc.ch == '/') {
					sc.Forward();
					while ((sc.ch < 0x80) && islower(sc.ch))
						sc.Forward();    // gobble regex flags
					sc.SetState(SCE_C_DEFAULT|activitySet);
				} else if (sc.ch == '\\' && (static_cast<Sci_Position>(sc.currentPos+1) < lineEndNext)) {
					// Gobble up the escaped character
					sc.Forward();
				} else if (sc.ch == '[') {
					inRERange = true;
				} else if (sc.ch == ']') {
					inRERange = false;
				}
				break;
			case SCE_C_STRINGEOL:
				if (sc.atLineStart) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				}
				break;
			case SCE_C_VERBATIM:
				if (options.verbatimStringsAllowEscapes && (sc.ch == '\\')) {
					sc.Forward(); // Skip all characters after the backslash
				} else if (sc.ch == '\"') {
					if (sc.chNext == '\"') {
						sc.Forward();
					} else {
						sc.ForwardSetState(SCE_C_DEFAULT|activitySet);
					}
				}
				break;
			case SCE_C_TRIPLEVERBATIM:
				if (sc.Match(R"(""")")) {
					while (sc.Match('"')) {
						sc.Forward();
					}
					sc.SetState(SCE_C_DEFAULT|activitySet);
				}
				break;
			case SCE_C_UUID:
				if (sc.atLineEnd || sc.ch == ')') {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				}
				break;
			case SCE_C_TASKMARKER:
				if (isoperator(sc.ch) || IsASpace(sc.ch)) {
					sc.SetState(styleBeforeTaskMarker|activitySet);
					styleBeforeTaskMarker = SCE_C_DEFAULT;
				}
		}

		if (sc.atLineEnd && !atLineEndBeforeSwitch) {
			// State exit processing consumed characters up to end of line.
			lineCurrent++;
			lineEndNext = styler.LineEnd(lineCurrent);
			vlls.Add(lineCurrent, preproc);
		}

		// Determine if a new state should be entered.
		if (MaskActive(sc.state) == SCE_C_DEFAULT) {
			if (sc.Match('@', '\"')) {
				sc.SetState(SCE_C_VERBATIM|activitySet);
				sc.Forward();
			} else if (options.triplequotedStrings && sc.Match(R"(""")")) {
				sc.SetState(SCE_C_TRIPLEVERBATIM|activitySet);
				sc.Forward(2);
			} else if (options.hashquotedStrings && sc.Match('#', '\"')) {
				sc.SetState(SCE_C_HASHQUOTEDSTRING|activitySet);
				sc.Forward();
			} else if (options.backQuotedStrings && sc.Match('`')) {
				sc.SetState(SCE_C_STRINGRAW|activitySet);
				rawStringTerminator = "`";
			} else if (IsADigit(sc.ch) || (sc.ch == '.' && IsADigit(sc.chNext))) {
				if (lastWordWasUUID) {
					sc.SetState(SCE_C_UUID|activitySet);
					lastWordWasUUID = false;
				} else {
					sc.SetState(SCE_C_NUMBER|activitySet);
				}
			} else if (!sc.atLineEnd && (setWordStart.Contains(sc.ch) || (sc.ch == '@'))) {
				if (lastWordWasUUID) {
					sc.SetState(SCE_C_UUID|activitySet);
					lastWordWasUUID = false;
				} else {
					sc.SetState(SCE_C_IDENTIFIER|activitySet);
				}
			} else if (sc.Match('/', '*')) {
				if (sc.Match("/**") || sc.Match("/*!")) {	// Support of Qt/Doxygen doc. style
					sc.SetState(SCE_C_COMMENTDOC|activitySet);
				} else {
					sc.SetState(SCE_C_COMMENT|activitySet);
				}
				sc.Forward();	// Eat the * so it isn't used for the end of the comment
			} else if (sc.Match('/', '/')) {
				if ((sc.Match("///") && !sc.Match("////")) || sc.Match("//!"))
					// Support of Qt/Doxygen doc. style
					sc.SetState(SCE_C_COMMENTLINEDOC|activitySet);
				else
					sc.SetState(SCE_C_COMMENTLINE|activitySet);
			} else if (sc.ch == '/'
				   && (setOKBeforeRE.Contains(chPrevNonWhite)
				       || followsReturnKeyword(sc, styler))
				   && (!setCouldBePostOp.Contains(chPrevNonWhite)
				       || !FollowsPostfixOperator(sc, styler))) {
				sc.SetState(SCE_C_REGEX|activitySet);	// JavaScript's RegEx
				inRERange = false;
			} else if (sc.ch == '\"') {
				if (sc.chPrev == 'R') {
					styler.Flush();
					if (MaskActive(styler.StyleAt(sc.currentPos - 1)) == SCE_C_STRINGRAW) {
						sc.SetState(SCE_C_STRINGRAW|activitySet);
						rawStringTerminator = ")";
						for (Sci_Position termPos = sc.currentPos + 1;; termPos++) {
							const char chTerminator = styler.SafeGetCharAt(termPos, '(');
							if (chTerminator == '(')
								break;
							rawStringTerminator += chTerminator;
						}
						rawStringTerminator += '\"';
					} else {
						sc.SetState(SCE_C_STRING|activitySet);
					}
				} else {
					sc.SetState(SCE_C_STRING|activitySet);
				}
				isIncludePreprocessor = false;	// ensure that '>' won't end the string
			} else if (isIncludePreprocessor && sc.ch == '<') {
				sc.SetState(SCE_C_STRING|activitySet);
			} else if (sc.ch == '\'') {
				sc.SetState(SCE_C_CHARACTER|activitySet);
			} else if (sc.ch == '#' && visibleChars == 0) {
				// Preprocessor commands are alone on their line
				sc.SetState(SCE_C_PREPROCESSOR|activitySet);
				// Skip whitespace between # and preprocessor word
				do {
					sc.Forward();
				} while ((sc.ch == ' ' || sc.ch == '\t') && sc.More());
				if (sc.atLineEnd) {
					sc.SetState(SCE_C_DEFAULT|activitySet);
				} else if (sc.Match("include")) {
					isIncludePreprocessor = true;
				} else {
					if (options.trackPreprocessor) {
						if (sc.Match("ifdef") || sc.Match("ifndef")) {
							const bool isIfDef = sc.Match("ifdef");
							const int startRest = isIfDef ? 5 : 6;
							std::string restOfLine = GetRestOfLine(styler, sc.currentPos + startRest + 1, false);
							bool foundDef = preprocessorDefinitions.find(restOfLine) != preprocessorDefinitions.end();
							preproc.StartSection(isIfDef == foundDef);
						} else if (sc.Match("if")) {
							std::string restOfLine = GetRestOfLine(styler, sc.currentPos + 2, true);
							const bool ifGood = EvaluateExpression(restOfLine, preprocessorDefinitions);
							preproc.StartSection(ifGood);
						} else if (sc.Match("else")) {
							if (!preproc.CurrentIfTaken()) {
								preproc.InvertCurrentLevel();
								activitySet = preproc.IsInactive() ? activeFlag : 0;
								if (!activitySet)
									sc.ChangeState(SCE_C_PREPROCESSOR|activitySet);
							} else if (!preproc.IsInactive()) {
								preproc.InvertCurrentLevel();
								activitySet = preproc.IsInactive() ? activeFlag : 0;
								if (!activitySet)
									sc.ChangeState(SCE_C_PREPROCESSOR|activitySet);
							}
						} else if (sc.Match("elif")) {
							// Ensure only one chosen out of #if .. #elif .. #elif .. #else .. #endif
							if (!preproc.CurrentIfTaken()) {
								// Similar to #if
								std::string restOfLine = GetRestOfLine(styler, sc.currentPos + 4, true);
								const bool ifGood = EvaluateExpression(restOfLine, preprocessorDefinitions);
								if (ifGood) {
									preproc.InvertCurrentLevel();
									activitySet = preproc.IsInactive() ? activeFlag : 0;
									if (!activitySet)
										sc.ChangeState(SCE_C_PREPROCESSOR|activitySet);
								}
							} else if (!preproc.IsInactive()) {
								preproc.InvertCurrentLevel();
								activitySet = preproc.IsInactive() ? activeFlag : 0;
								if (!activitySet)
									sc.ChangeState(SCE_C_PREPROCESSOR|activitySet);
							}
						} else if (sc.Match("endif")) {
							preproc.EndSection();
							activitySet = preproc.IsInactive() ? activeFlag : 0;
							sc.ChangeState(SCE_C_PREPROCESSOR|activitySet);
						} else if (sc.Match("define")) {
							if (options.updatePreprocessor && !preproc.IsInactive()) {
								std::string restOfLine = GetRestOfLine(styler, sc.currentPos + 6, true);
								size_t startName = 0;
								while ((startName < restOfLine.length()) && IsSpaceOrTab(restOfLine[startName]))
									startName++;
								size_t endName = startName;
								while ((endName < restOfLine.length()) && setWord.Contains(static_cast<unsigned char>(restOfLine[endName])))
									endName++;
								std::string key = restOfLine.substr(startName, endName-startName);
								if ((endName < restOfLine.length()) && (restOfLine.at(endName) == '(')) {
									// Macro
									size_t endArgs = endName;
									while ((endArgs < restOfLine.length()) && (restOfLine[endArgs] != ')'))
										endArgs++;
									std::string args = restOfLine.substr(endName + 1, endArgs - endName - 1);
									size_t startValue = endArgs+1;
									while ((startValue < restOfLine.length()) && IsSpaceOrTab(restOfLine[startValue]))
										startValue++;
									std::string value;
									if (startValue < restOfLine.length())
										value = restOfLine.substr(startValue);
									preprocessorDefinitions[key] = SymbolValue(value, args);
									ppDefineHistory.push_back(PPDefinition(lineCurrent, key, value, false, args));
									definitionsChanged = true;
								} else {
									// Value
									size_t startValue = endName;
									while ((startValue < restOfLine.length()) && IsSpaceOrTab(restOfLine[startValue]))
										startValue++;
									std::string value = restOfLine.substr(startValue);
									if (OnlySpaceOrTab(value))
										value = "1";	// No value defaults to 1
									preprocessorDefinitions[key] = value;
									ppDefineHistory.push_back(PPDefinition(lineCurrent, key, value));
									definitionsChanged = true;
								}
							}
						} else if (sc.Match("undef")) {
							if (options.updatePreprocessor && !preproc.IsInactive()) {
								const std::string restOfLine = GetRestOfLine(styler, sc.currentPos + 5, false);
								std::vector<std::string> tokens = Tokenize(restOfLine);
								if (tokens.size() >= 1) {
									const std::string key = tokens[0];
									preprocessorDefinitions.erase(key);
									ppDefineHistory.push_back(PPDefinition(lineCurrent, key, "", true));
									definitionsChanged = true;
								}
							}
						}
					}
				}
			} else if (isoperator(sc.ch)) {
				sc.SetState(SCE_C_OPERATOR|activitySet);
			}
		}

		if (!IsASpace(sc.ch) && !IsSpaceEquiv(MaskActive(sc.state))) {
			chPrevNonWhite = sc.ch;
			visibleChars++;
		}
		continuationLine = false;
		sc.Forward();
	}
	const bool rawStringsChanged = rawStringTerminators.Merge(rawSTNew, lineCurrent);
	if (definitionsChanged || rawStringsChanged)
		styler.ChangeLexerState(startPos, startPos + length);
	sc.Complete();
}

// Store both the current line's fold level and the next lines in the
// level store to make it easy to pick up with each increment
// and to make it possible to fiddle the current level for "} else {".

void SCI_METHOD LexerCPP::Fold(Sci_PositionU startPos, Sci_Position length, int initStyle, IDocument *pAccess) {

	if (!options.fold)
		return;

	LexAccessor styler(pAccess);

	const Sci_PositionU endPos = startPos + length;
	int visibleChars = 0;
	bool inLineComment = false;
	Sci_Position lineCurrent = styler.GetLine(startPos);
	int levelCurrent = SC_FOLDLEVELBASE;
	if (lineCurrent > 0)
		levelCurrent = styler.LevelAt(lineCurrent-1) >> 16;
	Sci_PositionU lineStartNext = styler.LineStart(lineCurrent+1);
	int levelMinCurrent = levelCurrent;
	int levelNext = levelCurrent;
	char chNext = styler[startPos];
	int styleNext = MaskActive(styler.StyleAt(startPos));
	int style = MaskActive(initStyle);
	const bool userDefinedFoldMarkers = !options.foldExplicitStart.empty() && !options.foldExplicitEnd.empty();
	for (Sci_PositionU i = startPos; i < endPos; i++) {
		const char ch = chNext;
		chNext = styler.SafeGetCharAt(i + 1);
		const int stylePrev = style;
		style = styleNext;
		styleNext = MaskActive(styler.StyleAt(i + 1));
		const bool atEOL = i == (lineStartNext-1);
		if ((style == SCE_C_COMMENTLINE) || (style == SCE_C_COMMENTLINEDOC))
			inLineComment = true;
		if (options.foldComment && options.foldCommentMultiline && IsStreamCommentStyle(style) && !inLineComment) {
			if (!IsStreamCommentStyle(stylePrev)) {
				levelNext++;
			} else if (!IsStreamCommentStyle(styleNext) && !atEOL) {
				// Comments don't end at end of line and the next character may be unstyled.
				levelNext--;
			}
		}
		if (options.foldComment && options.foldCommentExplicit && ((style == SCE_C_COMMENTLINE) || options.foldExplicitAnywhere)) {
			if (userDefinedFoldMarkers) {
				if (styler.Match(i, options.foldExplicitStart.c_str())) {
					levelNext++;
				} else if (styler.Match(i, options.foldExplicitEnd.c_str())) {
					levelNext--;
				}
			} else {
				if ((ch == '/') && (chNext == '/')) {
					const char chNext2 = styler.SafeGetCharAt(i + 2);
					if (chNext2 == '{') {
						levelNext++;
					} else if (chNext2 == '}') {
						levelNext--;
					}
				}
			}
		}
		if (options.foldPreprocessor && (style == SCE_C_PREPROCESSOR)) {
			if (ch == '#') {
				Sci_PositionU j = i + 1;
				while ((j < endPos) && IsASpaceOrTab(styler.SafeGetCharAt(j))) {
					j++;
				}
				if (styler.Match(j, "region") || styler.Match(j, "if")) {
					levelNext++;
				} else if (styler.Match(j, "end")) {
					levelNext--;
				}

				if (options.foldPreprocessorAtElse && (styler.Match(j, "else") || styler.Match(j, "elif"))) {
					levelMinCurrent--;
				}
			}
		}
		if (options.foldSyntaxBased && (style == SCE_C_OPERATOR)) {
			if (ch == '{' || ch == '[' || ch == '(') {
				// Measure the minimum before a '{' to allow
				// folding on "} else {"
				if (options.foldAtElse && levelMinCurrent > levelNext) {
					levelMinCurrent = levelNext;
				}
				levelNext++;
			} else if (ch == '}' || ch == ']' || ch == ')') {
				levelNext--;
			}
		}
		if (!IsASpace(ch))
			visibleChars++;
		if (atEOL || (i == endPos-1)) {
			int levelUse = levelCurrent;
			if ((options.foldSyntaxBased && options.foldAtElse) ||
				(options.foldPreprocessor && options.foldPreprocessorAtElse)
			) {
				levelUse = levelMinCurrent;
			}
			int lev = levelUse | levelNext << 16;
			if (visibleChars == 0 && options.foldCompact)
				lev |= SC_FOLDLEVELWHITEFLAG;
			if (levelUse < levelNext)
				lev |= SC_FOLDLEVELHEADERFLAG;
			if (lev != styler.LevelAt(lineCurrent)) {
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			lineStartNext = styler.LineStart(lineCurrent+1);
			levelCurrent = levelNext;
			levelMinCurrent = levelCurrent;
			if (atEOL && (i == static_cast<Sci_PositionU>(styler.Length()-1))) {
				// There is an empty line at end of file so give it same level and empty
				styler.SetLevel(lineCurrent, (levelCurrent | levelCurrent << 16) | SC_FOLDLEVELWHITEFLAG);
			}
			visibleChars = 0;
			inLineComment = false;
		}
	}
}

void LexerCPP::EvaluateTokens(std::vector<std::string> &tokens, const SymbolTable &preprocessorDefinitions) {

	// Remove whitespace tokens
	tokens.erase(std::remove_if(tokens.begin(), tokens.end(), OnlySpaceOrTab), tokens.end());

	// Evaluate defined statements to either 0 or 1
	for (size_t i=0; (i+1)<tokens.size();) {
		if (tokens[i] == "defined") {
			const char *val = "0";
			if (tokens[i+1] == "(") {
				if (((i + 2)<tokens.size()) && (tokens[i + 2] == ")")) {
					// defined()
					tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 3);
				} else if (((i+3)<tokens.size()) && (tokens[i+3] == ")")) {
					// defined(<identifier>)
					SymbolTable::const_iterator it = preprocessorDefinitions.find(tokens[i+2]);
					if (it != preprocessorDefinitions.end()) {
						val = "1";
					}
					tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 4);
				} else {
					// Spurious '(' so erase as more likely to result in false
					tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 2);
				}
			} else {
				// defined <identifier>
				SymbolTable::const_iterator it = preprocessorDefinitions.find(tokens[i+1]);
				if (it != preprocessorDefinitions.end()) {
					val = "1";
				}
				tokens.erase(tokens.begin() + i + 1, tokens.begin() + i + 2);
			}
			tokens[i] = val;
		} else {
			i++;
		}
	}

	// Evaluate identifiers
	const size_t maxIterations = 100;
	size_t iterations = 0;	// Limit number of iterations in case there is a recursive macro.
	for (size_t i = 0; (i<tokens.size()) && (iterations < maxIterations);) {
		iterations++;
		if (setWordStart.Contains(static_cast<unsigned char>(tokens[i][0]))) {
			SymbolTable::const_iterator it = preprocessorDefinitions.find(tokens[i]);
			if (it != preprocessorDefinitions.end()) {
				// Tokenize value
				std::vector<std::string> macroTokens = Tokenize(it->second.value);
				if (it->second.IsMacro()) {
					if ((i + 1 < tokens.size()) && (tokens.at(i + 1) == "(")) {
						// Create map of argument name to value
						std::vector<std::string> argumentNames = StringSplit(it->second.arguments, ',');
						std::map<std::string, std::string> arguments;
						size_t arg = 0;
						size_t tok = i+2;
						while ((tok < tokens.size()) && (arg < argumentNames.size()) && (tokens.at(tok) != ")")) {
							if (tokens.at(tok) != ",") {
								arguments[argumentNames.at(arg)] = tokens.at(tok);
								arg++;
							}
							tok++;
						}

						// Remove invocation
						tokens.erase(tokens.begin() + i, tokens.begin() + tok + 1);

						// Substitute values into macro
						macroTokens.erase(std::remove_if(macroTokens.begin(), macroTokens.end(), OnlySpaceOrTab), macroTokens.end());

						for (size_t iMacro = 0; iMacro < macroTokens.size();) {
							if (setWordStart.Contains(static_cast<unsigned char>(macroTokens[iMacro][0]))) {
								std::map<std::string, std::string>::const_iterator itFind = arguments.find(macroTokens[iMacro]);
								if (itFind != arguments.end()) {
									// TODO: Possible that value will be expression so should insert tokenized form
									macroTokens[iMacro] = itFind->second;
								}
							}
							iMacro++;
						}

						// Insert results back into tokens
						tokens.insert(tokens.begin() + i, macroTokens.begin(), macroTokens.end());

					} else {
						i++;
					}
				} else {
					// Remove invocation
					tokens.erase(tokens.begin() + i);
					// Insert results back into tokens
					tokens.insert(tokens.begin() + i, macroTokens.begin(), macroTokens.end());
				}
			} else {
				// Identifier not found and value defaults to zero
				tokens[i] = "0";
			}
		} else {
			i++;
		}
	}

	// Find bracketed subexpressions and recurse on them
	BracketPair bracketPair = FindBracketPair(tokens);
	while (bracketPair.itBracket != tokens.end()) {
		std::vector<std::string> inBracket(bracketPair.itBracket + 1, bracketPair.itEndBracket);
		EvaluateTokens(inBracket, preprocessorDefinitions);

		// The insertion is done before the removal because there were failures with the opposite approach
		tokens.insert(bracketPair.itBracket, inBracket.begin(), inBracket.end());

		bracketPair = FindBracketPair(tokens);
		tokens.erase(bracketPair.itBracket, bracketPair.itEndBracket + 1);

		bracketPair = FindBracketPair(tokens);
	}

	// Evaluate logical negations
	for (size_t j=0; (j+1)<tokens.size();) {
		if (setNegationOp.Contains(tokens[j][0])) {
			int isTrue = atoi(tokens[j+1].c_str());
			if (tokens[j] == "!")
				isTrue = !isTrue;
			std::vector<std::string>::iterator itInsert =
				tokens.erase(tokens.begin() + j, tokens.begin() + j + 2);
			tokens.insert(itInsert, isTrue ? "1" : "0");
		} else {
			j++;
		}
	}

	// Evaluate expressions in precedence order
	enum precedence { precArithmetic, precRelative, precLogical };
	for (int prec=precArithmetic; prec <= precLogical; prec++) {
		// Looking at 3 tokens at a time so end at 2 before end
		for (size_t k=0; (k+2)<tokens.size();) {
			const char chOp = tokens[k+1][0];
			if (
				((prec==precArithmetic) && setArithmethicOp.Contains(chOp)) ||
				((prec==precRelative) && setRelOp.Contains(chOp)) ||
				((prec==precLogical) && setLogicalOp.Contains(chOp))
				) {
				const int valA = atoi(tokens[k].c_str());
				const int valB = atoi(tokens[k+2].c_str());
				int result = 0;
				if (tokens[k+1] == "+")
					result = valA + valB;
				else if (tokens[k+1] == "-")
					result = valA - valB;
				else if (tokens[k+1] == "*")
					result = valA * valB;
				else if (tokens[k+1] == "/")
					result = valA / (valB ? valB : 1);
				else if (tokens[k+1] == "%")
					result = valA % (valB ? valB : 1);
				else if (tokens[k+1] == "<")
					result = valA < valB;
				else if (tokens[k+1] == "<=")
					result = valA <= valB;
				else if (tokens[k+1] == ">")
					result = valA > valB;
				else if (tokens[k+1] == ">=")
					result = valA >= valB;
				else if (tokens[k+1] == "==")
					result = valA == valB;
				else if (tokens[k+1] == "!=")
					result = valA != valB;
				else if (tokens[k+1] == "||")
					result = valA || valB;
				else if (tokens[k+1] == "&&")
					result = valA && valB;
				char sResult[30];
				sprintf(sResult, "%d", result);
				std::vector<std::string>::iterator itInsert =
					tokens.erase(tokens.begin() + k, tokens.begin() + k + 3);
				tokens.insert(itInsert, sResult);
			} else {
				k++;
			}
		}
	}
}

std::vector<std::string> LexerCPP::Tokenize(const std::string &expr) const {
	// Break into tokens
	std::vector<std::string> tokens;
	const char *cp = expr.c_str();
	while (*cp) {
		std::string word;
		if (setWord.Contains(static_cast<unsigned char>(*cp))) {
			// Identifiers and numbers
			while (setWord.Contains(static_cast<unsigned char>(*cp))) {
				word += *cp;
				cp++;
			}
		} else if (IsSpaceOrTab(*cp)) {
			while (IsSpaceOrTab(*cp)) {
				word += *cp;
				cp++;
			}
		} else if (setRelOp.Contains(static_cast<unsigned char>(*cp))) {
			word += *cp;
			cp++;
			if (setRelOp.Contains(static_cast<unsigned char>(*cp))) {
				word += *cp;
				cp++;
			}
		} else if (setLogicalOp.Contains(static_cast<unsigned char>(*cp))) {
			word += *cp;
			cp++;
			if (setLogicalOp.Contains(static_cast<unsigned char>(*cp))) {
				word += *cp;
				cp++;
			}
		} else {
			// Should handle strings, characters, and comments here
			word += *cp;
			cp++;
		}
		tokens.push_back(word);
	}
	return tokens;
}

bool LexerCPP::EvaluateExpression(const std::string &expr, const SymbolTable &preprocessorDefinitions) {
	std::vector<std::string> tokens = Tokenize(expr);

	EvaluateTokens(tokens, preprocessorDefinitions);

	// "0" or "" -> false else true
	const bool isFalse = tokens.empty() ||
		((tokens.size() == 1) && ((tokens[0] == "") || tokens[0] == "0"));
	return !isFalse;
}

LexerModule lmCPP(SCLEX_CPP, LexerCPP::LexerFactoryCPP, "cpp", cppWordLists);
LexerModule lmCPPNoCase(SCLEX_CPPNOCASE, LexerCPP::LexerFactoryCPPInsensitive, "cppnocase", cppWordLists);
