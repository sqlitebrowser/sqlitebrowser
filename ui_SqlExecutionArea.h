/********************************************************************************
** Form generated from reading UI file 'SqlExecutionArea.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SQLEXECUTIONAREA_H
#define UI_SQLEXECUTIONAREA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ExtendedTableWidget.h"
#include "sqltextedit.h"

QT_BEGIN_NAMESPACE

class Ui_SqlExecutionArea
{
public:
    QAction *actionExportCsv;
    QAction *actionSaveAsView;
    QVBoxLayout *verticalLayout_2;
    QSplitter *splitter;
    SqlTextEdit *editEditor;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    ExtendedTableWidget *tableResult;
    QHBoxLayout *horizontalLayout;
    QTextEdit *editErrors;
    QToolButton *buttonSave;

    void setupUi(QWidget *SqlExecutionArea)
    {
        if (SqlExecutionArea->objectName().isEmpty())
            SqlExecutionArea->setObjectName(QStringLiteral("SqlExecutionArea"));
        SqlExecutionArea->resize(365, 482);
        actionExportCsv = new QAction(SqlExecutionArea);
        actionExportCsv->setObjectName(QStringLiteral("actionExportCsv"));
        actionSaveAsView = new QAction(SqlExecutionArea);
        actionSaveAsView->setObjectName(QStringLiteral("actionSaveAsView"));
        verticalLayout_2 = new QVBoxLayout(SqlExecutionArea);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        splitter = new QSplitter(SqlExecutionArea);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        editEditor = new SqlTextEdit(splitter);
        editEditor->setObjectName(QStringLiteral("editEditor"));
        splitter->addWidget(editEditor);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        tableResult = new ExtendedTableWidget(layoutWidget);
        tableResult->setObjectName(QStringLiteral("tableResult"));
        tableResult->setEditTriggers(QAbstractItemView::NoEditTriggers);

        verticalLayout->addWidget(tableResult);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        editErrors = new QTextEdit(layoutWidget);
        editErrors->setObjectName(QStringLiteral("editErrors"));
        editErrors->setMaximumSize(QSize(16777215, 80));
        QFont font;
        font.setFamily(QStringLiteral("Monospace"));
        font.setPointSize(8);
        editErrors->setFont(font);
        editErrors->setAcceptDrops(false);
        editErrors->setFrameShape(QFrame::StyledPanel);
        editErrors->setFrameShadow(QFrame::Sunken);
        editErrors->setTabChangesFocus(true);
        editErrors->setUndoRedoEnabled(false);
        editErrors->setReadOnly(true);

        horizontalLayout->addWidget(editErrors);

        buttonSave = new QToolButton(layoutWidget);
        buttonSave->setObjectName(QStringLiteral("buttonSave"));
        buttonSave->setEnabled(false);
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/save_table"), QSize(), QIcon::Normal, QIcon::Off);
        buttonSave->setIcon(icon);
        buttonSave->setPopupMode(QToolButton::InstantPopup);

        horizontalLayout->addWidget(buttonSave);


        verticalLayout->addLayout(horizontalLayout);

        splitter->addWidget(layoutWidget);

        verticalLayout_2->addWidget(splitter);

        QWidget::setTabOrder(editEditor, tableResult);
        QWidget::setTabOrder(tableResult, editErrors);
        QWidget::setTabOrder(editErrors, buttonSave);

        retranslateUi(SqlExecutionArea);
        QObject::connect(actionExportCsv, SIGNAL(triggered()), SqlExecutionArea, SLOT(saveAsCsv()));
        QObject::connect(actionSaveAsView, SIGNAL(triggered()), SqlExecutionArea, SLOT(saveAsView()));

        QMetaObject::connectSlotsByName(SqlExecutionArea);
    } // setupUi

    void retranslateUi(QWidget *SqlExecutionArea)
    {
        SqlExecutionArea->setWindowTitle(QApplication::translate("SqlExecutionArea", "Form", 0));
        actionExportCsv->setText(QApplication::translate("SqlExecutionArea", "Export to &CSV", 0));
        actionSaveAsView->setText(QApplication::translate("SqlExecutionArea", "Save as &view", 0));
#ifndef QT_NO_TOOLTIP
        actionSaveAsView->setToolTip(QApplication::translate("SqlExecutionArea", "Save as view", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        editErrors->setToolTip(QApplication::translate("SqlExecutionArea", "Results of the last executed statements", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        editErrors->setWhatsThis(QApplication::translate("SqlExecutionArea", "This field shows the results and status codes of the last executed statements.", 0));
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class SqlExecutionArea: public Ui_SqlExecutionArea {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SQLEXECUTIONAREA_H
