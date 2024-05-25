#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

class QTreeWidgetItem;
class QFrame;
class QSslCertificate;
class QAbstractButton;

class ProxyDialog;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    enum Tabs
    {
        TabGeneral,
        TabDatabase,
        TabDataBrowser,
        TabSql,
        TabExtensions,
        TabRemote
    };

    explicit PreferencesDialog(QWidget* parent = nullptr, Tabs tab = TabGeneral);
    ~PreferencesDialog() override;

private slots:
    void loadSettings();
    void saveSettings(bool accept=true);

    void chooseLocation();
    void showColourDialog(QTreeWidgetItem* item, int column);
    void addExtension();
    void createBuiltinExtensionList();
    void removeExtension();
    void activateRemoteTab(bool active);
    void addClientCertificate();
    void removeClientCertificate();
    void chooseRemoteCloneDirectory();
    void updatePreviewFont();
    void adjustColorsToStyle(int style);
    void configureProxy();

    void showFileExtensionManager();
    void buttonBoxClicked(QAbstractButton* button);

private:
    Ui::PreferencesDialog* ui;

    ProxyDialog* m_proxyDialog;

    QStringList m_dbFileExtensions;

    void fillLanguageBox();
    void loadColorSetting(QFrame *frame, const std::string& name);
    void setColorSetting(QFrame* frame, const QColor &color);
    void saveColorSetting(QFrame* frame, const std::string& name);
    void addClientCertToTable(const QString& path, const QSslCertificate& cert);
    void exportSettings();
    void importSettings();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif
