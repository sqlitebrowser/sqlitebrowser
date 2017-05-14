#include "sqlitetypes.h"
#include "grammar/Sqlite3Lexer.hpp"
#include "grammar/Sqlite3Parser.hpp"

#include <sstream>
#include <QDebug>

namespace sqlb {

QStringList Field::Datatypes = QStringList() << "INTEGER" << "TEXT" << "BLOB" << "REAL" << "NUMERIC";

QString escapeIdentifier(QString id)
{
    return '`' + id.replace('`', "``") + '`';
}

QStringList fieldVectorToFieldNames(const FieldVector& vector)
{
    QStringList result;
    foreach(const FieldPtr& field, vector)
        result.append(escapeIdentifier(field->name()));
    return result;
}

/**
 * @brief The CreateTableWalker class
 * Goes trough the createtable AST and returns
 * Table object.
 */
class CreateTableWalker
{
public:
    explicit CreateTableWalker(antlr::RefAST r)
        : m_root(r)
    {}

    TablePtr table();

private:
    void parsecolumn(Table* table, antlr::RefAST c);

private:
    antlr::RefAST m_root;
};

/**
 * @brief The CreateIndexWalker class
 * Goes trough the createtable AST and returns
 * Index object.
 */
class CreateIndexWalker
{
public:
    explicit CreateIndexWalker(antlr::RefAST r)
        : m_root(r)
    {}

    IndexPtr index();

private:
    void parsecolumn(Index* index, antlr::RefAST c);

private:
    antlr::RefAST m_root;
};

ObjectPtr Object::parseSQL(Object::Types type, const QString& sSQL)
{
    // Parse SQL statement according to type
    ObjectPtr result;
    switch(type)
    {
    case Object::Types::Table:
        result = Table::parseSQL(sSQL);
        break;
    case Object::Types::Index:
        result = Index::parseSQL(sSQL);
        break;
    case Object::Types::View:
        result = View::parseSQL(sSQL);
        break;
    case Object::Types::Trigger:
        result = Trigger::parseSQL(sSQL);
        break;
    default:
        return ObjectPtr(nullptr);
    }

    // Strore the original SQL statement and return the result
    result->setOriginalSql(sSQL);
    return result;
}

QString Object::typeToString(Types type)
{
    switch(type)
    {
    case Types::Table: return "table";
    case Types::Index: return "index";
    case Types::View: return "view";
    case Types::Trigger: return "trigger";
    }
    return QString();
}

bool ForeignKeyClause::isSet() const
{
    return m_override.size() || m_table.size();
}

QString ForeignKeyClause::toString() const
{
    if(!isSet())
        return QString();

    if(m_override.size())
        return m_override;

    QString result = escapeIdentifier(m_table);

    if(m_columns.size())
    {
        result += "(";
        foreach(const QString& column, m_columns)
            result += escapeIdentifier(column) + ',';
        result.chop(1); // Remove last comma
        result += ")";
    }

    if(m_constraint.size())
        result += " " + m_constraint;

    return result;
}

void ForeignKeyClause::setFromString(const QString& fk)
{
    m_override = fk;
}

QString ForeignKeyClause::toSql(const FieldVector& applyOn) const
{
    QString result;
    if(!m_name.isNull())
        result += QString("CONSTRAINT %1 ").arg(escapeIdentifier(m_name));
    result += QString("FOREIGN KEY(%1) REFERENCES %2").arg(fieldVectorToFieldNames(applyOn).join(",")).arg(this->toString());

    return result;
}

QString UniqueConstraint::toSql(const FieldVector& applyOn) const
{
    QString result;
    if(!m_name.isNull())
        result += QString("CONSTRAINT %1 ").arg(escapeIdentifier(m_name));
    result += QString("UNIQUE(%1)").arg(fieldVectorToFieldNames(applyOn).join(","));

    return result;
}

QString PrimaryKeyConstraint::toSql(const FieldVector& applyOn) const
{
    QString result;
    if(!m_name.isNull())
        result += QString("CONSTRAINT %1 ").arg(escapeIdentifier(m_name));
    result += QString("PRIMARY KEY(%1)").arg(fieldVectorToFieldNames(applyOn).join(","));

    return result;
}

QString CheckConstraint::toSql(const FieldVector&) const
{
    QString result;
    if(!m_name.isNull())
        result += QString("CONSTRAINT %1 ").arg(escapeIdentifier(m_name));
    result += QString("CHECK(%1)").arg(m_expression);

    return result;
}

QString Field::toString(const QString& indent, const QString& sep) const
{
    QString str = indent + escapeIdentifier(m_name) + sep + m_type;
    if(m_notnull)
        str += " NOT NULL";
    if(!m_defaultvalue.isEmpty())
        str += QString(" DEFAULT %1").arg(m_defaultvalue);
    if(!m_check.isEmpty())
        str += " CHECK(" + m_check + ")";
    if(m_autoincrement)
        str += " PRIMARY KEY AUTOINCREMENT";
    if(m_unique)
        str += " UNIQUE";
    if(!m_collation.isEmpty())
        str += " COLLATE " + m_collation;
    return str;
}

bool Field::isText() const
{
    QString norm = m_type.trimmed().toLower();

    return     norm.startsWith("character")
            || norm.startsWith("varchar")
            || norm.startsWith("varying character")
            || norm.startsWith("nchar")
            || norm.startsWith("native character")
            || norm.startsWith("nvarchar")
            || norm == "text"
            || norm == "clob";
}

bool Field::isInteger() const
{
    QString norm = m_type.trimmed().toLower();

    return     norm == "int"
            || norm == "integer"
            || norm == "tinyint"
            || norm == "smallint"
            || norm == "mediumint"
            || norm == "bigint"
            || norm == "unsigned big int"
            || norm == "int2"
            || norm == "int8";
}

void Table::clear()
{
    m_rowidColumn = "_rowid_";
    m_fields.clear();
    m_constraints.clear();
    m_virtual = QString();
    m_temporary = false;
}
Table::~Table()
{
    clear();
}

Table& Table::operator=(const Table& rhs)
{
    // Base class
    Object::operator=(rhs);

    // Just assign the strings
    m_rowidColumn = rhs.m_rowidColumn;
    m_virtual = rhs.m_virtual;

    // Clear the fields and the constraints first in order to avoid duplicates and/or old data in the next step
    m_fields.clear();
    m_constraints.clear();

    // Make copies of the fields and the constraints. This is necessary in order to avoid any unwanted changes to the application's main database
    // schema representation just modifying a reference to the fields or constraints and thinking it operates on a copy.
    foreach(FieldPtr f, rhs.m_fields)
        addField(FieldPtr(new Field(*f)));
    for(auto it=rhs.m_constraints.constBegin();it!=rhs.m_constraints.constEnd();++it)   // TODO This is so ugly, it should be replaced really by anything else
    {
        FieldVector key;
        ConstraintPtr constraint;
        foreach(FieldPtr f, it.key())
            key.push_back(m_fields.at(findField(f->name())));
        if(it.value()->type() == Constraint::ConstraintTypes::PrimaryKeyConstraintType)
            constraint = ConstraintPtr(new PrimaryKeyConstraint(*(it.value().dynamicCast<PrimaryKeyConstraint>())));
        else if(it.value()->type() == Constraint::ConstraintTypes::UniqueConstraintType)
            constraint = ConstraintPtr(new UniqueConstraint(*(it.value().dynamicCast<UniqueConstraint>())));
        else if(it.value()->type() == Constraint::ConstraintTypes::ForeignKeyConstraintType)
            constraint = ConstraintPtr(new ForeignKeyClause(*(it.value().dynamicCast<ForeignKeyClause>())));
        else if(it.value()->type() == Constraint::ConstraintTypes::CheckConstraintType)
            constraint = ConstraintPtr(new CheckConstraint(*(it.value().dynamicCast<CheckConstraint>())));
        else
            qWarning() << "Unknown constraint type";
        addConstraint(key, constraint);
    }

    return *this;
}

void Table::addField(const FieldPtr& f)
{
    m_fields.append(FieldPtr(f));
}

bool Table::removeField(const QString& sFieldName)
{
    int index = findField(sFieldName);
    if( index != -1)
    {
        m_fields.remove(index);
        return true;
    }
    return false;
}

void Table::setFields(const FieldVector &fields)
{
    clear();
    m_fields = fields;
}

void Table::setField(int index, FieldPtr f)
{
    FieldPtr oldField = m_fields[index];
    m_fields[index] = f;

    // Update all constraints. If an existing field is updated but was used in a constraint, the pointers in the constraint key needs to be updated
    // to the new field, too.
    if(oldField)
    {
        ConstraintMap::iterator it = m_constraints.begin();
        while(it != m_constraints.end())
        {
            // Loop through all fields mentioned in a foreign key
            FieldVector fields = it.key();
            bool modified = false;
            for(int i=0;i<fields.size();++i)
            {
                // If the field that is being modified is in there update it to the new field and set a flag that something has changed.
                // This is used below to know when to update the map key
                if(fields[i] == oldField)
                {
                    fields[i] = f;
                    modified = true;
                }
            }
            if(modified)
            {
                // When we need to update the map key, we insert a new constraint using the updated field vector and the old
                // constraint information, and delete the old one afterwards
                m_constraints.insert(fields, it.value());
                it = m_constraints.erase(it);
            } else {
                ++it;
            }
        }
    }
}

int Table::findField(const QString &sname) const
{
    for(int i = 0; i < m_fields.count(); ++i)
    {
        if(m_fields.at(i)->name().compare(sname, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

int Table::findPk() const
{
    // TODO This is a stupid function (and always was) which should be fixed/improved

    FieldVector pk = primaryKey();
    if(pk.empty())
        return -1;
    else
        return findField(pk.at(0)->name());
}

QStringList Table::fieldList() const
{
    QStringList sl;

    foreach(const FieldPtr& f, m_fields) {
        sl << f->toString();
    }

    return sl;
}

QStringList Table::fieldNames() const
{
    QStringList sl;

    foreach(FieldPtr f, m_fields)
        sl << f->name();

    return sl;
}

FieldInfoList Table::fieldInformation() const
{
    FieldInfoList result;
    foreach(FieldPtr f, m_fields)
        result.append({f->name(), f->type(), f->toString("  ", " ")});
    return result;
}

bool Table::hasAutoIncrement() const
{
    foreach(FieldPtr f, m_fields) {
        if(f->autoIncrement())
            return true;
    }
    return false;
}

ObjectPtr Table::parseSQL(const QString &sSQL)
{
    std::stringstream s;
    s << sSQL.toStdString();
    Sqlite3Lexer lex(s);

    Sqlite3Parser parser(lex);

    antlr::ASTFactory ast_factory;
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    try
    {
        parser.createtable();
        CreateTableWalker ctw(parser.getAST());

        return ctw.table();
    }
    catch(antlr::ANTLRException& ex)
    {
        qCritical() << "Sqlite parse error: " << QString::fromStdString(ex.toString()) << "(" << sSQL << ")";
    }
    catch(...)
    {
        qCritical() << "Sqlite parse error: " << sSQL; //TODO
    }

    return TablePtr(new Table(""));
}

QString Table::sql(bool ifNotExists) const
{
    // Special handling for virtual tables: just build an easy create statement and copy the using part in there
    if(isVirtual())
        return QString("CREATE VIRTUAL TABLE %1 USING %2;").arg(escapeIdentifier(m_name)).arg(m_virtual);

    // This is a normal table, not a virtual one
    QString sql = QString("CREATE %1TABLE%2 %3 (\n")
            .arg(m_temporary ? QString("TEMPORARY ") : QString(""))
            .arg(ifNotExists ? QString(" IF NOT EXISTS") : QString(""))
            .arg(escapeIdentifier(m_name));

    sql += fieldList().join(",\n");

    // Constraints
    ConstraintMap::const_iterator it = m_constraints.constBegin();
    bool autoincrement = hasAutoIncrement();
    while(it != m_constraints.constEnd())
    {
        // Ignore auto increment primary key constraint
        if((!autoincrement || it.value()->type() != Constraint::PrimaryKeyConstraintType))
        {
            // Ignore all constraints without any fields, except for check constraints which don't rely on a field vector
            if(!(it.key().isEmpty() && it.value()->type() != Constraint::CheckConstraintType))
            {
                sql += QString(",\n\t");
                sql += it.value()->toSql(it.key());
            }
        }
        ++it;
    }

    sql += "\n)";

    // without rowid
    if(isWithoutRowidTable())
        sql += " WITHOUT ROWID";

    return sql + ";";
}

void Table::addConstraint(FieldVector fields, ConstraintPtr constraint)
{
    m_constraints.insert(fields, constraint);
}

void Table::setConstraint(FieldVector fields, ConstraintPtr constraint)
{
    // Delete any old constraints of this type for these fields
    removeConstraints(fields, constraint->type());

    // Add the new constraint to the table, effectively overwriting all old constraints for that fields/type combination
    addConstraint(fields, constraint);
}

void Table::removeConstraints(FieldVector fields, Constraint::ConstraintTypes type)
{
    QList<ConstraintPtr> list = constraints(fields, type);

    for (ConstraintPtr c : list) {
        m_constraints.remove(fields, c);
    }
}

ConstraintPtr Table::constraint(FieldVector fields, Constraint::ConstraintTypes type) const
{
    QList<ConstraintPtr> list = constraints(fields, type);
    if(list.size())
        return list.at(0);
    else
        return ConstraintPtr(nullptr);
}

QList<ConstraintPtr> Table::constraints(FieldVector fields, Constraint::ConstraintTypes type) const
{
    QList<ConstraintPtr> clist;
    if(fields.isEmpty())
        clist = m_constraints.values();
    else
        clist = m_constraints.values(fields);

    if(type == Constraint::NoType)
    {
        return clist;
    } else {
        QList<ConstraintPtr> clist_typed;
        foreach(const ConstraintPtr& ptr, clist)
        {
            if(ptr->type() == type)
                clist_typed.push_back(ptr);
        }
        return clist_typed;
    }
}

void Table::setConstraints(const ConstraintMap& constraints)
{
    m_constraints = constraints;
}

FieldVector& Table::primaryKeyRef()
{
    return const_cast<FieldVector&>(static_cast<const Table*>(this)->primaryKey());
}

const FieldVector& Table::primaryKey() const
{
    auto it = m_constraints.constBegin();
    while(it != m_constraints.constEnd())
    {
        if(it.value()->type() == Constraint::PrimaryKeyConstraintType)
            return it.key();
        ++it;
    }

    static FieldVector emptyFieldVector;
    return emptyFieldVector;
}

namespace
{
QString identifier(antlr::RefAST ident)
{
    QString sident = ident->getText().c_str();
    if(ident->getType() == sqlite3TokenTypes::QUOTEDID ||
       ident->getType() == Sqlite3Lexer::QUOTEDLITERAL ||
       ident->getType() == sqlite3TokenTypes::STRINGLITERAL)
    {
        // Remember the way the identifier is quoted
        QChar quoteChar = sident.at(0);

        // Remove first and final character, i.e. the quotes
        sident.remove(0, 1);
        sident.chop(1);

        // Replace all remaining occurences of two succeeding quote characters and replace them
        // by a single instance. This is done because two quotes can be used as a means of escaping
        // the quote character, thus only the visual representation has its two quotes, the actual
        // name contains only one.
        sident.replace(QString(quoteChar) + quoteChar, quoteChar);
    }

    return sident;
}

QString textAST(antlr::RefAST t)
{
    // When this is called for a KEYWORDASTABLENAME token, we must take the child's content to get the actual value
    // instead of 'KEYWORDASTABLENAME' as a string. The same applies for  KEYWORDASCOLUMNNAME tokens.
    if(t != antlr::nullAST && (t->getType() == sqlite3TokenTypes::KEYWORDASTABLENAME || t->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME))
        return t->getFirstChild()->getText().c_str();
    else
        return t->getText().c_str();
}

QString concatTextAST(antlr::RefAST t, bool withspace = false)
{
    QStringList stext;
    while(t != antlr::nullAST)
    {
        stext.append(textAST(t));
        t = t->getNextSibling();
    }
    return stext.join(withspace ? " " : "");
}
}

namespace {
QString tablename(const antlr::RefAST& n)
{
    if(n->getType() == sqlite3TokenTypes::KEYWORDASTABLENAME)
        return concatTextAST(n->getFirstChild());
    else
        return identifier(n);
}
QString columnname(const antlr::RefAST& n)
{
    if(n->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME)
        return concatTextAST(n->getFirstChild());
    else if(n->getType() == sqlite3TokenTypes::INDEXEDCOLUMN)
        return columnname(n->getFirstChild());
    else
        return identifier(n);
}
}

TablePtr CreateTableWalker::table()
{
    Table* tab = new Table("");
    tab->setFullyParsed(true);

    if( m_root ) //CREATE TABLE
    {
        antlr::RefAST s = m_root->getFirstChild();

        // If the primary tree isn't filled, this isn't a normal CREATE TABLE statement. Switch to the next alternative tree.
        if(s == 0)
            s = m_root->getNextSibling();

        // Skip to table name
        bool is_virtual_table = false;
        while(s->getType() != Sqlite3Lexer::ID &&
              s->getType() != Sqlite3Lexer::QUOTEDID &&
              s->getType() != Sqlite3Lexer::QUOTEDLITERAL &&
              s->getType() != Sqlite3Lexer::STRINGLITERAL &&
              s->getType() != sqlite3TokenTypes::KEYWORDASTABLENAME)
        {
            // Is this one of these virtual tables?
            if(s->getType() == Sqlite3Lexer::VIRTUAL)
                is_virtual_table = true;

            s = s->getNextSibling();
        }

        // Extract and set table name
        tab->setName(tablename(s));

        // Special handling for virtual tables. If this is a virtual table, extract the USING part and skip all the
        // rest of this function because virtual tables don't have column definitons
        if(is_virtual_table)
        {
            s = s->getNextSibling(); // USING
            s = s->getNextSibling(); // module name
            tab->setVirtualUsing(concatTextAST(s, true));
            tab->setFullyParsed(false);

            return TablePtr(tab);
        }

        // This is a normal table, not a virtual one
        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::COLUMNDEF)
        {
            parsecolumn(tab, column->getFirstChild());
            column = column->getNextSibling(); //COMMA or RPAREN
            column = column->getNextSibling(); //null or tableconstraint

            s = s->getNextSibling();            // COLUMNDEF
            s = s->getNextSibling();            // COMMA or RPAREN
        }

        // now we are finished or it is a tableconstraint
        while(s != antlr::nullAST)
        {
            // Is this a 'without rowid' definiton?
            if(s->getType() != sqlite3TokenTypes::WITHOUT)
            {
                // It's not, so treat this as table constraints

                antlr::RefAST tc = s->getFirstChild();

                // Extract constraint name, if there is any
                QString constraint_name;
                if(tc->getType() == sqlite3TokenTypes::CONSTRAINT)
                {
                    tc = tc->getNextSibling();          // CONSTRAINT
                    constraint_name = identifier(tc);
                    tc = tc->getNextSibling();          // identifier
                }

                switch(tc->getType())
                {
                case sqlite3TokenTypes::PRIMARY:
                {
                    PrimaryKeyConstraint* pk = new PrimaryKeyConstraint;
                    pk->setName(constraint_name);

                    tc = tc->getNextSibling()->getNextSibling(); // skip primary and key
                    tc = tc->getNextSibling(); // skip LPAREN

                    FieldVector fields;
                    do
                    {
                        antlr::RefAST indexed_column = tc->getFirstChild();

                        QString col = columnname(indexed_column);
                        FieldPtr field = tab->field(tab->findField(col));
                        fields.push_back(field);

                        indexed_column = indexed_column->getNextSibling();
                        if(indexed_column != antlr::nullAST
                                && (indexed_column->getType() == sqlite3TokenTypes::ASC
                                    || indexed_column->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::COLLATE)
                        {
                            indexed_column = indexed_column->getNextSibling();      // COLLATE
                            // TODO save collation name
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();      // collation name
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::AUTOINCREMENT)
                        {
                            field->setAutoIncrement(true);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        tc = tc->getNextSibling();      // indexed column

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    tab->addConstraint(fields, ConstraintPtr(pk));
                }
                break;
                case sqlite3TokenTypes::UNIQUE:
                {
                    UniqueConstraint* unique = new UniqueConstraint;
                    unique->setName(constraint_name);

                    tc = tc->getNextSibling(); // skip UNIQUE
                    tc = tc->getNextSibling(); // skip LPAREN
                    FieldVector fields;
                    do
                    {
                        antlr::RefAST indexed_column = tc->getFirstChild();

                        QString col = columnname(indexed_column);
                        FieldPtr field = tab->field(tab->findField(col));
                        fields.push_back(field);

                        indexed_column = indexed_column->getNextSibling();
                        if(indexed_column != antlr::nullAST
                                && (indexed_column->getType() == sqlite3TokenTypes::ASC
                                    || indexed_column->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();
                        }

                        if(indexed_column != antlr::nullAST && indexed_column->getType() == sqlite3TokenTypes::COLLATE)
                        {
                            indexed_column = indexed_column->getNextSibling();      // COLLATE
                            // TODO save collation name
                            tab->setFullyParsed(false);
                            indexed_column = indexed_column->getNextSibling();      // collation name
                        }

                        tc = tc->getNextSibling();      // indexed column

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    if(fields.size() == 1 && constraint_name.isEmpty())
                    {
                        fields[0]->setUnique(true);
                        delete unique;
                    } else {
                        tab->addConstraint(fields, ConstraintPtr(unique));
                    }
                }
                break;
                case sqlite3TokenTypes::FOREIGN:
                {
                    ForeignKeyClause* fk = new ForeignKeyClause;
                    fk->setName(constraint_name);

                    tc = tc->getNextSibling();  // FOREIGN
                    tc = tc->getNextSibling();  // KEY
                    tc = tc->getNextSibling();  // LPAREN

                    FieldVector fields;
                    do
                    {
                        QString col = columnname(tc);
                        FieldPtr field = tab->field(tab->findField(col));
                        fields.push_back(field);

                        tc = tc->getNextSibling();

                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                            tc = tc->getNextSibling(); // skip ident and comma
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    tc = tc->getNextSibling();
                    tc = tc->getNextSibling();  // REFERENCES

                    fk->setTable(identifier(tc));
                    tc = tc->getNextSibling();       // identifier

                    if(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::LPAREN)
                    {
                        tc = tc->getNextSibling();  // LPAREN

                        QStringList fk_cols;
                        while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN)
                        {
                            if(tc->getType() != sqlite3TokenTypes::COMMA)
                                fk_cols.push_back(identifier(tc));
                            tc = tc->getNextSibling();
                        }
                        fk->setColumns(fk_cols);

                        tc = tc->getNextSibling();  // RPAREN
                    }

                    fk->setConstraint(concatTextAST(tc, true));
                    tab->addConstraint(fields, ConstraintPtr(fk));
                }
                break;
                case sqlite3TokenTypes::CHECK:
                {
                    CheckConstraint* check = new CheckConstraint;
                    check->setName(constraint_name);

                    tc = tc->getNextSibling(); // skip CHECK
                    tc = tc->getNextSibling(); // skip LPAREN

                    int num_paren = 1;
                    QString expr;
                    while(tc)
                    {
                        if(tc->getType() == sqlite3TokenTypes::LPAREN)
                            num_paren++;
                        else if(tc->getType() == sqlite3TokenTypes::RPAREN)
                            num_paren--;

                        if(num_paren == 0)
                            break;

                        expr.append(textAST(tc));

                        tc = tc->getNextSibling();
                    }

                    check->setExpression(expr);
                    tab->addConstraint(FieldVector(), ConstraintPtr(check));
                }
                break;
                default:
                {
                    qWarning() << "unknown table constraint in " << tab->name();
                    tab->setFullyParsed(false);
                }
                    break;
                }

                s = s->getNextSibling(); //COMMA or RPAREN
                if(s->getType() == sqlite3TokenTypes::COMMA || s->getType() == sqlite3TokenTypes::RPAREN)
                    s = s->getNextSibling();
            } else {
                // It is

                s = s->getNextSibling();    // WITHOUT
                s = s->getNextSibling();    // ROWID

                tab->setRowidColumn(tab->fields().at(tab->findPk())->name());
            }
        }
    }

    return TablePtr(tab);
}

void CreateTableWalker::parsecolumn(Table* table, antlr::RefAST c)
{
    QString colname;
    QString type = "TEXT";
    bool autoincrement = false;
    bool notnull = false;
    bool unique = false;
    QString defaultvalue;
    QString check;
    QString collation;
    sqlb::PrimaryKeyConstraint* primaryKey = 0;
    sqlb::ForeignKeyClause* foreignKey = 0;

    colname = columnname(c);
    c = c->getNextSibling(); //type?
    if(c != antlr::nullAST && c->getType() == sqlite3TokenTypes::TYPE_NAME)
    {
        type = concatTextAST(c->getFirstChild(), true);
        c = c->getNextSibling();
    }

    // finished with type parsing
    // now columnconstraints
    while(c != antlr::nullAST)
    {
        antlr::RefAST con = c->getFirstChild();

        // Extract constraint name, if there is any
        QString constraint_name;
        if(con->getType() == sqlite3TokenTypes::CONSTRAINT)
        {
            con = con->getNextSibling();          // CONSTRAINT
            constraint_name = identifier(con);
            con = con->getNextSibling();          // identifier
        }

        switch(con->getType())
        {
        case sqlite3TokenTypes::PRIMARY:
        {
            primaryKey = new PrimaryKeyConstraint;
            primaryKey->setName(constraint_name);

            con = con->getNextSibling()->getNextSibling(); // skip KEY
            if(con != antlr::nullAST && (con->getType() == sqlite3TokenTypes::ASC
                                         || con->getType() == sqlite3TokenTypes::DESC))
            {
                table->setFullyParsed(false);
                con = con->getNextSibling(); //skip
            }
            if(con != antlr::nullAST && con->getType() == sqlite3TokenTypes::AUTOINCREMENT)
                autoincrement = true;
        }
        break;
        case sqlite3TokenTypes::NOT:
        {
            // TODO Support constraint names here
            if(!constraint_name.isEmpty())
                table->setFullyParsed(false);

            notnull = true;
        }
        break;
        case sqlite3TokenTypes::NULL_T:
        {
            notnull = false;
        }
        break;
        case sqlite3TokenTypes::CHECK:
        {
            // TODO Support constraint names here
            if(!constraint_name.isEmpty())
                table->setFullyParsed(false);

            con = con->getNextSibling(); //LPAREN
            check = concatTextAST(con, true);
            // remove parenthesis
            check.remove(check.length()-1, 1);
            check.remove(0,1);
            check = check.trimmed();
        }
        break;
        case sqlite3TokenTypes::DEFAULT:
        {
            // TODO Support constraint names here
            if(!constraint_name.isEmpty())
                table->setFullyParsed(false);

            con = con->getNextSibling(); //SIGNEDNUMBER,STRING,LPAREN
            defaultvalue = concatTextAST(con);
        }
        break;
        case sqlite3TokenTypes::UNIQUE:
        {
            // TODO Support constraint names here
            if(!constraint_name.isEmpty())
                table->setFullyParsed(false);

            unique = true;
        }
        break;
        case sqlite3TokenTypes::REFERENCES:
        {
            con = con->getNextSibling();    // REFERENCES

            foreignKey = new ForeignKeyClause;
            foreignKey->setTable(identifier(con));
            foreignKey->setName(constraint_name);
            con = con->getNextSibling();    // identifier

            if(con != antlr::nullAST && con->getType() == sqlite3TokenTypes::LPAREN)
            {
                con = con->getNextSibling();    // LPAREN

                QStringList fk_cols;
                while(con != antlr::nullAST && con->getType() != sqlite3TokenTypes::RPAREN)
                {
                    if(con->getType() != sqlite3TokenTypes::COMMA)
                        fk_cols.push_back(identifier(con));
                    con = con->getNextSibling();
                }
                foreignKey->setColumns(fk_cols);

                con = con->getNextSibling();    // RPAREN
            }

            foreignKey->setConstraint(concatTextAST(con, true));
        }
        break;
        case sqlite3TokenTypes::COLLATE:
        {
            con = con->getNextSibling();    // COLLATE
            collation = identifier(con);
            con = con->getNextSibling();    // collation name
        }
        break;
        default:
        {
            qWarning() << "unknown column constraint in " << table->name() << "." << colname;
            table->setFullyParsed(false);
        }
        break;
        }
        c = c->getNextSibling();
    }

    FieldPtr f = FieldPtr(new Field(colname, type, notnull, defaultvalue, check, unique, collation));
    f->setAutoIncrement(autoincrement);
    table->addField(f);

    if(foreignKey)
        table->addConstraint({f}, ConstraintPtr(foreignKey));
    if(primaryKey)
    {
        FieldVector v;
        if(table->constraint(v, Constraint::PrimaryKeyConstraintType))
        {
            table->primaryKeyRef().push_back(f);

            // Delete useless primary key constraint. There already is a primary key object for this table, we
            // don't need another one.
            delete primaryKey;
        } else {
            table->addConstraint({f}, ConstraintPtr(primaryKey));
        }
    }
}



QString IndexedColumn::toString(const QString& indent, const QString& sep) const
{
    QString name = m_isExpression ? m_name : escapeIdentifier(m_name);
    QString order = (m_order.isEmpty() ? "" : (sep + m_order));
    return indent + name + order;
}

Index::~Index()
{
    clear();
}

Index& Index::operator=(const Index& rhs)
{
    // Base class
    Object::operator=(rhs);

    // Just assign the easy stuff
    m_unique = rhs.m_unique;
    m_table = rhs.m_table;
    m_whereExpr = rhs.m_whereExpr;

    // Make copies of the column
    foreach(IndexedColumnPtr c, rhs.m_columns)
        addColumn(IndexedColumnPtr(new IndexedColumn(*c)));

    return *this;
}

void Index::clear()
{
    m_name.clear();
    m_unique = false;
    m_table.clear();
    m_whereExpr.clear();
    m_columns.clear();
}

bool Index::removeColumn(const QString& name)
{
    int index = findColumn(name);
    if(index != -1)
    {
        m_columns.remove(index);
        return true;
    }
    return false;
}

void Index::setColumns(const IndexedColumnVector& columns)
{
    m_columns = columns;
}

int Index::findColumn(const QString& name) const
{
    for(int i=0;i<m_columns.count();++i)
    {
        if(m_columns.at(i)->name().compare(name, Qt::CaseInsensitive) == 0)
            return i;
    }
    return -1;
}

QStringList Index::columnSqlList() const
{
    QStringList sl;

    foreach(const IndexedColumnPtr& c, m_columns)
        sl << c->toString();

    return sl;
}

QString Index::sql(bool ifNotExists) const
{
    // Start CREATE (UNIQUE) INDEX statement
    QString sql = QString("CREATE %1INDEX%2 %3 ON %4 (\n")
            .arg(m_unique ? QString("UNIQUE ") : QString(""))
            .arg(ifNotExists ? QString(" IF NOT EXISTS") : QString(""))
            .arg(escapeIdentifier(m_name))
            .arg(escapeIdentifier(m_table));

    // Add column list
    sql += columnSqlList().join(",\n");

    // Add partial index bit
    sql += QString("\n)");
    if(!m_whereExpr.isEmpty())
        sql += QString(" WHERE ") + m_whereExpr;

    return sql + ";";
}

FieldInfoList Index::fieldInformation() const
{
    FieldInfoList result;
    foreach(IndexedColumnPtr c, m_columns)
        result.append({c->name(), c->order(), c->toString("  ", " ")});
    return result;
}

ObjectPtr Index::parseSQL(const QString& sSQL)
{
    std::stringstream s;
    s << sSQL.toStdString();
    Sqlite3Lexer lex(s);

    Sqlite3Parser parser(lex);

    antlr::ASTFactory ast_factory;
    parser.initializeASTFactory(ast_factory);
    parser.setASTFactory(&ast_factory);

    try
    {
        parser.createindex();
        CreateIndexWalker ctw(parser.getAST());

        return ctw.index();
    }
    catch(antlr::ANTLRException& ex)
    {
        qCritical() << "Sqlite parse error: " << QString::fromStdString(ex.toString()) << "(" << sSQL << ")";
    }
    catch(...)
    {
        qCritical() << "Sqlite parse error: " << sSQL; //TODO
    }

    return IndexPtr(new Index(""));
}

IndexPtr CreateIndexWalker::index()
{
    Index* index = new Index("");
    index->setFullyParsed(true);

    if(m_root)  // CREATE INDEX
    {
        antlr::RefAST s = m_root->getFirstChild();

        // Skip to index name
        while(s->getType() != Sqlite3Lexer::ID &&
              s->getType() != Sqlite3Lexer::QUOTEDID &&
              s->getType() != Sqlite3Lexer::QUOTEDLITERAL &&
              s->getType() != Sqlite3Lexer::STRINGLITERAL &&
              s->getType() != sqlite3TokenTypes::KEYWORDASTABLENAME)
        {
            // Is this a unique index?
            if(s->getType() == Sqlite3Lexer::UNIQUE)
                index->setUnique(true);

            s = s->getNextSibling();
        }

        // Extract and set index name
        index->setName(tablename(s));

        // Get table name
        s = s->getNextSibling(); // ON
        s = s->getNextSibling(); // table name
        index->setTable(tablename(s));

        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::INDEXEDCOLUMN)
        {
            parsecolumn(index, column->getFirstChild());
            column = column->getNextSibling(); // COMMA or RPAREN
            column = column->getNextSibling(); // null or WHERE

            s = s->getNextSibling();            // COLUMNDEF
            s = s->getNextSibling();            // COMMA or RPAREN
        }

        // Now we are finished or it is a partial index
        if(s != antlr::nullAST)
        {
            // This should be a 'where' then
            if(s->getType() != sqlite3TokenTypes::WHERE)
            {
                // It is something else
                index->setFullyParsed(false);
            } else {
                s = s->getNextSibling();        // expr
                index->setWhereExpr(concatTextAST(s, true));
            }
        }
    }

    return IndexPtr(index);
}

void CreateIndexWalker::parsecolumn(Index* index, antlr::RefAST c)
{
    QString name;
    bool isExpression;
    QString order;

    // First count the number of nodes used for the name or the expression. We reach the end of the name nodes list when we either
    // get to the end of the list, get to a COMMA or a RPAREN, or get to the COLLATE keyword or get to the ASC/DESC keywords.
    // Then see how many items there are: if it's one it's a normal index column with only a column name. In this case get the identifier.
    // If it's more than one item it's an expression. In this case get all the items as they are.
    int number_of_name_items = 0;
    antlr::RefAST n = c;
    while(n != antlr::nullAST
          && n->getType() != sqlite3TokenTypes::COLLATE
          && n->getType() != sqlite3TokenTypes::ASC
          && n->getType() != sqlite3TokenTypes::DESC
          && n->getType() != sqlite3TokenTypes::COMMA
          && n->getType() != sqlite3TokenTypes::RPAREN)
    {
        number_of_name_items++;
        n = n->getNextSibling();
    }
    if(number_of_name_items == 1)
    {
        name = identifier(c);
        isExpression = false;
        c = c->getNextSibling();
    } else {
        for(int i=0;i<number_of_name_items;i++)
        {
            name += c->getText().c_str() + QString(" ");
            c = c->getNextSibling();
        }
        name.chop(1);
        isExpression = true;
    }

    // Parse the rest of the column definition
    while(c != antlr::nullAST)
    {
        switch(c->getType())
        {
        case sqlite3TokenTypes::ASC:
        case sqlite3TokenTypes::DESC:
            order = c->getText().c_str();
            break;
        default:
            // TODO Add support for COLLATE
            index->setFullyParsed(false);
        }

        c = c->getNextSibling();
    }

    index->addColumn(IndexedColumnPtr(new IndexedColumn(name, isExpression, order)));
}



View::~View()
{
    clear();
}

ObjectPtr View::parseSQL(const QString& /*sSQL*/)
{
    // TODO

    return ViewPtr(new View(""));
}

void View::clear()
{
    m_fields.clear();
}

void View::addField(const FieldPtr& f)
{
    m_fields.append(FieldPtr(f));
}

void View::setFields(const FieldVector& fields)
{
    clear();
    m_fields = fields;
}

QStringList View::fieldNames() const
{
    QStringList sl;

    foreach(FieldPtr f, m_fields)
        sl << f->name();

    return sl;
}

FieldInfoList View::fieldInformation() const
{
    FieldInfoList result;
    foreach(FieldPtr f, m_fields)
        result.append({f->name(), f->type(), f->toString("  ", " ")});
    return result;
}



ObjectPtr Trigger::parseSQL(const QString& /*sSQL*/)
{
    // TODO

    return TriggerPtr(new Trigger(""));
}

} //namespace sqlb
