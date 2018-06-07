#include "FilterLineEdit.h"
#include "Settings.h"

#include <QTimer>
#include <QKeyEvent>

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
                    "x~y\tRange: values between x and y"));

    // Immediately emit the delayed filter value changed signal if the user presses the enter or the return key or
    // the line edit widget loses focus
    connect(this, SIGNAL(editingFinished()), this, SLOT(delayedSignalTimerTriggered()));
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
