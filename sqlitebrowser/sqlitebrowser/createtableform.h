#ifndef CREATETABLEFORM_H
#define CREATETABLEFORM_H

#include <qvariant.h>

/* QT_BEGIN_NAMESPACE and QT_END_NAMESPACE weren't introduced into QT right away... */
#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif

#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#include <QtCore/QVariant>

#include <QtGui/QGroupBox>
#include <Qt3Support/Q3Header>
//#include <Qt3Support/Q3ListView>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QHeaderView>

#include <Qt3Support/Q3MimeSourceFactory>


#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_createTableForm
{
public:
    QVBoxLayout *mainVBoxLayout;

    QHBoxLayout *topTableLayout;

    QGroupBox *groupBox1;
    QVBoxLayout *groupBoxLayout;
    QLabel *textLabel1;
    QLineEdit *tablenameLineEdit;

    QVBoxLayout *vboxLayout1;
    //sQ3ListView *fieldListView;
    QTreeWidget *treeWidget;

    QSpacerItem *spacer4;

    QHBoxLayout *fieldActionsButtonLayout;
    QPushButton *buttonAddField;
    QPushButton *buttonDeleteField;

    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacer6;
    QHBoxLayout *bottomButtonLayout;
    QPushButton *buttonCreate;
    QPushButton *buttonCancel;

    void setupUi(QDialog *createTableForm)
    {
        if (createTableForm->objectName().isEmpty()){
            createTableForm->setObjectName(QString::fromUtf8("createTableForm"));
        }
        createTableForm->setWindowIcon(QIcon(":/icons/db_new"));

        createTableForm->resize(309, 320);

        //** Main Layout
        mainVBoxLayout = new QVBoxLayout(createTableForm);
        mainVBoxLayout->setSpacing(6);
        mainVBoxLayout->setContentsMargins(11, 11, 11, 11);
        mainVBoxLayout->setObjectName(QString::fromUtf8("vboxLayout"));

        //**** Top Table Box ****
        topTableLayout = new QHBoxLayout();
        topTableLayout->setSpacing(6);
        //topTableLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel1 = new QLabel(createTableForm);
        //textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        topTableLayout->addWidget(textLabel1);

        tablenameLineEdit = new QLineEdit(createTableForm);
        //tablenameLineEdit->setObjectName(QString::fromUtf8("tablenameLineEdit"));

        topTableLayout->addWidget(tablenameLineEdit);


        mainVBoxLayout->addLayout(topTableLayout);

        //**** Fields GroupBox
        groupBox1 = new QGroupBox(createTableForm);
        groupBox1->setObjectName(QString::fromUtf8("groupBox1"));
        //groupBox1->setColumnLayout(0, Qt::Vertical);
        //groupBox1->layout()->setSpacing(6);
        //groupBox1->layout()->setContentsMargins(11, 11, 11, 11);
        groupBoxLayout = new QVBoxLayout();
        groupBox1->setLayout(groupBoxLayout);
        //QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBox1->layout());
        //if (boxlayout)
         //   boxlayout->addLayout(vboxLayout1);
        //vboxLayout1->setAlignment(Qt::AlignTop);
        //vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));

        treeWidget = new QTreeWidget();
        groupBoxLayout->addWidget(treeWidget);
        treeWidget->headerItem()->setText(0, QApplication::translate("createTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(1, QApplication::translate("createTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        treeWidget->setRootIsDecorated(false);
        treeWidget->setAlternatingRowColors(true);

        /*
        fieldListView = new Q3ListView(groupBox1);
        fieldListView->addColumn(QApplication::translate("createTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setClickEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->header()->setResizeEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->addColumn(QApplication::translate("createTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setClickEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->header()->setResizeEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->setObjectName(QString::fromUtf8("fieldListView"));
        fieldListView->setResizePolicy(Q3ScrollView::Manual);
        fieldListView->setResizeMode(Q3ListView::AllColumns);

        groupBoxLayout->addWidget(fieldListView);
        */

        //** Field Action Buttons Box
        fieldActionsButtonLayout = new QHBoxLayout();
        fieldActionsButtonLayout->setSpacing(6);
        fieldActionsButtonLayout->setObjectName(QString::fromUtf8("hboxLayout1"));
        //spacer4 = new QSpacerItem(111, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        fieldActionsButtonLayout->addStretch(10); // force right
        //hboxLayout1->addItem(spacer4);

        //*** Delete Field Button
        buttonDeleteField = new QPushButton(groupBox1);
        buttonDeleteField->setObjectName(QString::fromUtf8("buttonDeleteField"));
        buttonDeleteField->setEnabled(false);
        buttonDeleteField->setIcon(QIcon(":/icons/field_delete"));
        fieldActionsButtonLayout->addWidget(buttonDeleteField);

        //*** Add Field Button
        buttonAddField = new QPushButton(groupBox1);
        buttonAddField->setObjectName(QString::fromUtf8("buttonAddField"));
        buttonAddField->setIcon(QIcon(":/icons/field_add"));
        fieldActionsButtonLayout->addWidget(buttonAddField);




        groupBoxLayout->addLayout(fieldActionsButtonLayout);


        mainVBoxLayout->addWidget(groupBox1);

        //***************************************
        //*** Bottom Dialog action Buttons
        bottomButtonLayout = new QHBoxLayout();
        bottomButtonLayout->setSpacing(6);
        //bottomButtonLayout->setObjectName(QString::fromUtf8("hboxLayout2"));
        bottomButtonLayout->addStretch(10); // force right

        //** Cancel Button
        buttonCancel = new QPushButton(createTableForm);
        //buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setIcon(QIcon(":/icons/cancel"));
        bottomButtonLayout->addWidget(buttonCancel);

        //** Create Button
        buttonCreate = new QPushButton(createTableForm);
        //buttonCreate->setObjectName(QString::fromUtf8("buttonCreate"));
        buttonCreate->setIcon(QIcon(":/icons/save"));
        bottomButtonLayout->addWidget(buttonCreate);




        mainVBoxLayout->addLayout(bottomButtonLayout);


        retranslateUi(createTableForm);
        QObject::connect(buttonCancel, SIGNAL(clicked()), createTableForm, SLOT(reject()));
        QObject::connect(buttonCreate, SIGNAL(clicked()), createTableForm, SLOT(confirmCreate()));
        QObject::connect(buttonAddField, SIGNAL(clicked()), createTableForm, SLOT(addField()));
        QObject::connect(buttonDeleteField, SIGNAL(clicked()), createTableForm, SLOT(deleteField()));
        //QObject::connect(fieldListView, SIGNAL(selectionChanged()), createTableForm, SLOT(fieldSelectionChanged()));
        QObject::connect(treeWidget, SIGNAL(itemSelectionChanged()), createTableForm, SLOT(fieldSelectionChanged()));

        QMetaObject::connectSlotsByName(createTableForm);
    } // setupUi

    void retranslateUi(QDialog *createTableForm)
    {
        createTableForm->setWindowTitle(QApplication::translate("createTableForm", "Create Table", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("createTableForm", "Table name:", 0, QApplication::UnicodeUTF8));
        tablenameLineEdit->setText(QString());
#ifndef QT_NO_TOOLTIP
        tablenameLineEdit->setProperty("toolTip", QVariant(QApplication::translate("createTableForm", "Enter the name for the new table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        tablenameLineEdit->setProperty("whatsThis", QVariant(QApplication::translate("createTableForm", "Use this control to enter the name of the table to be created.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        groupBox1->setTitle(QApplication::translate("createTableForm", "Define fields:", 0, QApplication::UnicodeUTF8));
        //fieldListView->header()->setLabel(0, QApplication::translate("createTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        //fieldListView->header()->setLabel(1, QApplication::translate("createTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        //fieldListView->clear();

        //Q3ListViewItem *__item = new Q3ListViewItem(fieldListView);
        //__item->setText(0, QApplication::translate("createTableForm", "New Item", 0, QApplication::UnicodeUTF8));
        //__item->setText(1, QString());
        //__item->setPixmap(0, QPixmap());
        //__item->setPixmap(1, QPixmap());
        buttonAddField->setText(QApplication::translate("createTableForm", "Add", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonAddField->setProperty("toolTip", QVariant(QApplication::translate("createTableForm", "Add a new field definition", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonAddField->setProperty("whatsThis", QVariant(QApplication::translate("createTableForm", "This button is used to add a new field definition to your table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonDeleteField->setText(QApplication::translate("createTableForm", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonDeleteField->setProperty("toolTip", QVariant(QApplication::translate("createTableForm", "Delete current field definition", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        buttonDeleteField->setProperty("whatsThis", QVariant(QApplication::translate("createTableForm", "This button is used to delete the currently selected field definition from your table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonCreate->setText(QApplication::translate("createTableForm", "Create", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCreate->setProperty("toolTip", QVariant(QApplication::translate("createTableForm", "Create the table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("createTableForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("createTableForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class createTableForm: public Ui_createTableForm {};
} // namespace Ui

QT_END_NAMESPACE

class createTableForm : public QDialog, public Ui::createTableForm
{
    Q_OBJECT

public:
    createTableForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~createTableForm();

    QString createStatement;

public slots:
    virtual void confirmCreate();
    virtual void addField();
    virtual void deleteField();
    virtual void fieldSelectionChanged();

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // CREATETABLEFORM_H
