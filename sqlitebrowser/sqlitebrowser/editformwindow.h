#ifndef EDITFORMWINDOW_H
#define EDITFORMWINDOW_H

#include <qvariant.h>


#include <Qt3Support/Q3MainWindow>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3ToolBar>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_editFormWindow
{
public:
    QAction *fileNewAction;
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *filePrintAction;
    QAction *fileExitAction;
    QAction *editUndoAction;
    QAction *editRedoAction;
    QAction *editCutAction;
    QAction *editCopyAction;
    QAction *editPasteAction;
    QAction *editFindAction;
    QAction *helpContentsAction;
    QAction *helpIndexAction;
    QAction *helpAboutAction;
    QWidget *widget;
    QVBoxLayout *vboxLayout;
    Q3ToolBar *toolBar;
    QMenuBar *MenuBar;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    void setupUi(Q3MainWindow *editFormWindow)
    {
        if (editFormWindow->objectName().isEmpty())
            editFormWindow->setObjectName(QString::fromUtf8("editFormWindow"));
        editFormWindow->resize(331, 435);
        editFormWindow->setDockWindowsMovable(true);
        fileNewAction = new QAction(editFormWindow);
        fileNewAction->setObjectName(QString::fromUtf8("fileNewAction"));
        fileNewAction->setName("fileNewAction");
        const QIcon icon = QIcon(qPixmapFromMimeSource("filenew"));
        fileNewAction->setIcon(icon);
        fileOpenAction = new QAction(editFormWindow);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileOpenAction->setName("fileOpenAction");
        const QIcon icon1 = QIcon(qPixmapFromMimeSource("fileopen"));
        fileOpenAction->setIcon(icon1);
        fileSaveAction = new QAction(editFormWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAction->setName("fileSaveAction");
        const QIcon icon2 = QIcon(qPixmapFromMimeSource("filesave"));
        fileSaveAction->setIcon(icon2);
        fileSaveAsAction = new QAction(editFormWindow);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        fileSaveAsAction->setName("fileSaveAsAction");
        filePrintAction = new QAction(editFormWindow);
        filePrintAction->setObjectName(QString::fromUtf8("filePrintAction"));
        filePrintAction->setName("filePrintAction");
        const QIcon icon3 = QIcon(qPixmapFromMimeSource("print"));
        filePrintAction->setIcon(icon3);
        fileExitAction = new QAction(editFormWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        fileExitAction->setName("fileExitAction");
        editUndoAction = new QAction(editFormWindow);
        editUndoAction->setObjectName(QString::fromUtf8("editUndoAction"));
        editUndoAction->setName("editUndoAction");
        const QIcon icon4 = QIcon(qPixmapFromMimeSource("undo"));
        editUndoAction->setIcon(icon4);
        editRedoAction = new QAction(editFormWindow);
        editRedoAction->setObjectName(QString::fromUtf8("editRedoAction"));
        editRedoAction->setName("editRedoAction");
        const QIcon icon5 = QIcon(qPixmapFromMimeSource("redo"));
        editRedoAction->setIcon(icon5);
        editCutAction = new QAction(editFormWindow);
        editCutAction->setObjectName(QString::fromUtf8("editCutAction"));
        editCutAction->setName("editCutAction");
        const QIcon icon6 = QIcon(qPixmapFromMimeSource("editcut"));
        editCutAction->setIcon(icon6);
        editCopyAction = new QAction(editFormWindow);
        editCopyAction->setObjectName(QString::fromUtf8("editCopyAction"));
        editCopyAction->setName("editCopyAction");
        const QIcon icon7 = QIcon(qPixmapFromMimeSource("editcopy"));
        editCopyAction->setIcon(icon7);
        editPasteAction = new QAction(editFormWindow);
        editPasteAction->setObjectName(QString::fromUtf8("editPasteAction"));
        editPasteAction->setName("editPasteAction");
        const QIcon icon8 = QIcon(qPixmapFromMimeSource("editpaste"));
        editPasteAction->setIcon(icon8);
        editFindAction = new QAction(editFormWindow);
        editFindAction->setObjectName(QString::fromUtf8("editFindAction"));
        editFindAction->setName("editFindAction");
        const QIcon icon9 = QIcon(qPixmapFromMimeSource("searchfind"));
        editFindAction->setIcon(icon9);
        helpContentsAction = new QAction(editFormWindow);
        helpContentsAction->setObjectName(QString::fromUtf8("helpContentsAction"));
        helpContentsAction->setName("helpContentsAction");
        helpIndexAction = new QAction(editFormWindow);
        helpIndexAction->setObjectName(QString::fromUtf8("helpIndexAction"));
        helpIndexAction->setName("helpIndexAction");
        helpAboutAction = new QAction(editFormWindow);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        helpAboutAction->setName("helpAboutAction");
        widget = new QWidget(editFormWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        vboxLayout = new QVBoxLayout(widget);
        vboxLayout->setSpacing(6);
        vboxLayout->setMargin(11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        vboxLayout->setContentsMargins(0, 0, 0, 0);
        editFormWindow->setCentralWidget(widget);
        toolBar = new Q3ToolBar(editFormWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MenuBar = new QMenuBar(editFormWindow);
        MenuBar->setObjectName(QString::fromUtf8("MenuBar"));
        fileMenu = new QMenu(MenuBar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        editMenu = new QMenu(MenuBar);
        editMenu->setObjectName(QString::fromUtf8("editMenu"));
        helpMenu = new QMenu(MenuBar);
        helpMenu->setObjectName(QString::fromUtf8("helpMenu"));

        toolBar->addAction(fileOpenAction);
        toolBar->addAction(fileSaveAction);
        toolBar->addAction(filePrintAction);
        toolBar->addSeparator();
        toolBar->addAction(editCutAction);
        toolBar->addAction(editCopyAction);
        toolBar->addAction(editPasteAction);
        toolBar->addAction(helpContentsAction);
        MenuBar->addAction(fileMenu->menuAction());
        MenuBar->addAction(editMenu->menuAction());
        MenuBar->addAction(helpMenu->menuAction());
        fileMenu->addAction(fileOpenAction);
        fileMenu->addAction(fileSaveAction);
        fileMenu->addSeparator();
        fileMenu->addAction(filePrintAction);
        fileMenu->addSeparator();
        editMenu->addAction(editUndoAction);
        editMenu->addAction(editRedoAction);
        editMenu->addSeparator();
        editMenu->addAction(editCutAction);
        editMenu->addAction(editCopyAction);
        editMenu->addAction(editPasteAction);
        helpMenu->addAction(helpAboutAction);

        retranslateUi(editFormWindow);

        QMetaObject::connectSlotsByName(editFormWindow);
    } // setupUi

    void retranslateUi(Q3MainWindow *editFormWindow)
    {
        editFormWindow->setWindowTitle(QApplication::translate("editFormWindow", "Edit database cell", 0, QApplication::UnicodeUTF8));
        fileNewAction->setIconText(QApplication::translate("editFormWindow", "New", 0, QApplication::UnicodeUTF8));
        fileNewAction->setText(QApplication::translate("editFormWindow", "&New", 0, QApplication::UnicodeUTF8));
        fileNewAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("editFormWindow", "&Import", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setText(QApplication::translate("editFormWindow", "&Import", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+I", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("editFormWindow", "&Export", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setText(QApplication::translate("editFormWindow", "&Export", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+E", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setIconText(QApplication::translate("editFormWindow", "Save As", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setText(QApplication::translate("editFormWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setShortcut(QString());
        filePrintAction->setIconText(QApplication::translate("editFormWindow", "Print", 0, QApplication::UnicodeUTF8));
        filePrintAction->setText(QApplication::translate("editFormWindow", "&Print...", 0, QApplication::UnicodeUTF8));
        filePrintAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+P", 0, QApplication::UnicodeUTF8));
        fileExitAction->setIconText(QApplication::translate("editFormWindow", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setText(QApplication::translate("editFormWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QString());
        editUndoAction->setIconText(QApplication::translate("editFormWindow", "Undo", 0, QApplication::UnicodeUTF8));
        editUndoAction->setText(QApplication::translate("editFormWindow", "&Undo", 0, QApplication::UnicodeUTF8));
        editUndoAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        editRedoAction->setIconText(QApplication::translate("editFormWindow", "Redo", 0, QApplication::UnicodeUTF8));
        editRedoAction->setText(QApplication::translate("editFormWindow", "&Redo", 0, QApplication::UnicodeUTF8));
        editRedoAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        editCutAction->setIconText(QApplication::translate("editFormWindow", "Cut", 0, QApplication::UnicodeUTF8));
        editCutAction->setText(QApplication::translate("editFormWindow", "&Cut", 0, QApplication::UnicodeUTF8));
        editCutAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+X", 0, QApplication::UnicodeUTF8));
        editCopyAction->setIconText(QApplication::translate("editFormWindow", "Copy", 0, QApplication::UnicodeUTF8));
        editCopyAction->setText(QApplication::translate("editFormWindow", "C&opy", 0, QApplication::UnicodeUTF8));
        editCopyAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+C", 0, QApplication::UnicodeUTF8));
        editPasteAction->setIconText(QApplication::translate("editFormWindow", "Paste", 0, QApplication::UnicodeUTF8));
        editPasteAction->setText(QApplication::translate("editFormWindow", "&Paste", 0, QApplication::UnicodeUTF8));
        editPasteAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+V", 0, QApplication::UnicodeUTF8));
        editFindAction->setIconText(QApplication::translate("editFormWindow", "Find", 0, QApplication::UnicodeUTF8));
        editFindAction->setText(QApplication::translate("editFormWindow", "&Find...", 0, QApplication::UnicodeUTF8));
        editFindAction->setShortcut(QApplication::translate("editFormWindow", "Ctrl+F", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setIconText(QApplication::translate("editFormWindow", "Contents", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setText(QApplication::translate("editFormWindow", "&Contents...", 0, QApplication::UnicodeUTF8));
        helpContentsAction->setShortcut(QString());
        helpIndexAction->setIconText(QApplication::translate("editFormWindow", "Index", 0, QApplication::UnicodeUTF8));
        helpIndexAction->setText(QApplication::translate("editFormWindow", "&Index...", 0, QApplication::UnicodeUTF8));
        helpIndexAction->setShortcut(QString());
        helpAboutAction->setIconText(QApplication::translate("editFormWindow", "&What is", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setText(QApplication::translate("editFormWindow", "&What is", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        toolBar->setLabel(QApplication::translate("editFormWindow", "Tools", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("editFormWindow", "&File", 0, QApplication::UnicodeUTF8));
        editMenu->setTitle(QApplication::translate("editFormWindow", "&Edit", 0, QApplication::UnicodeUTF8));
        helpMenu->setTitle(QApplication::translate("editFormWindow", "&Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class editFormWindow: public Ui_editFormWindow {};
} // namespace Ui

QT_END_NAMESPACE

class editFormWindow : public Q3MainWindow, public Ui::editFormWindow
{
    Q_OBJECT

public:
    editFormWindow(QWidget* parent = 0, const char* name = 0, Qt::WindowFlags fl = Qt::WType_TopLevel);
    ~editFormWindow();

public slots:
    virtual void fileNew();
    virtual void fileOpen();
    virtual void fileSave();
    virtual void fileSaveAs();
    virtual void filePrint();
    virtual void fileExit();
    virtual void editUndo();
    virtual void editRedo();
    virtual void editCut();
    virtual void editCopy();
    virtual void editPaste();
    virtual void editFind();
    virtual void helpIndex();
    virtual void helpContents();
    virtual void helpAbout();

protected slots:
    virtual void languageChange();

};

#endif // EDITFORMWINDOW_H
