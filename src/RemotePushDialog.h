#ifndef REMOTEPUSHDIALOG_H
#define REMOTEPUSHDIALOG_H

#include <QDialog>

class QRegExpValidator;

namespace Ui {
class RemotePushDialog;
}

class RemotePushDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemotePushDialog(QWidget* parent, const QString& host, const QString& clientCert,
                              const QString& name = QString(), const QString& branch = QString());
    ~RemotePushDialog() override;

    QString name() const;
    QString commitMessage() const;
    QString licence() const;
    bool isPublic() const;
    QString branch() const;
    bool forcePush() const;

private:
    Ui::RemotePushDialog* ui;

    // Connection details
    QString m_host;
    QString m_clientCert;

    // Validators
    QRegExpValidator* m_nameValidator;
    QRegExpValidator* m_branchValidator;

protected slots:
    void checkInput();
    void reloadBranchList(const QString& select_branch = QString());
    void accept() override;
};

#endif
