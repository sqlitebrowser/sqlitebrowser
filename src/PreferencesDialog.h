#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QHash>

class QTreeWidgetItem;
class QFrame;
class QSslCertificate;
class QAbstractButton;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget* parent = nullptr);
    ~PreferencesDialog() override;

private slots:
    void loadSettings();
    void saveSettings();

    void chooseLocation();
    void showColourDialog(QTreeWidgetItem* item, int column);
    void addExtension();
    void removeExtension();
    void activateRemoteTab(bool active);
    void addClientCertificate();
    void removeClientCertificate();
    void chooseRemoteCloneDirectory();
    void updatePreviewFont();

    void on_buttonManageFileExtension_clicked();
    void on_buttonBox_clicked(QAbstractButton* button);

private:
    Ui::PreferencesDialog *ui;

    QStringList m_dbFileExtensions;

    void fillLanguageBox();
    void loadColorSetting(QFrame *frame, const QString &name);
    void setColorSetting(QFrame *frame, const QColor &color);
    void saveColorSetting(QFrame *frame, const QString &name);
    void addClientCertToTable(const QString& path, const QSslCertificate& cert);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif
