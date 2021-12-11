#ifndef PROXYDIALOG_H
#define PROXYDIALOG_H

#include <QDialog>

namespace Ui {
class ProxyDialog;
}

class ProxyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProxyDialog(QWidget* parent = nullptr);
    ~ProxyDialog() override;

    void saveSettings() const;

private slots:
    void proxyTypeChanged(int new_index);
    void authenticationRequiredChanged(bool required);

private:
    Ui::ProxyDialog* ui;
};

#endif
