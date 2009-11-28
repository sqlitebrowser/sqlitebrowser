#ifndef DELETEINDEXFORM_H
#define DELETEINDEXFORM_H

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

class Ui_deleteIndexForm
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

    void setupUi(QDialog *deleteIndexForm)
    {
        if (deleteIndexForm->objectName().isEmpty())
            deleteIndexForm->setObjectName(QString::fromUtf8("deleteIndexForm"));
        deleteIndexForm->resize(236, 137);
        vboxLayout = new QVBoxLayout(deleteIndexForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(deleteIndexForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        comboOptions = new QComboBox(deleteIndexForm);
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

        buttonDelete = new QPushButton(deleteIndexForm);
        buttonDelete->setObjectName(QString::fromUtf8("buttonDelete"));

        hboxLayout1->addWidget(buttonDelete);

        buttonCancel = new QPushButton(deleteIndexForm);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setDefault(true);

        hboxLayout1->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(deleteIndexForm);
        QObject::connect(buttonDelete, SIGNAL(clicked()), deleteIndexForm, SLOT(confirmDelete()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), deleteIndexForm, SLOT(reject()));

        QMetaObject::connectSlotsByName(deleteIndexForm);
    } // setupUi

    void retranslateUi(QDialog *deleteIndexForm)
    {
        deleteIndexForm->setWindowTitle(QApplication::translate("deleteIndexForm", "Delete Index", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("deleteIndexForm", "Index name:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboOptions->setProperty("toolTip", QVariant(QApplication::translate("deleteIndexForm", "Choose the index to delete", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboOptions->setProperty("whatsThis", QVariant(QApplication::translate("deleteIndexForm", "Use this control to select the name of the index to be deleted", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonDelete->setText(QApplication::translate("deleteIndexForm", "Delete", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonDelete->setProperty("toolTip", QVariant(QApplication::translate("deleteIndexForm", "Delete the selected index", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("deleteIndexForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("deleteIndexForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class deleteIndexForm: public Ui_deleteIndexForm {};
} // namespace Ui

QT_END_NAMESPACE

class deleteIndexForm : public QDialog, public Ui::deleteIndexForm
{
    Q_OBJECT

public:
    deleteIndexForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~deleteIndexForm();

    QString option;

public slots:
    virtual void confirmDelete();
    virtual void populateOptions( QStringList entries );

protected slots:
    virtual void languageChange();

};

#endif // DELETEINDEXFORM_H
