#ifndef EDITFIELDFORM_H
#define EDITFIELDFORM_H

#include <qvariant.h>

/* QT_BEGIN_NAMESPACE and QT_END_NAMESPACE weren't introduced into QT right away... */
#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif

#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#include <QtCore/QVariant>

#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QRadioButton>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QStatusBar>

#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_editFieldForm
{
public:

    DBBrowserDB pdb;


    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QLabel *lblFieldName;
    QLineEdit *nameLineEdit;

    QLabel *lblFieldType;
    QButtonGroup *groupRadioTypes;
    QPushButton *cancelButton;
    QPushButton *saveButton;

    QStatusBar *statusBar;

    void setupUi(QDialog *editFieldForm)
    {
        if (editFieldForm->objectName().isEmpty()){
            editFieldForm->setObjectName(QString::fromUtf8("editFieldForm"));
        }
        editFieldForm->resize(352, 140);

        QVBoxLayout *outerContainerLayout = new QVBoxLayout();
        outerContainerLayout->setContentsMargins(0,0,0,0);
        editFieldForm->setLayout(outerContainerLayout);

        QVBoxLayout *mainVBoxLayout = new QVBoxLayout();
        mainVBoxLayout->setContentsMargins(20,20,20,20);
        outerContainerLayout->addLayout(mainVBoxLayout);

        //** Field Name lbl
        lblFieldName = new QLabel(editFieldForm);
        lblFieldName->setObjectName(QString::fromUtf8("textLabel1"));
        lblFieldName->setWordWrap(false);
        mainVBoxLayout->addWidget(lblFieldName);

        //** Fiel Name EDIT
        nameLineEdit = new QLineEdit(editFieldForm);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));
        mainVBoxLayout->addWidget(nameLineEdit);

        mainVBoxLayout->addSpacing(20);


        //** Field Type lbl
        lblFieldType = new QLabel(editFieldForm);
        lblFieldType->setObjectName(QString::fromUtf8("textLabel2"));
        lblFieldType->setWordWrap(false);
        mainVBoxLayout->addWidget(lblFieldType);


        //**** Field Types Radios
        QVBoxLayout *radioLayout = new QVBoxLayout();
        mainVBoxLayout->addLayout(radioLayout);
        radioLayout->setContentsMargins(20, 0 ,10 ,0);

        groupRadioTypes = new QButtonGroup();
        groupRadioTypes->setExclusive(true);

        QRadioButton *radioTEXT = new QRadioButton();
        radioTEXT->setText(QApplication::translate("addFieldForm", "TEXT", 0, QApplication::UnicodeUTF8));
        radioTEXT->setProperty("field_type", QVariant("TEXT"));
        radioLayout->addWidget(radioTEXT);
        groupRadioTypes->addButton(radioTEXT);

        QRadioButton *radioNUMERIC = new QRadioButton();
        radioNUMERIC->setText(QApplication::translate("addFieldForm", "NUMERIC", 0, QApplication::UnicodeUTF8));
        radioNUMERIC->setProperty("field_type", QVariant("NUMERIC"));
        radioLayout->addWidget(radioNUMERIC);
        groupRadioTypes->addButton(radioNUMERIC);

        QRadioButton *radioBLOB = new QRadioButton();
        radioBLOB->setText(QApplication::translate("addFieldForm", "BLOB", 0, QApplication::UnicodeUTF8));
        radioBLOB->setProperty("field_type", QVariant("BLOB"));
        radioLayout->addWidget(radioBLOB);
        groupRadioTypes->addButton(radioBLOB);

        QRadioButton *radioINTPRIMARY = new QRadioButton();
        radioINTPRIMARY->setText(QApplication::translate("addFieldForm", "INTEGER PRIMARY KEY", 0, QApplication::UnicodeUTF8));
        radioINTPRIMARY->setProperty("field_type", QVariant("INTEGER PRIMARY KEY"));
        radioLayout->addWidget(radioINTPRIMARY);
        groupRadioTypes->addButton(radioINTPRIMARY);

        QRadioButton *radioCustom = new QRadioButton();
        radioCustom->setText(QApplication::translate("addFieldForm", "Custom", 0, QApplication::UnicodeUTF8));
        radioCustom->setProperty("field_type", QVariant("__custom__"));
        radioLayout->addWidget(radioCustom);
        groupRadioTypes->addButton(radioCustom);

        QLineEdit *txtCustomType = new QLineEdit();
        radioLayout->addWidget(txtCustomType);
        txtCustomType->setDisabled(true);
        



        mainVBoxLayout->addSpacing(20);

        //*** Bottom Button Layout
        QHBoxLayout *bottomButtonBox = new QHBoxLayout();
        mainVBoxLayout->addLayout(bottomButtonBox);
        bottomButtonBox->setSpacing(6);
        bottomButtonBox->addStretch(10);

        //** Cancel Button
        cancelButton = new QPushButton(editFieldForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setIcon(QIcon(":/icons/cancel"));
        bottomButtonBox->addWidget(cancelButton);

        //** Save Button
        saveButton = new QPushButton(editFieldForm);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(false);
        saveButton->setIcon(QIcon(":/icons/save"));
        bottomButtonBox->addWidget(saveButton);

        statusBar = new QStatusBar();
        outerContainerLayout->addWidget(statusBar);


        nameLineEdit->setFocus();

        retranslateUi(editFieldForm);
        QObject::connect(cancelButton, SIGNAL(clicked()), editFieldForm, SLOT(reject()));
        QObject::connect(saveButton, SIGNAL(clicked()), editFieldForm, SLOT(confirmEdit()));
        QObject::connect(nameLineEdit, SIGNAL(textChanged(QString)), editFieldForm, SLOT(enableSave()));

        QObject::connect(groupRadioTypes, SIGNAL(buttonClicked(QAbstractButton*)), editFieldForm, SLOT(on_radio_button_clicked(QAbstractButton*)));

        QMetaObject::connectSlotsByName(editFieldForm);
    } // setupUi


    void retranslateUi(QDialog *editFieldForm)
    {
        editFieldForm->setWindowTitle(QApplication::translate("editFieldForm", "Edit field name and type", 0, QApplication::UnicodeUTF8));
        lblFieldName->setText(QApplication::translate("editFieldForm", "Field name:", 0, QApplication::UnicodeUTF8));
        lblFieldType->setText(QApplication::translate("editFieldForm", "Field type:", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("editFieldForm", "Cancel", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("editFieldForm", "Apply Changes", 0, QApplication::UnicodeUTF8));

    } // retranslateUi

};

namespace Ui {
    class editFieldForm: public Ui_editFieldForm {};
} // namespace Ui

QT_END_NAMESPACE

class editFieldForm : public QDialog, public Ui::editFieldForm
{
    Q_OBJECT

public:
    editFieldForm(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~editFieldForm();

    void setDB(DBBrowserDB &db);

    QString table_name;
    QString field_name;
    QString field_type;
    QString original_field_name;
    bool is_new;

public slots:
    virtual void setInitialValues( bool is_new, QString table, QString fld_name, QString fld_type );
    virtual void confirmEdit();
    virtual void enableSave();
    virtual void getCustomType();

    virtual void on_radio_button_clicked(QAbstractButton*);

protected slots:
    virtual void languageChange();

};

#endif // EDITFIELDFORM_H
