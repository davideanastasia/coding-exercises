#include <gtest/gtest.h>

template <typename Type>
void minmax(Type i1, Type i2, Type i3, Type& min, Type& max)
{
    if ( i1 > i2 ) {
        if ( i1 > i3 ) {
            // i1 > b and i1 > g
            max = i1;
            if ( i3 > i2 ) {
                min = i2;
            } else {
                min = i3;
            }
        } else {
            // b >= i1 and i1 > g
            max = i3;
            min = i2;
        }
    } else {
        // i2 >= i1
        if ( i3 > i2 ) {
            max = i3;
            min = i1;
        } else {
            // i2 >= i3
            max = i2;
            if ( i3 < i1 ) {
                min = i3;
            } else {
                min = i1;
            }
        }
    }
}

TEST(MinMax, Test1) {

    int min;
    int max;

    minmax(3, 2, 1, min, max);

    ASSERT_EQ(max, 3);
    ASSERT_EQ(min, 1);

    minmax(2,2,1, min, max);

    ASSERT_EQ(max, 2);
    ASSERT_EQ(min, 1);

    minmax(2,1,3, min, max);

    ASSERT_EQ(max, 3);
    ASSERT_EQ(min, 1);

    minmax(1,2,1, min, max);

    ASSERT_EQ(max, 2);
    ASSERT_EQ(min, 1);

    minmax(0,1,1, min, max);

    ASSERT_EQ(max, 1);
    ASSERT_EQ(min, 0);

    minmax(3,2,3, min, max);

    ASSERT_EQ(max, 3);
    ASSERT_EQ(min, 2);

}
