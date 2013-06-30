// given a vector that represent a matrix of N*N elements, calculate a new
// vector that contains the numbers in a spiral order, starting from the top
// left corner and walking clockwise

#include <cassert>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <boost/assign.hpp>
#include <boost/assign/std/vector.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;


typedef vector<int> matrix;

enum Direction {
    RIGHT,
    BOTTOM,
    LEFT,
    TOP
};

int idx(int r, int c, int totC)
{
    return r*totC + c;
}

bool isValid(int r, int c, int size)
{
    return (r >= 0 && r < size && c >= 0 && c < size);
}

struct Position {
    Position(int r_, int c_)
        : r(r_), c(c_)
    {}

    int r;
    int c;
};

matrix computeSpiral(const matrix& m)
{
    int size = sqrt(m.size());

    Direction d = RIGHT;
    vector<bool> visited(m.size(), false);
    matrix output;

    queue<Position> tiles;
    tiles.push( Position(0, 0) );

    while ( !tiles.empty() ) {
        Position p = tiles.front();
        tiles.pop();

        output.push_back( m[ idx(p.r, p.c, size) ] );
        visited[ idx(p.r, p.c, size) ] = true;

        // find next neighbour ... this piece of code can probably be written
        // better...
        int tests = 4;
        while ( tests ) {
            switch ( d )
            {
            case RIGHT: {
                if ( isValid(p.r, p.c+1, size) && !visited[ idx(p.r, p.c+1, size)] ) {
                    tiles.push( Position(p.r, p.c+1) );
                    tests = 0;
                } else {
                    d = BOTTOM;
                    --tests;
                }
            } break;
            case BOTTOM: {
                if ( isValid(p.r+1, p.c, size) && !visited[ idx(p.r+1, p.c, size)] ) {
                    tiles.push( Position(p.r+1, p.c) );
                    tests = 0;
                } else {
                    d = LEFT;
                    --tests;
                }
            } break;
            case LEFT: {
                if ( isValid(p.r, p.c-1, size) && !visited[ idx(p.r, p.c-1, size)] ) {
                    tiles.push( Position(p.r, p.c-1) );
                    tests = 0;
                } else {
                    d = TOP;
                    --tests;
                }
            } break;
            case TOP: {
                if ( isValid(p.r-1, p.c, size) && !visited[ idx(p.r-1, p.c, size)] ) {
                    tiles.push( Position(p.r-1, p.c) );
                    tests = 0;
                } else {
                    d = RIGHT;
                    --tests;
                }
            } break;
            }
        }
    }

    return output;
}

int main()
{
    matrix inputdata;
    inputdata +=
            1,3,7,9,17,
            8,2,4,10,18,
            16,5,6,11,25,
            15,13,14,12,22,
            23,24,19,20,21;

    matrix spiral = computeSpiral(inputdata);


    std::copy(spiral.begin(), spiral.end(),
              std::ostream_iterator<matrix::value_type>(std::cout, " "));
    std::cout << std::endl;


    return 0;
}

