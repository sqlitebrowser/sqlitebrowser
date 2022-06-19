#include "FilterLineEdit.h"
#include "Settings.h"

#include <QTimer>
#include <QKeyEvent>
#include <QMenu>
#include <QWhatsThis>

FilterLineEdit::FilterLineEdit(QWidget* parent, std::vector<FilterLineEdit*>* filters, size_t columnnum) :
    QLineEdit(parent),
    filterList(filters),
    columnNumber(columnnum),
    conditional_format(true)
{
    setPlaceholderText(tr("Filter"));
    setClearButtonEnabled(true);
    setProperty("column", static_cast<int>(columnnum));            // Store the column number for later use

    // Introduce a timer for delaying the signal triggered whenever the user changes the filter value.
    // The idea here is that the textChanged() event isn't connected to the update filter slot directly anymore
    // but instead there this timer mechanism in between: whenever the user changes the filter the delay timer
    // is (re)started. As soon as the user stops typing the timer has a chance to trigger and call the
    // delayedSignalTimerTriggered() method which then stops the timer and emits the delayed signal.
    delaySignalTimer = new QTimer(this);
    delaySignalTimer->setInterval(Settings::getValue("databrowser", "filter_delay").toInt());  // This is the milliseconds of not-typing we want to wait before triggering
    connect(this, &FilterLineEdit::textChanged, delaySignalTimer, static_cast<void (QTimer::*)()>(&QTimer::start));
    connect(delaySignalTimer, &QTimer::timeout, this, &FilterLineEdit::delayedSignalTimerTriggered);

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
    connect(this, &FilterLineEdit::editingFinished, this, &FilterLineEdit::delayedSignalTimerTriggered);

    // Prepare for adding the What's This information and filter helper actions to the context menu
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &FilterLineEdit::customContextMenuRequested, this, &FilterLineEdit::showContextMenu);
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
        if(filterList && columnNumber < filterList->size() - 1)
        {
            filterList->at(columnNumber + 1)->setFocus();
            event->accept();
        }
    } else if(event->key() == Qt::Key_Backtab) {
        if(filterList && columnNumber > 0)
        {
            filterList->at(columnNumber - 1)->setFocus();
            event->accept();
        }
    }
}

void FilterLineEdit::focusInEvent(QFocusEvent* event)
{
    QLineEdit::focusInEvent(event);
    emit filterFocused();
}

void FilterLineEdit::clear()
{
    // When programmatically clearing the line edit's value make sure the effects are applied immediately, i.e.
    // bypass the delayed signal timer
    QLineEdit::clear();
    delayedSignalTimerTriggered();
}

void FilterLineEdit::setText(const QString& text)
{
    // When programmatically setting the line edit's value make sure the effects are applied immediately, i.e.
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

    QMenu* filterMenu = editContextMenu->addMenu(tr("Set Filter Expression"));

    QAction* whatsThisAction = new QAction(QIcon(":/icons/whatis"), tr("What's This?"), editContextMenu);
    connect(whatsThisAction, &QAction::triggered, this, [&]() {
            QWhatsThis::showText(pos, whatsThis(), this);
        });

    QAction* isNullAction = new QAction(tr("Is NULL"), editContextMenu);
    connect(isNullAction, &QAction::triggered, this, [&]() {
            setText("=NULL");
        });

    QAction* isNotNullAction = new QAction(tr("Is not NULL"), editContextMenu);
    connect(isNotNullAction, &QAction::triggered, this, [&]() {
            setText("<>NULL");
        });

    QAction* isEmptyAction = new QAction(tr("Is empty"), editContextMenu);
    connect(isEmptyAction, &QAction::triggered, this, [&]() {
            setText("=''");
        });

    QAction* isNotEmptyAction = new QAction(tr("Is not empty"), editContextMenu);
    connect(isNotEmptyAction, &QAction::triggered, this, [&]() {
            setText("<>''");
        });
    // Simplify this if we ever support a NOT LIKE filter
    QAction* notContainingAction = new QAction(tr("Not containing..."), editContextMenu);
    connect(notContainingAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("/^((?!"), QString(").)*$/"));
        });
    QAction* equalToAction = new QAction(tr("Equal to..."), editContextMenu);
    connect(equalToAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("="));
        });
    QAction* notEqualToAction = new QAction(tr("Not equal to..."), editContextMenu);
    connect(notEqualToAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("<>"));
        });
    QAction* greaterThanAction = new QAction(tr("Greater than..."), editContextMenu);
    connect(greaterThanAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString (">"));
        });
    QAction* lessThanAction = new QAction(tr("Less than..."), editContextMenu);
    connect(lessThanAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("<"));
        });
    QAction* greaterEqualAction = new QAction(tr("Greater or equal..."), editContextMenu);
    connect(greaterEqualAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString (">="));
        });
    QAction* lessEqualAction = new QAction(tr("Less or equal..."), editContextMenu);
    connect(lessEqualAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("<="));
        });
    QAction* inRangeAction = new QAction(tr("In range..."), editContextMenu);
    connect(inRangeAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("?~"));
        });

    QAction* regexpAction = new QAction(tr("Regular expression..."), editContextMenu);
    connect(regexpAction, &QAction::triggered, this, [&]() {
            setFilterHelper(QString ("/"), QString ("/"));
        });


    if(conditional_format)
    {
        QAction* conditionalFormatAction;
        if (text().isEmpty()) {
            conditionalFormatAction = new QAction(QIcon(":/icons/clear_cond_formats"), tr("Clear All Conditional Formats"), editContextMenu);
            connect(conditionalFormatAction, &QAction::triggered, this, [&]() {
                    emit clearAllCondFormats();
            });
        } else {
            conditionalFormatAction = new QAction(QIcon(":/icons/add_cond_format"), tr("Use for Conditional Format"), editContextMenu);
            connect(conditionalFormatAction, &QAction::triggered, this, [&]() {
                    emit addFilterAsCondFormat(text());
            });
        }
        QAction* editCondFormatsAction = new QAction(QIcon(":/icons/edit_cond_formats"), tr("Edit Conditional Formats..."), editContextMenu);
        connect(editCondFormatsAction, &QAction::triggered, this, [&]() {
            emit editCondFormats();
        });
        editContextMenu->addSeparator();

        editContextMenu->addAction(conditionalFormatAction);
        editContextMenu->addAction(editCondFormatsAction);
    }


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
