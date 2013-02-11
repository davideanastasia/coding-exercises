#include "SimpleHash.h"
#include <gtest/gtest.h>

TEST(SimpleHash, BasicInsert)
{
    SimpleHash myHashMap(1000);

    ASSERT_TRUE(myHashMap.insert(1, 10));

    int value;
    ASSERT_TRUE(myHashMap.get(1, value));
    ASSERT_EQ(value, 10);
}

TEST(SimpleHash, SameKeyTwice)
{
    SimpleHash myHashMap(1000);

    ASSERT_TRUE(myHashMap.insert(1, 10));

    int value;
    ASSERT_TRUE(myHashMap.get(1, value));
    ASSERT_EQ(value, 10);

    ASSERT_TRUE(myHashMap.insert(1, 12));
    ASSERT_TRUE(myHashMap.get(1, value));
    ASSERT_EQ(value, 12);
}
