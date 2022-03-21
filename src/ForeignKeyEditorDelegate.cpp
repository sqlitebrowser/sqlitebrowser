#include "sqlitedb.h"
#include "ForeignKeyEditorDelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>

class ForeignKeyEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ForeignKeyEditor(QWidget* parent = nullptr)
        : QWidget(parent)
        , tablesComboBox(new QComboBox(this))
        , idsComboBox(new QComboBox(this))
        , clauseEdit(new QLineEdit(this))
        , m_btnReset(new QPushButton(tr("&Reset"), this))
    {
        idsComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
        clauseEdit->setPlaceholderText(tr("Foreign key clauses (ON UPDATE, ON DELETE etc.)"));

        QHBoxLayout* layout = new QHBoxLayout(this);
        layout->addWidget(tablesComboBox);
        layout->addWidget(idsComboBox);
        layout->addWidget(clauseEdit);
        layout->addWidget(m_btnReset);
        layout->setSpacing(0);
        layout->setMargin(0);
        setLayout(layout);

        connect(m_btnReset, &QPushButton::clicked, this, [&]
        {
            tablesComboBox->setCurrentIndex(-1);
            idsComboBox->setCurrentIndex(-1);
            clauseEdit->clear();
        });

        connect(tablesComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
                [=](int index)
        {
            // reset ids combo box
            idsComboBox->setCurrentIndex(-1);

            // disable clauses editor if none of tables is selected
            bool enableClausesEditor = (index!= -1);
            clauseEdit->setEnabled(enableClausesEditor);
        });
    }

    QString getSql() const
    {
        if (tablesComboBox->currentText().isEmpty())
            return QString();

        const QString table     = sqlb::escapeIdentifier(tablesComboBox->currentText());
        const QString clauses   = clauseEdit->text();

        QString id = idsComboBox->currentText();
        if (!id.isEmpty())
            id = QString("(%1)").arg(sqlb::escapeIdentifier(id));

        return QString("%1%2 %3").arg(
                table,
                id,
                clauses)
                .trimmed();
    }

    QComboBox* tablesComboBox;
    QComboBox* idsComboBox;
    QLineEdit* clauseEdit; // for ON CASCADE and such
    std::shared_ptr<sqlb::ForeignKeyClause> foreignKey;   // This can be used for storing a pointer to the foreign key object currently edited

private:
    QPushButton* m_btnReset;
};

ForeignKeyEditorDelegate::ForeignKeyEditorDelegate(const DBBrowserDB& db, sqlb::Table& table, QObject* parent)
    : QStyledItemDelegate(parent)
    , m_db(db)
    , m_table(table)
{
    for(const auto& it : m_db.schemata)
    {
        for(const auto& jt : it.second.tables)
        {
            // Don't insert the current table into the list. The name and fields of the current table are always taken from the m_table reference
            if(jt.first != m_table.name())
                m_tablesIds.insert({jt.first, jt.second->fieldNames()});
        }
    }
}

QWidget* ForeignKeyEditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    ForeignKeyEditor* editor = new ForeignKeyEditor(parent);
    editor->setAutoFillBackground(true);

    connect(editor->tablesComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            [=](int idx)
    {
        QString tableName = editor->tablesComboBox->itemText(idx);

        QComboBox* box = editor->idsComboBox;
        box->clear();
        box->addItem(QString());                // for those heroes who don't like to specify key explicitly

        // For recursive foreign keys get the field list from the m_table reference. For other foreign keys from the prepared field lists.
        if(tableName.toStdString() == m_table.name())
        {
            for(const auto& n : m_table.fieldNames())
                box->addItem(QString::fromStdString(n));
        } else {
            for(const auto& n : m_tablesIds[tableName.toStdString()])
                box->addItem(QString::fromStdString(n));
        }


        box->setCurrentIndex(0);
    });

    editor->tablesComboBox->clear();
    editor->tablesComboBox->addItem(QString::fromStdString(m_table.name()));    // For recursive foreign keys
    editor->tablesComboBox->insertSeparator(1);
    for(const auto& i : m_tablesIds)
        editor->tablesComboBox->addItem(QString::fromStdString(i.first));

    return editor;
}

void ForeignKeyEditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    ForeignKeyEditor* fkEditor = static_cast<ForeignKeyEditor*>(editor);

    size_t column = static_cast<size_t>(index.row()); // weird? I know right
    const sqlb::Field& field = m_table.fields.at(column);
    fkEditor->foreignKey = m_table.foreignKey({field.name()});
    if (fkEditor->foreignKey) {
        fkEditor->tablesComboBox->setCurrentText(QString::fromStdString(fkEditor->foreignKey->table()));
        fkEditor->clauseEdit->setText(QString::fromStdString(fkEditor->foreignKey->constraint()));
        if (!fkEditor->foreignKey->columns().empty())
            fkEditor->idsComboBox->setCurrentText(QString::fromStdString(fkEditor->foreignKey->columns().front()));
    } else {
        fkEditor->tablesComboBox->setCurrentIndex(-1);
    }
}

void ForeignKeyEditorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ForeignKeyEditor* fkEditor = static_cast<ForeignKeyEditor*>(editor);
    QString sql = fkEditor->getSql();

    size_t column = static_cast<size_t>(index.row());
    const sqlb::Field& field = m_table.fields.at(column);
    if (sql.isEmpty() && fkEditor->foreignKey) {
        // Remove the foreign key
        m_table.removeConstraint(fkEditor->foreignKey);
    } else {
        // Create a new foreign key object if required
        if(!fkEditor->foreignKey)
        {
            fkEditor->foreignKey = std::make_shared<sqlb::ForeignKeyClause>();
            m_table.addConstraint({field.name()}, fkEditor->foreignKey);
        }

        // Set data
        const auto table  = fkEditor->tablesComboBox->currentText().toStdString();
        const auto id     = fkEditor->idsComboBox->currentText().toStdString();
        const auto clause = fkEditor->clauseEdit->text().trimmed().toStdString();
        fkEditor->foreignKey->setTable(table);
        if(id.empty())
            fkEditor->foreignKey->setColumns({});
        else
            fkEditor->foreignKey->setColumns({id});
        fkEditor->foreignKey->setConstraint(clause);
    }

    model->setData(index, sql);
}

void ForeignKeyEditorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

#include "ForeignKeyEditorDelegate.moc"
