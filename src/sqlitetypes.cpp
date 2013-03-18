#include "sqlitetypes.h"
#include "grammar/Sqlite3Lexer.hpp"
#include "grammar/Sqlite3Parser.hpp"

#include <sstream>
#include <QDebug>

namespace sqlb {

QStringList Field::Datatypes = QStringList() << "INTEGER" << "TEXT" << "BLOB" << "REAL" << "NUMERIC";

QString Field::toString(const QString& indent, const QString& sep) const
{
    QString str = indent + m_name + sep + m_type;
    if(m_notnull)
        str += " NOT NULL";
    if(!m_defaultvalue.isEmpty())
        str += " DEFAULT " + m_defaultvalue;
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
    m_primarykey.clear();
}

Table::~Table()
{
    clear();
}

void Table::addField(const FieldPtr& f)
{
    m_fields.append(FieldPtr(f));
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

bool Table::setPrimaryKey(const FieldVector& pk)
{
    foreach(FieldPtr f, pk) {
        if(!m_fields.contains(f))
            return false;
    }

    m_primarykey = pk;
    return true;
}

bool Table::setPrimaryKey(FieldPtr pk, bool autoincrement)
{
    if(m_fields.contains(pk))
    {
        if(pk->isInteger() && autoincrement)
            pk->setAutoIncrement(true);
        m_primarykey.clear();
        m_primarykey.append(pk);
        return true;
    }

    return false;
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
        qCritical() << "Sqlite parse error."; //TODO
    }

    return Table("");
}

QString Table::sql() const
{
    QString sql = QString("CREATE TABLE `%1` (\n").arg(m_name);

    sql += fieldList().join(",\n");

    // primary key
    if( m_primarykey.size() > 0 && !hasAutoIncrement())
    {
        sql += ",\n\tPRIMARY KEY(";
        for(FieldVector::ConstIterator it = m_primarykey.constBegin();
            it != m_primarykey.constEnd();
            ++it)
        {
            sql += (*it)->name();
            if(*it != m_primarykey.last())
                sql += ",";
        }
        sql += ")";
    }

    return sql + "\n);";
}

namespace
{
QString identifier(antlr::RefAST ident)
{
    QString sident = ident->getText().c_str();
    if(ident->getType() == sqlite3TokenTypes::QUOTEDID)
    {
        sident.remove(0, 1);
        sident.remove(sident.length() - 1, 1);
    }
    return sident;
}

QString concatTextAST(antlr::RefAST t)
{
    QString stext;
    while(t != antlr::nullAST)
    {
        stext.append(t->getText().c_str());
        t = t->getNextSibling();
    }
    return stext;
}
}

Table CreateTableWalker::table()
{
    Table tab("");

    if( m_root ) //CREATE TABLE
    {
        antlr::RefAST s = m_root->getFirstChild();
        //qDebug() << m_root->toStringTree().cd;
        //skip to tablename
        while(s->getType() != Sqlite3Lexer::ID && s->getType() != Sqlite3Lexer::QUOTEDID) s = s->getNextSibling();
        tab.setName(identifier(s));

        s = s->getNextSibling(); // LPAREN
        s = s->getNextSibling(); // first column name
        antlr::RefAST column = s;
        // loop columndefs
        FieldVector pks;
        while(column != antlr::nullAST && column->getType() == sqlite3TokenTypes::COLUMNDEF)
        {
            FieldPtr f;
            bool pk = parsecolumn(f, column->getFirstChild());
            tab.addField(f);
            if(pk)
                pks.append(FieldPtr(f));
            column = column->getNextSibling(); //COMMA or RPAREN
            column = column->getNextSibling(); //null or tableconstraint
        }

        tab.setPrimaryKey(pks);

        // now we are finished or it is a tableconstraint
        while(s != antlr::nullAST)
        {
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
                pks.clear();
                do
                {
                    QString col = identifier(tc);
                    int fieldindex = tab.findField(col);
                    if(fieldindex != -1)
                        pks.append(tab.fields().at(fieldindex));
                    tc = tc->getNextSibling(); // skip ident and comma
                    tc = tc->getNextSibling();
                } while(tc != antlr::nullAST && tc->getType() != sqlite3TokenTypes::RPAREN);

                tab.setPrimaryKey(pks);
            }
            break;
            default: break;
            }

            s = s->getNextSibling(); //COMMA or RPAREN
            s = s->getNextSibling();
        }
    }

    return tab;
}

bool CreateTableWalker::parsecolumn(FieldPtr& f, antlr::RefAST c)
{
    QString columnname;
    QString type = "TEXT";
    bool autoincrement = false;
    bool primarykey = false;
    bool notnull = false;
    QString defaultvalue;
    QString check;

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
            check = concatTextAST(con);
            // remove parenthesis
            check.remove(check.length()-1, 1);
            check.remove(0,1);
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

    f = FieldPtr( new Field(columnname, type, notnull, defaultvalue, check));
    f->setAutoIncrement(autoincrement);
    return primarykey;
}

} //namespace sqlb
