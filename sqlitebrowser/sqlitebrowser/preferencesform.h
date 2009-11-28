#ifndef PREFERENCESFORM_H
#define PREFERENCESFORM_H

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
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_preferencesForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *textLabel1;
    QSpacerItem *spacer9;
    QComboBox *encodingComboBox;
    QHBoxLayout *hboxLayout1;
    QLabel *textLabel2;
    QSpacerItem *spacer10;
    QComboBox *defaultdataComboBox;
    QHBoxLayout *hboxLayout2;
    QLabel *textLabel1_2;
    QSpacerItem *spacer9_2;
    QComboBox *defaultTextComboBox;
    QHBoxLayout *hboxLayout3;
    QLabel *textLabel3;
    QSpacerItem *spacer11;
    QLineEdit *locationEdit;
    QPushButton *setLocationButton;
    QSpacerItem *spacer13;
    QHBoxLayout *hboxLayout4;
    QSpacerItem *spacer12;
    QPushButton *OKButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *preferencesForm)
    {
        if (preferencesForm->objectName().isEmpty())
            preferencesForm->setObjectName(QString::fromUtf8("preferencesForm"));
        preferencesForm->resize(470, 198);
        vboxLayout = new QVBoxLayout(preferencesForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        textLabel1 = new QLabel(preferencesForm);
        textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
        textLabel1->setWordWrap(false);

        hboxLayout->addWidget(textLabel1);

        spacer9 = new QSpacerItem(31, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer9);

        encodingComboBox = new QComboBox(preferencesForm);
        encodingComboBox->setObjectName(QString::fromUtf8("encodingComboBox"));

        hboxLayout->addWidget(encodingComboBox);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        textLabel2 = new QLabel(preferencesForm);
        textLabel2->setObjectName(QString::fromUtf8("textLabel2"));
        textLabel2->setWordWrap(false);

        hboxLayout1->addWidget(textLabel2);

        spacer10 = new QSpacerItem(21, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer10);

        defaultdataComboBox = new QComboBox(preferencesForm);
        defaultdataComboBox->setObjectName(QString::fromUtf8("defaultdataComboBox"));

        hboxLayout1->addWidget(defaultdataComboBox);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        textLabel1_2 = new QLabel(preferencesForm);
        textLabel1_2->setObjectName(QString::fromUtf8("textLabel1_2"));
        textLabel1_2->setWordWrap(false);

        hboxLayout2->addWidget(textLabel1_2);

        spacer9_2 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacer9_2);

        defaultTextComboBox = new QComboBox(preferencesForm);
        defaultTextComboBox->setObjectName(QString::fromUtf8("defaultTextComboBox"));

        hboxLayout2->addWidget(defaultTextComboBox);


        vboxLayout->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
        hboxLayout3->setSpacing(6);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        textLabel3 = new QLabel(preferencesForm);
        textLabel3->setObjectName(QString::fromUtf8("textLabel3"));
        textLabel3->setWordWrap(false);

        hboxLayout3->addWidget(textLabel3);

        spacer11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacer11);

        locationEdit = new QLineEdit(preferencesForm);
        locationEdit->setObjectName(QString::fromUtf8("locationEdit"));
        locationEdit->setEnabled(false);

        hboxLayout3->addWidget(locationEdit);

        setLocationButton = new QPushButton(preferencesForm);
        setLocationButton->setObjectName(QString::fromUtf8("setLocationButton"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(setLocationButton->sizePolicy().hasHeightForWidth());
        setLocationButton->setSizePolicy(sizePolicy);

        hboxLayout3->addWidget(setLocationButton);


        vboxLayout->addLayout(hboxLayout3);

        spacer13 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacer13);

        hboxLayout4 = new QHBoxLayout();
        hboxLayout4->setSpacing(6);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        spacer12 = new QSpacerItem(51, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout4->addItem(spacer12);

        OKButton = new QPushButton(preferencesForm);
        OKButton->setObjectName(QString::fromUtf8("OKButton"));

        hboxLayout4->addWidget(OKButton);

        cancelButton = new QPushButton(preferencesForm);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setDefault(true);

        hboxLayout4->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout4);


        retranslateUi(preferencesForm);
        QObject::connect(OKButton, SIGNAL(clicked()), preferencesForm, SLOT(saveSettings()));
        QObject::connect(cancelButton, SIGNAL(clicked()), preferencesForm, SLOT(reject()));
        QObject::connect(defaultdataComboBox, SIGNAL(activated(int)), preferencesForm, SLOT(defaultDataChanged(int)));
        QObject::connect(encodingComboBox, SIGNAL(activated(int)), preferencesForm, SLOT(encodingChanged(int)));
        QObject::connect(setLocationButton, SIGNAL(clicked()), preferencesForm, SLOT(chooseLocation()));
        QObject::connect(defaultTextComboBox, SIGNAL(activated(int)), preferencesForm, SLOT(defaultTextChanged(int)));

        QMetaObject::connectSlotsByName(preferencesForm);
    } // setupUi

    void retranslateUi(QDialog *preferencesForm)
    {
        preferencesForm->setWindowTitle(QApplication::translate("preferencesForm", "Preferences", 0, QApplication::UnicodeUTF8));
        textLabel1->setText(QApplication::translate("preferencesForm", "Database encoding:", 0, QApplication::UnicodeUTF8));
        encodingComboBox->clear();
        encodingComboBox->insertItems(0, QStringList()
         << QApplication::translate("preferencesForm", "UTF8 (Unicode)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesForm", "Latin1 (8bit)", 0, QApplication::UnicodeUTF8)
        );
        textLabel2->setText(QApplication::translate("preferencesForm", "Default data for new records:", 0, QApplication::UnicodeUTF8));
        defaultdataComboBox->clear();
        defaultdataComboBox->insertItems(0, QStringList()
         << QApplication::translate("preferencesForm", "NULL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesForm", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesForm", "Empty string", 0, QApplication::UnicodeUTF8)
        );
        textLabel1_2->setText(QApplication::translate("preferencesForm", "Default text editor format:", 0, QApplication::UnicodeUTF8));
        defaultTextComboBox->clear();
        defaultTextComboBox->insertItems(0, QStringList()
         << QApplication::translate("preferencesForm", "Plain", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("preferencesForm", "Auto", 0, QApplication::UnicodeUTF8)
        );
        textLabel3->setText(QApplication::translate("preferencesForm", "Default location:", 0, QApplication::UnicodeUTF8));
        setLocationButton->setText(QApplication::translate("preferencesForm", "...", 0, QApplication::UnicodeUTF8));
        OKButton->setText(QApplication::translate("preferencesForm", "O&K", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("preferencesForm", "Ca&ncel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class preferencesForm: public Ui_preferencesForm {};
} // namespace Ui

QT_END_NAMESPACE

class preferencesForm : public QDialog, public Ui::preferencesForm
{
    Q_OBJECT

public:
    preferencesForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~preferencesForm();

    QString defaulttext;
    QString defaultlocation;
    QString defaultnewdata;
    QString defaultencoding;

public slots:
    virtual void defaultDataChanged( int which );
    virtual void defaultTextChanged( int which );
    virtual void encodingChanged( int which );
    virtual void chooseLocation();
    virtual void loadSettings();
    virtual void saveSettings();

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // PREFERENCESFORM_H
