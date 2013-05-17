#ifndef SQLTEXTEDIT_H
#define SQLTEXTEDIT_H

#include <QPlainTextEdit>

class QCompleter;
class QAbstractItemModel;

/**
 * @brief The SqlTextEdit class
 * With basic table and fieldname auto completion.
 * This class is based on the Qt custom completion example.
 */
class SqlTextEdit : public QPlainTextEdit
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
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent* e);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

private:
    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(SqlTextEdit *editor) : QWidget(editor), parent(editor) {}
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent* event);
        SqlTextEdit* parent;
    };

    LineNumberArea* lineNumberArea;
    QString identifierUnderCursor() const;

private slots:
    void insertCompletion(const QString& completion);
    void highlightCurrentLine();
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect& rect, int dy);

private:
    QCompleter* m_Completer;
    QAbstractItemModel* m_defaultCompleterModel;
    FieldCompleterModelMap m_fieldCompleterMap;
};

#endif // SQLTEXTEDIT_H
