#ifndef EDITFIELDFORM_H
#define EDITFIELDFORM_H

#include <qvariant.h>


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
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_editFieldForm
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *vboxLayout;
    QLabel *textLabel1;
    QLabel *textLabel2;
    QSpacerItem *spacer17;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer15;
    QPushButton *cancelButton;
    QPushButton *saveButton;
    QGridLayout *gridLayout1;
    QHBoxLayout *hboxLayout1;
    QComboBox *typeBox;
    QToolButton *typeButton;
    QLineEdit *nameLineEdit;

    void setupUi(QDialog *editFieldForm)
    {
        if (editFieldForm->objectName().isEmpty())
            editFieldForm->setObjectName(QString::fromUtf8("editFieldForm"));
        editFieldForm->resize(352, 140);
        gridLayout = new QGridLayout(editFieldForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        textLabel1 = new QLabel(editFieldForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        vboxLayout->addWidget(textLabel1);

        textLabel2 = new QLabel(editFieldForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        vboxLayout->addWidget(textLabel2);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

        spacer17 = new QSpacerItem(20, 14, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer17, 1, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer15 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer15);

        cancelButton = new QPushButton(editFieldForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        saveButton = new QPushButton(editFieldForm);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setEnabled(false);
        saveButton->setDefault(true);

        hboxLayout->addWidget(saveButton);


        gridLayout->addLayout(hboxLayout, 2, 0, 1, 2);

        gridLayout1 = new QGridLayout();
        gridLayout1->setSpacing(6);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        typeBox = new QComboBox(editFieldForm);
        typeBox->setObjectName(QString::fromUtf8("typeBox"));

        hboxLayout1->addWidget(typeBox);

        typeButton = new QToolButton(editFieldForm);
        typeButton->setObjectName(QString::fromUtf8("typeButton"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(typeButton->sizePolicy().hasHeightForWidth());
        typeButton->setSizePolicy(sizePolicy);

        hboxLayout1->addWidget(typeButton);


        gridLayout1->addLayout(hboxLayout1, 1, 0, 1, 1);

        nameLineEdit = new QLineEdit(editFieldForm);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        gridLayout1->addWidget(nameLineEdit, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout1, 0, 1, 1, 1);


        retranslateUi(editFieldForm);
        QObject::connect(cancelButton, SIGNAL(clicked()), editFieldForm, SLOT(reject()));
        QObject::connect(saveButton, SIGNAL(clicked()), editFieldForm, SLOT(confirmEdit()));
        QObject::connect(nameLineEdit, SIGNAL(textChanged(QString)), editFieldForm, SLOT(enableSave()));
        QObject::connect(typeButton, SIGNAL(clicked()), editFieldForm, SLOT(getCustomType()));

        QMetaObject::connectSlotsByName(editFieldForm);
    } // setupUi

    void retranslateUi(QDialog *editFieldForm)
    {
        editFieldForm->setWindowTitle(QApplication::translate("editFieldForm", "Edit field name and type", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("editFieldForm", "Field name:", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("editFieldForm", "Field type:", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("editFieldForm", "Cancel", 0, QApplication::UnicodeUTF8));
        saveButton->setText(QApplication::translate("editFieldForm", "Apply Changes", 0, QApplication::UnicodeUTF8));
        typeBox->clear();
        typeBox->insertItems(0, QStringList()
         << QString()
         << QApplication::translate("editFieldForm", "TEXT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("editFieldForm", "NUMERIC", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("editFieldForm", "BLOB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("editFieldForm", "INTEGER PRIMARY KEY", 0, QApplication::UnicodeUTF8)
        );
        typeButton->setText(QApplication::translate("editFieldForm", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        typeButton->setProperty("toolTip", QVariant(QApplication::translate("editFieldForm", "Custom type", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        typeButton->setProperty("whatsThis", QVariant(QApplication::translate("editFieldForm", "Click this button to enter a new type for the field", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class editFieldForm: public Ui_editFieldForm {};
} // namespace Ui

QT_END_NAMESPACE

class editFieldForm : public QDialog, public Ui::editFieldForm
{
    Q_OBJECT

public:
    editFieldForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~editFieldForm();

    QString name;
    QString type;

public slots:
    virtual void setInitialValues( QString name, QString type );
    virtual void confirmEdit();
    virtual void enableSave();
    virtual void getCustomType();

protected slots:
    virtual void languageChange();

};

#endif // EDITFIELDFORM_H
