/*
 * GrafixMask - TopCoder
 *
 * Full description here:
 * http://community.topcoder.com/stat?c=problem_statement&pm=2998&rd=5857
 *
 */

#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <gtest/gtest.h>

using namespace std;

const size_t ROWS = 400;
const size_t COLS = 600;

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

size_t pixelPos(const Point& p)
{
    return (p.y_*COLS + p.x_);
}

Point pixelPos(size_t idx)
{
    return Point(idx%COLS, idx/COLS);
}

struct Rectangle
{
    Rectangle(const Point& ul, const Point& br)
        : ul_(ul), br_(br)
    {}

    Point ul_;
    Point br_;
};

ostream& operator<<(ostream& out, const Rectangle& rec)
{
    out << "[" << rec.ul_ << "," << rec.br_ << "]";
    return out;
}

typedef vector<Rectangle> Rectangles;
typedef vector<bool> Mask;

void convert(const vector<string>& strRectangles,
             Rectangles& rectangles)
{
    // ---
    int upper; int left;
    int bottom; int right;
    // ---
    for (size_t idx = 0; idx < strRectangles.size(); ++ idx)
    {
        istringstream iss(strRectangles[idx]);
        if ( (iss >> upper) && (iss >> left) && (iss >> bottom) && (iss >> right) )
        {
            rectangles.push_back( Rectangle(Point(left, upper),
                                            Point(right, bottom)) );
        }
    }
}

void removeRectangles(Mask& mask, const Rectangles& rectangles)
{
    // for each rectangle
    for (size_t idx = 0; idx < rectangles.size(); ++idx)
    {
        const Rectangle& rect = rectangles[idx];
        for (int row = rect.ul_.y_; row <= rect.br_.y_; ++row) {
            for (int col = rect.ul_.x_; col <= rect.br_.x_; ++col) {
                mask[ pixelPos(Point(col, row)) ] = false;
            }
        }
    }
}

void printMask(const Mask& mask)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            cout << ((mask[ pixelPos(Point(col, row)) ] == true) ? "-": "o");
        }
        cout << "\n";
    }
    cout << "\n";
}

vector<int> sortedAreas(const vector<string>& rectangles)
{
    Rectangles rects;
    convert(rectangles, rects);

    Mask mask(ROWS*COLS, true);
    removeRectangles(mask, rects);

    vector<int> outputList;
    for (size_t idx = 0; idx < mask.size(); ++idx)
    {
        if ( mask[idx] ) {
            std::queue<Point> points;
            size_t counter = 0;

            points.push( pixelPos(idx) );

            while ( points.size() )
            {
                // get point, remove from queue!
                Point p = points.front();
                points.pop();

                if ( (p.x_ >= 0 && p.x_ < COLS) && (p.y_ >= 0 && p.y_ < ROWS) &&
                     mask[ pixelPos(p) ] )
                {
                    // the point is valid
                    // increment counter
                    ++counter;
                    // invalid pixel
                    mask[ pixelPos(p) ] = false;
                    // add neighbours to queue
                    points.push( Point(p.x_ - 1, p.y_ - 1) );
                    points.push( Point(p.x_    , p.y_ - 1) );
                    points.push( Point(p.x_ + 1, p.y_ - 1) );
                    points.push( Point(p.x_ - 1, p.y_    ) );
                    points.push( Point(p.x_ + 1, p.y_    ) );
                    points.push( Point(p.x_ - 1, p.y_ + 1) );
                    points.push( Point(p.x_    , p.y_ + 1) );
                    points.push( Point(p.x_ + 1, p.y_ + 1) );
                }
            }
            outputList.push_back(counter);
        }
    }

    // sort before output!
    std::sort(outputList.begin(), outputList.end());
    return outputList;
}

TEST(GrafixMask, TestPoint)
{
    EXPECT_EQ( pixelPos( Point(20, 50) ), 50*COLS + 20);
    EXPECT_EQ( pixelPos( Point(0, 50) ), 50*COLS);
    EXPECT_EQ( pixelPos( Point(15, 0) ), 15);

    Point p = pixelPos(50*COLS + 10);
    EXPECT_EQ(p.x_, 10);
    EXPECT_EQ(p.y_, 50);

    p = pixelPos(50*COLS);
    EXPECT_EQ(p.x_, 0);
    EXPECT_EQ(p.y_, 50);

    p = pixelPos(20*COLS + 50);
    EXPECT_EQ(p.x_, 50);
    EXPECT_EQ(p.y_, 20);
}

TEST(GrafixMask, TestConvert)
{
    vector<string> rectangles;
    rectangles.push_back("0 292 399 307");

    Rectangles rects;
    convert(rectangles, rects);

    ASSERT_EQ(rects.size(), 1);
    EXPECT_EQ(rects[0].ul_.x_, 292);
    EXPECT_EQ(rects[0].ul_.y_, 0);
    EXPECT_EQ(rects[0].br_.x_, 307);
    EXPECT_EQ(rects[0].br_.y_, 399);

}

TEST(GrafixMask, Test0)
{
    vector<string> rectangles;
    rectangles.push_back("0 292 399 307");

    vector<int> areas = sortedAreas(rectangles);

    ASSERT_EQ(areas.size(), 2);
    ASSERT_EQ(areas[0], 116800);
    ASSERT_EQ(areas[1], 116800);
}

TEST(GrafixMask, Test1)
{
    vector<string> rectangles;
    rectangles.push_back("48 192 351 207");
    rectangles.push_back("48 392 351 407");
    rectangles.push_back("120 52 135 547");
    rectangles.push_back("260 52 275 547");

    vector<int> areas = sortedAreas(rectangles);

    ASSERT_EQ(areas.size(), 2);
    ASSERT_EQ(areas[0], 22816);
    ASSERT_EQ(areas[1], 192608);
}

TEST(GrafixMask, Test2)
{
    vector<string> rectangles;
    rectangles.push_back("0 192 399 207");
    rectangles.push_back("0 392 399 407");
    rectangles.push_back("120 0 135 599");
    rectangles.push_back("260 0 275 599");

    vector<int> areas = sortedAreas(rectangles);

    ASSERT_EQ(areas.size(), 9);
    ASSERT_EQ(areas[0], 22080);
    ASSERT_EQ(areas[1], 22816);
    ASSERT_EQ(areas[2], 22816);
    ASSERT_EQ(areas[3], 23040);
    ASSERT_EQ(areas[4], 23040);
    ASSERT_EQ(areas[5], 23808);
    ASSERT_EQ(areas[6], 23808);
    ASSERT_EQ(areas[7], 23808);
    ASSERT_EQ(areas[8], 23808);
}

TEST(GrafixMask, Test3)
{
    vector<string> rectangles;
    rectangles.push_back("0 20 399 20");
    rectangles.push_back("0 44 399 44");
    rectangles.push_back("0 68 399 68");
    rectangles.push_back("0 92 399 92");
    rectangles.push_back("0 116 399 116");
    rectangles.push_back("0 140 399 140");
    rectangles.push_back("0 164 399 164");
    rectangles.push_back("0 188 399 188");
    rectangles.push_back("0 212 399 212");
    rectangles.push_back("0 236 399 236");
    rectangles.push_back("0 260 399 260");
    rectangles.push_back("0 284 399 284");
    rectangles.push_back("0 308 399 308");
    rectangles.push_back("0 332 399 332");
    rectangles.push_back("0 356 399 356");
    rectangles.push_back("0 380 399 380");
    rectangles.push_back("0 404 399 404");
    rectangles.push_back("0 428 399 428");
    rectangles.push_back("0 452 399 452");
    rectangles.push_back("0 476 399 476");
    rectangles.push_back("0 500 399 500");
    rectangles.push_back("0 524 399 524");
    rectangles.push_back("0 548 399 548");
    rectangles.push_back("0 572 399 572");
    rectangles.push_back("0 596 399 596");
    rectangles.push_back("5 0 5 599");
    rectangles.push_back("21 0 21 599");
    rectangles.push_back("37 0 37 599");
    rectangles.push_back("53 0 53 599");
    rectangles.push_back("69 0 69 599");
    rectangles.push_back("85 0 85 599");
    rectangles.push_back("101 0 101 599");
    rectangles.push_back("117 0 117 599");
    rectangles.push_back("133 0 133 599");
    rectangles.push_back("149 0 149 599");
    rectangles.push_back("165 0 165 599");
    rectangles.push_back("181 0 181 599");
    rectangles.push_back("197 0 197 599");
    rectangles.push_back("213 0 213 599");
    rectangles.push_back("229 0 229 599");
    rectangles.push_back("245 0 245 599");
    rectangles.push_back("261 0 261 599");
    rectangles.push_back("277 0 277 599");
    rectangles.push_back("293 0 293 599");
    rectangles.push_back("309 0 309 599");
    rectangles.push_back("325 0 325 599");
    rectangles.push_back("341 0 341 599");
    rectangles.push_back("357 0 357 599");
    rectangles.push_back("373 0 373 599");
    rectangles.push_back("389 0 389 599");

    vector<int> areas = sortedAreas(rectangles);

//    ASSERT_EQ(areas.size(), 9);
    ASSERT_EQ(areas[0], 15);
    ASSERT_EQ(areas[1], 30);
    ASSERT_EQ(areas[2], 45);
    ASSERT_EQ(areas[3], 45);
//    ASSERT_EQ(areas[4], 23040);
//    ASSERT_EQ(areas[5], 23808);
//    ASSERT_EQ(areas[6], 23808);
//    ASSERT_EQ(areas[7], 23808);
//    ASSERT_EQ(areas[8], 23808);
}
