#ifndef ADDFIELDTYPEFORM_H
#define ADDFIELDTYPEFORM_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_addFieldTypeForm
{
public:
    QGridLayout *gridLayout;
    QLineEdit *typeNameEdit;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer5;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *addFieldTypeForm)
    {
        if (addFieldTypeForm->objectName().isEmpty())
            addFieldTypeForm->setObjectName(QString::fromUtf8("addFieldTypeForm"));
        addFieldTypeForm->resize(294, 98);
        gridLayout = new QGridLayout(addFieldTypeForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        typeNameEdit = new QLineEdit(addFieldTypeForm);
        typeNameEdit->setObjectName(QString::fromUtf8("typeNameEdit"));

        gridLayout->addWidget(typeNameEdit, 0, 0, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer5 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer5);

        cancelButton = new QPushButton(addFieldTypeForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        okButton = new QPushButton(addFieldTypeForm);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setDefault(true);

        hboxLayout->addWidget(okButton);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 1);


        retranslateUi(addFieldTypeForm);
        QObject::connect(okButton, SIGNAL(clicked()), addFieldTypeForm, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), addFieldTypeForm, SLOT(reject()));

        QMetaObject::connectSlotsByName(addFieldTypeForm);
    } // setupUi

    void retranslateUi(QDialog *addFieldTypeForm)
    {
        addFieldTypeForm->setWindowTitle(QApplication::translate("addFieldTypeForm", "Enter field type", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("addFieldTypeForm", "Cancel", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("addFieldTypeForm", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class addFieldTypeForm: public Ui_addFieldTypeForm {};
} // namespace Ui

QT_END_NAMESPACE

class addFieldTypeForm : public QDialog, public Ui::addFieldTypeForm
{
    Q_OBJECT

public:
    addFieldTypeForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~addFieldTypeForm();

protected slots:
    virtual void languageChange();

};

#endif // ADDFIELDTYPEFORM_H
