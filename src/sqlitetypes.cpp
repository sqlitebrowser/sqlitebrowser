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
    m_fields.clear();
    m_rowidColumn = "_rowid_";
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

int Table::findField(const QString &sname)
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
    for(int i = 0; i < m_fields.count(); ++i)
    {
        if(m_fields.at(i)->primaryKey())
            return i;
    }
    return -1;
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
        return qMakePair(ctw.table(), ctw.modifysupported());
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
    QString sql = QString("CREATE TABLE %1 (\n").arg(escapeIdentifier(m_name));

    sql += fieldList().join(",\n");

    // primary key
    if(!hasAutoIncrement())
    {
        QString pk = ",\n\tPRIMARY KEY(";
        bool pks_found = false;
        foreach(FieldPtr f, m_fields)
        {
            if(f->primaryKey())
            {
                pk += f->name() + ",";
                pks_found = true;
            }
        }
        pk.chop(1);
        if(pks_found)
            sql += pk + ")";
    }

    // foreign keys
    foreach(FieldPtr f, m_fields)
    {
        if(f->foreignKey().isSet())
            sql += QString(",\n\tFOREIGN KEY(%1) REFERENCES %2").arg(escapeIdentifier(f->name())).arg(f->foreignKey().toString());
    }

    sql += "\n)";

    // without rowid
    if(isWithoutRowidTable())
        sql += " WITHOUT ROWID";

    return sql + ";";
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
        //skip to tablename
        while(s->getType() != Sqlite3Lexer::ID &&
              s->getType() != Sqlite3Lexer::QUOTEDID &&
              s->getType() != Sqlite3Lexer::QUOTEDLITERAL &&
              s->getType() != Sqlite3Lexer::STRINGLITERAL &&
              s->getType() != sqlite3TokenTypes::KEYWORDASTABLENAME)
        {
            s = s->getNextSibling();
        }

        tab.setName(tablename(s));

        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::COLUMNDEF)
        {
            FieldPtr f;
            parsecolumn(f, column->getFirstChild());
            tab.addField(f);
            column = column->getNextSibling(); //COMMA or RPAREN
            column = column->getNextSibling(); //null or tableconstraint
        }

        // now we are finished or it is a tableconstraint
        while(s != antlr::nullAST)
        {
            // Is this a 'without rowid' definiton?
            if(s->getType() != sqlite3TokenTypes::WITHOUT)
            {
                // It's not, so treat this as table constraints

                antlr::RefAST tc = s->getFirstChild();
                // skip constraint name, if there is any
                if(tc->getType() == sqlite3TokenTypes::CONSTRAINT)
                    tc = tc->getNextSibling()->getNextSibling();

                switch(tc->getType())
                {
                case sqlite3TokenTypes::PRIMARY:
                {
                    tc = tc->getNextSibling()->getNextSibling(); // skip primary and key
                    tc = tc->getNextSibling(); // skip LPAREN
                    do
                    {
                        QString col = columnname(tc);
                        int fieldindex = tab.findField(col);
                        if(fieldindex != -1)
                            tab.fields().at(fieldindex)->setPrimaryKey(true);

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
                            tab.fields().at(fieldindex)->setAutoIncrement(true);
                            tc = tc->getNextSibling();
                        }
                        while(tc != antlr::nullAST && tc->getType() == sqlite3TokenTypes::COMMA)
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        }
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);
                }
                break;
                case sqlite3TokenTypes::UNIQUE:
                {
                    tc = tc->getNextSibling(); // skip UNIQUE
                    tc = tc->getNextSibling(); // skip LPAREN
                    QVector<int> uniquefieldsindex;
                    do
                    {
                        QString col = columnname(tc);
                        int fieldindex = tab.findField(col);
                        if(fieldindex != -1)
                            uniquefieldsindex.append(fieldindex);

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

                    if(uniquefieldsindex.size() == 1)
                    {
                        tab.fields().at(uniquefieldsindex[0])->setUnique(true);
                    }
                    else
                    {
                        // else save on table a unique with more than one field
                        m_bModifySupported = false;
                    }
                }
                break;
                case sqlite3TokenTypes::FOREIGN:
                {
                    sqlb::ForeignKeyClause fk;

                    tc = tc->getNextSibling();  // FOREIGN
                    tc = tc->getNextSibling();  // KEY
                    tc = tc->getNextSibling();  // LPAREN
                    QString column_name = columnname(tc);
                    tc = tc->getNextSibling();  // identifier
                    if(tc->getType() == sqlite3TokenTypes::COMMA)
                    {
                        // No support for composite foreign keys
                        m_bModifySupported = false;
                        break;
                    }
                    tc = tc->getNextSibling();  // RPAREN
                    tc = tc->getNextSibling();  // REFERENCES

                    fk.setTable(identifier(tc));
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
                        fk.setColumns(fk_cols);

                        tc = tc->getNextSibling();  // RPAREN
                    }

                    fk.setConstraint(concatTextAST(tc, true));

                    tab.fields().at(tab.findField(column_name))->setForeignKey(fk);
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

void CreateTableWalker::parsecolumn(FieldPtr& f, antlr::RefAST c)
{
    QString colname;
    QString type = "TEXT";
    bool autoincrement = false;
    bool primarykey = false;
    bool notnull = false;
    bool unique = false;
    QString defaultvalue;
    QString check;
    sqlb::ForeignKeyClause foreignKey;

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

        // skip constraint name, if there is any
        if(con->getType() == sqlite3TokenTypes::CONSTRAINT)
        {
            m_bModifySupported = false;
            con = con->getNextSibling()->getNextSibling();
        }

        switch(con->getType())
        {
        case sqlite3TokenTypes::PRIMARY:
        {
            primarykey = true;
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
            notnull = true;
        }
        break;
        case sqlite3TokenTypes::CHECK:
        {
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
            con = con->getNextSibling(); //SIGNEDNUMBER,STRING,LPAREN
            defaultvalue = concatTextAST(con);
        }
        break;
        case sqlite3TokenTypes::UNIQUE:
        {
            unique = true;
        }
        break;
        case sqlite3TokenTypes::REFERENCES:
        {
            con = con->getNextSibling();    // REFERENCES

            foreignKey.setTable(identifier(con));
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
                foreignKey.setColumns(fk_cols);

                con = con->getNextSibling();    // RPAREN
            }

            foreignKey.setConstraint(concatTextAST(con, true));
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

    f = FieldPtr( new Field(colname, type, notnull, defaultvalue, check, primarykey, unique));
    f->setAutoIncrement(autoincrement);
    f->setForeignKey(foreignKey);
}

} //namespace sqlb
