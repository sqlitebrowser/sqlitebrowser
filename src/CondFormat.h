#ifndef CONDFORMAT_H
#define CONDFORMAT_H

#include <QString>
#include <QColor>

// Conditional formatting for given format to table cells based on a specified condition.
class CondFormat
{
public:
    CondFormat() {};
    explicit CondFormat(const QString& filter, const QColor& color, const QString& encoding = QString());

    static QString filterToSqlCondition(const QString& value, const QString& encoding = QString());

private:
    QString m_sqlCondition;
    QString m_filter;
    QColor m_color;

public:
    QString sqlCondition() const { return m_sqlCondition; };
    QString filter() const { return m_filter; };
    QColor color() const { return m_color; };

};

#endif // CONDFORMAT_H
