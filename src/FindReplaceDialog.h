#ifndef FindReplaceDialog_H
#define FindReplaceDialog_H

#include <QDialog>
#include <QAbstractButton>

class ExtendedScintilla;

namespace Ui {
class FindReplaceDialog;
}

class FindReplaceDialog : public QDialog
{
    Q_OBJECT

public:

    explicit FindReplaceDialog(QWidget* parent = nullptr);
    ~FindReplaceDialog() override;
    void setExtendedScintilla(ExtendedScintilla* scintilla);
    void show();

private slots:
    bool findNext();
    void replace();
    void findAll();
    void replaceAll();
    void cancelFind();
    void help();
    void close();
    void reject() override;
    void buttonBox_clicked(QAbstractButton* button);

private:
    bool findFirst(bool wrap, bool forward);
    void searchAll(bool replace);
    void indicateSelection();
    void clearIndicators();
    Ui::FindReplaceDialog* ui;
    ExtendedScintilla* m_scintilla;
    int foundIndicatorNumber;
    bool findInProgress;
};

#endif
