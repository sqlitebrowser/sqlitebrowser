#ifndef FINDFORM_H
#define FINDFORM_H

#include <QTableWidget>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "sqlitedb.h"

class Ui_findForm
{
public:
    QVBoxLayout *vboxLayout;
    QGridLayout *gridLayout;
    QPushButton *searchButton;
    QComboBox *findFieldCombobox;
    QLineEdit *searchLine;
    QComboBox *findOperatorComboBox;
    QTableWidget *findTableWidget;
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

        findTableWidget = new QTableWidget(findForm);
        findTableWidget->setColumnCount(2);
        findTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem( QObject::tr("Record") ));
        findTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem( QObject::tr("Data") ));
        findTableWidget->setObjectName(QString::fromUtf8("findListView"));
        findTableWidget->setMidLineWidth(30);
        findTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        //findTableWidget->setResizePolicy(Q3ScrollView::Manual);
        //findTableWidget->setResizeMode(Q3ListView::LastColumn);

        vboxLayout->addWidget(findTableWidget);

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
        QObject::connect(findTableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), findForm, SLOT(recordSelected(QTableWidgetItem*)));

        QMetaObject::connectSlotsByName(findForm);
    } // setupUi

    void retranslateUi(QDialog *findForm)
    {
        findForm->setWindowTitle(QObject::tr("Find"));
        searchButton->setText(QObject::tr("Search"));
        searchButton->setProperty("toolTip", QVariant(QObject::tr("Perform the search")));
        searchButton->setProperty("whatsThis", QVariant(QObject::tr("This button starts the search process")));

        findFieldCombobox->clear();
        findFieldCombobox->insertItems(0, QStringList()
            << QObject::tr("user")
        );
        findFieldCombobox->setProperty("toolTip", QVariant(QObject::tr("Field to be searched")));
        findFieldCombobox->setProperty("whatsThis", QVariant(QObject::tr("Use this control to select the field to be searched in the current table")));

        searchLine->setProperty("toolTip", QVariant(QObject::tr("Enter values or words to search")));
        searchLine->setProperty("whatsThis", QVariant(QObject::tr("This is a place to enter the word or number to be searched in the database")));

        findOperatorComboBox->clear();
        findOperatorComboBox->insertItems(0, QStringList()
         << QObject::tr("=")
         << QObject::tr("contains")
         << QObject::tr(">")
         << QObject::tr(">=")
         << QObject::tr("<=")
         << QObject::tr("<")
        );
        findOperatorComboBox->setProperty("toolTip", QVariant(QObject::tr("Search criteria: use 'contains' for partial matches")));
        findOperatorComboBox->setProperty("whatsThis", QVariant(QObject::tr("This control is used to select the search criteria used to look for the search term in the database. Use '=' or 'contains' to find words, and the comparison symbols to filter numeric data.")));

        findTableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem( QObject::tr("Record") ));
        findTableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem( QObject::tr("Data") ));
        findTableWidget->setProperty("whatsThis", QVariant(QObject::tr("Results of the search will appear in this area. Click on a result to select the corresponding record in the database")));

        resultsLabel->setText(QObject::tr("Found:"));
    } // retranslateUi

};

namespace Ui {
    class findForm: public Ui_findForm {};
} // namespace Ui

class findForm : public QDialog, public Ui::findForm
{
    Q_OBJECT

public:
    findForm(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~findForm();

public slots:
    virtual void showResults( resultMap rmap );
    virtual void find();
    virtual void resetFields( QStringList fieldlist );
    virtual void resetResults();
    virtual void recordSelected( QTableWidgetItem * witem );
    virtual void closeEvent( QCloseEvent * );

signals:
    void lookfor(const QString&, const QString&, const QString&);
    void showrecord(int);
    void goingAway();

protected slots:
    virtual void languageChange();

};

#endif // FINDFORM_H
