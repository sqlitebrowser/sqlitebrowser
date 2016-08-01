#ifndef COMMANDS_H
#define COMMANDS_H

/** \cond docNever */

#include <QUndoStack>

#include "chunks.h"

/*! CharCommand is a class to provid undo/redo functionality in QHexEdit.
A QUndoCommand represents a single editing action on a document. CharCommand
is responsable for manipulations on single chars. It can insert. overwrite and
remove characters. A manipulation stores allways two actions
1. redo (or do) action
2. undo action.

CharCommand also supports command compression via mergeWidht(). This allows
the user to execute a undo command contation e.g. 3 steps in a single command.
If you for example insert a new byt "34" this means for the editor doing 3
steps: insert a "00", overwrite it with "03" and the overwrite it with "34". These
3 steps are combined into a single step, insert a "34".

The byte array oriented commands are just put into a set of single byte commands,
which are pooled together with the macroBegin() and macroEnd() functionality of
Qt's QUndoStack.
*/

class UndoStack : public QUndoStack
{
    Q_OBJECT

public:
    UndoStack(Chunks *chunks, QObject * parent=0);
    void insert(qint64 pos, char c);
    void insert(qint64 pos, const QByteArray &ba);
    void removeAt(qint64 pos, qint64 len=1);
    void overwrite(qint64 pos, char c);
    void overwrite(qint64 pos, int len, const QByteArray &ba);

private:
    Chunks * _chunks;
    QObject * _parent;
};

/** \endcond docNever */

#endif // COMMANDS_H
