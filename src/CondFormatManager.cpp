#include "CondFormatManager.h"
#include "ui_CondFormatManager.h"
#include "CondFormat.h"
#include "Settings.h"
#include "FilterLineEdit.h"

#include <QColorDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QPushButton>
#include <QMessageBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QComboBox>
#include <QStyledItemDelegate>

// Styled Item Delegate for non-editable columns (all except Condition)
class DefaultDelegate: public QStyledItemDelegate {
public:
    explicit DefaultDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget* /* parent */, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const override {
        return nullptr;
    }
};

// Styled Item Delegate for the Condition column as a filter line editor.
class FilterEditDelegate: public QStyledItemDelegate {

public:
    explicit FilterEditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const override {
        FilterLineEdit* filterEditor = new FilterLineEdit(parent);
        filterEditor->setConditionFormatContextMenuEnabled(false);
        return filterEditor;
    }
};

CondFormatManager::CondFormatManager(const std::vector<CondFormat>& condFormats, const QString& encoding, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CondFormatManager),
    m_condFormats(condFormats),
    m_encoding(encoding)
{
    ui->setupUi(this);

    for(const CondFormat& aCondFormat : condFormats)
        addItem(aCondFormat);

    // Make condition editable as a filter line editor.
    ui->tableCondFormats->setEditTriggers(QAbstractItemView::AllEditTriggers);
    ui->tableCondFormats->setItemDelegateForColumn(ColumnFilter, new FilterEditDelegate(this));

    // Make columns not text-editable, except for the condition.
    for(int col = ColumnForeground; col < ColumnFilter; ++col) {
        ui->tableCondFormats->setItemDelegateForColumn(col, new DefaultDelegate(this));
        ui->tableCondFormats->resizeColumnToContents(col);
    }

    connect(ui->buttonAdd, &QToolButton::clicked, this, &CondFormatManager::addNewItem);
    connect(ui->buttonRemove, &QToolButton::clicked, this, &CondFormatManager::removeItem);

    connect(ui->buttonDown, &QToolButton::clicked, this, &CondFormatManager::downItem);
    connect(ui->buttonUp, &QToolButton::clicked, this, &CondFormatManager::upItem);

    connect(ui->tableCondFormats, &QTreeWidget::itemClicked, this, &CondFormatManager::itemClicked);
}

CondFormatManager::~CondFormatManager()
{
    delete ui;
}

void CondFormatManager::addNewItem()
{
    // Clear focus from the current widget, so if there is some input, it is saved now.
    QWidget* currentWidget = ui->tableCondFormats->itemWidget(ui->tableCondFormats->currentItem(), ColumnFilter);
    if (currentWidget != nullptr)
        currentWidget->clearFocus();

    QFont font = QFont(Settings::getValue("databrowser", "font").toString());
    font.setPointSize(Settings::getValue("databrowser", "fontsize").toInt());

    CondFormat newCondFormat("", QColor(Settings::getValue("databrowser", "reg_fg_colour").toString()),
                             m_condFormatPalette.nextSerialColor(Palette::appHasDarkTheme()),
                             font,
                             CondFormat::AlignLeft,
                             m_encoding);
    addItem(newCondFormat);
}

void CondFormatManager::addItem(const CondFormat& aCondFormat)
{
    int i = ui->tableCondFormats->topLevelItemCount();
    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->tableCondFormats);
    newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

    newItem->setForeground(ColumnForeground, aCondFormat.foregroundColor());
    newItem->setBackground(ColumnForeground, aCondFormat.foregroundColor());
    newItem->setForeground(ColumnBackground, aCondFormat.backgroundColor());
    newItem->setBackground(ColumnBackground, aCondFormat.backgroundColor());
    newItem->setToolTip(ColumnBackground, tr("Click to select color"));
    newItem->setToolTip(ColumnForeground, tr("Click to select color"));

    QFontComboBox* fontCombo = new QFontComboBox(ui->tableCondFormats);
    fontCombo->setCurrentFont(aCondFormat.font());
    // Avoid that the font combo box gets too wide and work around a possible Qt bug where the box overlaps the next column.
    fontCombo->setMaximumWidth(150);
    ui->tableCondFormats->setItemWidget(newItem, ColumnFont, fontCombo);

    QSpinBox* sizeBox = new QSpinBox(ui->tableCondFormats);
    sizeBox->setMinimum(1);
    sizeBox->setValue(aCondFormat.font().pointSize());
    ui->tableCondFormats->setItemWidget(newItem, ColumnSize, sizeBox);

    newItem->setCheckState(ColumnBold, aCondFormat.isBold() ? Qt::Checked : Qt::Unchecked);
    newItem->setCheckState(ColumnItalic, aCondFormat.isItalic() ? Qt::Checked : Qt::Unchecked);
    newItem->setCheckState(ColumnUnderline, aCondFormat.isUnderline() ? Qt::Checked : Qt::Unchecked);

    QComboBox* alignCombo = new QComboBox(ui->tableCondFormats);
    alignCombo->addItems(CondFormat::alignmentTexts());
    alignCombo->setCurrentIndex(aCondFormat.alignment());
    ui->tableCondFormats->setItemWidget(newItem, ColumnAlignment, alignCombo);

    newItem->setText(ColumnFilter, aCondFormat.filter());
    ui->tableCondFormats->insertTopLevelItem(i, newItem);
}

void CondFormatManager::removeItem()
{
    QTreeWidgetItem* item = ui->tableCondFormats->takeTopLevelItem(ui->tableCondFormats->currentIndex().row());
    delete item;
}

void CondFormatManager::moveItem(int offset)
{
    if (!ui->tableCondFormats->currentIndex().isValid())
        return;

    int selectedRow = ui->tableCondFormats->currentIndex().row();
    int newRow = selectedRow + offset;
    if(newRow < 0 || newRow >= ui->tableCondFormats->topLevelItemCount())
        return;

    QTreeWidgetItem* item = ui->tableCondFormats->topLevelItem(selectedRow);

    // Rescue widgets, since they will be deleted, and add them later.
    QFontComboBox* fontCombo = qobject_cast<QFontComboBox*>(ui->tableCondFormats->itemWidget(item, ColumnFont));
    QFontComboBox* fontCombo2 = new QFontComboBox(ui->tableCondFormats);
    fontCombo2->setCurrentFont(fontCombo->currentFont());

    QSpinBox* sizeBox = qobject_cast<QSpinBox*>(ui->tableCondFormats->itemWidget(item, ColumnSize));
    QSpinBox* sizeBox2 = new QSpinBox(ui->tableCondFormats);
    sizeBox2->setValue(sizeBox->value());
    sizeBox2->setMinimum(sizeBox->minimum());

    QComboBox* alignCombo = qobject_cast<QComboBox*>(ui->tableCondFormats->itemWidget(item, ColumnAlignment));
    QComboBox* alignCombo2 = new QComboBox(ui->tableCondFormats);
    alignCombo2->addItems(CondFormat::alignmentTexts());
    alignCombo2->setCurrentIndex(alignCombo->currentIndex());

    item = ui->tableCondFormats->takeTopLevelItem(selectedRow);
    ui->tableCondFormats->insertTopLevelItem(newRow, item);

    // Restore widgets and state
    ui->tableCondFormats->setItemWidget(item, ColumnFont, fontCombo2);
    ui->tableCondFormats->setItemWidget(item, ColumnSize, sizeBox2);
    ui->tableCondFormats->setItemWidget(item, ColumnAlignment, alignCombo2);
    ui->tableCondFormats->setCurrentIndex(ui->tableCondFormats->currentIndex().sibling(newRow,
                                                                                       ui->tableCondFormats->currentIndex().column()));
    // This is added so the widgets are readjusted. Otherwise they can be misplaced when moving an item to the top.
    ui->tableCondFormats->adjustSize();
}

void CondFormatManager::upItem()
{
    moveItem(-1);
}

void CondFormatManager::downItem()
{
    moveItem(+1);
}

std::vector<CondFormat> CondFormatManager::getCondFormats() const
{
    std::vector<CondFormat> result;

    for (int i = 0; i < ui->tableCondFormats->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem* item = ui->tableCondFormats->topLevelItem(i);

        QFontComboBox* fontCombo = qobject_cast<QFontComboBox*>(ui->tableCondFormats->itemWidget(item, ColumnFont));
        QSpinBox* sizeBox = qobject_cast<QSpinBox*>(ui->tableCondFormats->itemWidget(item, ColumnSize));
        QFont font = fontCombo->currentFont();
        font.setPointSize(sizeBox->value());
        font.setBold(item->checkState(ColumnBold) == Qt::Checked);
        font.setItalic(item->checkState(ColumnItalic) == Qt::Checked);
        font.setUnderline(item->checkState(ColumnUnderline) == Qt::Checked);
        QComboBox* alignCombo = qobject_cast<QComboBox*>(ui->tableCondFormats->itemWidget(item, ColumnAlignment));

        result.emplace_back(item->text(ColumnFilter),
                            item->background(ColumnForeground).color(),
                            item->background(ColumnBackground).color(),
                            font,
                            static_cast<CondFormat::Alignment>(alignCombo->currentIndex()),
                            m_encoding);
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
        }
        break;
    }
    case ColumnBold:
    case ColumnItalic:
    case ColumnUnderline:
        item->setCheckState(column, item->checkState(column) != Qt::Checked ? Qt::Checked : Qt::Unchecked);
        break;
    default:
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
