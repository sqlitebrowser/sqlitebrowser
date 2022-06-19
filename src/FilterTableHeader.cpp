#include "FilterTableHeader.h"
#include "FilterLineEdit.h"

#include <QApplication>
#include <QTableView>
#include <QScrollBar>

FilterTableHeader::FilterTableHeader(QTableView* parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    // Activate the click signals to allow sorting
    setSectionsClickable(true);

    // But use our own indicators allowing multi-column sorting
    setSortIndicatorShown(false);

    // Make sure to not automatically resize the columns according to the contents
    setSectionResizeMode(QHeaderView::Interactive);

    // Highlight column headers of selected cells to emulate spreadsheet behaviour
    setHighlightSections(true);

    // Do some connects: Basically just resize and reposition the input widgets whenever anything changes
    connect(this, &FilterTableHeader::sectionResized, this, &FilterTableHeader::adjustPositions);
    connect(this, &FilterTableHeader::sectionClicked, this, &FilterTableHeader::adjustPositions);
    connect(parent->horizontalScrollBar(), &QScrollBar::valueChanged, this, &FilterTableHeader::adjustPositions);
    connect(parent->verticalScrollBar(), &QScrollBar::valueChanged, this, &FilterTableHeader::adjustPositions);

    // Set custom context menu handling
    setContextMenuPolicy(Qt::CustomContextMenu);
}

void FilterTableHeader::generateFilters(size_t number, size_t number_of_hidden_filters)
{
    // Delete all the current filter widgets
    qDeleteAll(filterWidgets);
    filterWidgets.clear();

    // And generate a bunch of new ones
    for(size_t i=0;i < number; ++i)
    {
        FilterLineEdit* l = new FilterLineEdit(this, &filterWidgets, i);
        l->setVisible(i >= number_of_hidden_filters);
        connect(l, &FilterLineEdit::delayedTextChanged, this, &FilterTableHeader::inputChanged);
        connect(l, &FilterLineEdit::filterFocused, this, [this](){emit filterFocused();});
        connect(l, &FilterLineEdit::addFilterAsCondFormat, this, &FilterTableHeader::addFilterAsCondFormat);
        connect(l, &FilterLineEdit::clearAllCondFormats, this, &FilterTableHeader::clearAllCondFormats);
        connect(l, &FilterLineEdit::editCondFormats, this, &FilterTableHeader::editCondFormats);
        filterWidgets.push_back(l);
    }

    // Position them correctly
    updateGeometries();
}

QSize FilterTableHeader::sizeHint() const
{
    // For the size hint just take the value of the standard implementation and add the height of a input widget to it if necessary
    QSize s = QHeaderView::sizeHint();
    if(filterWidgets.size())
        s.setHeight(s.height() + filterWidgets.at(0)->sizeHint().height() + 4); // The 4 adds just adds some extra space
    return s;
}

void FilterTableHeader::updateGeometries()
{
    // If there are any input widgets add a viewport margin to the header to generate some empty space for them which is not affected by scrolling
    if(filterWidgets.size())
        setViewportMargins(0, 0, 0, filterWidgets.at(0)->sizeHint().height());
    else
        setViewportMargins(0, 0, 0, 0);

    // Now just call the parent implementation and reposition the input widgets
    QHeaderView::updateGeometries();
    adjustPositions();
}

void FilterTableHeader::adjustPositions()
{
    // The two adds some extra space between the header label and the input widget
    const int y = QHeaderView::sizeHint().height() + 2;
    // Loop through all widgets
    for(int i=0;i < static_cast<int>(filterWidgets.size()); ++i)
    {
        // Get the current widget, move it and resize it
        QWidget* w = filterWidgets.at(static_cast<size_t>(i));
        if (QApplication::layoutDirection() == Qt::RightToLeft)
            w->move(width() - (sectionPosition(i) + sectionSize(i) - offset()), y);
        else
            w->move(sectionPosition(i) - offset(), y);
        w->resize(sectionSize(i), w->sizeHint().height());
    }
}

void FilterTableHeader::inputChanged(const QString& new_value)
{
    // Just get the column number and the new value and send them to anybody interested in filter changes
    emit filterChanged(sender()->property("column").toUInt(), new_value);
}

void FilterTableHeader::addFilterAsCondFormat(const QString& filter)
{
    // Just get the column number and the new value and send them to anybody interested in new conditional formatting
    emit addCondFormat(sender()->property("column").toUInt(), filter);
}

void FilterTableHeader::clearAllCondFormats()
{
    // Just get the column number and send it to anybody responsible or interested in clearing conditional formatting
    emit allCondFormatsCleared(sender()->property("column").toUInt());
}

void FilterTableHeader::editCondFormats()
{
    // Just get the column number and the new value and send them to anybody interested in editing conditional formatting
    emit condFormatsEdited(sender()->property("column").toUInt());
}

void FilterTableHeader::clearFilters()
{
    for(FilterLineEdit* filterLineEdit : filterWidgets)
        filterLineEdit->clear();
}

void FilterTableHeader::setFilter(size_t column, const QString& value)
{
    if(column < filterWidgets.size())
        filterWidgets.at(column)->setText(value);
}

QString FilterTableHeader::filterValue(size_t column) const
{
    return filterWidgets[column]->text();
}
