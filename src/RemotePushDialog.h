#ifndef REMOTEPUSHDIALOG_H
#define REMOTEPUSHDIALOG_H

#include <QDialog>

class RemoteDatabase;
class QRegExpValidator;

namespace Ui {
class RemotePushDialog;
}

class RemotePushDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemotePushDialog(QWidget* parent, RemoteDatabase& remote, const QString& host, const QString& clientCert,
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

    // Suggested branch to preselect
    QString m_suggestedBranch;

    // Reference to the remote database object which is stored somewhere in the main window
    RemoteDatabase& remoteDatabase;

    // Validators
    QRegExpValidator* m_nameValidator;
    QRegExpValidator* m_branchValidator;

protected slots:
    void checkInput();
    void accept() override;

    void reloadBranchList();

    void fillInLicences(const std::vector<std::pair<std::string, std::string>>& licences);
    void fillInBranches(const std::vector<std::string>& branches, const std::string& default_branch);
};

#endif
