// === Problem =================================================================
// A board of 4x4 char is given, together with two functions:
//  isPrefix( )
//      return true if the string is prefix for a word (the trailer of a word)
//  isWord( )
//      return true if the string IS a valid word for a Dictionary (unknown)
// The target is to find ALL the valid word that can be formed on the board
// with a never-breaking path (from a cell, you can move to all its neighbours,
// but never jump) and without passing twice on the same cell
//
// === Solution explaination ===================================================
// I've used a simple breath-first search algorithm, using an helper class
// (called Path) to maintain the visited tiles for that particular path (hence
// never passing twice on the same cell) and the string that can be formed
// from that path (so I store the string and not the ordered list of cells
// visited). At the same time, I maintain the coordinates of the last visited
// cell, so that I know where I can go on the next op (all the neighbours).
//

#include <iostream>
#include <set>
#include <string>
#include <iterator>
#include <queue>

#include <boost/assign.hpp>

using namespace std;
using namespace boost;
using namespace boost::assign;

typedef set<string> Dictionary;

bool isPrefix(const string& /*str*/) {
    return true;
}

bool isWord(const string& str) {
    static Dictionary v = list_of
            ("davide")
            ("divide")
            ("fire")
            ("reader")
            ("did")
            ("read")
            ("dead")
            ("ride")
            ("avid")
            ("five")
            ("dave");

    return v.count(str);
}

typedef struct __Board { char c[4][4]; } Board;
typedef struct __Flags { bool b[4][4]; } Flags;

class Path {
public:
    Path() {
        for (size_t r = 0; r < 4; r++) {
            for (size_t c = 0; c < 4; c++) {
                visited.b[r][c] = false;
            }
        }
    }

    string str;
    int row;
    int col;
    Flags visited;
};

// the implementation is pretty easy, as it is a breadth-first search
// the two helper function, isPrefix and isWord are important to block the
// search in the board when a result cannot be found
void findWords(Dictionary& output, const Board& b,
               size_t initRow, size_t initCol)
{
    std::queue<Path> paths;

    Path tmp;
    tmp.col = initCol;
    tmp.row = initRow;
    tmp.visited.b[initRow][initCol] = true;
    tmp.str += b.c[initRow][initCol];

    paths.push( tmp );
    while ( !paths.empty() )
    {
        // get first element of the queue...
        Path current = paths.front();
        paths.pop();

//        std::cout << "Visiting " << current.str << " at "
//                  << current.row << "," << current.col << std::endl;

        // checking whether it is a word
        if ( isWord(current.str) )
        {
            // std::cout << "Found: " << current.str << "\n";
            output.insert(current.str);
        }

        if ( isPrefix(current.str) ) {
            // add all neighbours in the queue...
            for (int r = current.row - 1 ; r <= current.row + 1; ++r) {
                for (int c = current.col - 1; c <= current.col + 1; ++c) {
                    // if coordinate is inside the board...
                    if ( (r >= 0 && r <= 3) &&
                         (c >= 0 && c <= 3) &&
                         (current.visited.b[r][c] == false) )
                    {
                        // create copy
                        tmp = current;

                        tmp.col = c;
                        tmp.row = r;
                        tmp.visited.b[r][c] = true;
                        tmp.str += b.c[r][c];

                        // insert new node
                        paths.push( tmp );
                    }
                }
            }
        }
    }
}

Dictionary findWords(const Board& b)
{
    Dictionary v;
    for ( size_t r = 0; r < 4; ++r ) {
        for ( size_t c = 0; c < 4; ++c ) {
            findWords(v, b, r, c);
        }
    }
    return v;
}

static
char init_vector[4][4] = {
    'd', 'a', 'd', 'f',
    'i', 'v', 'i', 'r',
    'd', 'e', 'd', 'e',
    'r', 'a', 'd', 'f'
};

int main()
{
    // allocate board
    Board board;
    memcpy(board.c, init_vector, sizeof(char)*16);

    Dictionary output = findWords(board);

    std::copy(output.begin(), output.end(),
              std::ostream_iterator<string>(cout, "\n"));
}
