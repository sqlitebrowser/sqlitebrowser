#ifndef EDITFORM_H
#define EDITFORM_H

#include <qvariant.h>


#include <Qt3Support/Q3Frame>
#include <Qt3Support/Q3MimeSourceFactory>
#include <Qt3Support/Q3TextEdit>
#include <Qt3Support/Q3WidgetStack>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QFile>
#include <QtGui/QImage>
#include <QtGui/QStackedWidget>
#include <stdlib.h>

QT_BEGIN_NAMESPACE

class Ui_editForm
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QToolButton *importButton;
    QToolButton *exportButton;
    QSpacerItem *spacer11_2;
    QToolButton *clearButton;
    QStackedWidget *editWidgetStack;
    QWidget *WStackPage;
    QGridLayout *gridLayout;
    Q3TextEdit *textEditor;
    QWidget *WStackPage1;
    QGridLayout *gridLayout1;
    QLabel *editPixmap;
    Q3Frame *frame9;
    QVBoxLayout *vboxLayout1;
    QLabel *currentTypeLabel;
    QLabel *currentDataInfo;
    QHBoxLayout *hboxLayout1;
    QSpacerItem *spacer11;
    QPushButton *closeButton;
    QPushButton *saveChangesButton;

    void setupUi(QDialog *editForm)
    {
        if (editForm->objectName().isEmpty())
            editForm->setObjectName(QString::fromUtf8("editForm"));
        editForm->resize(366, 431);
        editForm->setModal(true);
        vboxLayout = new QVBoxLayout(editForm);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(11, 11, 11, 11);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        importButton = new QToolButton(editForm);
        importButton->setObjectName(QString::fromUtf8("importButton"));
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(1), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(importButton->sizePolicy().hasHeightForWidth());
        importButton->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(importButton);

        exportButton = new QToolButton(editForm);
        exportButton->setObjectName(QString::fromUtf8("exportButton"));
        sizePolicy.setHeightForWidth(exportButton->sizePolicy().hasHeightForWidth());
        exportButton->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(exportButton);

        spacer11_2 = new QSpacerItem(81, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacer11_2);

        clearButton = new QToolButton(editForm);
        clearButton->setObjectName(QString::fromUtf8("clearButton"));
        sizePolicy.setHeightForWidth(clearButton->sizePolicy().hasHeightForWidth());
        clearButton->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(clearButton);


        vboxLayout->addLayout(hboxLayout);

        editWidgetStack = new QStackedWidget(editForm);
        editWidgetStack->setObjectName(QString::fromUtf8("editWidgetStack"));
        WStackPage = new QWidget(editWidgetStack);
        WStackPage->setObjectName(QString::fromUtf8("WStackPage"));
        gridLayout = new QGridLayout(WStackPage);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textEditor = new Q3TextEdit(WStackPage);
        textEditor->setObjectName(QString::fromUtf8("textEditor"));
        textEditor->setTextFormat(Qt::PlainText);

        gridLayout->addWidget(textEditor, 0, 0, 1, 1);

        editWidgetStack->addWidget(WStackPage);
        WStackPage1 = new QWidget(editWidgetStack);
        WStackPage1->setObjectName(QString::fromUtf8("WStackPage1"));
        gridLayout1 = new QGridLayout(WStackPage1);
        gridLayout1->setSpacing(6);
        gridLayout1->setContentsMargins(11, 11, 11, 11);
        gridLayout1->setObjectName(QString::fromUtf8("gridLayout1"));
        gridLayout1->setContentsMargins(0, 0, 0, 0);
        editPixmap = new QLabel(WStackPage1);
        editPixmap->setObjectName(QString::fromUtf8("editPixmap"));
        editPixmap->setMaximumSize(QSize(320, 240));
        editPixmap->setScaledContents(true);
        editPixmap->setAlignment(Qt::AlignCenter);
        editPixmap->setWordWrap(false);

        gridLayout1->addWidget(editPixmap, 0, 0, 1, 1);

        editWidgetStack->addWidget(WStackPage1);

        vboxLayout->addWidget(editWidgetStack);

        frame9 = new Q3Frame(editForm);
        frame9->setObjectName(QString::fromUtf8("frame9"));
        frame9->setFrameShape(QFrame::Box);
        frame9->setFrameShadow(QFrame::Sunken);
        vboxLayout1 = new QVBoxLayout(frame9);
        vboxLayout1->setSpacing(6);
        vboxLayout1->setMargin(11);
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        currentTypeLabel = new QLabel(frame9);
        currentTypeLabel->setObjectName(QString::fromUtf8("currentTypeLabel"));
        currentTypeLabel->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        currentTypeLabel->setWordWrap(false);

        vboxLayout1->addWidget(currentTypeLabel);

        currentDataInfo = new QLabel(frame9);
        currentDataInfo->setObjectName(QString::fromUtf8("currentDataInfo"));
        currentDataInfo->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        currentDataInfo->setWordWrap(true);

        vboxLayout1->addWidget(currentDataInfo);


        vboxLayout->addWidget(frame9);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        spacer11 = new QSpacerItem(90, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacer11);

        closeButton = new QPushButton(editForm);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setDefault(true);

        hboxLayout1->addWidget(closeButton);

        saveChangesButton = new QPushButton(editForm);
        saveChangesButton->setObjectName(QString::fromUtf8("saveChangesButton"));
        saveChangesButton->setEnabled(false);
        saveChangesButton->setDefault(false);

        hboxLayout1->addWidget(saveChangesButton);


        vboxLayout->addLayout(hboxLayout1);


        retranslateUi(editForm);
        QObject::connect(importButton, SIGNAL(clicked()), editForm, SLOT(importData()));
        QObject::connect(exportButton, SIGNAL(clicked()), editForm, SLOT(exportData()));
        QObject::connect(saveChangesButton, SIGNAL(clicked()), editForm, SLOT(saveChanges()));
        QObject::connect(clearButton, SIGNAL(clicked()), editForm, SLOT(clearData()));
        QObject::connect(textEditor, SIGNAL(textChanged()), editForm, SLOT(editTextChanged()));
        QObject::connect(closeButton, SIGNAL(clicked()), editForm, SLOT(close()));
        QObject::connect(textEditor, SIGNAL(clicked(int,int)), editForm, SLOT(editorClicked()));

        QMetaObject::connectSlotsByName(editForm);
    } // setupUi

    void retranslateUi(QDialog *editForm)
    {
        editForm->setWindowTitle(QApplication::translate("editForm", "Edit database cell", 0, QApplication::UnicodeUTF8));
        importButton->setText(QApplication::translate("editForm", "Import", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        importButton->setProperty("toolTip", QVariant(QApplication::translate("editForm", "Import text", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        importButton->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "Opens a file dialog used to import text to this database cell.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        exportButton->setText(QApplication::translate("editForm", "Export", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        exportButton->setProperty("toolTip", QVariant(QApplication::translate("editForm", "Export text", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        exportButton->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "Opens a file dialog used to export the contents of this database cell to a text file.", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        clearButton->setText(QApplication::translate("editForm", "Clear", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        clearButton->setProperty("toolTip", QVariant(QApplication::translate("editForm", "Clear cell data", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        clearButton->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "Erases the contents of the cell", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_TOOLTIP
        frame9->setProperty("toolTip", QVariant(QString()));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        frame9->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "This area displays information about the data present in this database cell", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        currentTypeLabel->setText(QApplication::translate("editForm", "Type of data currently in cell: Empty", 0, QApplication::UnicodeUTF8));
        currentDataInfo->setText(QApplication::translate("editForm", "Data information", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("editForm", "Close", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        closeButton->setProperty("toolTip", QVariant(QApplication::translate("editForm", "Close", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        closeButton->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "Close the window without saving changes", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
        saveChangesButton->setText(QApplication::translate("editForm", "Apply Changes", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        saveChangesButton->setProperty("toolTip", QVariant(QApplication::translate("editForm", "Save changes to database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        saveChangesButton->setProperty("whatsThis", QVariant(QApplication::translate("editForm", "Close this window saving changes to the database", 0, QApplication::UnicodeUTF8)));
#endif // QT_NO_WHATSTHIS
    } // retranslateUi

};

namespace Ui {
    class editForm: public Ui_editForm {};
} // namespace Ui

QT_END_NAMESPACE

class editForm : public QDialog, public Ui::editForm
{
    Q_OBJECT

public:
    editForm(QWidget* parent = 0, const char* name = 0, bool modal = false, Qt::WindowFlags fl = 0);
    ~editForm();

    int curCol;
    int curRow;
    QString defaultlocation;

public slots:
    virtual void reset();
    virtual void setModified( bool modifiedstate );
    virtual void enableExport( bool enabled );
    virtual void enableTextEditor( bool enabled );
    virtual void setTextFormat( QString format );
    virtual void setDataType( int type, int size );
    virtual void closeEvent( QCloseEvent * );
    virtual void loadText( QString text, int row, int col );
    virtual void importData();
    virtual void exportData();
    virtual void clearData();
    virtual void saveChanges();
    virtual void editTextChanged();
    virtual void editorClicked();

signals:
    void goingAway();
    void updateRecordText(int, int, QString);
    void updateRecordBinary(int, int, unsigned char *);

protected:
    bool modified;
    int dataType;
    int dataSize;
    int dataDepth;

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

};

#endif // EDITFORM_H
