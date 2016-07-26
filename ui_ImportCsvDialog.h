/********************************************************************************
** Form generated from reading UI file 'ImportCsvDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTCSVDIALOG_H
#define UI_IMPORTCSVDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImportCsvDialog
{
public:
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout;
    QLabel *labelName;
    QLineEdit *editName;
    QLabel *labelHeader;
    QCheckBox *checkboxHeader;
    QLabel *labelSeparator;
    QHBoxLayout *horizontalLayout;
    QComboBox *comboSeparator;
    QLineEdit *editCustomSeparator;
    QSpacerItem *horizontalSpacer;
    QLabel *labelQuote;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *comboQuote;
    QLineEdit *editCustomQuote;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelEncoding;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboEncoding;
    QLineEdit *editCustomEncoding;
    QSpacerItem *horizontalSpacer_3;
    QLabel *labelTrim;
    QCheckBox *checkBoxTrimFields;
    QTableWidget *tablePreview;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ImportCsvDialog)
    {
        if (ImportCsvDialog->objectName().isEmpty())
            ImportCsvDialog->setObjectName(QStringLiteral("ImportCsvDialog"));
        ImportCsvDialog->resize(738, 490);
        verticalLayout = new QVBoxLayout(ImportCsvDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        labelName = new QLabel(ImportCsvDialog);
        labelName->setObjectName(QStringLiteral("labelName"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelName);

        editName = new QLineEdit(ImportCsvDialog);
        editName->setObjectName(QStringLiteral("editName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, editName);

        labelHeader = new QLabel(ImportCsvDialog);
        labelHeader->setObjectName(QStringLiteral("labelHeader"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelHeader);

        checkboxHeader = new QCheckBox(ImportCsvDialog);
        checkboxHeader->setObjectName(QStringLiteral("checkboxHeader"));

        formLayout->setWidget(1, QFormLayout::FieldRole, checkboxHeader);

        labelSeparator = new QLabel(ImportCsvDialog);
        labelSeparator->setObjectName(QStringLiteral("labelSeparator"));

        formLayout->setWidget(3, QFormLayout::LabelRole, labelSeparator);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        comboSeparator = new QComboBox(ImportCsvDialog);
        comboSeparator->setObjectName(QStringLiteral("comboSeparator"));

        horizontalLayout->addWidget(comboSeparator);

        editCustomSeparator = new QLineEdit(ImportCsvDialog);
        editCustomSeparator->setObjectName(QStringLiteral("editCustomSeparator"));
        editCustomSeparator->setMaxLength(1);

        horizontalLayout->addWidget(editCustomSeparator);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        formLayout->setLayout(3, QFormLayout::FieldRole, horizontalLayout);

        labelQuote = new QLabel(ImportCsvDialog);
        labelQuote->setObjectName(QStringLiteral("labelQuote"));

        formLayout->setWidget(4, QFormLayout::LabelRole, labelQuote);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        comboQuote = new QComboBox(ImportCsvDialog);
        comboQuote->setObjectName(QStringLiteral("comboQuote"));

        horizontalLayout_2->addWidget(comboQuote);

        editCustomQuote = new QLineEdit(ImportCsvDialog);
        editCustomQuote->setObjectName(QStringLiteral("editCustomQuote"));
        editCustomQuote->setMaxLength(1);

        horizontalLayout_2->addWidget(editCustomQuote);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        formLayout->setLayout(4, QFormLayout::FieldRole, horizontalLayout_2);

        labelEncoding = new QLabel(ImportCsvDialog);
        labelEncoding->setObjectName(QStringLiteral("labelEncoding"));

        formLayout->setWidget(5, QFormLayout::LabelRole, labelEncoding);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        comboEncoding = new QComboBox(ImportCsvDialog);
        comboEncoding->setObjectName(QStringLiteral("comboEncoding"));

        horizontalLayout_3->addWidget(comboEncoding);

        editCustomEncoding = new QLineEdit(ImportCsvDialog);
        editCustomEncoding->setObjectName(QStringLiteral("editCustomEncoding"));

        horizontalLayout_3->addWidget(editCustomEncoding);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);


        formLayout->setLayout(5, QFormLayout::FieldRole, horizontalLayout_3);

        labelTrim = new QLabel(ImportCsvDialog);
        labelTrim->setObjectName(QStringLiteral("labelTrim"));

        formLayout->setWidget(6, QFormLayout::LabelRole, labelTrim);

        checkBoxTrimFields = new QCheckBox(ImportCsvDialog);
        checkBoxTrimFields->setObjectName(QStringLiteral("checkBoxTrimFields"));
        checkBoxTrimFields->setChecked(true);

        formLayout->setWidget(6, QFormLayout::FieldRole, checkBoxTrimFields);


        verticalLayout->addLayout(formLayout);

        tablePreview = new QTableWidget(ImportCsvDialog);
        tablePreview->setObjectName(QStringLiteral("tablePreview"));

        verticalLayout->addWidget(tablePreview);

        buttonBox = new QDialogButtonBox(ImportCsvDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        labelName->setBuddy(editName);
        labelHeader->setBuddy(checkboxHeader);
        labelSeparator->setBuddy(comboSeparator);
        labelQuote->setBuddy(comboQuote);
        labelEncoding->setBuddy(comboEncoding);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(editName, checkboxHeader);
        QWidget::setTabOrder(checkboxHeader, comboSeparator);
        QWidget::setTabOrder(comboSeparator, editCustomSeparator);
        QWidget::setTabOrder(editCustomSeparator, comboQuote);
        QWidget::setTabOrder(comboQuote, editCustomQuote);
        QWidget::setTabOrder(editCustomQuote, comboEncoding);
        QWidget::setTabOrder(comboEncoding, editCustomEncoding);
        QWidget::setTabOrder(editCustomEncoding, checkBoxTrimFields);
        QWidget::setTabOrder(checkBoxTrimFields, tablePreview);

        retranslateUi(ImportCsvDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ImportCsvDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ImportCsvDialog, SLOT(reject()));
        QObject::connect(checkboxHeader, SIGNAL(toggled(bool)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(comboSeparator, SIGNAL(currentIndexChanged(int)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(comboQuote, SIGNAL(currentIndexChanged(int)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(editName, SIGNAL(textChanged(QString)), ImportCsvDialog, SLOT(checkInput()));
        QObject::connect(editCustomSeparator, SIGNAL(textChanged(QString)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(editCustomQuote, SIGNAL(textChanged(QString)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(comboEncoding, SIGNAL(currentIndexChanged(int)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(editCustomEncoding, SIGNAL(textChanged(QString)), ImportCsvDialog, SLOT(updatePreview()));
        QObject::connect(checkBoxTrimFields, SIGNAL(toggled(bool)), ImportCsvDialog, SLOT(updatePreview()));

        QMetaObject::connectSlotsByName(ImportCsvDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportCsvDialog)
    {
        ImportCsvDialog->setWindowTitle(QApplication::translate("ImportCsvDialog", "Import CSV file", 0));
        labelName->setText(QApplication::translate("ImportCsvDialog", "&Table name", 0));
        labelHeader->setText(QApplication::translate("ImportCsvDialog", "&Column names in first line", 0));
        checkboxHeader->setText(QString());
        labelSeparator->setText(QApplication::translate("ImportCsvDialog", "Field &separator", 0));
        comboSeparator->clear();
        comboSeparator->insertItems(0, QStringList()
         << QApplication::translate("ImportCsvDialog", ",", 0)
         << QApplication::translate("ImportCsvDialog", ";", 0)
         << QApplication::translate("ImportCsvDialog", "Tab", 0)
         << QApplication::translate("ImportCsvDialog", "|", 0)
         << QApplication::translate("ImportCsvDialog", "Other", 0)
        );
        labelQuote->setText(QApplication::translate("ImportCsvDialog", "&Quote character", 0));
        comboQuote->clear();
        comboQuote->insertItems(0, QStringList()
         << QApplication::translate("ImportCsvDialog", "\"", 0)
         << QApplication::translate("ImportCsvDialog", "'", 0)
         << QString()
         << QApplication::translate("ImportCsvDialog", "Other", 0)
        );
        labelEncoding->setText(QApplication::translate("ImportCsvDialog", "&Encoding", 0));
        comboEncoding->clear();
        comboEncoding->insertItems(0, QStringList()
         << QApplication::translate("ImportCsvDialog", "UTF-8", 0)
         << QApplication::translate("ImportCsvDialog", "UTF-16", 0)
         << QApplication::translate("ImportCsvDialog", "ISO-8859-1", 0)
         << QApplication::translate("ImportCsvDialog", "Other", 0)
        );
        labelTrim->setText(QApplication::translate("ImportCsvDialog", "Trim fields?", 0));
        checkBoxTrimFields->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class ImportCsvDialog: public Ui_ImportCsvDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTCSVDIALOG_H
