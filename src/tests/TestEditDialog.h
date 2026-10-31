#ifndef TESTEDITDIALOG_H
#define TESTEDITDIALOG_H

#include <QObject>

class EditDialog;

class TestEditDialog : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void jsonbIsShownAsReadOnlyJson();
    void editorBecomesWritableAgainAfterJsonb();
    void otherBinaryDataIsNotShownAsText();

private:
    EditDialog* dialog = nullptr;
};

#endif // TESTEDITDIALOG_H
