#include "SelectItemsPopup.h"
#include "ui_SelectItemsPopup.h"

#include <QPushButton>

SelectItemsPopup::SelectItemsPopup(const std::vector<std::string>& available, const std::vector<std::string>& selected, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::SelectItemsPopup)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup);

    // Load initial items
    for(const auto& s : available)
    {
        if(std::find(selected.begin(), selected.end(), s) == selected.end())
            new QListWidgetItem(QString::fromStdString(s), ui->listAvailable);
    }
    for(const auto& s : selected)
        new QListWidgetItem(QString::fromStdString(s), ui->listSelected);
}

SelectItemsPopup::~SelectItemsPopup()
{
    delete ui;
}

std::vector<std::string> SelectItemsPopup::selectedItems() const
{
    std::vector<std::string> result;
    for(int i=0;i<ui->listSelected->count();i++)
        result.push_back(ui->listSelected->item(i)->text().toStdString());
    return result;
}

void SelectItemsPopup::selectItem(const QModelIndex& idx)
{
    // Get currently selected iitem if none was provided
    QListWidgetItem* item;
    if(idx.isValid())
        item = ui->listAvailable->item(idx.row());
    else
        item = ui->listAvailable->currentItem();

    if(!item)
        return;

    // Add it to the selected items list
    new QListWidgetItem(item->text(), ui->listSelected);

    // Remove it from available items list
    delete item;
}

void SelectItemsPopup::unselectItem(const QModelIndex& idx)
{
    // Get currently selected iitem if none was provided
    QListWidgetItem* item;
    if(idx.isValid())
        item = ui->listSelected->item(idx.row());
    else
        item = ui->listSelected->currentItem();

    if(!item)
        return;

    // Add it to the available items list
    new QListWidgetItem(item->text(), ui->listAvailable);

    // Remove it from selected items list
    delete item;
}

void SelectItemsPopup::resizeEvent(QResizeEvent*)
{
    // We modify the shape of the dialog to add an arrow shaped edge. See the ascii art image below for details. The edges
    // are numbered, their order is the same as in the polygon definition.

    /*
            /3\
           /   \
      1---2     4--------5
      |                  |
      |                  |
      7------------------6
    */

    const int arrow_height = ui->spacer->geometry().height();
    const int arrow_width = arrow_height * 3;
    const int arrow_position_div = 5;

    QPolygon poly;
    poly << QPoint(rect().x(), rect().y() + arrow_height)
         << QPoint(rect().x() + rect().width() / arrow_position_div - arrow_width / 2, rect().y() + arrow_height)
         << QPoint(rect().x() + rect().width() / arrow_position_div, rect().y())
         << QPoint(rect().x() + rect().width() / arrow_position_div + arrow_width / 2, rect().y() + arrow_height)
         << QPoint(rect().x() + rect().width(), rect().y() + arrow_height)
         << QPoint(rect().x() + rect().width(), rect().y() + rect().height())
         << QPoint(rect().x(), rect().y() + rect().height());
    setMask(QRegion(poly));
}

void SelectItemsPopup::buttonBoxClicked(QAbstractButton* button)
{
    if(button == ui->buttonBox->button(QDialogButtonBox::Apply))
        accept();
}

void SelectItemsPopup::moveItemUp()
{
    moveCurrentItem(false);
}

void SelectItemsPopup::moveItemDown()
{
    moveCurrentItem(true);
}

void SelectItemsPopup::moveCurrentItem(bool down)
{
    // Get current row number and calculate row number after the movement. Check the values
    int currentRow = ui->listSelected->currentRow();
    if(currentRow == -1)
        return;
    int newRow = currentRow + (down ? 1 : -1);
    if(newRow < 0)
        return;
    if(newRow >= ui->listSelected->count())
        return;

    // Swap items
    ui->listSelected->insertItem(newRow, ui->listSelected->takeItem(currentRow));

    // Select old item at new position
    ui->listSelected->setCurrentRow(newRow);
}
