#ifndef REMOTEPUSHDIALOG_H
#define REMOTEPUSHDIALOG_H

#include <QDialog>

class RemoteDatabase;

namespace Ui {
class RemotePushDialog;
}

class RemotePushDialog : public QDialog
{
    Q_OBJECT

public:
    RemotePushDialog(QWidget* parent, RemoteDatabase& remote, const QString& host, const QString& clientCert, const QString& name = QString());
    ~RemotePushDialog();

    QString name() const;
    QString commitMessage() const;
    QString licence() const;
    bool isPublic() const;

private:
    Ui::RemotePushDialog* ui;

    // Reference to the remote database object which is stored somewhere in the main window
    RemoteDatabase& remoteDatabase;

protected slots:
    void checkInput();
    virtual void accept();

    void fillInLicences(const QMap<QString, QString>& licences);
};

#endif
