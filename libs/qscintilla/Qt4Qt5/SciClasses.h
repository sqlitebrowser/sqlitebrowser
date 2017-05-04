// The definition of various Qt version independent classes used by the rest of
// the port.
//
// Copyright (c) 2017 Riverbank Computing Limited <info@riverbankcomputing.com>
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


#ifndef _SCICLASSES_H
#define _SCICLASSES_H

#include <QListWidget>
#include <QMenu>
#include <QSignalMapper>
#include <QWidget>

#include <Qsci/qsciglobal.h>


class QsciScintillaQt;
class QsciListBoxQt;


// A simple QWidget sub-class to implement a call tip.  This is not put into
// the Scintilla namespace because of moc's problems with preprocessor macros.
class QsciSciCallTip : public QWidget
{
    Q_OBJECT

public:
    QsciSciCallTip(QWidget *parent, QsciScintillaQt *sci_);
    ~QsciSciCallTip();

protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    QsciScintillaQt *sci;
};


// A popup menu where options correspond to a numeric command.  This is not put
// into the Scintilla namespace because of moc's problems with preprocessor
// macros.
class QsciSciPopup : public QMenu
{
    Q_OBJECT

public:
    QsciSciPopup();

    void addItem(const QString &label, int cmd, bool enabled,
            QsciScintillaQt *sci_);

private slots:
    void on_triggered(int cmd);

private:
    QsciScintillaQt *sci;
    QSignalMapper mapper;
};


// This sub-class of QListBox is needed to provide slots from which we can call
// QsciListBox's double-click callback (and you thought this was a C++
// program).  This is not put into the Scintilla namespace because of moc's
// problems with preprocessor macros.
class QsciSciListBox : public QListWidget
{
    Q_OBJECT

public:
    QsciSciListBox(QWidget *parent, QsciListBoxQt *lbx_);
    virtual ~QsciSciListBox();

    void addItemPixmap(const QPixmap &pm, const QString &txt);

    int find(const QString &prefix);
    QString text(int n);

protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    void handleSelection();

private:
    QsciListBoxQt *lbx;
};

#endif
