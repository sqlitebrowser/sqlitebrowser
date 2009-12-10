#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_aboutForm
{
public:
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

    void setupUi(QDialog *aboutForm)
    {
        if (aboutForm->objectName().isEmpty())
            aboutForm->setObjectName(QString::fromUtf8("aboutForm"));
        aboutForm->resize(542, 257);
        vboxLayout = new QVBoxLayout(aboutForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer5 = new QSpacerItem(424, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer5);

        okButton = new QPushButton(aboutForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout->addWidget(okButton);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 3);

        spacer13 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(spacer13, 0, 1, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        pixmapLabel1 = new QLabel(aboutForm);
        pixmapLabel1->setObjectName(QString::fromUtf8("pixmapLabel1"));
        pixmapLabel1->setPixmap(QPixmap(qPixmapFromMimeSource("128.png")));
        pixmapLabel1->setScaledContents(false);
        pixmapLabel1->setWordWrap(false);

        vboxLayout1->addWidget(pixmapLabel1);

        spacer12 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacer12);


        gridLayout->addLayout(vboxLayout1, 0, 0, 1, 1);

        textLabel1 = new QLabel(aboutForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        gridLayout->addWidget(textLabel1, 0, 2, 1, 1);


        vboxLayout->addLayout(gridLayout);


        retranslateUi(aboutForm);
        QObject::connect(okButton, SIGNAL(clicked()), aboutForm, SLOT(close()));

        QMetaObject::connectSlotsByName(aboutForm);
    } // setupUi

    void retranslateUi(QDialog *aboutForm)
    {
        aboutForm->setWindowTitle(QApplication::translate("aboutForm", "About", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("aboutForm", "Version 2.0b1\n"
"\n"
"SQLite Database Browser is an open source, public domain, freeware visual tool\n used to create, design and edit SQLite 3.x database files.\n"
"This program was built with version 3.6.18 of the SQLite engine.\n"
"\n"
"More information at http://sqlitebrowser.sourceforge.net.", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("aboutForm", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class aboutForm: public Ui_aboutForm {};
} // namespace Ui

QT_END_NAMESPACE

class aboutForm : public QDialog, public Ui::aboutForm
{
    Q_OBJECT

public:
    aboutForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~aboutForm();

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // ABOUTFORM_H
