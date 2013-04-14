#include "FilterTableHeader.h"
#include <QLineEdit>
#include <QTableView>
#include <QScrollBar>

FilterTableHeader::FilterTableHeader(QTableView* parent) :
    QHeaderView(Qt::Horizontal, parent)
{
    // Activate the click signals to allow sorting
    setClickable(true);

    // Do some connects: Basically just resize and reposition the input widgets whenever anything changes
    connect(this, SIGNAL(sectionResized(int,int,int)), this, SLOT(adjustPositions()));
    connect(parent->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustPositions()));
    connect(parent->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(adjustPositions()));
}

void FilterTableHeader::generateFilters(int number)
{
    // Delete all the current filter widgets
    for(int i=0;i<filterWidgets.size();i++)
        delete filterWidgets.at(i);
    filterWidgets.clear();

    // And generate a bunch of new ones
    for(int i=0;i<number;i++)
    {
        QLineEdit* l = new QLineEdit(this);
        l->setPlaceholderText(tr("Filter"));
        l->setProperty("column", i);            // Store the column number for later use
        l->setVisible(i>0);                     // This hides the first input widget which belongs to the hidden rowid column
        connect(l, SIGNAL(textChanged(QString)), this, SLOT(inputChanged(QString)));
        filterWidgets.push_back(l);
    }

    // Position them correctly
    adjustPositions();
}

QSize FilterTableHeader::sizeHint() const
{
    // For the size hint just take the value of the standard implementation and add the height of a input widget to it if necessary
    QSize s = QHeaderView::sizeHint();
    if(filterWidgets.size())
        s.setHeight(s.height() + filterWidgets.at(0)->sizeHint().height() + 5); // The 5 adds just adds some extra space
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
    // Loop through all widgets
    for(int i=0;i<filterWidgets.size();++i)
    {
        // Get the current widget, move it and reisize it
        QWidget* w = filterWidgets.at(i);
        w->move(sectionPosition(i) - offset(), filterWidgets.at(i)->sizeHint().height() + 2);   // The two adds some extra space between the header label and the input widget
        w->resize(sectionSize(i), filterWidgets.at(i)->sizeHint().height());
    }
}

void FilterTableHeader::inputChanged(const QString& new_value)
{
    // Just get the column number and the new value and send them to anybody interested in filter changes
    emit filterChanged(sender()->property("column").toInt(), new_value);
}
