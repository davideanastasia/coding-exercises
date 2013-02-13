#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <boost/chrono.hpp>

namespace perftest
{
typedef boost::chrono::high_resolution_clock  clock;
typedef boost::chrono::microseconds           microseconds;
typedef boost::chrono::milliseconds           milliseconds;

clock::time_point now()
{
    return clock::now();
}

microseconds intervalUs(const clock::time_point& t1, const clock::time_point& t0)
{
    return boost::chrono::duration_cast<microseconds>(t1 - t0);
}

milliseconds intervalMs(const clock::time_point& t1, const clock::time_point& t0)
{
    return boost::chrono::duration_cast<milliseconds>(t1 - t0);
}

class StopWatch
{
    clock::time_point start_;
public:
    StopWatch()
        : start_(clock::now())
    {}

    clock::time_point restart()         { start_ = clock::now(); return start_; }
    microseconds elapsedUs()            { return intervalUs(now(), start_); }
    milliseconds elapsedMs()            { return intervalMs(now(), start_); }
};

} // perftest

#endif // STOPWATCH_H
