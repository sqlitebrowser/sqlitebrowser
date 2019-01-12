#ifndef TESTROWCACHE_H
#define TESTROWCACHE_H

#include <QObject>

class TestRowCache : public QObject
{
    Q_OBJECT

public:
    TestRowCache();
    ~TestRowCache();

private slots:
    void construction();
    void setGet();
    void insert();
    void erase();
    void smallestNonAvailableRange();
};

#endif
