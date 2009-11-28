#ifndef CREATEINDEXFORM_H
#define CREATEINDEXFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3GroupBox>
#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_createIndexForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel2;
    QLineEdit *indexLineEdit;
    Q3GroupBox *groupBox2;
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout1;
    QComboBox *comboTables;
    QComboBox *comboFields;
    QComboBox *comboOrder;
    QComboBox *comboUnique;
    QVBoxLayout *vboxLayout2;
    QLabel *textLabel3;
    QLabel *textLabel4;
    QLabel *textLabel5;
    QLabel *textLabel6;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer8;
    QPushButton *buttonCreate;
    QPushButton *buttonCancel;

    void setupUi(QDialog *createIndexForm)
    {
        if (createIndexForm->objectName().isEmpty())
            createIndexForm->setObjectName(QString::fromUtf8("createIndexForm"));
        createIndexForm->resize(300, 258);
        vboxLayout = new QVBoxLayout(createIndexForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(createIndexForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        indexLineEdit = new QLineEdit(createIndexForm);
        indexLineEdit->setObjectName(QString::fromUtf8("indexLineEdit"));

        hboxLayout->addWidget(indexLineEdit);


        vboxLayout->addLayout(hboxLayout);

        groupBox2 = new Q3GroupBox(createIndexForm);
        groupBox2->setObjectName(QString::fromUtf8("groupBox2"));
        groupBox2->setColumnLayout(0, Qt::Vertical);
        groupBox2->layout()->setSpacing(6);
        groupBox2->layout()->setContentsMargins(11, 11, 11, 11);
        gridLayout = new QGridLayout();
        QBoxLayout *boxlayout = qobject_cast<QBoxLayout *>(groupBox2->layout());
        if (boxlayout)
            boxlayout->addLayout(gridLayout);
        gridLayout->setAlignment(Qt::AlignTop);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        comboTables = new QComboBox(groupBox2);
        comboTables->setObjectName(QString::fromUtf8("comboTables"));

        vboxLayout1->addWidget(comboTables);

        comboFields = new QComboBox(groupBox2);
        comboFields->setObjectName(QString::fromUtf8("comboFields"));

        vboxLayout1->addWidget(comboFields);

        comboOrder = new QComboBox(groupBox2);
        comboOrder->setObjectName(QString::fromUtf8("comboOrder"));

        vboxLayout1->addWidget(comboOrder);

        comboUnique = new QComboBox(groupBox2);
        comboUnique->setObjectName(QString::fromUtf8("comboUnique"));

        vboxLayout1->addWidget(comboUnique);


        gridLayout->addLayout(vboxLayout1, 0, 1, 1, 1);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        textLabel3 = new QLabel(groupBox2);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        textLabel3->setWordWrap(false);

        vboxLayout2->addWidget(textLabel3);

        textLabel4 = new QLabel(groupBox2);
        textLabel4->setObjectName(QString::fromUtf8("textLabel4"));
        textLabel4->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        textLabel4->setWordWrap(false);

        vboxLayout2->addWidget(textLabel4);

        textLabel5 = new QLabel(groupBox2);
        textLabel5->setObjectName(QString::fromUtf8("textLabel5"));
        textLabel5->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        textLabel5->setWordWrap(false);

        vboxLayout2->addWidget(textLabel5);

        textLabel6 = new QLabel(groupBox2);
        textLabel6->setObjectName(QString::fromUtf8("textLabel6"));
        textLabel6->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        textLabel6->setWordWrap(false);

        vboxLayout2->addWidget(textLabel6);


        gridLayout->addLayout(vboxLayout2, 0, 0, 1, 1);


        vboxLayout->addWidget(groupBox2);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacer8 = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer8);

        buttonCreate = new QPushButton(createIndexForm);
        buttonCreate->setObjectName(QString::fromUtf8("buttonCreate"));

        hboxLayout1->addWidget(buttonCreate);

        buttonCancel = new QPushButton(createIndexForm);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setDefault(true);

        hboxLayout1->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(createIndexForm);
        QObject::connect(buttonCreate, SIGNAL(clicked()), createIndexForm, SLOT(confirmCreate()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), createIndexForm, SLOT(reject()));
        QObject::connect(comboTables, SIGNAL(activated(QString)), createIndexForm, SLOT(tableSelected(QString)));

        QMetaObject::connectSlotsByName(createIndexForm);
    } // setupUi

    void retranslateUi(QDialog *createIndexForm)
    {
        createIndexForm->setWindowTitle(QApplication::translate("createIndexForm", "Create Index", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("createIndexForm", "Index name:", 0, QApplication::UnicodeUTF8));
        indexLineEdit->setText(QString());
#ifndef QT_NO_TOOLTIP
        indexLineEdit->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Enter the name for the new index", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        indexLineEdit->setProperty("whatsThis", QVariant(QApplication::translate("createIndexForm", "This area contains the name of the index to be created", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        groupBox2->setTitle(QApplication::translate("createIndexForm", "Define properties:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboTables->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Choose the table to index", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboTables->setProperty("whatsThis", QVariant(QApplication::translate("createIndexForm", "This control is used to select the table to be indexed. Changing the selected table will automatically update the fields available in the control below", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        comboFields->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Choose the field to be indexed", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboFields->setProperty("whatsThis", QVariant(QApplication::translate("createIndexForm", "This control specifies the field to be used as an index", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        comboOrder->clear();
        comboOrder->insertItems(0, QStringList()
         << QApplication::translate("createIndexForm", "Ascending", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("createIndexForm", "Descending", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboOrder->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Choose the index order", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboOrder->setProperty("whatsThis", QVariant(QApplication::translate("createIndexForm", "This option controls the ordering of the index. Ascending is the recommended ordering", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        comboUnique->clear();
        comboUnique->insertItems(0, QStringList()
         << QApplication::translate("createIndexForm", "Allowed", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("createIndexForm", "Not allowed", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        comboUnique->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Allow duplicate values in the index field", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboUnique->setProperty("whatsThis", QVariant(QApplication::translate("createIndexForm", "This control determines if the indexed field allows duplicate values to be inserted into the database. Attempting to insert a duplicate value in an indexed fiield that does not allow this option will generate an error", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        textLabel3->setText(QApplication::translate("createIndexForm", "Table to index:", 0, QApplication::UnicodeUTF8));
        textLabel4->setText(QApplication::translate("createIndexForm", "Field to index:", 0, QApplication::UnicodeUTF8));
        textLabel5->setText(QApplication::translate("createIndexForm", "Indexing order:", 0, QApplication::UnicodeUTF8));
        textLabel6->setText(QApplication::translate("createIndexForm", "Duplicate values:", 0, QApplication::UnicodeUTF8));
        buttonCreate->setText(QApplication::translate("createIndexForm", "Create", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCreate->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Create Index", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("createIndexForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("createIndexForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class createIndexForm: public Ui_createIndexForm {};
} // namespace Ui

QT_END_NAMESPACE

class createIndexForm : public QDialog, public Ui::createIndexForm
{
    Q_OBJECT

public:
    createIndexForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~createIndexForm();

    tableMap mtablemap;
    QString createStatement;

public slots:
    virtual void tableSelected( const QString & entry );
    virtual void confirmCreate();
    virtual void populateTable( tableMap rmap );

protected slots:
    virtual void languageChange();

};

#endif // CREATEINDEXFORM_H
