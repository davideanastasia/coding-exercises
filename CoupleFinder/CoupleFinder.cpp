// === Problem =================================================================
// Find all the couples in a vector of integers that summed together match a
// target value (simple stuff!)

#include <iostream>
#include <vector>
#include <utility>
#include <set>
#include <iterator>
#include <algorithm>

using namespace std;

typedef vector<int> Numbers;
typedef pair<int, int> Pair;
typedef set< Pair > Pairs;

ostream& operator<<(ostream& out, const Pair& p)
{
    out << "(" << p.first << "," << p.second << ")";
    return out;
}

void coupleFinder(int target, const Numbers& data, Pairs& out)
{
    Pairs tmp;
    // if we could use in place of an std::map an hash container, the algorithm
    // would be O(N), instead of O(N logN)
    std::set<int> visited;
    for (Numbers::const_iterator it = data.begin(), itEnd = data.end();
         it != itEnd; ++it )
    {
        int mate = target - *it;
        if ( visited.count( mate ) ) {
            tmp.insert( Pair(*it, mate) );
        }
        visited.insert( *it );
    }
    out.swap( tmp );
}

int main()
{
    // Numbers num = {1,2,3,5,4}; // C++11 way... :(
    Numbers num(5);
    num[0] = 1; num[1] = 2; num[2] = 3; num[3] = 5; num[4] = 4;

    Pairs out;
    coupleFinder(5, num, out);

    std::copy(num.begin(), num.end(), ostream_iterator<int>(cout, " "));
    cout << " = ";
    for (Pairs::const_iterator it = out.begin(), itEnd = out.end();
         it != itEnd; ++it)
    {
        cout << *it << " ";
    }
    cout << "\n";

    return 0;
}
