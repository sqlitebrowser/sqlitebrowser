
#include "../RowCache.h"

#include "gtest/gtest.h"

using C = RowCache<int>;

TEST(RowCache, Construction)
{
    C c;

    EXPECT_EQ(0u, c.numSet());

    EXPECT_FALSE(c.count(0));
    EXPECT_FALSE(c.count(1));
    EXPECT_FALSE(c.count(2));

    EXPECT_THROW(c.at(0), std::out_of_range);
}

TEST(RowCache, set_get)
{
    C c;

    c.set(1, 10);
    c.set(5, 50);
    c.set(0, 0);
    c.set(6, 60);
    c.set(100, 1000);

    EXPECT_EQ(5u, c.numSet());
    EXPECT_EQ(4u, c.numSegments()); // the '0' set after the '1' position does not merge currently

    int cnt = 0;
    const C & cc = c;
    for(size_t i = 0; i < 200; i++) {
        if(c.count(i)) {
            EXPECT_EQ(10*i, c.at(i));
            EXPECT_EQ(10*i, cc.at(i));
            cnt++;
        } else {
            EXPECT_THROW(c.at(i), std::out_of_range);
            EXPECT_THROW(cc.at(i), std::out_of_range);
        }
    }
    EXPECT_EQ(5, cnt);
}

TEST(RowCache, insert)
{
    C c;

    c.insert(3, 30);
    EXPECT_EQ(1u, c.numSet());
    EXPECT_EQ(1u, c.numSegments());
    EXPECT_EQ(30, c.at(3));

    c.insert(3, 31);
    EXPECT_EQ(2u, c.numSet());
    EXPECT_EQ(1u, c.numSegments());
    EXPECT_EQ(31, c.at(3));
    EXPECT_EQ(30, c.at(4));

    c.insert(0, 0);
    EXPECT_EQ(3u, c.numSet());
    EXPECT_EQ(2u, c.numSegments());
    EXPECT_EQ(0, c.at(0));
    EXPECT_THROW(c.at(3), std::out_of_range);
    EXPECT_EQ(31, c.at(4));
    EXPECT_EQ(30, c.at(5));
    EXPECT_THROW(c.at(6), std::out_of_range);

    c.insert(1, 100);
    EXPECT_EQ(4u, c.numSet());
    EXPECT_EQ(2u, c.numSegments());
    EXPECT_EQ(0, c.at(0));
    EXPECT_EQ(100, c.at(1));
    EXPECT_EQ(31, c.at(5));
    EXPECT_EQ(30, c.at(6));

    c.insert(8, 1);
    EXPECT_EQ(5u, c.numSet());
    EXPECT_EQ(3u, c.numSegments());
    EXPECT_EQ(0, c.at(0));
    EXPECT_EQ(100, c.at(1));
    EXPECT_EQ(31, c.at(5));
    EXPECT_EQ(30, c.at(6));
    EXPECT_EQ(1, c.at(8));
}

TEST(RowCache, erase)
{
    C c;
    c.insert(3, 30);
    c.insert(3, 31);
    c.insert(0, 0);
    c.insert(8, 1);
    EXPECT_EQ(4u, c.numSet());
    EXPECT_EQ(3u, c.numSegments());
    EXPECT_EQ(0, c.at(0));
    EXPECT_EQ(31, c.at(4));
    EXPECT_EQ(30, c.at(5));
    EXPECT_EQ(1, c.at(8));

    // erase entire segment
    c.erase(0);
    EXPECT_EQ(3u, c.numSet());
    EXPECT_EQ(2u, c.numSegments());
    EXPECT_EQ(31, c.at(3));
    EXPECT_EQ(30, c.at(4));
    EXPECT_EQ(1, c.at(7));

    // erase inside segment
    c.erase(4);
    EXPECT_EQ(2u, c.numSet());
    EXPECT_EQ(2u, c.numSegments());
    EXPECT_EQ(31, c.at(3));
    EXPECT_EQ(1, c.at(6));

    // erase non-filled row
    c.erase(5);
    EXPECT_EQ(2u, c.numSet());
    EXPECT_EQ(2u, c.numSegments());
    EXPECT_EQ(31, c.at(3));
    EXPECT_EQ(1, c.at(5));

    c.erase(5);
    EXPECT_EQ(1u, c.numSet());
    EXPECT_EQ(1u, c.numSegments());
    EXPECT_EQ(31, c.at(3));

    c.erase(3);
    EXPECT_EQ(0u, c.numSet());
    EXPECT_EQ(0u, c.numSegments());
}

TEST(RowCache, smallestNonAvailableRange)
{
    C c;
    c.insert(3, 0);
    c.insert(3, 0);
    c.insert(0, 0);
    c.insert(8, 0);
    EXPECT_EQ(4u, c.numSet());
    EXPECT_TRUE(c.count(0));
    EXPECT_TRUE(c.count(4));
    EXPECT_TRUE(c.count(5));
    EXPECT_TRUE(c.count(8));

    using P = std::pair<size_t,size_t>;

    auto test = [&](int begin, int end) {
        P p{ begin, end };
        c.smallestNonAvailableRange(p.first, p.second);
        return p;
    };

    EXPECT_EQ(P( 0, 0), test( 0, 0));
    EXPECT_EQ(P( 1, 1), test( 0, 1));
    EXPECT_EQ(P( 1, 2), test( 0, 2));
    EXPECT_EQ(P( 1, 3), test( 0, 3));
    EXPECT_EQ(P( 1, 4), test( 0, 4));
    EXPECT_EQ(P( 1, 4), test( 0, 5));
    EXPECT_EQ(P( 1, 4), test( 0, 6));
    EXPECT_EQ(P( 1, 7), test( 0, 7));
    EXPECT_EQ(P( 1, 8), test( 0, 8));
    EXPECT_EQ(P( 1, 8), test( 0, 9));
    EXPECT_EQ(P( 1,10), test( 0,10));
    EXPECT_EQ(P( 1,10), test( 1,10));
    EXPECT_EQ(P( 2,10), test( 2,10));
    EXPECT_EQ(P( 3,10), test( 3,10));
    EXPECT_EQ(P( 6,10), test( 4,10));
    EXPECT_EQ(P( 6,10), test( 5,10));
    EXPECT_EQ(P( 6,10), test( 6,10));
    EXPECT_EQ(P( 7,10), test( 7,10));
    EXPECT_EQ(P( 9,10), test( 8,10));
    EXPECT_EQ(P( 9,10), test( 9,10));
    EXPECT_EQ(P(10,10), test(10,10));
}
