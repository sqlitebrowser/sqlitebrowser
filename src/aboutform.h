#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

class AboutDialog : public QDialog
{
    Q_OBJECT

private:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer5;
    QPushButton *okButton;
    QSpacerItem *spacer13;
    QVBoxLayout *vboxLayout1;
    QLabel *pixmapLabel1;
    QSpacerItem *spacer12;
    QLabel *textLabel1;

public:
    AboutDialog(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~AboutDialog();

protected slots:
    virtual void languageChange();

private:
    void init();
    void setupUi();
    void retranslateUi();

};

#endif // ABOUTFORM_H
