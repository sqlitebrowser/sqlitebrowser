#ifndef ADDFIELDFORM_H
#define ADDFIELDFORM_H

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

class Ui_addFieldForm
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *spacer17;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacer15;
    QPushButton *cancelButton;
    QPushButton *createButton;
    QVBoxLayout *vboxLayout;
    QLabel *textLabel1;
    QLabel *textLabel2;
    QVBoxLayout *vboxLayout1;
    QLineEdit *nameLineEdit;
    QHBoxLayout *hboxLayout1;
    QComboBox *typeBox;
    QToolButton *typeButton;

    void setupUi(QDialog *addFieldForm)
    {
        if (addFieldForm->objectName().isEmpty())
            addFieldForm->setObjectName(QString::fromUtf8("addFieldForm"));
        addFieldForm->resize(338, 136);
        gridLayout = new QGridLayout(addFieldForm);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        spacer17 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(spacer17, 1, 1, 1, 1);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        spacer15 = new QSpacerItem(41, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer15);

        cancelButton = new QPushButton(addFieldForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        createButton = new QPushButton(addFieldForm);
        createButton->setObjectName(QString::fromUtf8("createButton"));
        createButton->setDefault(true);

        hboxLayout->addWidget(createButton);


        gridLayout->addLayout(hboxLayout, 2, 0, 1, 2);

        vboxLayout = new QVBoxLayout();
        vboxLayout->setSpacing(6);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        textLabel1 = new QLabel(addFieldForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        vboxLayout->addWidget(textLabel1);

        textLabel2 = new QLabel(addFieldForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        vboxLayout->addWidget(textLabel2);


        gridLayout->addLayout(vboxLayout, 0, 0, 1, 1);

        vboxLayout1 = new QVBoxLayout();
        vboxLayout1->setSpacing(6);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        nameLineEdit = new QLineEdit(addFieldForm);
        nameLineEdit->setObjectName(QString::fromUtf8("nameLineEdit"));

        vboxLayout1->addWidget(nameLineEdit);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        typeBox = new QComboBox(addFieldForm);
        typeBox->setObjectName(QString::fromUtf8("typeBox"));

        hboxLayout1->addWidget(typeBox);

        typeButton = new QToolButton(addFieldForm);
        typeButton->setObjectName(QString::fromUtf8("typeButton"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(typeButton->sizePolicy().hasHeightForWidth());
        typeButton->setSizePolicy(sizePolicy);

        hboxLayout1->addWidget(typeButton);


        vboxLayout1->addLayout(hboxLayout1);


        gridLayout->addLayout(vboxLayout1, 0, 1, 1, 1);


        retranslateUi(addFieldForm);
        QObject::connect(cancelButton, SIGNAL(clicked()), addFieldForm, SLOT(reject()));
        QObject::connect(createButton, SIGNAL(clicked()), addFieldForm, SLOT(confirmAddField()));
        QObject::connect(typeButton, SIGNAL(clicked()), addFieldForm, SLOT(getCustomType()));

        QMetaObject::connectSlotsByName(addFieldForm);
    } // setupUi

    void retranslateUi(QDialog *addFieldForm)
    {
        addFieldForm->setWindowTitle(QApplication::translate("addFieldForm", "Add database field", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("addFieldForm", "Cancel", 0, QApplication::UnicodeUTF8));
        createButton->setText(QApplication::translate("addFieldForm", "Create", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("addFieldForm", "Field name:", 0, QApplication::UnicodeUTF8));
        textLabel2->setText(QApplication::translate("addFieldForm", "Field type:", 0, QApplication::UnicodeUTF8));
        typeBox->clear();
        typeBox->insertItems(0, QStringList()
         << QString()
         << QApplication::translate("addFieldForm", "TEXT", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("addFieldForm", "NUMERIC", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("addFieldForm", "BLOB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("addFieldForm", "INTEGER PRIMARY KEY", 0, QApplication::UnicodeUTF8)
        );
        typeButton->setText(QApplication::translate("addFieldForm", "...", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        typeButton->setProperty("toolTip", QVariant(QApplication::translate("addFieldForm", "Custom type", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        typeButton->setProperty("whatsThis", QVariant(QApplication::translate("addFieldForm", "Click this button to enter a new type for the field", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class addFieldForm: public Ui_addFieldForm {};
} // namespace Ui

QT_END_NAMESPACE

class addFieldForm : public QDialog, public Ui::addFieldForm
{
    Q_OBJECT

public:
    addFieldForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~addFieldForm();

    QString fname;
    QString ftype;

public slots:
    virtual void setInitialValues( QString name, QString type );
    virtual void confirmAddField();
    virtual void getCustomType();

protected slots:
    virtual void languageChange();

};

#endif // ADDFIELDFORM_H
