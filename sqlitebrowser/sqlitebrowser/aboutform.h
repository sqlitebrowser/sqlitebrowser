#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
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
    QLabel *textLabel1;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer5;
    QPushButton *okButton;

    void setupUi(QDialog *aboutForm)
    {
        if (aboutForm->objectName().isEmpty())
            aboutForm->setObjectName(QString::fromUtf8("aboutForm"));
        aboutForm->resize(514, 191);
        vboxLayout = new QVBoxLayout(aboutForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setMargin(11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        textLabel1 = new QLabel(aboutForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(7));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textLabel1->sizePolicy().hasHeightForWidth());
        textLabel1->setSizePolicy(sizePolicy);
        textLabel1->setAlignment(Qt::AlignVCenter);
        textLabel1->setWordWrap(true);

        vboxLayout->addWidget(textLabel1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer5 = new QSpacerItem(141, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer5);

        okButton = new QPushButton(aboutForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout->addWidget(okButton);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(aboutForm);
        QObject::connect(okButton, SIGNAL(clicked()), aboutForm, SLOT(close()));

        QMetaObject::connectSlotsByName(aboutForm);
    } // setupUi

    void retranslateUi(QDialog *aboutForm)
    {
        aboutForm->setWindowTitle(QApplication::translate("aboutForm", "About", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("aboutForm", "Version 1.3\n"
"\n"
"SQLite Database Browser is an open source, public domain, freeware visual tool used to create, design and edit SQLite 3.x database files.\n"
"This program was built with version 3.3.5 of the SQLite engine.\n"
"\n"
"More information at http://sqlitebrowser.sourceforge.net.", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("aboutForm", "Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(aboutForm);
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
