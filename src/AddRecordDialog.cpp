#include "AddRecordDialog.h"
#include "ui_AddRecordDialog.h"
#include "sqlitedb.h"
#include "Settings.h"

#include <QMessageBox>
#include <QPushButton>
#include <QKeyEvent>
#include <QStyledItemDelegate>
#include <QWhatsThis>
#include <QLineEdit>
#include <QMenu>

class NullLineEdit: public QLineEdit {
    Q_OBJECT
private:
    bool m_isNull;

public:
    explicit NullLineEdit(QWidget* parent=nullptr): QLineEdit(parent), m_isNull (true) {}

    bool isNull() {return m_isNull;}
    void setNull(bool value) {
        if (value) {
            clear();
            setStyleSheet("QLineEdit{ font-style: italic; }");
            setPlaceholderText(Settings::getValue("databrowser", "null_text").toString());
            setModified(false);
        } else {
            setStyleSheet("");
            setPlaceholderText("");
        }
        m_isNull = value;
    }
protected:
    void contextMenuEvent(QContextMenuEvent *event)
        {
            QMenu* editContextMenu = createStandardContextMenu();

            QAction* nullAction = new QAction(QIcon(":/icons/set_to_null"), tr("Set to NULL"), editContextMenu);
            connect(nullAction, &QAction::triggered, [&]() {
                    setNull(true);
                });
            nullAction->setShortcut(QKeySequence(tr("Alt+Del")));
            editContextMenu->addSeparator();
            editContextMenu->addAction(nullAction);

            editContextMenu->exec(event->globalPos());
            delete editContextMenu;
        }

    void keyPressEvent(QKeyEvent *evt) {
        // Alt+Del sets field to NULL
        if((evt->modifiers() & Qt::AltModifier) && (evt->key() == Qt::Key_Delete))
            setNull(true);
        else {
            // Remove any possible NULL mark when user starts typing
            setStyleSheet("");
            setPlaceholderText("");
            QLineEdit::keyPressEvent(evt);
        }
    }
};

// Styled Item Delegate for non-editable columns (all except Value)
class NoEditDelegate: public QStyledItemDelegate {
public:
    explicit NoEditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget* /* parent */, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const override {
        return nullptr;
    }
};

// Styled Item Delegate for editable columns (Value)
class EditDelegate: public QStyledItemDelegate {

public:
    explicit EditDelegate(QObject* parent=nullptr): QStyledItemDelegate(parent) {}
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem& /* option */, const QModelIndex& /* index */) const override {
        return new NullLineEdit(parent);
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const override {

        NullLineEdit* lineEditor = dynamic_cast<NullLineEdit*>(editor);
        // Set the editor in the null state (unless the user has actually written NULL)
        if (index.model()->data(index, Qt::UserRole).isNull() &&
            index.model()->data(index, Qt::DisplayRole) == Settings::getValue("databrowser", "null_text"))
            lineEditor->setNull(true);
        else {
            QStyledItemDelegate::setEditorData(editor, index);
            lineEditor->setNull(false);
        }
    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {

        NullLineEdit* lineEditor = dynamic_cast<NullLineEdit*>(editor);
        // Restore NULL text (unless the user has already modified the value)
        if (lineEditor->isNull() && !lineEditor->isModified()) {
            model->setData(index, Settings::getValue("databrowser", "null_text"), Qt::DisplayRole);
            model->setData(index, QVariant(), Qt::UserRole);
        } else {
            // Get isModified flag before calling setModelData
            bool modified = lineEditor->isModified();
            QStyledItemDelegate::setModelData(editor, model, index);
            // Copy the just edited data to the user role, so it can be later used in the SQL insert statement.
            if (modified) {
                lineEditor->setNull(false);
                model->setData(index, model->data(index, Qt::EditRole), Qt::UserRole);
            }
        }
    }
};

AddRecordDialog::AddRecordDialog(DBBrowserDB& db, const sqlb::ObjectIdentifier& tableName, QWidget* parent, const std::vector<std::string>& pseudo_pk)
    : QDialog(parent),
      ui(new Ui::AddRecordDialog),
      pdb(db),
      curTable(tableName),
      pseudo_pk(pseudo_pk)
{
    // Create UI
    ui->setupUi(this);
    connect(ui->treeWidget, SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(itemChanged(QTreeWidgetItem*,int)));

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

void AddRecordDialog::setDefaultsStyle(QTreeWidgetItem* item)
{
    // Default values are displayed with the style configured for NULL values in the Data Browser.
    QFont font;
    font.setItalic(true);
    item->setData(kValue, Qt::FontRole, font);
    item->setData(kValue, Qt::BackgroundRole, QColor(Settings::getValue("databrowser", "null_bg_colour").toString()));
    item->setData(kValue, Qt::ForegroundRole, QColor(Settings::getValue("databrowser", "null_fg_colour").toString()));
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

    sqlb::FieldVector fields;
    std::vector<sqlb::ConstraintPtr> fks;
    QStringList pk;

    // Initialize fields, fks and pk differently depending on whether it's a table or a view.
    const sqlb::ObjectPtr obj = pdb.getObjectByName(curTable);
    if (obj->type() == sqlb::Object::Table)
    {
        sqlb::TablePtr m_table = pdb.getObjectByName<sqlb::Table>(curTable);
        fields = m_table->fields;
        for(const sqlb::Field& f : fields)
            fks.push_back(m_table->constraint({f.name()}, sqlb::Constraint::ForeignKeyConstraintType));
        pk = m_table->primaryKey();
    } else {
        sqlb::ViewPtr m_view = pdb.getObjectByName<sqlb::View>(curTable);
        fields = m_view->fields;
        fks.resize(fields.size(), sqlb::ConstraintPtr(nullptr));
        for(const auto& col : pseudo_pk)
            pk << QString::fromStdString(col);
    }

    for(uint i = 0; i < fields.size(); i++)
    {
        const sqlb::Field& f = fields[i];
        QTreeWidgetItem *tbitem = new QTreeWidgetItem(ui->treeWidget);

        tbitem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);

        tbitem->setText(kName, f.name());
        tbitem->setText(kType, f.type());
        tbitem->setData(kType, Qt::UserRole, f.affinity());

        // NOT NULL fields are indicated in bold.
        if (f.notnull()) {
            QFont font;
            font.setBold(true);
            tbitem->setData(kName, Qt::FontRole, font);
        }
        if (contains(pk, f.name()))
            tbitem->setIcon(kName, QIcon(":/icons/field_key"));
        else if (fks[i])
            tbitem->setIcon(kName, QIcon(":/icons/field_fk"));
        else
            tbitem->setIcon(kName, QIcon(":/icons/field"));

        QString defaultValue = f.defaultValue();
        QString toolTip;

        if (f.autoIncrement())
            toolTip.append(tr("Auto-increment\n"));

        if (f.unique())
            toolTip.append(tr("Unique constraint\n"));

        if (!f.check().isEmpty())
            toolTip.append(tr("Check constraint:\t %1\n").arg (f.check()));

        auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(fks[i]);
        if(fk)
            toolTip.append(tr("Foreign key:\t %1\n").arg(fk->toString()));

        setDefaultsStyle(tbitem);

        // Display Role is used for displaying the default values.
        // Only when they are changed, the User Role is updated and then used in the INSERT query.
        if (!defaultValue.isEmpty()) {
            tbitem->setData(kValue, Qt::DisplayRole, f.defaultValue());
            toolTip.append(tr("Default value:\t %1\n").arg (defaultValue));
        } else
            tbitem->setData(kValue, Qt::DisplayRole, Settings::getValue("databrowser", "null_text"));


        if (!toolTip.isEmpty()) {
            // Chop last end-of-line
            toolTip.chop(1);
            tbitem->setToolTip(kValue, toolTip);
            tbitem->setToolTip(kType, toolTip);
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
        // User role contains now values entered by the user, that we actually need to insert.
        QVariant value = item->data(kValue, Qt::UserRole);

        if (!value.isNull()) {
            bool isNumeric;
            fields << sqlb::escapeIdentifier(item->text(kName));
            value.toDouble(&isNumeric);
            // If it has a numeric format and has no text affinity, do not quote it.
            if (isNumeric && item->data(kType, Qt::UserRole).toString() != "TEXT")
                vals << value.toString();
            else
                vals << QString("'%1'").arg(value.toString().replace("'", "''"));
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
    if (item->data(column, Qt::UserRole).isNull())
        setDefaultsStyle(item);
    else {
        // Restore default fore/background for the value column,
        // since the value has changed away from the default.
        QFont font;
        font.setItalic(false);
        item->setData(column, Qt::FontRole, font);
        item->setData(column, Qt::BackgroundRole, item->data(kName, Qt::BackgroundRole));
        item->setData(column, Qt::ForegroundRole, item->data(kName, Qt::ForegroundRole));
    }

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
                                 tr("Are you sure you want to restore all the entered values to their defaults?"),
                                 QMessageBox::RestoreDefaults | QMessageBox::Cancel,
                                 QMessageBox::Cancel) == QMessageBox::RestoreDefaults)
            populateFields();
    }
}

#include "AddRecordDialog.moc"
