#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>

using namespace std;

void shuffle(vector<int>& data) {
    for (int i = data.size()-1; i > 2; --i) {
        int idx = rand()%i;
        swap(data[idx], data[i]);
    }
}

int main() {
    srand(time(0));
    vector<int> v(7);
    v[0] = 0; v[1] = 1; v[2] = 2; v[3] = 3; v[4] = 4; v[5] = 5; v[6] = 6;

    shuffle(v);

    copy(v.begin(), v.end(),
         std::ostream_iterator<int>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
