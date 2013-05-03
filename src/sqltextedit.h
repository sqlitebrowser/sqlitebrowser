#ifndef SQLTEXTEDIT_H
#define SQLTEXTEDIT_H

#include <QTextEdit>

class QCompleter;
class QAbstractItemModel;

/**
 * @brief The SqlTextEdit class
 * With basic table and fieldname auto completion.
 * This class is based on the Qt custom completion example.
 */
class SqlTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit SqlTextEdit(QWidget *parent = 0);
    virtual ~SqlTextEdit();

    void setCompleter(QCompleter* completer);
    QCompleter* completer() const;
    void setDefaultCompleterModel(QAbstractItemModel* model);

    // map that associates table -> field model
    typedef QMap<QString,QAbstractItemModel*> FieldCompleterModelMap;

    QAbstractItemModel* addFieldCompleterModel(const QString& tablename, QAbstractItemModel *model);
    void insertFieldCompleterModels(const FieldCompleterModelMap& fieldmap);

protected:
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private:
    QString identifierUnderCursor() const;

private slots:
    void insertCompletion(const QString& completion);

private:
    QCompleter* m_Completer;
    QAbstractItemModel* m_defaultCompleterModel;
    FieldCompleterModelMap m_fieldCompleterMap;
};

#endif // SQLTEXTEDIT_H
