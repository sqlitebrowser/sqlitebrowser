#ifndef CONDFORMAT_H
#define CONDFORMAT_H

#include <QString>
#include <QColor>
#include <QFont>

// Conditional formatting for given format to table cells based on a specified condition.
class CondFormat
{
public:
    CondFormat() {}
    explicit CondFormat(const QString& filter, const QColor& foreground, const QColor& background, const QFont& font, const QString& encoding = QString());

    static QString filterToSqlCondition(const QString& value, const QString& encoding = QString());

private:
    QString m_sqlCondition;
    QString m_filter;
    QColor m_bgColor;
    QColor m_fgColor;
    QFont m_font;

public:
    QString sqlCondition() const { return m_sqlCondition; }
    QString filter() const { return m_filter; }
    QColor backgroundColor() const { return m_bgColor; }
    QColor foregroundColor() const { return m_fgColor; }
    bool isBold() const { return m_font.bold(); }
    bool isItalic() const { return m_font.italic(); }
    bool isUnderline() const { return m_font.underline(); }
    QFont font() const { return m_font; }
};

#endif // CONDFORMAT_H
