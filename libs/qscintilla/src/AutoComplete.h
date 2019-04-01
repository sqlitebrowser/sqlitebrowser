// Scintilla source code edit control
/** @file AutoComplete.h
 ** Defines the auto completion list box.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

namespace Scintilla {

/**
 */
class AutoComplete {
	bool active;
	std::string stopChars;
	std::string fillUpChars;
	char separator;
	char typesep; // Type seperator
	enum { maxItemLen=1000 };
	std::vector<int> sortMatrix;

public:

	bool ignoreCase;
	bool chooseSingle;
	std::unique_ptr<ListBox> lb;
	Sci::Position posStart;
	Sci::Position startLen;
	/// Should autocompletion be canceled if editor's currentPos <= startPos?
	bool cancelAtStartPos;
	bool autoHide;
	bool dropRestOfWord;
	unsigned int ignoreCaseBehaviour;
	int widthLBDefault;
	int heightLBDefault;
	/** SC_ORDER_PRESORTED:   Assume the list is presorted; selection will fail if it is not alphabetical<br />
	 *  SC_ORDER_PERFORMSORT: Sort the list alphabetically; start up performance cost for sorting<br />
	 *  SC_ORDER_CUSTOM:      Handle non-alphabetical entries; start up performance cost for generating a sorted lookup table
	 */
	int autoSort;

	AutoComplete();
	~AutoComplete();

	/// Is the auto completion list displayed?
	bool Active() const noexcept;

	/// Display the auto completion list positioned to be near a character position
	void Start(Window &parent, int ctrlID, Sci::Position position, Point location,
		Sci::Position startLen_, int lineHeight, bool unicodeMode, int technology);

	/// The stop chars are characters which, when typed, cause the auto completion list to disappear
	void SetStopChars(const char *stopChars_);
	bool IsStopChar(char ch) const noexcept;

	/// The fillup chars are characters which, when typed, fill up the selected word
	void SetFillUpChars(const char *fillUpChars_);
	bool IsFillUpChar(char ch) const noexcept;

	/// The separator character is used when interpreting the list in SetList
	void SetSeparator(char separator_);
	char GetSeparator() const noexcept;

	/// The typesep character is used for separating the word from the type
	void SetTypesep(char separator_);
	char GetTypesep() const noexcept;

	/// The list string contains a sequence of words separated by the separator character
	void SetList(const char *list);

	/// Return the position of the currently selected list item
	int GetSelection() const;

	/// Return the value of an item in the list
	std::string GetValue(int item) const;

	void Show(bool show);
	void Cancel();

	/// Move the current list element by delta, scrolling appropriately
	void Move(int delta);

	/// Select a list element that starts with word as the current element
	void Select(const char *word);
};

}

#endif
