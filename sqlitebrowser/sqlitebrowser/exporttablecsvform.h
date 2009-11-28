#ifndef EXPORTTABLECSVFORM_H
#define EXPORTTABLECSVFORM_H

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

class Ui_exportTableCSVForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel2;
    QComboBox *comboOptions;
    QSpacerItem *spacer13;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer11;
    QPushButton *buttonCancel;
    QPushButton *buttonExport;

    void setupUi(QDialog *exportTableCSVForm)
    {
        if (exportTableCSVForm->objectName().isEmpty())
            exportTableCSVForm->setObjectName(QString::fromUtf8("exportTableCSVForm"));
        exportTableCSVForm->resize(365, 150);
        vboxLayout = new QVBoxLayout(exportTableCSVForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel2 = new QLabel(exportTableCSVForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout->addWidget(textLabel2);

        comboOptions = new QComboBox(exportTableCSVForm);
        comboOptions->setObjectName(QString::fromUtf8("comboOptions"));

        hboxLayout->addWidget(comboOptions);


        vboxLayout->addLayout(hboxLayout);

        spacer13 = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer13);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacer11 = new QSpacerItem(29, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer11);

        buttonCancel = new QPushButton(exportTableCSVForm);
        buttonCancel->setObjectName(QString::fromUtf8("buttonCancel"));
        buttonCancel->setDefault(false);

        hboxLayout1->addWidget(buttonCancel);

        buttonExport = new QPushButton(exportTableCSVForm);
        buttonExport->setObjectName(QString::fromUtf8("buttonExport"));

        hboxLayout1->addWidget(buttonExport);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(exportTableCSVForm);
        QObject::connect(buttonExport, SIGNAL(clicked()), exportTableCSVForm, SLOT(exportPressed()));
        QObject::connect(buttonCancel, SIGNAL(clicked()), exportTableCSVForm, SLOT(reject()));

        QMetaObject::connectSlotsByName(exportTableCSVForm);
    } // setupUi

    void retranslateUi(QDialog *exportTableCSVForm)
    {
        exportTableCSVForm->setWindowTitle(QApplication::translate("exportTableCSVForm", "Choose table to export as CSV text", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("exportTableCSVForm", "Table name:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        comboOptions->setProperty("toolTip", QVariant(QApplication::translate("exportTableCSVForm", "Choose the table to delete", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        comboOptions->setProperty("whatsThis", QVariant(QApplication::translate("exportTableCSVForm", "Use this control to select the name of the table to be deleted", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        buttonCancel->setText(QApplication::translate("exportTableCSVForm", "Cancel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonCancel->setProperty("toolTip", QVariant(QApplication::translate("exportTableCSVForm", "Cancel and close dialog box", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
        buttonExport->setText(QApplication::translate("exportTableCSVForm", "Export", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        buttonExport->setProperty("toolTip", QVariant(QApplication::translate("exportTableCSVForm", "Delete the selected table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class exportTableCSVForm: public Ui_exportTableCSVForm {};
} // namespace Ui

QT_END_NAMESPACE

class exportTableCSVForm : public QDialog, public Ui::exportTableCSVForm
{
    Q_OBJECT

public:
    exportTableCSVForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~exportTableCSVForm();

    QString option;

public slots:
    virtual void exportPressed();
    virtual void populateOptions( QStringList entries );

protected slots:
    virtual void languageChange();

};

#endif // EXPORTTABLECSVFORM_H
