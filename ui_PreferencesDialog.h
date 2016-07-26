/********************************************************************************
** Form generated from reading UI file 'PreferencesDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCESDIALOG_H
#define UI_PREFERENCESDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFontComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "sqltextedit.h"

QT_BEGIN_NAMESPACE

class Ui_PreferencesDialog
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QFormLayout *formLayout_3;
    QVBoxLayout *verticalLayout_6;
    QComboBox *comboDefaultLocation;
    QHBoxLayout *horizontalLayout;
    QLineEdit *locationEdit;
    QToolButton *setLocationButton;
    QLabel *label_4;
    QLabel *label_9;
    QComboBox *languageComboBox;
    QLabel *labelUpdates;
    QCheckBox *checkUpdates;
    QWidget *tab;
    QFormLayout *formLayout;
    QLabel *label;
    QComboBox *encodingComboBox;
    QLabel *label_5;
    QCheckBox *foreignKeysCheckBox;
    QLabel *label_8;
    QCheckBox *checkHideSchemaLinebreaks;
    QLabel *label_2;
    QSpinBox *spinPrefetchSize;
    QPushButton *buttonDatabaseAdvanced;
    SqlTextEdit *editDatabaseDefaultSqlText;
    QLabel *labelDatabaseDefaultSqlText;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *defaultFieldTypeComboBox;
    QLabel *defaultFieldTypeLabel;
    QWidget *tab_5;
    QGridLayout *gridLayout_2;
    QVBoxLayout *verticalLayout_5;
    QGroupBox *groupBox_3;
    QFormLayout *formLayout_4;
    QLabel *label_11;
    QFontComboBox *comboDataBrowserFont;
    QLabel *label_17;
    QSpinBox *spinDataBrowserFontSize;
    QGroupBox *groupBox;
    QFormLayout *gridLayout;
    QLineEdit *txtNull;
    QLabel *label_12;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *lb_null;
    QLabel *lb_regular;
    QLabel *label_16;
    QFrame *fr_reg_fg;
    QLabel *lb_text;
    QLabel *lb_binary;
    QFrame *fr_null_fg;
    QLabel *lb_background;
    QFrame *fr_bin_fg;
    QFrame *fr_null_bg;
    QFrame *fr_bin_bg;
    QFrame *fr_reg_bg;
    QGroupBox *groupBox_4;
    QFormLayout *gridLayout_4;
    QLineEdit *editFilterEscape;
    QLabel *label_18;
    QLabel *label_19;
    QSpinBox *spinFilterDelay;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_4;
    QTreeWidget *treeSyntaxHighlighting;
    QFormLayout *formLayout_2;
    QLabel *label_6;
    QSpinBox *spinEditorFontSize;
    QLabel *label_7;
    QSpinBox *spinLogFontSize;
    QLabel *label_13;
    QSpinBox *spinTabSize;
    QLabel *label_14;
    QFontComboBox *comboEditorFont;
    QLabel *label_15;
    QCheckBox *checkErrorIndicators;
    QLabel *label_10;
    QCheckBox *checkHorizontalTiling;
    QLabel *label_20;
    QCheckBox *checkAutoCompletion;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *listExtensions;
    QVBoxLayout *verticalLayout_2;
    QToolButton *buttonAddExtension;
    QToolButton *buttonRemoveExtension;
    QSpacerItem *verticalSpacer;
    QCheckBox *checkRegexDisabled;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *PreferencesDialog)
    {
        if (PreferencesDialog->objectName().isEmpty())
            PreferencesDialog->setObjectName(QStringLiteral("PreferencesDialog"));
        PreferencesDialog->resize(590, 555);
        PreferencesDialog->setModal(true);
        verticalLayout = new QVBoxLayout(PreferencesDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tabWidget = new QTabWidget(PreferencesDialog);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        formLayout_3 = new QFormLayout(tab_4);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        comboDefaultLocation = new QComboBox(tab_4);
        comboDefaultLocation->setObjectName(QStringLiteral("comboDefaultLocation"));

        verticalLayout_6->addWidget(comboDefaultLocation);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        locationEdit = new QLineEdit(tab_4);
        locationEdit->setObjectName(QStringLiteral("locationEdit"));
        locationEdit->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(locationEdit->sizePolicy().hasHeightForWidth());
        locationEdit->setSizePolicy(sizePolicy);
        locationEdit->setMinimumSize(QSize(316, 0));

        horizontalLayout->addWidget(locationEdit);

        setLocationButton = new QToolButton(tab_4);
        setLocationButton->setObjectName(QStringLiteral("setLocationButton"));

        horizontalLayout->addWidget(setLocationButton);


        verticalLayout_6->addLayout(horizontalLayout);


        formLayout_3->setLayout(1, QFormLayout::FieldRole, verticalLayout_6);

        label_4 = new QLabel(tab_4);
        label_4->setObjectName(QStringLiteral("label_4"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, label_4);

        label_9 = new QLabel(tab_4);
        label_9->setObjectName(QStringLiteral("label_9"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, label_9);

        languageComboBox = new QComboBox(tab_4);
        languageComboBox->setObjectName(QStringLiteral("languageComboBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(languageComboBox->sizePolicy().hasHeightForWidth());
        languageComboBox->setSizePolicy(sizePolicy1);
        languageComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        languageComboBox->setMinimumContentsLength(35);
        languageComboBox->setIconSize(QSize(20, 15));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, languageComboBox);

        labelUpdates = new QLabel(tab_4);
        labelUpdates->setObjectName(QStringLiteral("labelUpdates"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, labelUpdates);

        checkUpdates = new QCheckBox(tab_4);
        checkUpdates->setObjectName(QStringLiteral("checkUpdates"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, checkUpdates);

        tabWidget->addTab(tab_4, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        formLayout = new QFormLayout(tab);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        formLayout->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout->setFormAlignment(Qt::AlignHCenter|Qt::AlignTop);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        encodingComboBox = new QComboBox(tab);
        encodingComboBox->insertItems(0, QStringList()
         << QStringLiteral("UTF-8")
         << QStringLiteral("UTF-16")
        );
        encodingComboBox->setObjectName(QStringLiteral("encodingComboBox"));

        formLayout->setWidget(0, QFormLayout::FieldRole, encodingComboBox);

        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_5);

        foreignKeysCheckBox = new QCheckBox(tab);
        foreignKeysCheckBox->setObjectName(QStringLiteral("foreignKeysCheckBox"));

        formLayout->setWidget(1, QFormLayout::FieldRole, foreignKeysCheckBox);

        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_8);

        checkHideSchemaLinebreaks = new QCheckBox(tab);
        checkHideSchemaLinebreaks->setObjectName(QStringLiteral("checkHideSchemaLinebreaks"));

        formLayout->setWidget(2, QFormLayout::FieldRole, checkHideSchemaLinebreaks);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label_2);

        spinPrefetchSize = new QSpinBox(tab);
        spinPrefetchSize->setObjectName(QStringLiteral("spinPrefetchSize"));
        spinPrefetchSize->setMinimum(255);
        spinPrefetchSize->setMaximum(1000000);

        formLayout->setWidget(3, QFormLayout::FieldRole, spinPrefetchSize);

        buttonDatabaseAdvanced = new QPushButton(tab);
        buttonDatabaseAdvanced->setObjectName(QStringLiteral("buttonDatabaseAdvanced"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/down"), QSize(), QIcon::Normal, QIcon::Off);
        buttonDatabaseAdvanced->setIcon(icon);
        buttonDatabaseAdvanced->setCheckable(true);

        formLayout->setWidget(5, QFormLayout::FieldRole, buttonDatabaseAdvanced);

        editDatabaseDefaultSqlText = new SqlTextEdit(tab);
        editDatabaseDefaultSqlText->setObjectName(QStringLiteral("editDatabaseDefaultSqlText"));
        editDatabaseDefaultSqlText->setMinimumSize(QSize(20, 0));
        editDatabaseDefaultSqlText->setMaximumSize(QSize(16777215, 200));

        formLayout->setWidget(7, QFormLayout::FieldRole, editDatabaseDefaultSqlText);

        labelDatabaseDefaultSqlText = new QLabel(tab);
        labelDatabaseDefaultSqlText->setObjectName(QStringLiteral("labelDatabaseDefaultSqlText"));

        formLayout->setWidget(7, QFormLayout::LabelRole, labelDatabaseDefaultSqlText);

        horizontalSpacer_2 = new QSpacerItem(180, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        formLayout->setItem(8, QFormLayout::FieldRole, horizontalSpacer_2);

        defaultFieldTypeComboBox = new QComboBox(tab);
        defaultFieldTypeComboBox->setObjectName(QStringLiteral("defaultFieldTypeComboBox"));

        formLayout->setWidget(4, QFormLayout::FieldRole, defaultFieldTypeComboBox);

        defaultFieldTypeLabel = new QLabel(tab);
        defaultFieldTypeLabel->setObjectName(QStringLiteral("defaultFieldTypeLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, defaultFieldTypeLabel);

        tabWidget->addTab(tab, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        gridLayout_2 = new QGridLayout(tab_5);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        groupBox_3 = new QGroupBox(tab_5);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        formLayout_4 = new QFormLayout(groupBox_3);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, label_11);

        comboDataBrowserFont = new QFontComboBox(groupBox_3);
        comboDataBrowserFont->setObjectName(QStringLiteral("comboDataBrowserFont"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, comboDataBrowserFont);

        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName(QStringLiteral("label_17"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, label_17);

        spinDataBrowserFontSize = new QSpinBox(groupBox_3);
        spinDataBrowserFontSize->setObjectName(QStringLiteral("spinDataBrowserFontSize"));

        formLayout_4->setWidget(1, QFormLayout::FieldRole, spinDataBrowserFontSize);


        verticalLayout_5->addWidget(groupBox_3);

        groupBox = new QGroupBox(tab_5);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout = new QFormLayout(groupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        txtNull = new QLineEdit(groupBox);
        txtNull->setObjectName(QStringLiteral("txtNull"));
        txtNull->setText(QStringLiteral(""));
        txtNull->setMaxLength(15);
        txtNull->setAlignment(Qt::AlignCenter);

        gridLayout->setWidget(0, QFormLayout::FieldRole, txtNull);

        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout->setWidget(0, QFormLayout::LabelRole, label_12);


        verticalLayout_5->addWidget(groupBox);

        groupBox_2 = new QGroupBox(tab_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        lb_null = new QLabel(groupBox_2);
        lb_null->setObjectName(QStringLiteral("lb_null"));
        lb_null->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(lb_null, 2, 0, 1, 1);

        lb_regular = new QLabel(groupBox_2);
        lb_regular->setObjectName(QStringLiteral("lb_regular"));
        lb_regular->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(lb_regular, 4, 0, 1, 1);

        label_16 = new QLabel(groupBox_2);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_3->addWidget(label_16, 0, 0, 1, 1);

        fr_reg_fg = new QFrame(groupBox_2);
        fr_reg_fg->setObjectName(QStringLiteral("fr_reg_fg"));
        fr_reg_fg->setFocusPolicy(Qt::StrongFocus);
        fr_reg_fg->setAutoFillBackground(true);
        fr_reg_fg->setFrameShape(QFrame::StyledPanel);
        fr_reg_fg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_reg_fg, 4, 1, 1, 1);

        lb_text = new QLabel(groupBox_2);
        lb_text->setObjectName(QStringLiteral("lb_text"));
        lb_text->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_text, 0, 1, 1, 1);

        lb_binary = new QLabel(groupBox_2);
        lb_binary->setObjectName(QStringLiteral("lb_binary"));
        lb_binary->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_3->addWidget(lb_binary, 3, 0, 1, 1);

        fr_null_fg = new QFrame(groupBox_2);
        fr_null_fg->setObjectName(QStringLiteral("fr_null_fg"));
        fr_null_fg->setFocusPolicy(Qt::StrongFocus);
        fr_null_fg->setAutoFillBackground(true);
        fr_null_fg->setFrameShape(QFrame::StyledPanel);
        fr_null_fg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_null_fg, 2, 1, 1, 1);

        lb_background = new QLabel(groupBox_2);
        lb_background->setObjectName(QStringLiteral("lb_background"));
        lb_background->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(lb_background, 0, 2, 1, 1);

        fr_bin_fg = new QFrame(groupBox_2);
        fr_bin_fg->setObjectName(QStringLiteral("fr_bin_fg"));
        fr_bin_fg->setFocusPolicy(Qt::StrongFocus);
        fr_bin_fg->setAutoFillBackground(true);
        fr_bin_fg->setFrameShape(QFrame::StyledPanel);
        fr_bin_fg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_bin_fg, 3, 1, 1, 1);

        fr_null_bg = new QFrame(groupBox_2);
        fr_null_bg->setObjectName(QStringLiteral("fr_null_bg"));
        fr_null_bg->setFocusPolicy(Qt::StrongFocus);
        fr_null_bg->setAutoFillBackground(true);
        fr_null_bg->setFrameShape(QFrame::StyledPanel);
        fr_null_bg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_null_bg, 2, 2, 1, 1);

        fr_bin_bg = new QFrame(groupBox_2);
        fr_bin_bg->setObjectName(QStringLiteral("fr_bin_bg"));
        fr_bin_bg->setFocusPolicy(Qt::StrongFocus);
        fr_bin_bg->setAutoFillBackground(true);
        fr_bin_bg->setFrameShape(QFrame::StyledPanel);
        fr_bin_bg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_bin_bg, 3, 2, 1, 1);

        fr_reg_bg = new QFrame(groupBox_2);
        fr_reg_bg->setObjectName(QStringLiteral("fr_reg_bg"));
        fr_reg_bg->setFocusPolicy(Qt::StrongFocus);
        fr_reg_bg->setAutoFillBackground(true);
        fr_reg_bg->setFrameShape(QFrame::StyledPanel);
        fr_reg_bg->setFrameShadow(QFrame::Raised);

        gridLayout_3->addWidget(fr_reg_bg, 4, 2, 1, 1);


        verticalLayout_5->addWidget(groupBox_2);

        groupBox_4 = new QGroupBox(tab_5);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        gridLayout_4 = new QFormLayout(groupBox_4);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        editFilterEscape = new QLineEdit(groupBox_4);
        editFilterEscape->setObjectName(QStringLiteral("editFilterEscape"));
        editFilterEscape->setMaxLength(1);

        gridLayout_4->setWidget(1, QFormLayout::FieldRole, editFilterEscape);

        label_18 = new QLabel(groupBox_4);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_4->setWidget(1, QFormLayout::LabelRole, label_18);

        label_19 = new QLabel(groupBox_4);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_4->setWidget(0, QFormLayout::LabelRole, label_19);

        spinFilterDelay = new QSpinBox(groupBox_4);
        spinFilterDelay->setObjectName(QStringLiteral("spinFilterDelay"));
        spinFilterDelay->setMaximum(5000);

        gridLayout_4->setWidget(0, QFormLayout::FieldRole, spinFilterDelay);


        verticalLayout_5->addWidget(groupBox_4);


        gridLayout_2->addLayout(verticalLayout_5, 0, 0, 2, 2);

        tabWidget->addTab(tab_5, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        verticalLayout_4 = new QVBoxLayout(tab_2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        treeSyntaxHighlighting = new QTreeWidget(tab_2);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeSyntaxHighlighting);
        __qtreewidgetitem->setText(0, QStringLiteral("keyword"));
        new QTreeWidgetItem(treeSyntaxHighlighting);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeSyntaxHighlighting);
        __qtreewidgetitem1->setText(0, QStringLiteral("table"));
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem(treeSyntaxHighlighting);
        __qtreewidgetitem2->setText(0, QStringLiteral("comment"));
        QTreeWidgetItem *__qtreewidgetitem3 = new QTreeWidgetItem(treeSyntaxHighlighting);
        __qtreewidgetitem3->setText(0, QStringLiteral("identifier"));
        QTreeWidgetItem *__qtreewidgetitem4 = new QTreeWidgetItem(treeSyntaxHighlighting);
        __qtreewidgetitem4->setText(0, QStringLiteral("string"));
        new QTreeWidgetItem(treeSyntaxHighlighting);
        treeSyntaxHighlighting->setObjectName(QStringLiteral("treeSyntaxHighlighting"));
        treeSyntaxHighlighting->setProperty("showDropIndicator", QVariant(false));
        treeSyntaxHighlighting->setRootIsDecorated(false);

        verticalLayout_4->addWidget(treeSyntaxHighlighting);

        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, label_6);

        spinEditorFontSize = new QSpinBox(tab_2);
        spinEditorFontSize->setObjectName(QStringLiteral("spinEditorFontSize"));
        spinEditorFontSize->setMinimum(1);

        formLayout_2->setWidget(1, QFormLayout::FieldRole, spinEditorFontSize);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QStringLiteral("label_7"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, label_7);

        spinLogFontSize = new QSpinBox(tab_2);
        spinLogFontSize->setObjectName(QStringLiteral("spinLogFontSize"));
        spinLogFontSize->setMinimum(1);

        formLayout_2->setWidget(2, QFormLayout::FieldRole, spinLogFontSize);

        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QStringLiteral("label_13"));

        formLayout_2->setWidget(3, QFormLayout::LabelRole, label_13);

        spinTabSize = new QSpinBox(tab_2);
        spinTabSize->setObjectName(QStringLiteral("spinTabSize"));
        spinTabSize->setMinimum(1);
        spinTabSize->setMaximum(16);
        spinTabSize->setValue(4);

        formLayout_2->setWidget(3, QFormLayout::FieldRole, spinTabSize);

        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QStringLiteral("label_14"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, label_14);

        comboEditorFont = new QFontComboBox(tab_2);
        comboEditorFont->setObjectName(QStringLiteral("comboEditorFont"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, comboEditorFont);

        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QStringLiteral("label_15"));

        formLayout_2->setWidget(5, QFormLayout::LabelRole, label_15);

        checkErrorIndicators = new QCheckBox(tab_2);
        checkErrorIndicators->setObjectName(QStringLiteral("checkErrorIndicators"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, checkErrorIndicators);

        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QStringLiteral("label_10"));

        formLayout_2->setWidget(6, QFormLayout::LabelRole, label_10);

        checkHorizontalTiling = new QCheckBox(tab_2);
        checkHorizontalTiling->setObjectName(QStringLiteral("checkHorizontalTiling"));

        formLayout_2->setWidget(6, QFormLayout::FieldRole, checkHorizontalTiling);

        label_20 = new QLabel(tab_2);
        label_20->setObjectName(QStringLiteral("label_20"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, label_20);

        checkAutoCompletion = new QCheckBox(tab_2);
        checkAutoCompletion->setObjectName(QStringLiteral("checkAutoCompletion"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, checkAutoCompletion);


        verticalLayout_4->addLayout(formLayout_2);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_3 = new QVBoxLayout(tab_3);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_3 = new QLabel(tab_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        listExtensions = new QListWidget(tab_3);
        listExtensions->setObjectName(QStringLiteral("listExtensions"));
        listExtensions->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listExtensions->setProperty("showDropIndicator", QVariant(false));

        horizontalLayout_2->addWidget(listExtensions);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        buttonAddExtension = new QToolButton(tab_3);
        buttonAddExtension->setObjectName(QStringLiteral("buttonAddExtension"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/load_extension"), QSize(), QIcon::Normal, QIcon::Off);
        buttonAddExtension->setIcon(icon1);

        verticalLayout_2->addWidget(buttonAddExtension);

        buttonRemoveExtension = new QToolButton(tab_3);
        buttonRemoveExtension->setObjectName(QStringLiteral("buttonRemoveExtension"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/icons/remove_extension"), QSize(), QIcon::Normal, QIcon::Off);
        buttonRemoveExtension->setIcon(icon2);

        verticalLayout_2->addWidget(buttonRemoveExtension);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_2);


        verticalLayout_3->addLayout(horizontalLayout_2);

        checkRegexDisabled = new QCheckBox(tab_3);
        checkRegexDisabled->setObjectName(QStringLiteral("checkRegexDisabled"));

        verticalLayout_3->addWidget(checkRegexDisabled);

        tabWidget->addTab(tab_3, QString());

        verticalLayout->addWidget(tabWidget);

        buttonBox = new QDialogButtonBox(PreferencesDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);

#ifndef QT_NO_SHORTCUT
        label_4->setBuddy(locationEdit);
        label_9->setBuddy(languageComboBox);
        labelUpdates->setBuddy(checkUpdates);
        label->setBuddy(encodingComboBox);
        label_5->setBuddy(foreignKeysCheckBox);
        label_8->setBuddy(checkHideSchemaLinebreaks);
        label_2->setBuddy(spinPrefetchSize);
        labelDatabaseDefaultSqlText->setBuddy(editDatabaseDefaultSqlText);
        label_11->setBuddy(comboDataBrowserFont);
        label_17->setBuddy(spinDataBrowserFontSize);
        label_12->setBuddy(txtNull);
        label_18->setBuddy(editFilterEscape);
        label_19->setBuddy(spinFilterDelay);
        label_6->setBuddy(spinEditorFontSize);
        label_7->setBuddy(spinLogFontSize);
        label_13->setBuddy(spinTabSize);
        label_14->setBuddy(comboEditorFont);
        label_15->setBuddy(checkErrorIndicators);
        label_10->setBuddy(checkHorizontalTiling);
        label_20->setBuddy(checkAutoCompletion);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(tabWidget, comboDefaultLocation);
        QWidget::setTabOrder(comboDefaultLocation, locationEdit);
        QWidget::setTabOrder(locationEdit, setLocationButton);
        QWidget::setTabOrder(setLocationButton, languageComboBox);
        QWidget::setTabOrder(languageComboBox, checkUpdates);
        QWidget::setTabOrder(checkUpdates, encodingComboBox);
        QWidget::setTabOrder(encodingComboBox, foreignKeysCheckBox);
        QWidget::setTabOrder(foreignKeysCheckBox, checkHideSchemaLinebreaks);
        QWidget::setTabOrder(checkHideSchemaLinebreaks, spinPrefetchSize);
        QWidget::setTabOrder(spinPrefetchSize, defaultFieldTypeComboBox);
        QWidget::setTabOrder(defaultFieldTypeComboBox, buttonDatabaseAdvanced);
        QWidget::setTabOrder(buttonDatabaseAdvanced, editDatabaseDefaultSqlText);
        QWidget::setTabOrder(editDatabaseDefaultSqlText, comboDataBrowserFont);
        QWidget::setTabOrder(comboDataBrowserFont, spinDataBrowserFontSize);
        QWidget::setTabOrder(spinDataBrowserFontSize, txtNull);
        QWidget::setTabOrder(txtNull, fr_null_fg);
        QWidget::setTabOrder(fr_null_fg, fr_null_bg);
        QWidget::setTabOrder(fr_null_bg, fr_bin_fg);
        QWidget::setTabOrder(fr_bin_fg, fr_bin_bg);
        QWidget::setTabOrder(fr_bin_bg, fr_reg_fg);
        QWidget::setTabOrder(fr_reg_fg, fr_reg_bg);
        QWidget::setTabOrder(fr_reg_bg, spinFilterDelay);
        QWidget::setTabOrder(spinFilterDelay, editFilterEscape);
        QWidget::setTabOrder(editFilterEscape, treeSyntaxHighlighting);
        QWidget::setTabOrder(treeSyntaxHighlighting, comboEditorFont);
        QWidget::setTabOrder(comboEditorFont, spinEditorFontSize);
        QWidget::setTabOrder(spinEditorFontSize, spinLogFontSize);
        QWidget::setTabOrder(spinLogFontSize, spinTabSize);
        QWidget::setTabOrder(spinTabSize, checkAutoCompletion);
        QWidget::setTabOrder(checkAutoCompletion, checkErrorIndicators);
        QWidget::setTabOrder(checkErrorIndicators, checkHorizontalTiling);
        QWidget::setTabOrder(checkHorizontalTiling, listExtensions);
        QWidget::setTabOrder(listExtensions, buttonAddExtension);
        QWidget::setTabOrder(buttonAddExtension, buttonRemoveExtension);
        QWidget::setTabOrder(buttonRemoveExtension, checkRegexDisabled);

        retranslateUi(PreferencesDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PreferencesDialog, SLOT(saveSettings()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PreferencesDialog, SLOT(reject()));
        QObject::connect(buttonAddExtension, SIGNAL(clicked()), PreferencesDialog, SLOT(addExtension()));
        QObject::connect(buttonRemoveExtension, SIGNAL(clicked()), PreferencesDialog, SLOT(removeExtension()));
        QObject::connect(treeSyntaxHighlighting, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), PreferencesDialog, SLOT(showColourDialog(QTreeWidgetItem*,int)));
        QObject::connect(setLocationButton, SIGNAL(clicked()), PreferencesDialog, SLOT(chooseLocation()));
        QObject::connect(buttonDatabaseAdvanced, SIGNAL(toggled(bool)), labelDatabaseDefaultSqlText, SLOT(setVisible(bool)));
        QObject::connect(buttonDatabaseAdvanced, SIGNAL(toggled(bool)), editDatabaseDefaultSqlText, SLOT(setVisible(bool)));

        tabWidget->setCurrentIndex(4);


        QMetaObject::connectSlotsByName(PreferencesDialog);
    } // setupUi

    void retranslateUi(QDialog *PreferencesDialog)
    {
        PreferencesDialog->setWindowTitle(QApplication::translate("PreferencesDialog", "Preferences", 0));
        comboDefaultLocation->clear();
        comboDefaultLocation->insertItems(0, QStringList()
         << QApplication::translate("PreferencesDialog", "Remember last location", 0)
         << QApplication::translate("PreferencesDialog", "Always use this location", 0)
         << QApplication::translate("PreferencesDialog", "Remember last location for session only", 0)
        );
        setLocationButton->setText(QApplication::translate("PreferencesDialog", "...", 0));
        label_4->setText(QApplication::translate("PreferencesDialog", "Default &location", 0));
        label_9->setText(QApplication::translate("PreferencesDialog", "Lan&guage", 0));
        labelUpdates->setText(QApplication::translate("PreferencesDialog", "Automatic &updates", 0));
        checkUpdates->setText(QApplication::translate("PreferencesDialog", "enabled", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("PreferencesDialog", "&General", 0));
        label->setText(QApplication::translate("PreferencesDialog", "Database &encoding", 0));
#ifndef QT_NO_TOOLTIP
        label_5->setToolTip(QApplication::translate("PreferencesDialog", "Open databases with foreign keys enabled.", 0));
#endif // QT_NO_TOOLTIP
        label_5->setText(QApplication::translate("PreferencesDialog", "&Foreign keys", 0));
        foreignKeysCheckBox->setText(QApplication::translate("PreferencesDialog", "enabled", 0));
        label_8->setText(QApplication::translate("PreferencesDialog", "Remove line breaks in schema &view", 0));
        label_2->setText(QApplication::translate("PreferencesDialog", "Prefetch block si&ze", 0));
        buttonDatabaseAdvanced->setText(QApplication::translate("PreferencesDialog", "Advanced", 0));
        labelDatabaseDefaultSqlText->setText(QApplication::translate("PreferencesDialog", "SQL to execute after opening database", 0));
        defaultFieldTypeLabel->setText(QApplication::translate("PreferencesDialog", "Default field type", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("PreferencesDialog", "&Database", 0));
        groupBox_3->setTitle(QApplication::translate("PreferencesDialog", "Font", 0));
        label_11->setText(QApplication::translate("PreferencesDialog", "&Font", 0));
        label_17->setText(QApplication::translate("PreferencesDialog", "Font si&ze:", 0));
        groupBox->setTitle(QApplication::translate("PreferencesDialog", "NULL fields", 0));
        label_12->setText(QApplication::translate("PreferencesDialog", "&Text", 0));
        groupBox_2->setTitle(QApplication::translate("PreferencesDialog", "Field colors", 0));
        lb_null->setText(QApplication::translate("PreferencesDialog", "NULL", 0));
        lb_regular->setText(QApplication::translate("PreferencesDialog", "Regular", 0));
        label_16->setText(QString());
        lb_text->setText(QApplication::translate("PreferencesDialog", "Text", 0));
        lb_binary->setText(QApplication::translate("PreferencesDialog", "Binary", 0));
        lb_background->setText(QApplication::translate("PreferencesDialog", "Background", 0));
        groupBox_4->setTitle(QApplication::translate("PreferencesDialog", "Filters", 0));
        label_18->setText(QApplication::translate("PreferencesDialog", "Escape character", 0));
        label_19->setText(QApplication::translate("PreferencesDialog", "Delay time (&ms)", 0));
#ifndef QT_NO_TOOLTIP
        spinFilterDelay->setToolTip(QApplication::translate("PreferencesDialog", "Set the waiting time before a new filter value is applied. Can be set to 0 for disabling waiting.", 0));
#endif // QT_NO_TOOLTIP
        tabWidget->setTabText(tabWidget->indexOf(tab_5), QApplication::translate("PreferencesDialog", "Data &Browser", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeSyntaxHighlighting->headerItem();
        ___qtreewidgetitem->setText(5, QApplication::translate("PreferencesDialog", "Underline", 0));
        ___qtreewidgetitem->setText(4, QApplication::translate("PreferencesDialog", "Italic", 0));
        ___qtreewidgetitem->setText(3, QApplication::translate("PreferencesDialog", "Bold", 0));
        ___qtreewidgetitem->setText(2, QApplication::translate("PreferencesDialog", "Colour", 0));
        ___qtreewidgetitem->setText(1, QApplication::translate("PreferencesDialog", "Context", 0));
        ___qtreewidgetitem->setText(0, QApplication::translate("PreferencesDialog", "Settings name", 0));

        const bool __sortingEnabled = treeSyntaxHighlighting->isSortingEnabled();
        treeSyntaxHighlighting->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeSyntaxHighlighting->topLevelItem(0);
        ___qtreewidgetitem1->setText(1, QApplication::translate("PreferencesDialog", "Keyword", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = treeSyntaxHighlighting->topLevelItem(1);
        ___qtreewidgetitem2->setText(1, QApplication::translate("PreferencesDialog", "Function", 0));
        ___qtreewidgetitem2->setText(0, QApplication::translate("PreferencesDialog", "function", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = treeSyntaxHighlighting->topLevelItem(2);
        ___qtreewidgetitem3->setText(1, QApplication::translate("PreferencesDialog", "Table", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = treeSyntaxHighlighting->topLevelItem(3);
        ___qtreewidgetitem4->setText(1, QApplication::translate("PreferencesDialog", "Comment", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = treeSyntaxHighlighting->topLevelItem(4);
        ___qtreewidgetitem5->setText(1, QApplication::translate("PreferencesDialog", "Identifier", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = treeSyntaxHighlighting->topLevelItem(5);
        ___qtreewidgetitem6->setText(1, QApplication::translate("PreferencesDialog", "String", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = treeSyntaxHighlighting->topLevelItem(6);
        ___qtreewidgetitem7->setText(1, QApplication::translate("PreferencesDialog", "Current line", 0));
        ___qtreewidgetitem7->setText(0, QApplication::translate("PreferencesDialog", "currentline", 0));
        treeSyntaxHighlighting->setSortingEnabled(__sortingEnabled);

        label_6->setText(QApplication::translate("PreferencesDialog", "SQL &editor font size", 0));
        label_7->setText(QApplication::translate("PreferencesDialog", "SQL &log font size", 0));
        label_13->setText(QApplication::translate("PreferencesDialog", "Tab size", 0));
        label_14->setText(QApplication::translate("PreferencesDialog", "SQL editor &font", 0));
        label_15->setText(QApplication::translate("PreferencesDialog", "Error indicators", 0));
#ifndef QT_NO_TOOLTIP
        checkErrorIndicators->setToolTip(QApplication::translate("PreferencesDialog", "Enabling error indicators highlights the SQL code lines that caused errors during the last execution", 0));
#endif // QT_NO_TOOLTIP
        checkErrorIndicators->setText(QApplication::translate("PreferencesDialog", "enabled", 0));
        label_10->setText(QApplication::translate("PreferencesDialog", "Hori&zontal tiling", 0));
#ifndef QT_NO_TOOLTIP
        checkHorizontalTiling->setToolTip(QApplication::translate("PreferencesDialog", "If enabled the SQL code editor and the result table view are shown side by side instead of one over the other.", 0));
#endif // QT_NO_TOOLTIP
        checkHorizontalTiling->setText(QApplication::translate("PreferencesDialog", "enabled", 0));
        label_20->setText(QApplication::translate("PreferencesDialog", "Code co&mpletion", 0));
        checkAutoCompletion->setText(QApplication::translate("PreferencesDialog", "enabled", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("PreferencesDialog", "&SQL", 0));
        label_3->setText(QApplication::translate("PreferencesDialog", "Select extensions to load for every database:", 0));
#ifndef QT_NO_TOOLTIP
        buttonAddExtension->setToolTip(QApplication::translate("PreferencesDialog", "Add extension", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        buttonRemoveExtension->setToolTip(QApplication::translate("PreferencesDialog", "Remove extension", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        checkRegexDisabled->setToolTip(QApplication::translate("PreferencesDialog", "<html><head/><body><p>While supporting the REGEXP operator SQLite doesn't implement any regular expression<br/>algorithm but calls back the running application. DB Browser for SQLite implements this<br/>algorithm for you to let you use REGEXP out of the box. However, as there are multiple possible<br/>implementations of this and you might want to use another one, you're free to disable the<br/>application's implementation and load your own by using an extension. Requires restart of the application.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        checkRegexDisabled->setText(QApplication::translate("PreferencesDialog", "Disable Regular Expression extension", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("PreferencesDialog", "&Extensions", 0));
    } // retranslateUi

};

namespace Ui {
    class PreferencesDialog: public Ui_PreferencesDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCESDIALOG_H
