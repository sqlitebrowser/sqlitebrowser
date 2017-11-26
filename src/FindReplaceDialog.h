#ifndef FindReplaceDialog_H
#define FindReplaceDialog_H

#include <sqltextedit.h>

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
    ~FindReplaceDialog();
    void setSqlTextEdit(SqlTextEdit* sqlTextEdit);
    void show();

private slots:
    bool findNext();
    void replace();
    void findAll();
    void replaceAll();
    void help();
    void close();
    void reject();
    void buttonBox_clicked(QAbstractButton* button);

private:
    void indicateSelection();
    void clearIndicators();
    Ui::FindReplaceDialog* ui;
    SqlTextEdit* m_sqlTextEdit;
    int foundIndicatorNumber;
};

#endif
