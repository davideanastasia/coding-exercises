#include <iostream>
#include <cstdlib>
#include <map>

#include "skiplist.h"

using namespace std;

int getLevel() {
    int i = 1;
    while ( rand() > RAND_MAX/2 && i < 10 ) { i++; }
    return i;
}

typedef map<int, size_t> MyMap;
typedef skiplist<int, size_t, std::greater<int> > MySkipList;

int main()
{
    srand(time(NULL));

    const size_t iters = 10;

    MySkipList sklist;
    sklist.print( std::cout );

    int idx = 0;
    for (int i = 0; i < iters; i++) {
        sklist.insert(i*10 + 10, idx++);
    }

    for (int i = 0; i < iters; i++) {
        sklist.insert(i*10 + 3, idx++);
        sklist.insert(i*10 + 7, idx++);
    }
    sklist.print( std::cout );

    for (int i = 0; i < iters; i++) {
        sklist.remove(i*10 + 7);
    }
    sklist.print( std::cout );

    if ( sklist.find( 617 ) ) { cout << "Found" << endl; }
    else cout << "Not found!" << endl;

    MySkipList::iterator it = sklist.begin();

    while ( it != sklist.end() ) {
        std::cout << it->first << " " << it->second << std::endl;
        ++it;
    }

    MySkipList::const_iterator it_c = sklist.begin();

    while ( it_c != sklist.end() ) {
        std::cout << it_c->first << " " << it_c->second << std::endl;
        ++it_c;
    }

    return 0;
}

