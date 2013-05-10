//
// Given two vectors, find all the elements in the second one that are not
// contained in the first
//
// Solution in C++11
//

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>
#include <boost/assign.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

typedef set<int> SetInt;
typedef vector<int> VectorInt;

SetInt setDifference(const VectorInt& v1, const VectorInt& v2)
{
    SetInt output;
    SetInt elem1(v1.begin(), v1.end());
    for ( const int& i: v2 ) {
        if ( !elem1.count(i) ) {
            output.insert(i);
        }
    }
    return output;
}

int main()
{
    vector<int> v1 = { 1, 2 ,5 ,4 ,3 ,8, 5 };
    vector<int> v2 = { 10, 8, 7, 3, 5,1, 9, 2, 6 };

    SetInt o = setDifference(v1, v2);

    copy(o.begin(), o.end(),
         ostream_iterator<int>(cout, " "));
    cout << endl;
    return 0;
}
