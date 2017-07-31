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
    ForeignKeyEditor(QWidget* parent = Q_NULLPTR)
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
    const auto objects = m_db.getBrowsableObjects();
    for (auto& obj : objects) {
        if (obj->type() == sqlb::Object::Types::Table) {
            QString tableName = obj->name();
            m_tablesIds.insert(tableName, obj.dynamicCast<sqlb::Table>()->fieldNames());
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
        box->addItems(m_tablesIds[tableName]);
        box->setCurrentIndex(0);
    });

    editor->tablesComboBox->clear();
    editor->tablesComboBox->addItems(m_tablesIds.keys());

    return editor;
}

void ForeignKeyEditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    ForeignKeyEditor* fkEditor = static_cast<ForeignKeyEditor*>(editor);

    int column = index.row(); // weird? I know right
    sqlb::FieldPtr field = m_table.fields().at(column);
    QSharedPointer<sqlb::ForeignKeyClause> fk = m_table.constraint({field}, sqlb::Constraint::ForeignKeyConstraintType).dynamicCast<sqlb::ForeignKeyClause>();
    if (!fk.isNull()) {
        fkEditor->tablesComboBox->setCurrentText(fk->table());
        fkEditor->clauseEdit->setText(fk->constraint());
        if (!fk->columns().isEmpty())
            fkEditor->idsComboBox->setCurrentText(fk->columns().first());
    } else {
        fkEditor->tablesComboBox->setCurrentIndex(-1);
    }
}

void ForeignKeyEditorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    ForeignKeyEditor* fkEditor = static_cast<ForeignKeyEditor*>(editor);
    QString sql = fkEditor->getSql();

    int column = index.row();
    sqlb::FieldPtr field = m_table.fields().at(column);
    if (sql.isEmpty()) {
        // Remove the foreign key
        m_table.removeConstraints({field}, sqlb::Constraint::ConstraintTypes::ForeignKeyConstraintType);
    } else {
        // Set the foreign key
        sqlb::ForeignKeyClause* fk = new sqlb::ForeignKeyClause;

        const QString table     = fkEditor->tablesComboBox->currentText();
        const QString id        = fkEditor->idsComboBox->currentText();
        const QString clause    = fkEditor->clauseEdit->text();

        fk->setTable(table);

        if (!id.isEmpty())
            fk->setColumns({id});

        if (!clause.trimmed().isEmpty()) {
            fk->setConstraint(clause);
        }

        m_table.setConstraint({field}, sqlb::ConstraintPtr(fk));
    }

    model->setData(index, sql);
}

void ForeignKeyEditorDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

void ForeignKeyEditorDelegate::updateTablesList(const QString& oldTableName)
{
    // this is used for recursive table constraints when
    // table column references column within same table
    m_tablesIds.remove(oldTableName);
    m_tablesIds.insert(m_table.name(), m_table.fieldNames());
}

#include "ForeignKeyEditorDelegate.moc"
