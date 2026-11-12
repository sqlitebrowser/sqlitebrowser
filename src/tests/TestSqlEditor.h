#ifndef TESTSQLEDITOR_H
#define TESTSQLEDITOR_H

#include <QObject>
#include <QTemporaryDir>

class SqlTextEdit;

class TestSqlEditor : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void zoomKeys_data();
    void zoomKeys();
    void ctrlZeroStaysWithTheApplication();

    void marginClickIsOffByDefault();
    void toggleBlockCommentUsesSelection_data();
    void toggleBlockCommentUsesSelection();
    void toggleLineComments_data();
    void toggleLineComments();
    void toggleLineCommentsIsOneUndoStep();
    void marginClickSelectsLineByDefault();
    void marginClickTogglesComment();
    void marginClickOutsideMarginDoesNotToggle();
    void marginClickSelectsLineInReadOnlyEditor();
    void marginClickSelectsLineInCellEditor();

private:
    QTemporaryDir settingsDir;

    // The static SQL lexer is owned by the first SqlTextEdit that is created, so this one must outlive all the others
    SqlTextEdit* lexerOwner;
};

#endif // TESTSQLEDITOR_H
