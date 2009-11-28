#ifndef DELETETABLEFORM_H
#define DELETETABLEFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "QtCore/QStringRef"
#include "QtCore/QStringList"
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_deleteTableForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel2;
    QComboBox *comboOptions;
    QSpacerItem *spacer13;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer11;
    QPushButton *buttonDelete;
    QPushButton *buttonCancel;

    void setupUi(QDialog *deleteTableForm)
    {
        if (deleteTableForm->objectName().isEmpty())
            deleteTableForm->setObjectName(QString::fromUtf8("deleteTableForm"));
        deleteTableForm->resize(236, 137);
        vboxLayout = new QVBoxLayout(deleteTableForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(deleteTableForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        comboOptions = new QComboBox(deleteTableForm);
        comboOptions->setObjectName(QString::fromUtf8("comboOptions"));

        hboxLayout->addWidget(comboOptions);


        vboxLayout->addLayout(hboxLayout);

        spacer13 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer13);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacer11 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer11);

        buttonDelete = new QPushButton(deleteTableForm);
        buttonDelete->setObjectName(QString::fromUtf8("buttonDelete"));

        hboxLayout1->addWidget(buttonDelete);

        buttonCancel = new QPushButton(deleteTableForm);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setDefault(true);

        hboxLayout1->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(deleteTableForm);
        QObject::connect(buttonCancel, SIGNAL(clicked()), deleteTableForm, SLOT(reject()));
        QObject::connect(buttonDelete, SIGNAL(clicked()), deleteTableForm, SLOT(confirmDelete()));

        QMetaObject::connectSlotsByName(deleteTableForm);
    } // setupUi

    void retranslateUi(QDialog *deleteTableForm)
    {
        deleteTableForm->setWindowTitle(QApplication::translate("deleteTableForm", "Delete Table", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("deleteTableForm", "Table name:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboOptions->setProperty("toolTip", QVariant(QApplication::translate("deleteTableForm", "Choose the table to delete", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboOptions->setProperty("whatsThis", QVariant(QApplication::translate("deleteTableForm", "Use this control to select the name of the table to be deleted", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonDelete->setText(QApplication::translate("deleteTableForm", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonDelete->setProperty("toolTip", QVariant(QApplication::translate("deleteTableForm", "Delete the selected table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("deleteTableForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("deleteTableForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class deleteTableForm: public Ui_deleteTableForm {};
} // namespace Ui

QT_END_NAMESPACE

class deleteTableForm : public QDialog, public Ui::deleteTableForm
{
    Q_OBJECT

public:
    deleteTableForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~deleteTableForm();

    QString option;

public slots:
    virtual void confirmDelete();
    virtual void populateOptions( QStringList entries );

protected slots:
    virtual void languageChange();

};

#endif // DELETETABLEFORM_H
