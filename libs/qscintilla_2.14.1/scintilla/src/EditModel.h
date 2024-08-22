// Scintilla source code edit control
/** @file EditModel.h
 ** Defines the editor state that must be visible to EditorView.
 **/
// Copyright 1998-2014 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef EDITMODEL_H
#define EDITMODEL_H

namespace Scintilla {

/**
*/
class Caret {
public:
	bool active;
	bool on;
	int period;

	Caret();
};

class EditModel {
public:
	bool inOverstrike;
	int xOffset;		///< Horizontal scrolled amount in pixels
	bool trackLineWidth;

	SpecialRepresentations reprs;
	Caret caret;
	SelectionPosition posDrag;
	Sci::Position braces[2];
	int bracesMatchStyle;
	int highlightGuideColumn;
	Selection sel;
	bool primarySelection;

	enum IMEInteraction { imeWindowed, imeInline } imeInteraction;

	int foldFlags;
	int foldDisplayTextStyle;
	std::unique_ptr<IContractionState> pcs;
	// Hotspot support
	Range hotspot;
	Sci::Position hoverIndicatorPos;

	// Wrapping support
	int wrapWidth;

	Document *pdoc;

	EditModel();
	// Deleted so EditModel objects can not be copied.
	EditModel(const EditModel &) = delete;
	EditModel(EditModel &&) = delete;
	EditModel &operator=(const EditModel &) = delete;
	EditModel &operator=(EditModel &&) = delete;
	virtual ~EditModel();
	virtual Sci::Line TopLineOfMain() const = 0;
	virtual Point GetVisibleOriginInMain() const = 0;
	virtual Sci::Line LinesOnScreen() const = 0;
	virtual Range GetHotSpotRange() const = 0;
};

}

#endif
