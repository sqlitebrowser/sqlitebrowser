// Scintilla source code edit control
/** @file Position.h
 ** Defines global type name Position in the Sci internal namespace.
 **/
// Copyright 2015 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef POSITION_H
#define POSITION_H

/**
 * A Position is a position within a document between two characters or at the beginning or end.
 * Sometimes used as a character index where it identifies the character after the position.
 */

namespace Sci {

typedef int Position;

// A later version (4.x) of this file may:
//#if defined(SCI_LARGE_FILE_SUPPORT)
//typedef std::ptrdiff_t Position;
// or may allow runtime choice between different position sizes.

const Position invalidPosition = -1;

}

#endif
