#include <QtTest/QTest>

#include "TestRowCache.h"
#include "../RowCache.h"

QTEST_APPLESS_MAIN(TestRowCache)

TestRowCache::TestRowCache()
{
}

TestRowCache::~TestRowCache()
{
}

using C = RowCache<int>;

void TestRowCache::construction()
{
    C c;

    QCOMPARE(c.numSet(), static_cast<size_t>(0));

    QVERIFY(c.count(0) == false);
    QVERIFY(c.count(1) == false);
    QVERIFY(c.count(2) == false);

    QVERIFY_EXCEPTION_THROWN(c.at(0), std::out_of_range);
}

void TestRowCache::setGet()
{
    C c;

    c.set(1, 10);
    c.set(5, 50);
    c.set(0, 0);
    c.set(6, 60);
    c.set(100, 1000);

    QCOMPARE(c.numSet(), static_cast<size_t>(5));
    QCOMPARE(c.numSegments(), static_cast<size_t>(4)); // the '0' set after the '1' position does not merge currently

    int cnt = 0;
    const C & cc = c;
    for(size_t i = 0; i < 200; i++) {
        if(c.count(i)) {
            QCOMPARE(c.at(i), static_cast<int>(10*i));
            QCOMPARE(cc.at(i), static_cast<int>(10*i));
            cnt++;
        } else {
            QVERIFY_EXCEPTION_THROWN(c.at(i), std::out_of_range);
            QVERIFY_EXCEPTION_THROWN(cc.at(i), std::out_of_range);
        }
    }
    QCOMPARE(cnt, 5);
}

void TestRowCache::insert()
{
    C c;

    c.insert(3, 30);
    QCOMPARE(c.numSet(), static_cast<size_t>(1));
    QCOMPARE(c.numSegments(), static_cast<size_t>(1));
    QCOMPARE(c.at(3), 30);

    c.insert(3, 31);
    QCOMPARE(c.numSet(), static_cast<size_t>(2));
    QCOMPARE(c.numSegments(), static_cast<size_t>(1));
    QCOMPARE(c.at(3), 31);
    QCOMPARE(c.at(4), 30);

    c.insert(0, 0);
    QCOMPARE(c.numSet(), static_cast<size_t>(3));
    QCOMPARE(c.numSegments(), static_cast<size_t>(2));
    QCOMPARE(c.at(0), 0);
    QVERIFY_EXCEPTION_THROWN(c.at(3), std::out_of_range);
    QCOMPARE(c.at(4), 31);
    QCOMPARE(c.at(5), 30);
    QVERIFY_EXCEPTION_THROWN(c.at(6), std::out_of_range);

    c.insert(1, 100);
    QCOMPARE(c.numSet(), static_cast<size_t>(4));
    QCOMPARE(c.numSegments(), static_cast<size_t>(2));
    QCOMPARE(c.at(0), 0);
    QCOMPARE(c.at(1), 100);
    QCOMPARE(c.at(5), 31);
    QCOMPARE(c.at(6), 30);

    c.insert(8, 1);
    QCOMPARE(c.numSet(), static_cast<size_t>(5));
    QCOMPARE(c.numSegments(), static_cast<size_t>(3));
    QCOMPARE(c.at(0), 0);
    QCOMPARE(c.at(1), 100);
    QCOMPARE(c.at(5), 31);
    QCOMPARE(c.at(6), 30);
    QCOMPARE(c.at(8), 1);
}

void TestRowCache::erase()
{
    C c;
    c.insert(3, 30);
    c.insert(3, 31);
    c.insert(0, 0);
    c.insert(8, 1);
    QCOMPARE(c.numSet(), static_cast<size_t>(4));
    QCOMPARE(c.numSegments(), static_cast<size_t>(3));
    QCOMPARE(c.at(0), 0);
    QCOMPARE(c.at(4), 31);
    QCOMPARE(c.at(5), 30);
    QCOMPARE(c.at(8), 1);

    // erase entire segment
    c.erase(0);
    QCOMPARE(c.numSet(), static_cast<size_t>(3));
    QCOMPARE(c.numSegments(), static_cast<size_t>(2));
    QCOMPARE(c.at(3), 31);
    QCOMPARE(c.at(4), 30);
    QCOMPARE(c.at(7), 1);

    // erase inside segment
    c.erase(4);
    QCOMPARE(c.numSet(), static_cast<size_t>(2));
    QCOMPARE(c.numSegments(), static_cast<size_t>(2));
    QCOMPARE(c.at(3), 31);
    QCOMPARE(c.at(6), 1);

    // erase non-filled row
    c.erase(5);
    QCOMPARE(c.numSet(), static_cast<size_t>(2));
    QCOMPARE(c.numSegments(), static_cast<size_t>(2));
    QCOMPARE(c.at(3), 31);
    QCOMPARE(c.at(5), 1);

    c.erase(5);
    QCOMPARE(c.numSet(), static_cast<size_t>(1));
    QCOMPARE(c.numSegments(), static_cast<size_t>(1));
    QCOMPARE(c.at(3), 31);

    c.erase(3);
    QCOMPARE(c.numSet(), static_cast<size_t>(0));
    QCOMPARE(c.numSegments(), static_cast<size_t>(0));
}

void TestRowCache::smallestNonAvailableRange()
{
    C c;
    c.insert(3, 0);
    c.insert(3, 0);
    c.insert(0, 0);
    c.insert(8, 0);
    QCOMPARE(c.numSet(), static_cast<size_t>(4));
    QVERIFY(c.count(0));
    QVERIFY(c.count(4));
    QVERIFY(c.count(5));
    QVERIFY(c.count(8));

    using P = std::pair<size_t,size_t>;

    auto test = [&](size_t begin, size_t end) {
        P p{ begin, end };
        c.smallestNonAvailableRange(p.first, p.second);
        return p;
    };

    QCOMPARE(test( 0, 0), P( 0, 0));
    QCOMPARE(test( 0, 1), P( 1, 1));
    QCOMPARE(test( 0, 2), P( 1, 2));
    QCOMPARE(test( 0, 3), P( 1, 3));
    QCOMPARE(test( 0, 4), P( 1, 4));
    QCOMPARE(test( 0, 5), P( 1, 4));
    QCOMPARE(test( 0, 6), P( 1, 4));
    QCOMPARE(test( 0, 7), P( 1, 7));
    QCOMPARE(test( 0, 8), P( 1, 8));
    QCOMPARE(test( 0, 9), P( 1, 8));
    QCOMPARE(test( 0,10), P( 1,10));
    QCOMPARE(test( 1,10), P( 1,10));
    QCOMPARE(test( 2,10), P( 2,10));
    QCOMPARE(test( 3,10), P( 3,10));
    QCOMPARE(test( 4,10), P( 6,10));
    QCOMPARE(test( 5,10), P( 6,10));
    QCOMPARE(test( 6,10), P( 6,10));
    QCOMPARE(test( 7,10), P( 7,10));
    QCOMPARE(test( 8,10), P( 9,10));
    QCOMPARE(test( 9,10), P( 9,10));
    QCOMPARE(test(10,10), P(10,10));
}
