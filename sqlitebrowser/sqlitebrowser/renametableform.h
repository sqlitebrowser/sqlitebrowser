#ifndef RENAMETABLEFORM_H
#define RENAMETABLEFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_renameTableForm
{
public:
    QVBoxLayout *vboxLayout;
    QLineEdit *tablenameLineEdit;
    QSpacerItem *spacer27;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer26;
    QPushButton *closeButton;
    QPushButton *renameButton;

    void setupUi(QDialog *renameTableForm)
    {
        if (renameTableForm->objectName().isEmpty())
            renameTableForm->setObjectName(QString::fromUtf8("renameTableForm"));
        renameTableForm->resize(313, 101);
        vboxLayout = new QVBoxLayout(renameTableForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        tablenameLineEdit = new QLineEdit(renameTableForm);
        tablenameLineEdit->setObjectName(QString::fromUtf8("tablenameLineEdit"));

        vboxLayout->addWidget(tablenameLineEdit);

        spacer27 = new QSpacerItem(20, 21, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer27);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer26 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer26);

        closeButton = new QPushButton(renameTableForm);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        hboxLayout->addWidget(closeButton);

        renameButton = new QPushButton(renameTableForm);
        renameButton->setObjectName(QString::fromUtf8("renameButton"));
        renameButton->setDefault(true);

        hboxLayout->addWidget(renameButton);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(renameTableForm);
        QObject::connect(closeButton, SIGNAL(clicked()), renameTableForm, SLOT(reject()));
        QObject::connect(renameButton, SIGNAL(clicked()), renameTableForm, SLOT(renameClicked()));

        QMetaObject::connectSlotsByName(renameTableForm);
    } // setupUi

    void retranslateUi(QDialog *renameTableForm)
    {
        renameTableForm->setWindowTitle(QApplication::translate("renameTableForm", "Rename table", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("renameTableForm", "Cancel", 0, QApplication::UnicodeUTF8));
        renameButton->setText(QApplication::translate("renameTableForm", "Rename", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class renameTableForm: public Ui_renameTableForm {};
} // namespace Ui

QT_END_NAMESPACE

class renameTableForm : public QDialog, public Ui::renameTableForm
{
    Q_OBJECT

public:
    renameTableForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~renameTableForm();

    virtual QString getTableName();

public slots:
    virtual void renameClicked();
    virtual void setTableName( QString name );

protected slots:
    virtual void languageChange();

};

#endif // RENAMETABLEFORM_H
