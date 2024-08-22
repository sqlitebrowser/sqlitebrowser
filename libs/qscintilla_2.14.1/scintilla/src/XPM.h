// Scintilla source code edit control
/** @file XPM.h
 ** Define a classes to hold image data in the X Pixmap (XPM) and RGBA formats.
 **/
// Copyright 1998-2003 by Neil Hodgson <neilh@scintilla.org>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef XPM_H
#define XPM_H

#if defined(PLAT_QT)
#include <qimage.h>
#include <qpixmap.h>
#endif

namespace Scintilla {

/**
 * Hold a pixmap in XPM format.
 */
class XPM {
#if defined(PLAT_QT)
    QPixmap qpm;

public:
    XPM(const char *textForm);
    XPM(const char *const *linesForm);
    ~XPM() {}

    void Draw(Surface *surface, PRectangle &rc);
    int GetHeight() const {return qpm.height();}

    const QPixmap &Pixmap() const {return qpm;}
#else
	int height=1;
	int width=1;
	int nColours=1;
	std::vector<unsigned char> pixels;
	ColourDesired colourCodeTable[256];
	char codeTransparent=' ';
	ColourDesired ColourFromCode(int ch) const;
	void FillRun(Surface *surface, int code, int startX, int y, int x) const;
public:
	explicit XPM(const char *textForm);
	explicit XPM(const char *const *linesForm);
    XPM(const XPM &) = delete;
    XPM(XPM &&) = delete;
    XPM &operator=(const XPM &) = delete;
    XPM &operator=(XPM &&) = delete;
	~XPM();
	void Init(const char *textForm);
	void Init(const char *const *linesForm);
	/// Decompose image into runs and use FillRectangle for each run
	void Draw(Surface *surface, const PRectangle &rc);
	int GetHeight() const { return height; }
	int GetWidth() const { return width; }
	void PixelAt(int x, int y, ColourDesired &colour, bool &transparent) const;
private:
	static std::vector<const char *>LinesFormFromTextForm(const char *textForm);
#endif
};

/**
 * A translucent image stored as a sequence of RGBA bytes.
 */
class RGBAImage {
	int height;
	int width;
	float scale;
#if defined(PLAT_QT)
    QImage *qim;
#else
	std::vector<unsigned char> pixelBytes;
#endif
public:
	RGBAImage(int width_, int height_, float scale_, const unsigned char *pixels_);
	explicit RGBAImage(const XPM &xpm);
	// Deleted so RGBAImage objects can not be copied.
	RGBAImage(const RGBAImage &) = delete;
	RGBAImage(RGBAImage &&) = delete;
	RGBAImage &operator=(const RGBAImage &) = delete;
	RGBAImage &operator=(RGBAImage &&) = delete;
	virtual ~RGBAImage();
	int GetHeight() const { return height; }
	int GetWidth() const { return width; }
	float GetScale() const { return scale; }
	float GetScaledHeight() const { return height / scale; }
	float GetScaledWidth() const { return width / scale; }
#if !defined(PLAT_QT)
	int CountBytes() const;
#endif
	const unsigned char *Pixels() const;
	void SetPixel(int x, int y, ColourDesired colour, int alpha);
};

#if !defined(PLAT_QT)

/**
 * A collection of RGBAImage pixmaps indexed by integer id.
 */
class RGBAImageSet {
	typedef std::map<int, std::unique_ptr<RGBAImage>> ImageMap;
	ImageMap images;
	mutable int height;	///< Memorize largest height of the set.
	mutable int width;	///< Memorize largest width of the set.
public:
	RGBAImageSet();
	// Deleted so RGBAImageSet objects can not be copied.
	RGBAImageSet(const RGBAImageSet &) = delete;
	RGBAImageSet(RGBAImageSet &&) = delete;
	RGBAImageSet &operator=(const RGBAImageSet &) = delete;
	RGBAImageSet &operator=(RGBAImageSet &&) = delete;
	~RGBAImageSet();
	/// Remove all images.
	void Clear();
	/// Add an image.
	void Add(int ident, RGBAImage *image);
	/// Get image by id.
	RGBAImage *Get(int ident);
	/// Give the largest height of the set.
	int GetHeight() const;
	/// Give the largest width of the set.
	int GetWidth() const;
};

#endif

}

#endif
