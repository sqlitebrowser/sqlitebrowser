#include "aboutform.h"

#include <QApplication>
#include <QPixmap>

/*
 *  Constructs a aboutForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
AboutDialog::AboutDialog(QWidget* parent, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setupUi();

    init();
}

void AboutDialog::setupUi()
{
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("aboutForm"));
    //this->resize(542, 257);
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setSpacing(6);
    vboxLayout->setContentsMargins(11, 11, 11, 11);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(6);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(6);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    spacer5 = new QSpacerItem(424, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacer5);

    okButton = new QPushButton();
    okButton->setObjectName(QString::fromUtf8("okButton"));
    okButton->setDefault(true);

    hboxLayout->addWidget(okButton);


    gridLayout->addLayout(hboxLayout, 1, 0, 1, 3);

    spacer13 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(spacer13, 0, 1, 1, 1);

    vboxLayout1 = new QVBoxLayout();
    vboxLayout1->setSpacing(6);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    pixmapLabel1 = new QLabel();
    pixmapLabel1->setObjectName(QString::fromUtf8("pixmapLabel1"));
    pixmapLabel1->setPixmap(QPixmap(":/oldimages/128"));
    pixmapLabel1->setScaledContents(false);
    pixmapLabel1->setWordWrap(false);

    vboxLayout1->addWidget(pixmapLabel1);

    spacer12 = new QSpacerItem(20, 31, QSizePolicy::Minimum, QSizePolicy::Expanding);

    vboxLayout1->addItem(spacer12);


    gridLayout->addLayout(vboxLayout1, 0, 0, 1, 1);

    textLabel1 = new QLabel();
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
    textLabel1->setWordWrap(false);

    gridLayout->addWidget(textLabel1, 0, 2, 1, 1);


    vboxLayout->addLayout(gridLayout);


    retranslateUi();
    QObject::connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

    QMetaObject::connectSlotsByName(this);
} // setupUi

void AboutDialog::retranslateUi()
{
    this->setWindowTitle(QObject::tr("About"));
    textLabel1->setText(QObject::tr("<p>Version 2.0</font></p>\n"
"<p></p>\n"
"<p><font size=\"+1\">SQLite Database Browser is an open source, public domain, freeware visual tool used to\n"
"<p><font size=\"+1\">create, design and edit SQLite database files.</font></p>\n"
"<p></p>\n"
"<p>For more information on this program please visit our site at: http://sqlitebrowser.sf.net</p>\n"
"<p></p>\n"
"<p></p>\n"
"<p><font size=\"-1\">This software uses the LGPL Qt Toolkit from http://www.qtsoftware.com</p>\n"
"<p><font size=\"-1\">See LICENSING.txt and Qt_LICENSE_LGPL.txt for licensing terms and information.</p>"));
    okButton->setText(QObject::tr("Close"));
} // retranslateUi

/*
 *  Destroys the object and frees any allocated resources
 */
AboutDialog::~AboutDialog()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AboutDialog::languageChange()
{
    retranslateUi();
}

void AboutDialog::init()
{
    this->setWindowTitle(QApplication::applicationName());
}

