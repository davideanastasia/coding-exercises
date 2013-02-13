#ifdef __APPLE__
#define GTEST_USE_OWN_TR1_TUPLE 1 //See issue tracker #5
#endif

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdlib>

#include <gtest/gtest.h>

#include "polynomial.h"
#include "stopwatch.h"

using namespace std;

const size_t NUM = 12000000;

struct Random
{
    float operator()() const
    {
        return (static_cast<float>(rand())/RAND_MAX)*5.0f - 2.5f;
    }
};

template <typename InputIterator>
void compareVectors(InputIterator first1, InputIterator end1, InputIterator first2)
{
    while ( first1 != end1 )
    {
        ASSERT_EQ(*first1++, *first2++);
    }
}

TEST(Polynomial, Test)
{
    vector<float> samples(NUM);
    vector<float> result_c_macro(NUM);
    vector<float> result_variadic(NUM);
    // vector<float> result_variadic_sse(NUM);
    generate(samples.begin(), samples.end(), Random());

    perftest::StopWatch sw;
    transform(samples.begin(), samples.end(), result_c_macro.begin(),
                   poly_c_macro);
    float sw1macro = sw.elapsedMs().count();

    sw.restart();
    transform(samples.begin(), samples.end(), result_variadic.begin(),
                   poly_variadic<float>);
    float sw1variadic = sw.elapsedMs().count();

    cout << "C Macro: " << sw1macro << " msec\n";
    cout << "Variadic: " << sw1variadic << " msec\n";

    compareVectors(result_c_macro.begin(), result_c_macro.end(), result_variadic.begin());
    ASSERT_GE(sw1macro, sw1variadic);
}

#ifdef __SSE2__
TEST(Polynomial, TestSSE)
{
    vector<float> samples(NUM);
    vector<float> result_c_macro(NUM);
    vector<float> result_variadic(NUM);
    // vector<float> result_variadic_sse(NUM);
    generate(samples.begin(), samples.end(), Random());

    perftest::StopWatch sw;
    transform(samples.begin(), samples.end(), result_c_macro.begin(),
                   poly_c_macro);
    float sw1macro = sw.elapsedMs().count();

    sw.restart();
    transform(reinterpret_cast<__m128*>(samples.data()),
              reinterpret_cast<__m128*>(samples.data() + samples.size()),
              reinterpret_cast<__m128*>(result_variadic.data()),
              poly_variadic<__m128>);
    float sw1variadic = sw.elapsedMs().count();

    cout << "C Macro: " << sw1macro << " msec\n";
    cout << "Variadic: " << sw1variadic << " msec\n";

    compareVectors(result_c_macro.begin(), result_c_macro.end(), result_variadic.begin());
    ASSERT_GE(sw1macro, sw1variadic);
}
#endif // __SSE2__
