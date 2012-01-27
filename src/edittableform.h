#ifndef EDITTABLEFORM_H
#define EDITTABLEFORM_H

#include <qvariant.h>

/* QT_BEGIN_NAMESPACE and QT_END_NAMESPACE weren't introduced into QT right away... */
#ifndef QT_BEGIN_NAMESPACE
#define QT_BEGIN_NAMESPACE
#endif

#ifndef QT_END_NAMESPACE
#define QT_END_NAMESPACE
#endif

#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#include <QtGui/QGroupBox>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QStatusBar>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_editTableForm
{
public:
   // QGridLayout *gridLayout;
    //QVBoxLayout *vboxLayout;


    QLabel *tableLine;

    QTreeWidget *treeWidget;

    QToolButton *renameTableButton;

    QToolButton *renameFieldButton;
    QToolButton *removeFieldButton;
    QToolButton *addFieldButton;

    QPushButton *closeButton;

    QStatusBar *statusBar;

    void setupUi(QDialog *editTableForm)
    {
        if (editTableForm->objectName().isEmpty()){
            editTableForm->setObjectName(QString::fromUtf8("editTableForm"));
        }
        editTableForm->setWindowIcon(QIcon(":/icons/table"));
        //TODO remember sizes
        editTableForm->resize(500, 500);

        QVBoxLayout *mainVBoxLayout = new QVBoxLayout();
        editTableForm->setLayout(mainVBoxLayout);
        mainVBoxLayout->setSpacing(10);
        int m = 10;
        mainVBoxLayout->setContentsMargins(m,m,m,m);

        //******************************************
        //** Table Group Box
        QGroupBox *grpTable = new QGroupBox();
        mainVBoxLayout->addWidget(grpTable);
        grpTable->setTitle("Table");
        QHBoxLayout *grpTableLayout = new QHBoxLayout();
        grpTable->setLayout(grpTableLayout);
        grpTableLayout->setSpacing(0);

        //** Table Text
        tableLine = new QLabel(editTableForm);
        tableLine->setObjectName(QString::fromUtf8("tableLine"));
        tableLine->setStyleSheet("font-weight: bold; border: 1px solid #dddddd; background-color: white;");
        grpTableLayout->addWidget(tableLine);

        //** Rename Table
        renameTableButton = new QToolButton(editTableForm);
        renameTableButton->setObjectName(QString::fromUtf8("renameTableButton"));
        renameTableButton->setIcon(QIcon(":/icons/table_modify"));
        renameTableButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        grpTableLayout->addWidget(renameTableButton);


        //******************************************
        //** Fields Group Box
        QGroupBox *grpFields = new QGroupBox();
        mainVBoxLayout->addWidget(grpFields);
        grpFields->setTitle("Fields");
        QVBoxLayout *grpFieldsLayout = new QVBoxLayout();
        grpFields->setLayout(grpFieldsLayout);
        grpFieldsLayout->setSpacing(0);

        //** Fields Toolbar **
        QToolBar *toolBar = new QToolBar();
        grpFieldsLayout->addWidget(toolBar);

        //** Add Field
        addFieldButton = new QToolButton(editTableForm);
        addFieldButton->setObjectName(QString::fromUtf8("addFieldButton"));
        addFieldButton->setIcon(QIcon(":/icons/field_add"));
        addFieldButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->addWidget(addFieldButton);

        //** Rename Field
        renameFieldButton = new QToolButton(editTableForm);
        renameFieldButton->setObjectName(QString::fromUtf8("renameFieldButton"));
        renameFieldButton->setEnabled(false);
        renameFieldButton->setIcon(QIcon(":/icons/field_edit"));
        renameFieldButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->addWidget(renameFieldButton);

        //** Remove Field
        removeFieldButton = new QToolButton(editTableForm);
        removeFieldButton->setObjectName(QString::fromUtf8("removeFieldButton"));
        removeFieldButton->setEnabled(false);
        removeFieldButton->setIcon(QIcon(":/icons/field_delete"));
        removeFieldButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        toolBar->addWidget(removeFieldButton);

        //**** Tree Widget
        treeWidget = new QTreeWidget();
        grpFieldsLayout->addWidget(treeWidget);
        treeWidget->headerItem()->setText(0, QApplication::translate("createTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(1, QApplication::translate("createTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        treeWidget->setRootIsDecorated(false);
        treeWidget->setAlternatingRowColors(true);


        //*** Bottom button box
        QHBoxLayout *bottomButtonBox = new QHBoxLayout();
        mainVBoxLayout->addLayout(bottomButtonBox);
        bottomButtonBox->addStretch(10);
        closeButton = new QPushButton(editTableForm);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        bottomButtonBox->addWidget(closeButton);

        statusBar = new QStatusBar();
        mainVBoxLayout->addWidget(statusBar);

        retranslateUi(editTableForm);
        QObject::connect(closeButton, SIGNAL(clicked()), editTableForm, SLOT(accept()));
        QObject::connect(renameTableButton, SIGNAL(clicked()), editTableForm, SLOT(renameTable()));
        QObject::connect(removeFieldButton, SIGNAL(clicked()), editTableForm, SLOT(removeField()));
        QObject::connect(addFieldButton, SIGNAL(clicked()), editTableForm, SLOT(addField()));
        QObject::connect(renameFieldButton, SIGNAL(clicked()), editTableForm, SLOT(editField()));
        QObject::connect(treeWidget, SIGNAL(itemSelectionChanged()), editTableForm, SLOT(fieldSelectionChanged()));

        QMetaObject::connectSlotsByName(editTableForm);
    } // setupUi

    void retranslateUi(QDialog *editTableForm)
    {
        editTableForm->setWindowTitle(QApplication::translate("editTableForm", "Edit table definition", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(0, QApplication::translate("editTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        treeWidget->headerItem()->setText(1, QApplication::translate("editTableForm", "Field type", 0, QApplication::UnicodeUTF8));

        renameTableButton->setText(QApplication::translate("editTableForm", "Rename table", 0, QApplication::UnicodeUTF8));
        renameFieldButton->setText(QApplication::translate("editTableForm", "Edit field", 0, QApplication::UnicodeUTF8));
        removeFieldButton->setText(QApplication::translate("editTableForm", "Remove field", 0, QApplication::UnicodeUTF8));
        addFieldButton->setText(QApplication::translate("editTableForm", "Add field", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("editTableForm", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class editTableForm: public Ui_editTableForm {};
} // namespace Ui

QT_END_NAMESPACE

class editTableForm : public QDialog, public Ui::editTableForm
{
    Q_OBJECT

public:
    editTableForm(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~editTableForm();

    bool modified;
    QString curTable;

public slots:
    virtual void setActiveTable( DBBrowserDB * thedb, QString tableName );
    virtual void populateFields();
    virtual void renameTable();
    virtual void editField();
    virtual void addField();
    virtual void removeField();
    virtual void fieldSelectionChanged();

protected:
    QStringList types;
    QStringList fields;
    DBBrowserDB * pdb;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // EDITTABLEFORM_H
