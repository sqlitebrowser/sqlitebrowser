#ifndef EDITTABLEFORM_H
#define EDITTABLEFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
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
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_editTableForm
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QLineEdit *tableLine;
    Q3ListView *fieldListView;
    QVBoxLayout *vboxLayout1;
    QPushButton *renameTableButton;
    QSpacerItem *spacer31;
    QPushButton *renameFieldButton;
    QPushButton *removeFieldButton;
    QPushButton *addFieldButton;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer34;
    QPushButton *closeButton;

    void setupUi(QDialog *editTableForm)
    {
        if (editTableForm->objectName().isEmpty())
            editTableForm->setObjectName(QString::fromUtf8("editTableForm"));
        editTableForm->resize(428, 266);
        gridLayout = new QGridLayout(editTableForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        tableLine = new QLineEdit(editTableForm);
        tableLine->setObjectName(QString::fromUtf8("tableLine"));
        tableLine->setFocusPolicy(Qt::NoFocus);
        tableLine->setReadOnly(true);

        vboxLayout->addWidget(tableLine);

        fieldListView = new Q3ListView(editTableForm);
        fieldListView->addColumn(QApplication::translate("editTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setClickEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->header()->setResizeEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->addColumn(QApplication::translate("editTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setClickEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->header()->setResizeEnabled(true, fieldListView->header()->count() - 1);
        fieldListView->setObjectName(QString::fromUtf8("fieldListView"));
        fieldListView->setResizePolicy(Q3ScrollView::Manual);
        fieldListView->setResizeMode(Q3ListView::AllColumns);

        vboxLayout->addWidget(fieldListView);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        renameTableButton = new QPushButton(editTableForm);
        renameTableButton->setObjectName(QString::fromUtf8("renameTableButton"));

        vboxLayout1->addWidget(renameTableButton);

        spacer31 = new QSpacerItem(20, 23, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout1->addItem(spacer31);

        renameFieldButton = new QPushButton(editTableForm);
        renameFieldButton->setObjectName(QString::fromUtf8("renameFieldButton"));
        renameFieldButton->setEnabled(false);

        vboxLayout1->addWidget(renameFieldButton);

        removeFieldButton = new QPushButton(editTableForm);
        removeFieldButton->setObjectName(QString::fromUtf8("removeFieldButton"));
        removeFieldButton->setEnabled(false);

        vboxLayout1->addWidget(removeFieldButton);

        addFieldButton = new QPushButton(editTableForm);
        addFieldButton->setObjectName(QString::fromUtf8("addFieldButton"));

        vboxLayout1->addWidget(addFieldButton);


        gridLayout->addLayout(vboxLayout1, 0, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer34 = new QSpacerItem(161, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer34);

        closeButton = new QPushButton(editTableForm);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        hboxLayout->addWidget(closeButton);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);


        retranslateUi(editTableForm);
        QObject::connect(closeButton, SIGNAL(clicked()), editTableForm, SLOT(accept()));
        QObject::connect(renameTableButton, SIGNAL(clicked()), editTableForm, SLOT(renameTable()));
        QObject::connect(removeFieldButton, SIGNAL(clicked()), editTableForm, SLOT(removeField()));
        QObject::connect(addFieldButton, SIGNAL(clicked()), editTableForm, SLOT(addField()));
        QObject::connect(renameFieldButton, SIGNAL(clicked()), editTableForm, SLOT(editField()));
        QObject::connect(fieldListView, SIGNAL(selectionChanged()), editTableForm, SLOT(fieldSelectionChanged()));

        QMetaObject::connectSlotsByName(editTableForm);
    } // setupUi

    void retranslateUi(QDialog *editTableForm)
    {
        editTableForm->setWindowTitle(QApplication::translate("editTableForm", "Edit table definition", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setLabel(0, QApplication::translate("editTableForm", "Field name", 0, QApplication::UnicodeUTF8));
        fieldListView->header()->setLabel(1, QApplication::translate("editTableForm", "Field type", 0, QApplication::UnicodeUTF8));
        fieldListView->clear();

        Q3ListViewItem *__item = new Q3ListViewItem(fieldListView);
        __item->setText(0, QApplication::translate("editTableForm", "New Item", 0, QApplication::UnicodeUTF8));
        __item->setText(1, QString());
        __item->setPixmap(0, QPixmap());
        __item->setPixmap(1, QPixmap());
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
    editTableForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
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
