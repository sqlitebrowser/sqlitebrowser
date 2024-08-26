// This module implements the QsciPrinter class.
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


#include "Qsci/qsciprinter.h"

#if !defined(QT_NO_PRINTER)

#include <QPrinter>
#include <QPainter>
#include <QStack>

#include "Qsci/qsciscintillabase.h"


// The ctor.
QsciPrinter::QsciPrinter(QPrinter::PrinterMode mode)
    : QPrinter(mode), mag(0), wrap(QsciScintilla::WrapWord)
{
}


// The dtor.
QsciPrinter::~QsciPrinter()
{
}


// Format the page before the document text is drawn.
void QsciPrinter::formatPage(QPainter &, bool, QRect &, int)
{
}


// Print a range of lines to a printer using a supplied QPainter.
int QsciPrinter::printRange(QsciScintillaBase *qsb, QPainter &painter,
        int from, int to)
{
    // Sanity check.
    if (!qsb)
        return false;

    // Setup the printing area.
    QRect def_area;

    def_area.setX(0);
    def_area.setY(0);
    def_area.setWidth(width());
    def_area.setHeight(height());

    // Get the page range.
    int pgFrom, pgTo;

    pgFrom = fromPage();
    pgTo = toPage();

    // Find the position range.
    long startPos, endPos;

    endPos = qsb->SendScintilla(QsciScintillaBase::SCI_GETLENGTH);

    startPos = (from > 0 ? qsb -> SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,from) : 0);

    if (to >= 0)
    {
        long toPos = qsb -> SendScintilla(QsciScintillaBase::SCI_POSITIONFROMLINE,to + 1);

        if (endPos > toPos)
            endPos = toPos;
    }

    if (startPos >= endPos)
        return false;

    bool reverse = (pageOrder() == LastPageFirst);
    bool needNewPage = false;
    int nr_copies = supportsMultipleCopies() ? 1 : copyCount();

    qsb -> SendScintilla(QsciScintillaBase::SCI_SETPRINTMAGNIFICATION,mag);
    qsb -> SendScintilla(QsciScintillaBase::SCI_SETPRINTWRAPMODE,wrap);

    for (int i = 1; i <= nr_copies; ++i)
    {
        // If we are printing in reverse page order then remember the start
        // position of each page.
        QStack<long> pageStarts;

        int currPage = 1;
        long pos = startPos;

        while (pos < endPos)
        {
            // See if we have finished the requested page range.
            if (pgTo > 0 && pgTo < currPage)
                break;

            // See if we are going to render this page, or just see how much
            // would fit onto it.
            bool render = false;

            if (pgFrom == 0 || pgFrom <= currPage)
            {
                if (reverse)
                    pageStarts.push(pos);
                else
                {
                    render = true;

                    if (needNewPage)
                    {
                        if (!newPage())
                            return false;
                    }
                    else
                        needNewPage = true;
                }
            }

            QRect area = def_area;

            formatPage(painter,render,area,currPage);
            pos = qsb -> SendScintilla(QsciScintillaBase::SCI_FORMATRANGE,render,&painter,area,pos,endPos);

            ++currPage;
        }

        // All done if we are printing in normal page order.
        if (!reverse)
            continue;

        // Now go through each page on the stack and really print it.
        while (!pageStarts.isEmpty())
        {
            --currPage;

            long ePos = pos;
            pos = pageStarts.pop();

            if (needNewPage)
            {
                if (!newPage())
                    return false;
            }
            else
                needNewPage = true;

            QRect area = def_area;

            formatPage(painter,true,area,currPage);
            qsb->SendScintilla(QsciScintillaBase::SCI_FORMATRANGE,true,&painter,area,pos,ePos);
        }
    }

    return true;
}


// Print a range of lines to a printer using a default QPainter.
int QsciPrinter::printRange(QsciScintillaBase *qsb, int from, int to)
{
    QPainter painter(this);

    return printRange(qsb, painter, from, to);
}


// Set the print magnification in points.
void QsciPrinter::setMagnification(int magnification)
{
    mag = magnification;
}


// Set the line wrap mode.
void QsciPrinter::setWrapMode(QsciScintilla::WrapMode wmode)
{
    wrap = wmode;
}

#endif
