#ifndef TESTPASTE_H
#define TESTPASTE_H

#include <QObject>

class TestPaste : public QObject
{
    Q_OBJECT

private slots:
    void pasteBlobIntoSingleCell();
    void pasteBlobsIntoSeveralCells();
    void pasteBlobsAndTextIntoSeveralRows();
    void pasteTextIntoSeveralCells();
    void pasteNumberIntoIntegerColumn();
};

#endif // TESTPASTE_H
