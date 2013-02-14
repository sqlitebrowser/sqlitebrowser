#include "sqltextedit.h"

#include <QKeyEvent>
#include <QAbstractItemView>
#include <QCompleter>
#include <QScrollBar>
//#include <QDebug>

SqlTextEdit::SqlTextEdit(QWidget* parent) :
    QTextEdit(parent), m_Completer(0), m_defaultCompleterModel(0)
{
    // basic auto completer for sqliteedit
    m_Completer = new QCompleter(this);
    m_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_Completer->setCompletionMode(QCompleter::PopupCompletion);
    m_Completer->setWrapAround(false);
    m_Completer->setWidget(this);

    QObject::connect(m_Completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

SqlTextEdit::~SqlTextEdit()
{
    clearFieldCompleterModelMap();
    delete m_defaultCompleterModel;
}

void SqlTextEdit::setCompleter(QCompleter *completer)
{
    if (m_Completer)
        QObject::disconnect(m_Completer, 0, this, 0);

    m_Completer = completer;

    if (!m_Completer)
        return;

    m_Completer->setWidget(this);
    m_Completer->setCompletionMode(QCompleter::PopupCompletion);
    m_Completer->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(m_Completer, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}

QCompleter* SqlTextEdit::completer() const
{
    return m_Completer;
}

void SqlTextEdit::setDefaultCompleterModel(QAbstractItemModel *model)
{
    delete m_defaultCompleterModel;
    m_defaultCompleterModel = model;
    m_Completer->setModel(m_defaultCompleterModel);
}

void SqlTextEdit::clearFieldCompleterModelMap()
{
    QAbstractItemModel* model;
    foreach (model, m_fieldCompleterMap)
    {
        delete model;
    }
    m_fieldCompleterMap.clear();
}

QAbstractItemModel* SqlTextEdit::addFieldCompleterModel(const QString &tablename, QAbstractItemModel* model)
{
    m_fieldCompleterMap[tablename] = model;
    return model;
}

void SqlTextEdit::insertCompletion(const QString& completion)
{
    if (m_Completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - m_Completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor);

    // slight workaround for a field completion without any completionPrefix
    // eg. "tablename.;" if you would select a field completion and hit enter
    // without this workaround the text would be inserted after the ';'
    // because endofword moves to the end of the line
    if(tc.selectedText() == ".")
        tc.movePosition(QTextCursor::Right);
    else
        tc.movePosition(QTextCursor::EndOfWord);

    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}

namespace {
bool isSqliteIdentifierChar(QChar c) {
    return c.isLetterOrNumber() || c == '.' || c == '_';
}
}

/**
 * @brief SqlTextEdit::identifierUnderCursor
 * @return The partial or full sqlite identifier (table(.field)?)? under the cursor
 *         or a empty string.
 */
QString SqlTextEdit::identifierUnderCursor() const
{
    QTextCursor tc = textCursor();
    const int abspos = tc.position() - 1;
    tc.movePosition(QTextCursor::StartOfLine);
    const int linestartpos = tc.position();
    const int linepos = abspos - linestartpos;
    tc.select(QTextCursor::LineUnderCursor);
    QString line = tc.selectedText();
    int start = 0, end;

    // look where the identifier starts
    for( int i = linepos; i >= 0 && i < line.length() && start == 0; --i)
    {
        if( !(isSqliteIdentifierChar(line.at(i))))
            start = i + 1;
    }

    end = line.length();
    // see where the word ends
    for( int i = start; i < line.length() && i >= 0 && end == line.length(); ++i)
    {
        if( !(isSqliteIdentifierChar(line.at(i))))
            end = i;
    }

    // extract the identifier table.field
    QString identifier = line.mid(start, end - start);
    // check if it has a dot in it
    int dotpos = identifier.indexOf('.');

    // this is a little hack so editing a table name won't show fields
    // fields are only shown if type the word at the end
    if( dotpos > -1 && linepos + 1 != end )
        return identifier.left(dotpos);
    else
        return identifier;
}

void SqlTextEdit::focusInEvent(QFocusEvent *e)
{
    if (m_Completer)
        m_Completer->setWidget(this);
    QTextEdit::focusInEvent(e);
}

void SqlTextEdit::keyPressEvent(QKeyEvent *e)
{
    if (m_Completer && m_Completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space); // CTRL+SPACE
    if (!m_Completer || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    const bool cursorKey = e->key() == Qt::Key_Left ||
            e->key() == Qt::Key_Up ||
            e->key() == Qt::Key_Right ||
            e->key() == Qt::Key_Down;
    if (!m_Completer || (ctrlOrShift && e->text().isEmpty()) || cursorKey)
        return;

    QString identifier = identifierUnderCursor();
    QString table = identifier;
    QString field;
    int dotpos = 0;
    if((dotpos = identifier.indexOf('.')) > 0)
    {
        table = identifier.left(dotpos);
        field = identifier.mid(dotpos + 1);
    }
//    qDebug() << identifier << ":" << table << ":" << field;
    if( dotpos > 0 )
    {
        // swap model to field completion
        FieldCompleterModelMap::ConstIterator it = m_fieldCompleterMap.find(table.toLower());
        if( it != m_fieldCompleterMap.end() )
        {
            if( *it != m_Completer->model() )
                m_Completer->setModel(*it);
            if (field != m_Completer->completionPrefix()) {
                m_Completer->setCompletionPrefix(field);
                m_Completer->popup()->setCurrentIndex(m_Completer->completionModel()->index(0, 0));
            }
            QRect cr = cursorRect();
            cr.setWidth(m_Completer->popup()->sizeHintForColumn(0)
                        + m_Completer->popup()->verticalScrollBar()->sizeHint().width());
            m_Completer->complete(cr);
        }
        return;
    }

    // table completion mode
    if( m_Completer->model() != m_defaultCompleterModel )
        m_Completer->setModel(m_defaultCompleterModel);
    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| identifier.length() < 3
                      || eow.contains(e->text().right(1)))) {
        m_Completer->popup()->hide();
        return;
    }

    if (identifier != m_Completer->completionPrefix()) {
        m_Completer->setCompletionPrefix(identifier);
        m_Completer->popup()->setCurrentIndex(m_Completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(m_Completer->popup()->sizeHintForColumn(0)
                + m_Completer->popup()->verticalScrollBar()->sizeHint().width());
    m_Completer->complete(cr); // popup it up!
}
