#include "AddRecordDialog.h"
#include "ui_AddRecordDialog.h"
#include "sqlitetablemodel.h"
#include "sqlitedb.h"
#include "Settings.h"

#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QWhatsThis>
#include <QAbstractButton>
#include <QLineEdit>

// Styled Item Delegate for non-editable columns (all except Value)
class NoEditDelegate: public QStyledItemDelegate {
public:
    NoEditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return nullptr;
    }
};

// Styled Item Delegate for editable columns (Value)
class EditDelegate: public QStyledItemDelegate {
public:
    EditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
        return new QLineEdit(parent);
    }
};

AddRecordDialog::AddRecordDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& tableName, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::AddRecordDialog),
      pdb(db),
      curTable(tableName),
      m_table(tableName.name())
{
    // Create UI
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    m_table = *(pdb.getObjectByName(curTable).dynamicCast<sqlb::Table>());

    populateFields();

    ui->sqlTextEdit->setReadOnly(true);

    // Update UI
    ui->treeWidget->resizeColumnToContents(kName);
    ui->treeWidget->resizeColumnToContents(kType);
    ui->treeWidget->setFrameShape(QFrame::Box);
}

AddRecordDialog::~AddRecordDialog()
{
    delete ui;
}

void AddRecordDialog::keyPressEvent(QKeyEvent *evt)
{
    if((evt->modifiers() & Qt::ControlModifier)
            && (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return))
    {
        accept();
        return;
    }
    if(evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return)
        return;
    QDialog::keyPressEvent(evt);
}

void AddRecordDialog::populateFields()
{
    // disconnect the itemChanged signal or the SQL text will
    // be updated while filling the treewidget.
    disconnect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),
               this,SLOT(itemChanged(QTreeWidgetItem*,int)));

    ui->treeWidget->clear();

    // Allow all Edit Triggers, but they will only apply to the columns with
    // editors (Value)
    ui->treeWidget->setEditTriggers(QAbstractItemView::AllEditTriggers);

    // Disallow edition of columns except Value
    ui->treeWidget->setItemDelegateForColumn(kName, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kType, new NoEditDelegate(this));
    ui->treeWidget->setItemDelegateForColumn(kValue, new EditDelegate(this));

    sqlb::FieldVector fields = m_table.fields();
    sqlb::FieldVector pk = m_table.primaryKey();
    for(const sqlb::FieldPtr& f : fields)
    {
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);

        tbitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

        tbitem->setText(kName, f->name());
        tbitem->setText(kType, f->type());

        // NOT NULL fields are indicated in bold.
        if (f->notnull()) {
            QFont font;
            font.setBold(true);
            tbitem->setData(kName, Qt::FontRole, font);
        }
        if (pk.contains(f))
            tbitem->setIcon(kName, QIcon(":/icons/field_key"));
        else
            tbitem->setIcon(kName, QIcon(":/icons/field"));

        QString defaultValue = f->defaultValue();
        QString toolTip;

        if (f->autoIncrement())
            toolTip.append(tr("Auto-increment\n"));

        if (f->unique())
            toolTip.append(tr("Unique\n"));

        if (!f->check().isEmpty())
            toolTip.append(tr("Check:\t %1\n").arg (f->check()));

        QSharedPointer<sqlb::ForeignKeyClause> fk =
            m_table.constraint({f}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>();
        if(fk)
            toolTip.append(tr("Foreign key:\t %1\n").arg(fk->toString()));

        // Display Role is used for displaying the default values.
        // Only when they are changed, the User Role is updated and then used in the INSERT query.
        // Default values are displayed with the style configured for NULL values in the Data Browser.
        if (!defaultValue.isEmpty()) {
            QFont font;
            font.setItalic(true);
            tbitem->setData(kValue, Qt::DisplayRole, f->defaultValue());
            tbitem->setData(kValue, Qt::FontRole, font);
            tbitem->setData(kValue, Qt::BackgroundRole, QColor(Settings::getValue("databrowser", "null_bg_colour").toString()));
            tbitem->setData(kValue, Qt::ForegroundRole, QColor(Settings::getValue("databrowser", "null_fg_colour").toString()));
            toolTip.append(tr("Default value:\t %1\n").arg (defaultValue));
        } else
            tbitem->setData(kValue, Qt::DisplayRole, QVariant());

        if (!toolTip.isEmpty()) {
            // Chop last end-of-line
            toolTip.chop(1);
            tbitem->setToolTip(kValue, toolTip);
        }
    }

    updateSqlText();

    // and reconnect
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));
}

void AddRecordDialog::accept()
{
    if(!pdb.executeSQL(ui->sqlTextEdit->text()))
    {
        QMessageBox::warning(
            this,
            QApplication::applicationName(),
            tr("Error adding record. Message from database engine:\n\n%1").arg(pdb.lastError()));
        return;
    }

    QDialog::accept();
}

void AddRecordDialog::updateSqlText()
{
    QString stmt = QString("INSERT INTO %1").arg(curTable.toString());

    QStringList vals;
    QStringList fields;

    // If the User Role of the Value column is not null, the entered value is used
    // in the INSERT statement. Otherwise, SQLite just uses the default value for the field.
    for(int i = 0; i < ui->treeWidget->topLevelItemCount(); ++i)
    {
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(i);
        QVariant name = item->data(kName, Qt::DisplayRole);
        // User role contains now values entered by the user, that we actually need to insert.
        QVariant value = item->data(kValue, Qt::UserRole);

        if (!value.isNull()) {
            bool isNumeric;
            fields << sqlb::escapeIdentifier(name.toString());
            value.toString().toDouble(&isNumeric);
            if (isNumeric)
                vals << value.toString();
            else
                vals << QString("'%1'").arg(value.toString());
        }
    }

    if(fields.empty())
    {
        stmt.append(" DEFAULT VALUES;");
    } else {
        stmt.append("\n(");
        stmt.append(fields.join(", "));
        stmt.append(")\nVALUES (");
        stmt.append(vals.join(", "));
        stmt.append(");");
    }

    ui->sqlTextEdit->setText(stmt);
}

void AddRecordDialog::itemChanged(QTreeWidgetItem *item, int column)
{
    // Restore default fore/background for the value column,
    // since the value has changed away from the default.
    item->setData(kValue, Qt::BackgroundRole, item->data(kName, Qt::BackgroundRole));
    item->setData(kValue, Qt::ForegroundRole, item->data(kName, Qt::ForegroundRole));

    // Copy the just edited data to the user role, so it is used in the SQL query.
    item->setData(kValue, Qt::UserRole, item->data(kValue, Qt::EditRole));

    updateSqlText();
}

void AddRecordDialog::help()
{
    QWhatsThis::enterWhatsThisMode();
}

void AddRecordDialog::on_buttonBox_clicked(QAbstractButton* button)
{
    if (button == ui->buttonBox->button(QDialogButtonBox::Cancel))
        reject();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Save))
        accept();
    else if (button == ui->buttonBox->button(QDialogButtonBox::Help))
        help();
    else if (button == ui->buttonBox->button(QDialogButtonBox::RestoreDefaults)) {
        if (QMessageBox::warning(this,
                                 QApplication::applicationName(),
                                 tr("Are you sure you want to restore all the entered values to its defaults?"),
                                 QMessageBox::RestoreDefaults | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::RestoreDefaults)
            populateFields();
    }
}
