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
}
Table::~Table()
{
    clear();
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

bool Table::hasAutoIncrement() const
{
    foreach(FieldPtr f, m_fields) {
        if(f->autoIncrement())
            return true;
    }
    return false;
}

QPair<Table, bool> Table::parseSQL(const QString &sSQL)
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

        // Note: this needs to be done in two separate lines because otherwise the optimiser might decide to
        // fetch the value for the second part of the pair (the modify supported flag) first. If it does so it will
        // always be set to true because the table() method hasn't run yet and it's only set to false in there.
        sqlb::Table tab = ctw.table();
        return qMakePair(tab, ctw.modifysupported());
    }
    catch(antlr::ANTLRException& ex)
    {
        qCritical() << "Sqlite parse error: " << QString::fromStdString(ex.toString()) << "(" << sSQL << ")";
    }
    catch(...)
    {
        qCritical() << "Sqlite parse error: " << sSQL; //TODO
    }

    return qMakePair(Table(""), false);
}

QString Table::sql() const
{
    // Special handling for virtual tables: just build an easy create statement and copy the using part in there
    if(isVirtual())
        return QString("CREATE VIRTUAL TABLE %1 USING %2;").arg(escapeIdentifier(m_name)).arg(m_virtual);

    // This is a normal table, not a virtual one
    QString sql = QString("CREATE TABLE %1 (\n").arg(escapeIdentifier(m_name));

    sql += fieldList().join(",\n");

    // Constraints
    ConstraintMap::const_iterator it = m_constraints.constBegin();
    bool autoincrement = hasAutoIncrement();
    while(it != m_constraints.constEnd())
    {
        if((!autoincrement || it.value()->type() != Constraint::PrimaryKeyConstraintType) && !it.key().isEmpty())
        {
            sql += QString(",\n\t");
            sql += it.value()->toSql(it.key());
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

QString concatTextAST(antlr::RefAST t, bool withspace = false)
{
    // When this is called for a KEYWORDASTABLENAME token, we must take the child's content to get the actual value
    // instead of 'KEYWORDASTABLENAME' as a string. The same applies for  KEYWORDASCOLUMNNAME tokens.
    if(t != antlr::nullAST && (t->getType() == sqlite3TokenTypes::KEYWORDASTABLENAME || t->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME))
            return concatTextAST(t->getFirstChild());

    QStringList stext;
    while(t != antlr::nullAST)
    {
        stext.append(t->getText().c_str());
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
    else
        return identifier(n);
}
}

Table CreateTableWalker::table()
{
    Table tab("");

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
        tab.setName(tablename(s));

        // Special handling for virtual tables. If this is a virtual table, extract the USING part and skip all the
        // rest of this function because virtual tables don't have column definitons
        if(is_virtual_table)
        {
            s = s->getNextSibling(); // USING
            s = s->getNextSibling(); // module name
            tab.setVirtualUsing(concatTextAST(s, true));
            m_bModifySupported = false;

            // TODO Maybe get the column list using the 'pragma table_info()' approach we're using for views

            return tab;
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
                        QString col = columnname(tc);
                        FieldPtr field = tab.field(tab.findField(col));
                        fields.push_back(field);

                        tc = tc->getNextSibling();
                        if(tc != antlr::nullAST
                                && (tc->getType() == sqlite3TokenTypes::ASC
                                    || tc->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            m_bModifySupported = false;
                            tc = tc->getNextSibling();
                        }

                        if(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::AUTOINCREMENT)
                        {
                            field->setAutoIncrement(true);
                            tc = tc->getNextSibling();
                        }
                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                    tab.addConstraint(fields, ConstraintPtr(pk));
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
                        QString col = columnname(tc);
                        FieldPtr field = tab.field(tab.findField(col));
                        fields.push_back(field);

                        tc = tc->getNextSibling();
                        if(tc != antlr::nullAST
                                && (tc->getType() == sqlite3TokenTypes::ASC
                                    || tc->getType() == sqlite3TokenTypes::DESC))
                        {
                            // TODO save ASC / DESC information?
                            m_bModifySupported = false;
                            tc = tc->getNextSibling();
                        }

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
                        tab.addConstraint(fields, ConstraintPtr(unique));
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
                        FieldPtr field = tab.field(tab.findField(col));
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
                    tab.addConstraint(fields, ConstraintPtr(fk));
                }
                break;
                default:
                {
                    m_bModifySupported = false;
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

                tab.setRowidColumn(tab.fields().at(tab.findPk())->name());
            }
        }
    }

    return tab;
}

void CreateTableWalker::parsecolumn(Table& table, antlr::RefAST c)
{
    QString colname;
    QString type = "TEXT";
    bool autoincrement = false;
    bool notnull = false;
    bool unique = false;
    QString defaultvalue;
    QString check;
    sqlb::PrimaryKeyConstraint* primaryKey = 0;
    sqlb::ForeignKeyClause* foreignKey = 0;

    colname = columnname(c);
    c = c->getNextSibling(); //type?
    if(c != antlr::nullAST && c->getType() == sqlite3TokenTypes::TYPE_NAME)
    {
        type = concatTextAST(c->getFirstChild());
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
                m_bModifySupported = false;
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
                m_bModifySupported = false;

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
                m_bModifySupported = false;

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
                m_bModifySupported = false;

            con = con->getNextSibling(); //SIGNEDNUMBER,STRING,LPAREN
            defaultvalue = concatTextAST(con);
        }
        break;
        case sqlite3TokenTypes::UNIQUE:
        {
            // TODO Support constraint names here
            if(!constraint_name.isEmpty())
                m_bModifySupported = false;

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
        default:
        {
            m_bModifySupported = false;
        }
        break;
        }
        c = c->getNextSibling();
    }

    FieldPtr f = FieldPtr(new Field(colname, type, notnull, defaultvalue, check, unique));
    f->setAutoIncrement(autoincrement);
    table.addField(f);

    if(foreignKey)
        table.addConstraint({f}, ConstraintPtr(foreignKey));
    if(primaryKey)
    {
        FieldVector v;
        if(table.constraint(v, Constraint::PrimaryKeyConstraintType))
            table.primaryKeyRef().push_back(f);
        else
            table.addConstraint({f}, ConstraintPtr(primaryKey));
    }
}

} //namespace sqlb
