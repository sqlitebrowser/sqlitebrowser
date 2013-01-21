#include "SQLLogDock.h"
#include <QScrollBar>
#include "sqlitedb.h"
#include "SQLiteSyntaxHighlighter.h"

/*
 *  Constructs a sqlLogForm as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  true to construct a modal dialog.
 */
SQLLogDock::SQLLogDock(QWidget* parent)
    : QDockWidget(tr("SQL Log"), parent)
{
    setupUi();
}

void SQLLogDock::setupUi()
{
    contentWidget = new QWidget(this);
    this->setWidget(contentWidget);
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("sqlLogForm"));
    vboxLayout = new QVBoxLayout(contentWidget);
    vboxLayout->setMargin(2);
    vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));

    hboxLayout = new QHBoxLayout();
    hboxLayout->setSpacing(3);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
    textLabel1 = new QLabel(this);
    textLabel1->setObjectName(QString::fromUtf8("textLabel1"));
    textLabel1->setWordWrap(false);

    hboxLayout->addWidget(textLabel1);

    m_comboLogType = new QComboBox(this);
    m_comboLogType->setObjectName(QString::fromUtf8("comboBox3"));

    hboxLayout->addWidget(m_comboLogType);

    spacer10 = new QSpacerItem(150, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    hboxLayout->addItem(spacer10);

    clearButton = new QPushButton(this);
    clearButton->setObjectName(QString::fromUtf8("clearButton"));

    hboxLayout->addWidget(clearButton);

    vboxLayout->addLayout(hboxLayout);

    logStack = new QStackedWidget(this);
    logStack->setObjectName(QString::fromUtf8("logStack"));
    WStackPage = new QWidget(logStack);
    WStackPage->setObjectName(QString::fromUtf8("WStackPage"));
    gridLayout = new QGridLayout(WStackPage);
    gridLayout->setSpacing(3);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    userLogText = new QTextEdit(WStackPage);
    userLogText->setObjectName(QString::fromUtf8("userLogText"));
    userLogText->setReadOnly(true);

    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPixelSize(11);
    userLogText->setFont(font);

    mUserSqliteHighlighter = new SQLiteSyntaxHighlighter(userLogText->document());

    gridLayout->addWidget(userLogText, 0, 0, 1, 1);
    logStack->addWidget(WStackPage);
    WStackPage->setLayout(gridLayout);

    WStackPage1 = new QWidget(logStack);
    WStackPage1->setObjectName(QString::fromUtf8("WStackPage1"));
    vboxLayout1 = new QVBoxLayout(WStackPage1);
    vboxLayout1->setSpacing(3);
    vboxLayout1->setContentsMargins(11, 11, 11, 11);
    vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
    vboxLayout1->setContentsMargins(0, 0, 0, 0);
    appLogText = new QTextEdit(WStackPage1);
    appLogText->setObjectName(QString::fromUtf8("appLogText"));
    appLogText->setReadOnly(true);
    appLogText->setFont(font);

    mAppSqliteHighlighter = new SQLiteSyntaxHighlighter(appLogText->document());

    logStack->addWidget(WStackPage1);
    vboxLayout1->addWidget(appLogText);
    WStackPage1->setLayout(vboxLayout1);

    vboxLayout->addWidget(logStack);
    contentWidget->setLayout(vboxLayout);


    retranslateUi();
    QObject::connect(clearButton, SIGNAL(clicked()), this, SLOT(clearLog()));
    QObject::connect(m_comboLogType, SIGNAL(currentIndexChanged(int)), logStack, SLOT(setCurrentIndex(int)));

    QMetaObject::connectSlotsByName(this);
}

void SQLLogDock::retranslateUi()
{
    this->setWindowTitle(QObject::tr("SQL Log"));
    textLabel1->setText(QObject::tr("Show SQL submitted by:"));
    m_comboLogType->clear();
    m_comboLogType->insertItems(0, QStringList()
     << QObject::tr("User")
     << QObject::tr("Application")
    );
    clearButton->setText(QObject::tr("Clear"));
} // retranslateUi


/*
 *  Destroys the object and frees any allocated resources
 */
SQLLogDock::~SQLLogDock()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SQLLogDock::languageChange()
{
    retranslateUi();
}

void SQLLogDock::closeEvent( QCloseEvent * )
{
        emit goingAway();
}

void SQLLogDock::log( QString & statement, int msgtype)
{
    QScrollBar* sb = 0;
    if (msgtype==kLogMsg_User)
    {
        userLogText->append(statement);
        sb = userLogText->verticalScrollBar();
    } else {
        appLogText->append(statement);
        sb = appLogText->verticalScrollBar();
    }
    sb->setValue(sb->maximum());
}

void SQLLogDock::msgDBDirtyState( bool dirty)
{
    emit dbState(dirty);
}


void SQLLogDock::clearLog()
{
    if (logStack->currentIndex()==kLogMsg_User)
    {
        userLogText->clear();
    } else {
    appLogText->clear();
}
}

