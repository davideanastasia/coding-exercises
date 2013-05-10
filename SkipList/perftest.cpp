/// This code produces a performance test of the skiplist versus an std::map

#include <chrono>
#include <cstdlib>

namespace cr = std::chrono;

using namespace cr;

namespace perftest
{
typedef cr::high_resolution_clock clock;
typedef cr::microseconds microseconds;
typedef cr::milliseconds milliseconds;

clock::time_point now()
{
    return clock::now();
}

microseconds intervalUs(const clock::time_point& t1, const clock::time_point& t0)
{
    return cr::duration_cast<microseconds>(t1 - t0);
}

milliseconds intervalMs(const clock::time_point& t1, const clock::time_point& t0)
{
    return cr::duration_cast<milliseconds>(t1 - t0);
}

class StopWatch
{
    clock::time_point start_;
public:
    StopWatch()
        : start_(clock::now())
    {}

    clock::time_point restart() { start_ = clock::now(); return start_; }
    microseconds elapsedUs() { return intervalUs(now(), start_); }
    milliseconds elapsedMs() { return intervalMs(now(), start_); }
};

} // perftest

#include <algorithm>
#include <map>
#include "skiplist.h"

typedef skiplist<float, size_t> MySkipList;
typedef std::map<float, size_t> MyMap;

struct Rand {
    float operator()() const {
        return (float)rand()/RAND_MAX;
    }
};

int main()
{
    srand(0);   // set seed to zero: repetible result!

    std::vector<float> samples(10000);
    std::generate(samples.begin(), samples.end(),
                  Rand());

    std::cout << "Starting... " << samples.size() << " elements" << std::endl;

    MyMap mp;
    perftest::StopWatch sw;
    for (size_t idx = 0; idx < samples.size(); ++idx) {
        mp.insert(std::make_pair(samples[idx], idx));
    }
    float mapInsert = sw.elapsedUs().count();
    std::cout << "std::map insertion elapsed time: " << mapInsert << std::endl;

    MySkipList sk;
    sw.restart();
    for (size_t idx = 0; idx < samples.size(); ++idx) {
        sk.insert(samples[idx], idx);
    }
    float skiplistInsert = sw.elapsedUs().count();
    std::cout << "skiplist insertion elapsed time: " << skiplistInsert << std::endl;
    std::cout << "ratio: " << skiplistInsert/mapInsert << std::endl;

    sw.restart();
    for (size_t idx = 0; idx < samples.size(); ++idx) {
        mp.find(samples[idx]);
    }
    float mapSearch = sw.elapsedUs().count();
    std::cout << "std::map search elapsed time: " << mapSearch << std::endl;

    sw.restart();
    for (size_t idx = 0; idx < samples.size(); ++idx) {
        sk.find(samples[idx]);
    }
    float skiplistSearch = sw.elapsedUs().count();
    std::cout << "skiplist search elapsed time: " << skiplistSearch << std::endl;
    std::cout << "ratio: " << skiplistSearch/mapSearch << std::endl;

    // sk.print(std::cout);

    return 0;
}
