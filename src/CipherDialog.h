#ifndef CIPHERDIALOG_H
#define CIPHERDIALOG_H

#include <QDialog>

namespace Ui {
class CipherDialog;
}

class CipherDialog : public QDialog
{
    Q_OBJECT

public:
    // Set the encrypt parameter to true when the dialog is used to encrypt a database;
    // set it to false if the dialog is used to ask the user for the key to decrypt a file.
    explicit CipherDialog(QWidget* parent, bool encrypt);
    ~CipherDialog();

    // Allow read access to the input fields
    QString password() const;
    int pageSize() const;

private:
    Ui::CipherDialog* ui;
    bool encryptMode;

private slots:
    void checkInputFields();
};

#endif
