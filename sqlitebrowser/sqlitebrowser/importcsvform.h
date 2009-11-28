#ifndef IMPORTCSVFORM_H
#define IMPORTCSVFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3Table>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
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
#include <QtCore/QStringRef>
#include <QtCore/QStringList>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_importCSVForm
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QLabel *textLabel3;
    QLineEdit *tableNameEdit;
    QCheckBox *extractFieldNamesCheckbox;
    QHBoxLayout *hboxLayout;
    QVBoxLayout *vboxLayout1;
    QLabel *textLabel1;
    QLabel *textLabel2;
    QSpacerItem *spacer15;
    QVBoxLayout *vboxLayout2;
    QComboBox *fieldBox;
    QComboBox *quoteBox;
    QSpacerItem *spacer14;
    Q3Table *previewTable;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer13;
    QPushButton *cancelButton;
    QPushButton *createButton;

    void setupUi(QDialog *importCSVForm)
    {
        if (importCSVForm->objectName().isEmpty())
            importCSVForm->setObjectName(QString::fromUtf8("importCSVForm"));
        importCSVForm->resize(372, 382);
        importCSVForm->setModal(true);
        vboxLayout = new QVBoxLayout(importCSVForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textLabel3 = new QLabel(importCSVForm);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        gridLayout->addWidget(textLabel3, 0, 0, 1, 1);

        tableNameEdit = new QLineEdit(importCSVForm);
        tableNameEdit->setObjectName(QString::fromUtf8("tableNameEdit"));

        gridLayout->addWidget(tableNameEdit, 0, 1, 1, 1);

        extractFieldNamesCheckbox = new QCheckBox(importCSVForm);
        extractFieldNamesCheckbox->setObjectName(QString::fromUtf8("extractFieldNamesCheckbox"));

        gridLayout->addWidget(extractFieldNamesCheckbox, 2, 0, 1, 2);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        textLabel1 = new QLabel(importCSVForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        vboxLayout1->addWidget(textLabel1);

        textLabel2 = new QLabel(importCSVForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        vboxLayout1->addWidget(textLabel2);


        hboxLayout->addLayout(vboxLayout1);

        spacer15 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer15);

        vboxLayout2 = new QVBoxLayout();
        vboxLayout2->setSpacing(6);
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        fieldBox = new QComboBox(importCSVForm);
        fieldBox->setObjectName(QString::fromUtf8("fieldBox"));

        vboxLayout2->addWidget(fieldBox);

        quoteBox = new QComboBox(importCSVForm);
        quoteBox->setObjectName(QString::fromUtf8("quoteBox"));

        vboxLayout2->addWidget(quoteBox);


        hboxLayout->addLayout(vboxLayout2);


        gridLayout->addLayout(hboxLayout, 1, 0, 1, 2);


        vboxLayout->addLayout(gridLayout);

        spacer14 = new QSpacerItem(138, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        vboxLayout->addItem(spacer14);

        previewTable = new Q3Table(importCSVForm);
        previewTable->setObjectName(QString::fromUtf8("previewTable"));
        previewTable->setNumRows(0);
        previewTable->setNumCols(0);
        previewTable->setReadOnly(true);
        previewTable->setSelectionMode(Q3Table::NoSelection);

        vboxLayout->addWidget(previewTable);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacer13 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer13);

        cancelButton = new QPushButton(importCSVForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout1->addWidget(cancelButton);

        createButton = new QPushButton(importCSVForm);
        createButton->setObjectName(QString::fromUtf8("createButton"));

        hboxLayout1->addWidget(createButton);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(importCSVForm);
        QObject::connect(cancelButton, SIGNAL(clicked()), importCSVForm, SLOT(reject()));
        QObject::connect(createButton, SIGNAL(clicked()), importCSVForm, SLOT(createButtonPressed()));
        QObject::connect(fieldBox, SIGNAL(activated(int)), importCSVForm, SLOT(fieldSeparatorChanged()));
        QObject::connect(quoteBox, SIGNAL(activated(int)), importCSVForm, SLOT(textQuoteChanged()));
        QObject::connect(extractFieldNamesCheckbox, SIGNAL(toggled(bool)), importCSVForm, SLOT(extractFieldNamesChanged(bool)));

        QMetaObject::connectSlotsByName(importCSVForm);
    } // setupUi

    void retranslateUi(QDialog *importCSVForm)
    {
        importCSVForm->setWindowTitle(QApplication::translate("importCSVForm", "Create table from CSV file", 0, QApplication::UnicodeUTF8));
        textLabel3->setText(QApplication::translate("importCSVForm", "New table name:", 0, QApplication::UnicodeUTF8));
        extractFieldNamesCheckbox->setText(QApplication::translate("importCSVForm", "Extract field names from first line", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("importCSVForm", "Field separator:", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("importCSVForm", "Text quote character:", 0, QApplication::UnicodeUTF8));
        fieldBox->clear();
        fieldBox->insertItems(0, QStringList()
         << QApplication::translate("importCSVForm", ",", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("importCSVForm", ";", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("importCSVForm", "TAB", 0, QApplication::UnicodeUTF8)
        );
        quoteBox->clear();
        quoteBox->insertItems(0, QStringList()
         << QApplication::translate("importCSVForm", "\"", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("importCSVForm", "'", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("importCSVForm", "\\", 0, QApplication::UnicodeUTF8)
         << QString()
        );
        cancelButton->setText(QApplication::translate("importCSVForm", "Cancel", 0, QApplication::UnicodeUTF8));
        createButton->setText(QApplication::translate("importCSVForm", "Create", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class importCSVForm: public Ui_importCSVForm {};
} // namespace Ui

QT_END_NAMESPACE

class importCSVForm : public QDialog, public Ui::importCSVForm
{
    Q_OBJECT

public:
    importCSVForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~importCSVForm();

public slots:
    virtual void initialize( QString & csvfile, DBBrowserDB * db );
    virtual void createButtonPressed();
    virtual void preview();
    virtual void fieldSeparatorChanged();
    virtual void textQuoteChanged();
    virtual void extractFieldNamesChanged( bool enabled );

protected:
    QStringList curList;
    char quote;
    char sep;
    int numfields;
    QString csvfilename;
    DBBrowserDB * pdb;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // IMPORTCSVFORM_H
