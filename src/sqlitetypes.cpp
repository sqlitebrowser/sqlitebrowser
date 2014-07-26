#include "sqlitetypes.h"
#include "grammar/Sqlite3Lexer.hpp"
#include "grammar/Sqlite3Parser.hpp"

#include <sstream>
#include <QDebug>

namespace sqlb {

QStringList Field::Datatypes = QStringList() << "INTEGER" << "TEXT" << "BLOB" << "REAL" << "NUMERIC";

QString Field::toString(const QString& indent, const QString& sep) const
{
    QString str = indent + '`' + m_name + '`' + sep + m_type;
    if(m_notnull)
        str += " NOT NULL";
    if(!m_defaultvalue.isEmpty())
        str += QString(" DEFAULT '%1'").arg(m_defaultvalue);
    if(!m_check.isEmpty())
        str += " CHECK(" + m_check + ")";
    if(m_autoincrement)
        str += " PRIMARY KEY AUTOINCREMENT";
    return str;
}

bool Field::isText() const
{
    QString norm = m_type.trimmed().toLower();

    return     norm.startsWith("character")
            || norm.startsWith("varchar")
            || norm.startsWith("varying character")
            || norm.startsWith("nchar")
            || norm.startsWith("native charactar")
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
    m_rowidColumn = "rowid";
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
        if(sname == m_fields.at(i)->name())
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

    foreach(FieldPtr f, m_fields) {
        sl << f->toString();
    }

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

Table Table::parseSQL(const QString &sSQL)
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
    catch(...)
    {
        qCritical() << "Sqlite parse error: " << sSQL; //TODO
    }

    return Table("");
}
QString Table::emptyInsertStmt() const
{
    QString stmt = QString("INSERT INTO `%1`").arg(m_name);

    QStringList vals;
    QStringList fields;
    foreach(FieldPtr f, m_fields) {
        if(f->notnull())
        {
            fields << f->name();
            if( f->primaryKey() && f->isInteger() )
            {
                vals << "NULL";
            } else {
                if(f->isInteger())
                    vals << "0";
                else
                    vals << "''";
            }
        }
        else
        {
            // don't insert into fields with a default value
            // or we will never see it.
            if(f->defaultValue().length() == 0)
            {
                fields << f->name();
                vals << "NULL";
            }
        }
    }

    if(!fields.empty())
    {
        stmt.append("(`");
        stmt.append(fields.join("`,`"));
        stmt.append("`)");
    }
    stmt.append(" VALUES (");
    stmt.append(vals.join(","));
    stmt.append(");");

    return stmt;
}


QString Table::sql() const
{
    QString sql = QString("CREATE TABLE `%1` (\n").arg(m_name);

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
    sql += "\n)";

    // without rowid
    if(m_rowidColumn != "rowid")
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
        sident.remove(0, 1);
        sident.remove(sident.length() - 1, 1);
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

        tab.setName(identifier(s));

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
                        QString col = identifier(tc);
                        int fieldindex = tab.findField(col);
                        if(fieldindex != -1)
                            tab.fields().at(fieldindex)->setPrimaryKey(true);
                        do
                        {
                            tc = tc->getNextSibling(); // skip ident and comma
                        } while(tc->getType() == sqlite3TokenTypes::COMMA);
                    } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);
                }
                break;
                default: break;
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
    QString columnname;
    QString type = "TEXT";
    bool autoincrement = false;
    bool primarykey = false;
    bool notnull = false;
    QString defaultvalue;
    QString check;

    if(c->getType() == sqlite3TokenTypes::KEYWORDASCOLUMNNAME)
        columnname = concatTextAST(c->getFirstChild());
    else
        columnname = identifier(c);
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
            con = con->getNextSibling()->getNextSibling();

        switch(con->getType())
        {
        case sqlite3TokenTypes::PRIMARY:
        {
            primarykey = true;
            con = con->getNextSibling()->getNextSibling(); // skip KEY
            if(con != antlr::nullAST && (con->getType() == sqlite3TokenTypes::ASC
                                         || con->getType() == sqlite3TokenTypes::DESC))
                con = con->getNextSibling(); //skip
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
        default: break;
        }
        c = c->getNextSibling();
    }

    f = FieldPtr( new Field(columnname, type, notnull, defaultvalue, check, primarykey));
    f->setAutoIncrement(autoincrement);
}

} //namespace sqlb
