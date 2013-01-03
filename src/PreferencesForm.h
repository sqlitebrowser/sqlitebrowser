#ifndef PREFERENCESFORM_H
#define PREFERENCESFORM_H

#include <QDialog>

namespace Ui {
class preferencesForm;
}

class preferencesForm : public QDialog
{
    Q_OBJECT

public:
    preferencesForm(QWidget* parent = 0);
    ~preferencesForm();

    QString defaulttext;
    QString defaultlocation;
    QString defaultnewdata;
    QString defaultencoding;

public slots:
    virtual void defaultDataChanged( int which );
    virtual void defaultTextChanged( int which );
    virtual void encodingChanged( int which );
    virtual void chooseLocation();
    virtual void loadSettings();
    virtual void saveSettings();

private:
    Ui::preferencesForm *ui;
};

#endif // PREFERENCESFORM_H
