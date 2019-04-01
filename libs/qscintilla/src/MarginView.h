// Scintilla source code edit control
/** @file MarginView.h
 ** Defines the appearance of the editor margin.
 **/
// Copyright 1998-2014 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef MARGINVIEW_H
#define MARGINVIEW_H

namespace Scintilla {

void DrawWrapMarker(Surface *surface, PRectangle rcPlace, bool isEndMarker, ColourDesired wrapColour);

typedef void (*DrawWrapMarkerFn)(Surface *surface, PRectangle rcPlace, bool isEndMarker, ColourDesired wrapColour);

/**
* MarginView draws the margins.
*/
class MarginView {
public:
	std::unique_ptr<Surface> pixmapSelMargin;
	std::unique_ptr<Surface> pixmapSelPattern;
	std::unique_ptr<Surface> pixmapSelPatternOffset1;
	// Highlight current folding block
	HighlightDelimiter highlightDelimiter;

	int wrapMarkerPaddingRight; // right-most pixel padding of wrap markers
	/** Some platforms, notably PLAT_CURSES, do not support Scintilla's native
	 * DrawWrapMarker function for drawing wrap markers. Allow those platforms to
	 * override it instead of creating a new method in the Surface class that
	 * existing platforms must implement as empty. */
	DrawWrapMarkerFn customDrawWrapMarker;

	MarginView();

	void DropGraphics(bool freeObjects);
	void AllocateGraphics(const ViewStyle &vsDraw);
	void RefreshPixMaps(Surface *surfaceWindow, WindowID wid, const ViewStyle &vsDraw);
	void PaintMargin(Surface *surface, Sci::Line topLine, PRectangle rc, PRectangle rcMargin,
		const EditModel &model, const ViewStyle &vs);
};

}

#endif
