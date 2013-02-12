/*
* GeneralChess - TopCoder
*
* Full description here:
* http://community.topcoder.com/stat?c=problem_statement&pm=2430&rd=5072
*
*/

#include <sstream>
#include <iostream>
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <map>

#include <gtest/gtest.h>

using namespace std;

struct Point
{
    Point(int x, int y)
        : x_(x), y_(y)
    {}

    int x_;
    int y_;
};

ostream& operator<<(ostream& out, const Point& p)
{
    out << "(" << p.x_ << "," << p.y_ << ")";
    return out;
}

bool operator<(const Point& p1, const Point& p2)
{
    if ( p1.x_ <  p2.x_) return true;
    if ( p1.x_ == p2.x_) {
        if ( p1.y_ < p2.y_ ) return true;
    }
    return false;
}


typedef vector<Point> Points;
typedef vector<string> Strings;

Points
parsePoints(const Strings& pieces)
{
    Points pts;

    int coord1;
    int coord2;
    for (size_t idx = 0; idx < pieces.size(); ++idx)
    {
        // copy string!
        string tmp = pieces[idx];
        // replace comma with space
        size_t found = tmp.find_first_of(',');
        if ( found != string::npos )
        {
            tmp[found] = ' '; // replace with space!
        }
        else continue;

        // insert into string stream!
        istringstream iss(tmp);
        if ( (iss >> coord1) && (iss >> coord2) )
        {
            pts.push_back( Point(coord1, coord2) );
        }
    }
    return pts;
}

typedef map<Point, int> PointsHist;

Points attackPosition(const Points& pieces)
{
    PointsHist hist;
    // I build the histogram of the threating positions
    // for each of the pieces, I increment the counter for the threating
    // position
    for (size_t idx = 0; idx < pieces.size(); ++idx)
    {
        const Point& curr = pieces[idx];

        ++hist[ Point(curr.x_ + 2, curr.y_ + 1) ];
        ++hist[ Point(curr.x_ + 2, curr.y_ - 1) ];
        ++hist[ Point(curr.x_ + 1, curr.y_ + 2) ];
        ++hist[ Point(curr.x_ + 1, curr.y_ - 2) ];
        ++hist[ Point(curr.x_ - 2, curr.y_ + 1) ];
        ++hist[ Point(curr.x_ - 2, curr.y_ - 1) ];
        ++hist[ Point(curr.x_ - 1, curr.y_ + 2) ];
        ++hist[ Point(curr.x_ - 1, curr.y_ - 2) ];
    }

    Points threats;
    // scroll histogram to compute the best positions!
    for (PointsHist::iterator it = hist.begin(); it != hist.end(); ++it)
    {
        if ( it->second == (int)pieces.size() ) {
            threats.push_back( it->first );
        }
    }
    sort(threats.begin(), threats.end());
    return threats;
}

Points
attackPosition(const vector<string>& pieces)
{
    return attackPosition(parsePoints(pieces));
}

TEST(GeneralChess, Test0)
{
    Strings pieces;
    pieces.push_back("0,0");

    Points pts = attackPosition(pieces);

    ASSERT_EQ(pts.size(), 8);
    EXPECT_EQ(pts[0].x_, -2);
    EXPECT_EQ(pts[0].y_, -1);
    EXPECT_EQ(pts[3].x_, -1);
    EXPECT_EQ(pts[3].y_, 2);
    EXPECT_EQ(pts[6].x_, 2);
    EXPECT_EQ(pts[6].y_, -1);
}

TEST(GeneralChess, Test1)
{
    Strings pieces;
    pieces.push_back("2,1");
    pieces.push_back("-1,-2");

    Points pts = attackPosition(pieces);

//    copy(pts.begin(), pts.end(),
//         ostream_iterator<Point>(cout, "\n"));

    ASSERT_EQ(pts.size(), 2);
    EXPECT_EQ(pts[0].x_, 0);
    EXPECT_EQ(pts[0].y_, 0);
    EXPECT_EQ(pts[1].x_, 1);
    EXPECT_EQ(pts[1].y_, -1);
}

TEST(GeneralChess, Test2)
{
    Strings pieces;
    pieces.push_back("0,0");
    pieces.push_back("2,1");

    Points pts = attackPosition(pieces);

//    copy(pts.begin(), pts.end(),
//         ostream_iterator<Point>(cout, "\n"));

    ASSERT_EQ(pts.size(), 0);
}

TEST(GeneralChess, Test3)
{
    Strings pieces;
    pieces.push_back("-1000,1000");
    pieces.push_back("-999,999");
    pieces.push_back("-999,997");

    Points pts = attackPosition(pieces);

//    copy(pts.begin(), pts.end(),
//         ostream_iterator<Point>(cout, "\n"));

    ASSERT_EQ(pts.size(), 1);
    EXPECT_EQ(pts[0].x_, -1001);
    EXPECT_EQ(pts[0].y_, 998);
}
