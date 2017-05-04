#ifndef SQLTHREAD_H
#define SQLTHREAD_H

#include <QThread>

class DBBrowserDB;

class SqlThread : public QThread
{
    Q_OBJECT

public:
    SqlThread(DBBrowserDB& db, const QString& query, int column_count, QList<QByteArrayList>& result);

    void run();

signals:
    void done();

private:
    DBBrowserDB& m_db;
    QString m_query;
    int m_columnCount;
    QList<QByteArrayList>& m_result;
};

#endif
