#ifndef FindReplaceDialog_H
#define FindReplaceDialog_H

#include <ExtendedScintilla.h>

#include <QDialog>
#include <QAbstractButton>

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
    void help();
    void close();
    void reject() override;
    void buttonBox_clicked(QAbstractButton* button);

private:
    void indicateSelection();
    void clearIndicators();
    Ui::FindReplaceDialog* ui;
    ExtendedScintilla* m_scintilla;
    int foundIndicatorNumber;
};

#endif
