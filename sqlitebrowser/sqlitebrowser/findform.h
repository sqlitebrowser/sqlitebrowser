#ifndef FINDFORM_H
#define FINDFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3Header>
#include <Qt3Support/Q3ListView>
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
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

QT_BEGIN_NAMESPACE

class Ui_findForm
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QPushButton *searchButton;
    QComboBox *findFieldCombobox;
    QLineEdit *searchLine;
    QComboBox *findOperatorComboBox;
    Q3ListView *findListView;
    QHBoxLayout *hboxLayout;
    QLabel *resultsLabel;
    QSpacerItem *spacer10;

    void setupUi(QDialog *findForm)
    {
        if (findForm->objectName().isEmpty())
            findForm->setObjectName(QString::fromUtf8("findForm"));
        findForm->resize(239, 319);
        vboxLayout = new QVBoxLayout(findForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        searchButton = new QPushButton(findForm);
        searchButton->setObjectName(QString::fromUtf8("searchButton"));

        gridLayout->addWidget(searchButton, 1, 2, 1, 1);

        findFieldCombobox = new QComboBox(findForm);
        findFieldCombobox->setObjectName(QString::fromUtf8("findFieldCombobox"));

        gridLayout->addWidget(findFieldCombobox, 0, 0, 1, 1);

        searchLine = new QLineEdit(findForm);
        searchLine->setObjectName(QString::fromUtf8("searchLine"));

        gridLayout->addWidget(searchLine, 1, 0, 1, 2);

        findOperatorComboBox = new QComboBox(findForm);
        findOperatorComboBox->setObjectName(QString::fromUtf8("findOperatorComboBox"));

        gridLayout->addWidget(findOperatorComboBox, 0, 1, 1, 2);


        vboxLayout->addLayout(gridLayout);

        findListView = new Q3ListView(findForm);
        findListView->addColumn(QApplication::translate("findForm", "Record", 0, QApplication::UnicodeUTF8));
        findListView->header()->setClickEnabled(true, findListView->header()->count() - 1);
        findListView->header()->setResizeEnabled(true, findListView->header()->count() - 1);
        findListView->addColumn(QApplication::translate("findForm", "Data", 0, QApplication::UnicodeUTF8));
        findListView->header()->setClickEnabled(true, findListView->header()->count() - 1);
        findListView->header()->setResizeEnabled(true, findListView->header()->count() - 1);
        findListView->setObjectName(QString::fromUtf8("findListView"));
        findListView->setMidLineWidth(30);
        findListView->setResizePolicy(Q3ScrollView::Manual);
        findListView->setResizeMode(Q3ListView::LastColumn);

        vboxLayout->addWidget(findListView);

        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        resultsLabel = new QLabel(findForm);
        resultsLabel->setObjectName(QString::fromUtf8("resultsLabel"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(7), static_cast<QSizePolicy::Policy>(5));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(resultsLabel->sizePolicy().hasHeightForWidth());
        resultsLabel->setSizePolicy(sizePolicy);
        resultsLabel->setWordWrap(false);

        hboxLayout->addWidget(resultsLabel);

        spacer10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer10);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(findForm);
        QObject::connect(searchButton, SIGNAL(clicked()), findForm, SLOT(find()));
        QObject::connect(findListView, SIGNAL(clicked(Q3ListViewItem*)), findForm, SLOT(recordSelected(Q3ListViewItem*)));

        QMetaObject::connectSlotsByName(findForm);
    } // setupUi

    void retranslateUi(QDialog *findForm)
    {
        findForm->setWindowTitle(QApplication::translate("findForm", "Find", 0, QApplication::UnicodeUTF8));
        searchButton->setText(QApplication::translate("findForm", "Search", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        searchButton->setProperty("toolTip", QVariant(QApplication::translate("findForm", "Perform the search", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        searchButton->setProperty("whatsThis", QVariant(QApplication::translate("findForm", "This button starts the search process", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        findFieldCombobox->clear();
        findFieldCombobox->insertItems(0, QStringList()
         << QApplication::translate("findForm", "user", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        findFieldCombobox->setProperty("toolTip", QVariant(QApplication::translate("findForm", "Field to be searched", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        findFieldCombobox->setProperty("whatsThis", QVariant(QApplication::translate("findForm", "Use this control to select the field to be searched in the current table", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        searchLine->setProperty("toolTip", QVariant(QApplication::translate("findForm", "Enter values or words to search", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        searchLine->setProperty("whatsThis", QVariant(QApplication::translate("findForm", "This is a place to enter the word or number to be searched in the database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        findOperatorComboBox->clear();
        findOperatorComboBox->insertItems(0, QStringList()
         << QApplication::translate("findForm", "=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("findForm", "contains", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("findForm", ">", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("findForm", ">=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("findForm", "<=", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("findForm", "<", 0, QApplication::UnicodeUTF8)
        );
#ifndef QT_NO_TOOLTIP
        findOperatorComboBox->setProperty("toolTip", QVariant(QApplication::translate("findForm", "Search criteria: use 'contains' for partial matches", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        findOperatorComboBox->setProperty("whatsThis", QVariant(QApplication::translate("findForm", "This control is used to select the search criteria used to look for the search term in the database. Use '=' or 'contains' to find words, and the comparison symbols to filter numeric data.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        findListView->header()->setLabel(0, QApplication::translate("findForm", "Record", 0, QApplication::UnicodeUTF8));
        findListView->header()->setLabel(1, QApplication::translate("findForm", "Data", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        findListView->setProperty("whatsThis", QVariant(QApplication::translate("findForm", "Results of the search will appear in this area. Click on a result to select the corresponding record in the database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        resultsLabel->setText(QApplication::translate("findForm", "Found:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class findForm: public Ui_findForm {};
} // namespace Ui

QT_END_NAMESPACE

class findForm : public QDialog, public Ui::findForm
{
    Q_OBJECT

public:
    findForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~findForm();

public slots:
    virtual void showResults( resultMap rmap );
    virtual void find();
    virtual void resetFields( QStringList fieldlist );
    virtual void resetResults();
    virtual void recordSelected( Q3ListViewItem * witem );
    virtual void closeEvent( QCloseEvent * );

signals:
    void lookfor(const QString&, const QString&, const QString&);
    void showrecord(int);
    void goingAway();

protected slots:
    virtual void languageChange();

};

#endif // FINDFORM_H
