#include "CondFormatManager.h"
#include "ui_CondFormatManager.h"
#include "CondFormat.h"
#include "Settings.h"

#include <QColorDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QMessageBox>

CondFormatManager::CondFormatManager(const std::vector<CondFormat>& condFormats, const QString& encoding, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CondFormatManager),
    m_condFormats(condFormats),
    m_encoding(encoding)
{
    ui->setupUi(this);

    for(const CondFormat& aCondFormat : condFormats)
        addItem(aCondFormat);

    ui->tableCondFormats->setEditTriggers(QAbstractItemView::AllEditTriggers);

    connect(ui->buttonAdd, SIGNAL(clicked(bool)), this, SLOT(addNewItem()));
    connect(ui->buttonRemove, SIGNAL(clicked(bool)), this, SLOT(removeItem()));

    connect(ui->buttonDown, SIGNAL(clicked(bool)), this, SLOT(downItem()));
    connect(ui->buttonUp, SIGNAL(clicked(bool)), this, SLOT(upItem()));

    connect(ui->tableCondFormats, &QTreeWidget::itemClicked, this, &CondFormatManager::itemClicked);
}

CondFormatManager::~CondFormatManager()
{
    delete ui;
}

void CondFormatManager::addNewItem()
{
    CondFormat newCondFormat("", QColor(Settings::getValue("databrowser", "reg_fg_colour").toString()),
                             m_condFormatPalette.nextSerialColor(Palette::appHasDarkTheme()),
                             m_encoding);
    addItem(newCondFormat);
}

void CondFormatManager::addItem(const CondFormat& aCondFormat)
{
    int i = ui->tableCondFormats->topLevelItemCount();
    QTreeWidgetItem *newItem = new QTreeWidgetItem({"", "", aCondFormat.filter()});
    newItem->setForeground(ColumnForeground, aCondFormat.foregroundColor());
    newItem->setBackground(ColumnForeground, aCondFormat.foregroundColor());
    newItem->setForeground(ColumnBackground, aCondFormat.backgroundColor());
    newItem->setBackground(ColumnBackground, aCondFormat.backgroundColor());
    newItem->setToolTip(ColumnBackground, tr("Click to select color"));
    newItem->setToolTip(ColumnForeground, tr("Click to select color"));
    ui->tableCondFormats->insertTopLevelItem(i, newItem);
    ui->tableCondFormats->openPersistentEditor(newItem, ColumnFilter);
}

void CondFormatManager::removeItem()
{
    QTreeWidgetItem* item = ui->tableCondFormats->takeTopLevelItem(ui->tableCondFormats->currentIndex().row());
    delete item;
}

void CondFormatManager::upItem()
{
    if (!ui->tableCondFormats->currentIndex().isValid())
        return;

    int selectedRow = ui->tableCondFormats->currentIndex().row();
    if(selectedRow == 0)
        return;

    QTreeWidgetItem* item;
    item = ui->tableCondFormats->takeTopLevelItem(selectedRow);
    ui->tableCondFormats->insertTopLevelItem(selectedRow-1, item);
    ui->tableCondFormats->openPersistentEditor(item, ColumnFilter);
    ui->tableCondFormats->setCurrentIndex(ui->tableCondFormats->currentIndex().sibling(selectedRow-1,
                                                                                       ui->tableCondFormats->currentIndex().column()));
}

void CondFormatManager::downItem()
{
    if (!ui->tableCondFormats->currentIndex().isValid()) return;

    int selectedRow = ui->tableCondFormats->currentIndex().row();
    if(selectedRow == ui->tableCondFormats->topLevelItemCount() - 1)
        return;

    QTreeWidgetItem* item;
    item = ui->tableCondFormats->takeTopLevelItem(selectedRow);
    ui->tableCondFormats->insertTopLevelItem(selectedRow+1, item);
    ui->tableCondFormats->openPersistentEditor(item, ColumnFilter);
    ui->tableCondFormats->setCurrentIndex(ui->tableCondFormats->currentIndex().sibling(selectedRow+1,
                                                                                       ui->tableCondFormats->currentIndex().column()));
}

std::vector<CondFormat> CondFormatManager::getCondFormats()
{
    std::vector<CondFormat> result;
    for (int i = 0; i < ui->tableCondFormats->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->tableCondFormats->topLevelItem(i);
        result.emplace_back(item->text(ColumnFilter),
                            item->background(ColumnForeground).color(),
                            item->background(ColumnBackground).color(), m_encoding);
    }
    return result;
}


void CondFormatManager::itemClicked(QTreeWidgetItem* item, int column)
{
    switch (column) {
      case ColumnForeground:
      case ColumnBackground: {
          QColor color = QColorDialog::getColor(item->background(column).color(), this);
          if(color.isValid()) {
              item->setTextColor(column, color);
              item->setBackgroundColor(column, color);
              item->setToolTip(column, tr("Click to select color"));
          }
          break;
      }
    case ColumnFilter:
        // Nothing to do
        break;
    }
}

void CondFormatManager::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
        reject();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Ok))
        accept();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Help))
        QDesktopServices::openUrl(QUrl("https://github.com/sqlitebrowser/sqlitebrowser/wiki/Conditional-Formats"));
    else if (button == ui->buttonBox->button(QDialogButtonBox::Reset)) {
        if (QMessageBox::warning(this,
                                 QApplication::applicationName(),
                                 tr("Are you sure you want to clear all the conditional formats of this field?"),
                                 QMessageBox::Reset | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::Reset)
            if(ui->tableCondFormats->model()->hasChildren())
                ui->tableCondFormats->model()->removeRows(0, ui->tableCondFormats->model()->rowCount());
    }
}
