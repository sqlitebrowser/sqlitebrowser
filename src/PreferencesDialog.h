#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QVariant>
#include <QHash>

class QTreeWidgetItem;
class QFrame;

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget* parent = 0);
    ~PreferencesDialog();

private slots:
    virtual void loadSettings();
    virtual void saveSettings();

    virtual void chooseLocation();
    virtual void showColourDialog(QTreeWidgetItem* item, int column);
    virtual void addExtension();
    virtual void removeExtension();

private:
    Ui::PreferencesDialog *ui;

    void fillLanguageBox();
    void loadColorSetting(QFrame *frame, const QString &name);
    void saveColorSetting(QFrame *frame, const QString &name);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif
