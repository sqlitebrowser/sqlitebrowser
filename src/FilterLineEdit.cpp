#include "FilterLineEdit.h"
#include "Settings.h"

#include <QTimer>
#include <QKeyEvent>
#include <QMenu>
#include <QWhatsThis>

FilterLineEdit::FilterLineEdit(QWidget* parent, QList<FilterLineEdit*>* filters, int columnnum) : QLineEdit(parent), filterList(filters), columnNumber(columnnum)
{
    setPlaceholderText(tr("Filter"));
    setClearButtonEnabled(true);
    setProperty("column", columnnum);            // Store the column number for later use

    // Introduce a timer for delaying the signal triggered whenever the user changes the filter value.
    // The idea here is that the textChanged() event isn't connected to the update filter slot directly anymore
    // but instead there this timer mechanism in between: whenever the user changes the filter the delay timer
    // is (re)started. As soon as the user stops typing the timer has a chance to trigger and call the
    // delayedSignalTimerTriggered() method which then stops the timer and emits the delayed signal.
    delaySignalTimer = new QTimer(this);
    delaySignalTimer->setInterval(Settings::getValue("databrowser", "filter_delay").toInt());  // This is the milliseconds of not-typing we want to wait before triggering
    connect(this, SIGNAL(textChanged(QString)), delaySignalTimer, SLOT(start()));
    connect(delaySignalTimer, SIGNAL(timeout()), this, SLOT(delayedSignalTimerTriggered()));

    setWhatsThis(tr("These input fields allow you to perform quick filters in the currently selected table.\n"
                    "By default, the rows containing the input text are filtered out.\n"
                    "The following operators are also supported:\n"
                    "%\tWildcard\n"
                    ">\tGreater than\n"
                    "<\tLess than\n"
                    ">=\tEqual to or greater\n"
                    "<=\tEqual to or less\n"
                    "=\tEqual to: exact match\n"
                    "<>\tUnequal: exact inverse match\n"
                    "x~y\tRange: values between x and y\n"
                    "/regexp/\tValues matching the regular expression"));

    // Immediately emit the delayed filter value changed signal if the user presses the enter or the return key or
    // the line edit widget loses focus
    connect(this, SIGNAL(editingFinished()), this, SLOT(delayedSignalTimerTriggered()));

    // Prepare for adding the What's This information and filter helper actions to the context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));
}

void FilterLineEdit::delayedSignalTimerTriggered()
{
    // Stop the timer first to avoid triggering in intervals
    delaySignalTimer->stop();

    // Only emit text changed signal if the text has actually changed in comparison to the last emitted signal. This is necessary
    // because this method is also called whenever the line edit loses focus and not only when its text has definitely been changed.
    if(text() != lastValue)
    {
        // Emit the delayed signal using the current value
        emit delayedTextChanged(text());

        // Remember this value for the next time
        lastValue = text();
    }
}

void FilterLineEdit::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Tab)
    {
        if(columnNumber < filterList->size() - 1)
        {
            filterList->at(columnNumber + 1)->setFocus();
            event->accept();
        }
    } else if(event->key() == Qt::Key_Backtab) {
        if(columnNumber > 0)
        {
            filterList->at(columnNumber - 1)->setFocus();
            event->accept();
        }
    }
}

void FilterLineEdit::clear()
{
    // When programatically clearing the line edit's value make sure the effects are applied immediately, i.e.
    // bypass the delayed signal timer
    QLineEdit::clear();
    delayedSignalTimerTriggered();
}

void FilterLineEdit::setText(const QString& text)
{
    // When programatically setting the line edit's value make sure the effects are applied immediately, i.e.
    // bypass the delayed signal timer
    QLineEdit::setText(text);
    delayedSignalTimerTriggered();
}

void FilterLineEdit::setFilterHelper(const QString& filterOperator, const QString& operatorSuffix)
{
    setText(filterOperator + "?" + operatorSuffix);
    // Select the value for easy editing of the expression
    setSelection(filterOperator.length(), 1);
}

void FilterLineEdit::showContextMenu(const QPoint &pos)
{

    // This has to be created here, otherwise the set of enabled options would not update accordingly.
    QMenu* editContextMenu = createStandardContextMenu();
    editContextMenu->addSeparator();
    QString conditionalFormatLabel = text().isEmpty() ? tr("Clear All Conditional Formats") : tr("Use for Conditional Format");
    QAction* conditionalFormatAction = new QAction(conditionalFormatLabel, editContextMenu);
    connect(conditionalFormatAction, &QAction::triggered, [&]() {
        if (text().isEmpty())
            emit clearAllCondFormats();
        else
            emit addFilterAsCondFormat(text());
    });
    editContextMenu->addSeparator();

    QMenu* filterMenu = editContextMenu->addMenu(tr("Set Filter Expression"));

    QAction* whatsThisAction = new QAction(QIcon(":/icons/whatis"), tr("What's This?"), editContextMenu);
    connect(whatsThisAction, &QAction::triggered, [&]() {
            QWhatsThis::showText(pos, whatsThis(), this);
        });

    QAction* isNullAction = new QAction(tr("Is NULL"), editContextMenu);
    connect(isNullAction, &QAction::triggered, [&]() {
            setText("=NULL");
        });

    QAction* isNotNullAction = new QAction(tr("Is not NULL"), editContextMenu);
    connect(isNotNullAction, &QAction::triggered, [&]() {
            setText("<>NULL");
        });

    QAction* isEmptyAction = new QAction(tr("Is empty"), editContextMenu);
    connect(isEmptyAction, &QAction::triggered, [&]() {
            setText("=''");
        });

    QAction* isNotEmptyAction = new QAction(tr("Is not empty"), editContextMenu);
    connect(isNotEmptyAction, &QAction::triggered, [&]() {
            setText("<>''");
        });
    // Simplify this if we ever support a NOT LIKE filter
    QAction* notContainingAction = new QAction(tr("Not containing..."), editContextMenu);
    connect(notContainingAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("/^((?!"), QString(").)*$/"));
        });
    QAction* equalToAction = new QAction(tr("Equal to..."), editContextMenu);
    connect(equalToAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("="));
        });
    QAction* notEqualToAction = new QAction(tr("Not equal to..."), editContextMenu);
    connect(notEqualToAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("<>"));
        });
    QAction* greaterThanAction = new QAction(tr("Greater than..."), editContextMenu);
    connect(greaterThanAction, &QAction::triggered, [&]() {
            setFilterHelper(QString (">"));
        });
    QAction* lessThanAction = new QAction(tr("Less than..."), editContextMenu);
    connect(lessThanAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("<"));
        });
    QAction* greaterEqualAction = new QAction(tr("Greater or equal..."), editContextMenu);
    connect(greaterEqualAction, &QAction::triggered, [&]() {
            setFilterHelper(QString (">="));
        });
    QAction* lessEqualAction = new QAction(tr("Less or equal..."), editContextMenu);
    connect(lessEqualAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("<="));
        });
    QAction* inRangeAction = new QAction(tr("In range..."), editContextMenu);
    connect(inRangeAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("?~"));
        });

    QAction* regexpAction = new QAction(tr("Regular expression..."), editContextMenu);
    connect(regexpAction, &QAction::triggered, [&]() {
            setFilterHelper(QString ("/"), QString ("/"));
        });

    editContextMenu->addAction(conditionalFormatAction);

    filterMenu->addAction(whatsThisAction);
    filterMenu->addSeparator();
    filterMenu->addAction(isNullAction);
    filterMenu->addAction(isNotNullAction);
    filterMenu->addAction(isEmptyAction);
    filterMenu->addAction(isNotEmptyAction);
    filterMenu->addSeparator();
    filterMenu->addAction(notContainingAction);
    filterMenu->addAction(equalToAction);
    filterMenu->addAction(notEqualToAction);
    filterMenu->addAction(greaterThanAction);
    filterMenu->addAction(lessThanAction);
    filterMenu->addAction(greaterEqualAction);
    filterMenu->addAction(lessEqualAction);
    filterMenu->addAction(inRangeAction);
    filterMenu->addAction(regexpAction);
    editContextMenu->exec(mapToGlobal(pos));
}
