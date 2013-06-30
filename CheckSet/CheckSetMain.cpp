// in a vector of N integers, checks that all the numbers between 1 and N appear

#include <iostream>
#include <vector>
#include <set>
#include <cassert>

#include <boost/assign.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

bool checkSet(const vector<int>& data)
{
    const int max = data.size();
    const int min = 1;

    set<int> samples;
    for (size_t i = 0; i < data.size(); ++i)
    {
        int currentValue = data[i];
        if (currentValue < min || currentValue > max || samples.count(currentValue))
        {
            return false;
        }
        samples.insert( currentValue );
    }
    return true;
}

int main()
{
    vector<int> d1;
    d1 += 1,3,4,1;

    assert( checkSet(d1) == false );

    d1.clear();
    d1 += 1,2,3,4;
    assert( checkSet(d1) == true );
}
