#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QHash>

class QTreeWidgetItem;
class QFrame;
class QTableWidget;
class QSslCertificate;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget* parent = nullptr);
    ~PreferencesDialog();

private slots:
    virtual void loadSettings();
    virtual void saveSettings();

    virtual void chooseLocation();
    virtual void showColourDialog(QTreeWidgetItem* item, int column);
    virtual void addExtension();
    virtual void removeExtension();
    virtual void activateRemoteTab(bool active);
    virtual void addClientCertificate();
    virtual void removeClientCertificate();
    void chooseRemoteCloneDirectory();
    void updatePreviewFont();

    void on_buttonManageFileExtension_clicked();

private:
    Ui::PreferencesDialog *ui;

    QStringList m_dbFileExtensions;

    void fillLanguageBox();
    void loadColorSetting(QFrame *frame, const QString &name);
    void setColorSetting(QFrame *frame, const QColor &color);
    void saveColorSetting(QFrame *frame, const QString &name);
    void addClientCertToTable(const QString& path, const QSslCertificate& cert);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
