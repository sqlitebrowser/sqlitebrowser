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

        connect(m_btnReset, &QPushButton::clicked, [&]
        {
            tablesComboBox->setCurrentIndex(-1);
            idsComboBox->setCurrentIndex(-1);
            clauseEdit->clear();
        });

        connect(tablesComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
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

        return QString("%1%2 %3")
                .arg(table)
                .arg(id)
                .arg(clauses)
                .trimmed();
    }

    QComboBox* tablesComboBox;
    QComboBox* idsComboBox;
    QLineEdit* clauseEdit; // for ON CASCADE and such

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
        for(const auto& jt : it.second)
        {
            if(jt.second->type() == sqlb::Object::Types::Table)
                m_tablesIds.insert({jt.second->name(), std::dynamic_pointer_cast<sqlb::Table>(jt.second)->fieldNames()});
        }
    }
}

QWidget* ForeignKeyEditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    ForeignKeyEditor* editor = new ForeignKeyEditor(parent);
    editor->setAutoFillBackground(true);

    connect(editor->tablesComboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged),
            [=](const QString& tableName)
    {
        QComboBox* box = editor->idsComboBox;
        box->clear();
        box->addItem(QString());                // for those heroes who don't like to specify key explicitly
        for(const auto& n : m_tablesIds[tableName.toStdString()])
            box->addItem(QString::fromStdString(n));
        box->setCurrentIndex(0);
    });

    editor->tablesComboBox->clear();
    for(const auto& i : m_tablesIds)
        editor->tablesComboBox->addItem(QString::fromStdString(i.first));

    return editor;
}

void ForeignKeyEditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    ForeignKeyEditor* fkEditor = static_cast<ForeignKeyEditor*>(editor);

    size_t column = static_cast<size_t>(index.row()); // weird? I know right
    const sqlb::Field& field = m_table.fields.at(column);
    auto fk = std::dynamic_pointer_cast<sqlb::ForeignKeyClause>(m_table.constraint({field.name()}, sqlb::Constraint::ForeignKeyConstraintType));
    if (fk) {
        fkEditor->tablesComboBox->setCurrentText(QString::fromStdString(fk->table()));
        fkEditor->clauseEdit->setText(QString::fromStdString(fk->constraint()));
        if (!fk->columns().empty())
            fkEditor->idsComboBox->setCurrentText(QString::fromStdString(fk->columns().front()));
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
    if (sql.isEmpty()) {
        // Remove the foreign key
        m_table.removeConstraints({field.name()}, sqlb::Constraint::ConstraintTypes::ForeignKeyConstraintType);
    } else {
        // Set the foreign key
        sqlb::ForeignKeyClause* fk = new sqlb::ForeignKeyClause;

        const QString table     = fkEditor->tablesComboBox->currentText();
        const std::string id    = fkEditor->idsComboBox->currentText().toStdString();
        const QString clause    = fkEditor->clauseEdit->text();

        fk->setTable(table.toStdString());

        if (!id.empty())
            fk->setColumns({id});

        if (!clause.trimmed().isEmpty()) {
            fk->setConstraint(clause.toStdString());
        }

        m_table.setConstraint({field.name()}, sqlb::ConstraintPtr(fk));
    }

    model->setData(index, sql);
}

void ForeignKeyEditorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void ForeignKeyEditorDelegate::updateTablesList(const std::string& oldTableName)
{
    // this is used for recursive table constraints when
    // table column references column within same table
    m_tablesIds.erase(oldTableName);
    m_tablesIds.insert({m_table.name(), m_table.fieldNames()});
}

#include "ForeignKeyEditorDelegate.moc"
