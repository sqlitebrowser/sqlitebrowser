#ifndef CHOOSETABLEFORM_H
#define CHOOSETABLEFORM_H

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

QT_BEGIN_NAMESPACE

class Ui_chooseTableForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel2;
    QComboBox *comboOptions;
    QSpacerItem *spacer13;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer11;
    QPushButton *buttonEdit;
    QPushButton *buttonCancel;

    void setupUi(QDialog *chooseTableForm)
    {
        if (chooseTableForm->objectName().isEmpty())
            chooseTableForm->setObjectName(QString::fromUtf8("chooseTableForm"));
        chooseTableForm->resize(256, 163);
        vboxLayout = new QVBoxLayout(chooseTableForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(chooseTableForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        comboOptions = new QComboBox(chooseTableForm);
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

        buttonEdit = new QPushButton(chooseTableForm);
        buttonEdit->setObjectName(QString::fromUtf8("buttonEdit"));

        hboxLayout1->addWidget(buttonEdit);

        buttonCancel = new QPushButton(chooseTableForm);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setDefault(true);

        hboxLayout1->addWidget(buttonCancel);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(chooseTableForm);
        QObject::connect(buttonCancel, SIGNAL(clicked()), chooseTableForm, SLOT(reject()));
        QObject::connect(buttonEdit, SIGNAL(clicked()), chooseTableForm, SLOT(editPressed()));

        QMetaObject::connectSlotsByName(chooseTableForm);
    } // setupUi

    void retranslateUi(QDialog *chooseTableForm)
    {
        chooseTableForm->setWindowTitle(QApplication::translate("chooseTableForm", "Select table to edit", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("chooseTableForm", "Table name:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboOptions->setProperty("toolTip", QVariant(QApplication::translate("chooseTableForm", "Choose the table to delete", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboOptions->setProperty("whatsThis", QVariant(QApplication::translate("chooseTableForm", "Use this control to select the name of the table to be edited", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonEdit->setText(QApplication::translate("chooseTableForm", "Edit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonEdit->setProperty("toolTip", QVariant(QApplication::translate("chooseTableForm", "Edit table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonCancel->setText(QApplication::translate("chooseTableForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("chooseTableForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class chooseTableForm: public Ui_chooseTableForm {};
} // namespace Ui

QT_END_NAMESPACE

class chooseTableForm : public QDialog, public Ui::chooseTableForm
{
    Q_OBJECT

public:
    chooseTableForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~chooseTableForm();

    QString option;

public slots:
    virtual void editPressed();
    virtual void populateOptions( QStringList entries );

protected slots:
    virtual void languageChange();

};

#endif // CHOOSETABLEFORM_H
