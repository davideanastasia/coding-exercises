/*
 * SmartWorldToy - TopCoder
 *
 * A word composed of four Latin lowercase letters is given. With a single
 * button click you can change any letter to the previous or next letter in
 * alphabetical order (for example 'c' can be changed to 'b' or 'd').
 * The alphabet is circular, thus 'a' can become 'z', and 'z' can become 'a'
 * with one click.
 *
 * A collection of constraints is also given, each defining a set of forbidden
 * words. A constraint is composed of 4 strings of letters. A word is forbidden
 * if each of its characters is contained in corresponding string of a single
 * constraint, i.e. first letter is contained in the first string, the second
 * letter - in the second string, and so on. For example, the constraint
 * "lf a tc e" defines the words "late", "fate", "lace" and "face".
 *
 * You should find the minimum number of button presses required to reach the
 * word finish from the word start without passing through forbidden words, or
 * return -1 if this is not possible.
 */

#include <string>
#include <iostream>
#include <set>
#include <queue>
#include <iterator>
#include <sstream>
#include <cassert>

using namespace std;

typedef set< string > Dictionary;

Dictionary buildForbiddenDictionary(const string& forbiddenSeq)
{
    vector<string> tokens;
    istringstream iss(forbiddenSeq);
    copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter< vector<string> >(tokens));

    Dictionary dict;
    // build initial dictionary
    for (size_t idx = 0; idx < tokens[0].size(); ++idx)
    {
        dict.insert( tokens[0].substr(idx, 1) );
    }

    // for each letter in the
    for (size_t idx = 1; idx < tokens.size(); ++idx)
    {
        Dictionary innerDict;
        // for each word currently in the dictionary...
        for ( Dictionary::iterator it = dict.begin(), itEnd = dict.end(); it != itEnd; ++it)
        {
            for (size_t idxChar = 0; idxChar < tokens[idx].size(); ++idxChar)
            {
                innerDict.insert( *it + tokens[idx].substr(idxChar, 1) );
            }
        }

        dict.swap(innerDict);
    }

    return dict;
}

Dictionary buildForbiddenDictionary(const vector<string>& forbiddenSequences)
{
    Dictionary dict;

    for (size_t idx = 0; idx < forbiddenSequences.size(); ++idx)
    {
        Dictionary innerDict = buildForbiddenDictionary(forbiddenSequences[idx]);

        // insert inside the main dictionary
        dict.insert(innerDict.begin(),
                    innerDict.end());
    }

    return dict;
}

struct WordDepth
{
    WordDepth(const std::string& word,
              int depth)
        : word_(word)
        , depth_(depth)
    {}

    string word_;
    int depth_;
};

typedef std::queue< WordDepth > MyBreadthFirstSearch;

// I use the parameter as internal temporary
std::string switchLetter(std::string word, size_t pos, size_t upDown)
{
    assert( upDown == 1 || upDown == 0 );

    // down!
    if ( upDown == 0 )
    {
        char c = word[pos] - 1;
        word[pos] = (c == 96) ? 'z' : c;
    }
    else if ( upDown == 1 )
    {
        char c = word[pos] + 1;
        word[pos] = (c == 123) ? 'a' : c;
    }

    return word;
}

int countSteps(const string& beginWord,
               const string& targetWord,
               const Dictionary& forbiddenWords)
{
    Dictionary exploredNodes(forbiddenWords);
    MyBreadthFirstSearch queue;
    queue.push( WordDepth(beginWord, 0) );

    while ( queue.size() )
    {
        WordDepth curr( queue.front() );
        exploredNodes.insert( curr.word_ );
        queue.pop();

        // std::cout << "Depth : " << curr.depth_ << " / Word: " << curr.word_ << "\n";

        if ( curr.word_ == targetWord ) {
            // I have found my target!
            return curr.depth_;
        }

        // generate further nodes, and check whether we like them!
        for (size_t pos = 0; pos < curr.word_.size(); ++pos) {
            for (size_t ctrl = 0; ctrl <= 1; ++ctrl)
            {
                string newWord = switchLetter(curr.word_, pos, ctrl);
                // if not in the list of the banned word... add the the nodes
                // to be explored
                if ( !exploredNodes.count(newWord) )
                {
                    queue.push( WordDepth(newWord, curr.depth_+1));
                }
            }
        }
        // sleep(1);
    }

    return -1;
}

void test0()
{
    vector<string> forbiddenSequence;
    forbiddenSequence.push_back("a a a z");
    forbiddenSequence.push_back("a a z a");
    forbiddenSequence.push_back("a z a a");
    forbiddenSequence.push_back("z a a a");

    Dictionary forbiddenDictionary(buildForbiddenDictionary(forbiddenSequence));
    copy(forbiddenDictionary.begin(), forbiddenDictionary.end(),
         ostream_iterator<string>(cout, "\n"));

    std::cout << countSteps("aaaa", "zzzz",
                            forbiddenDictionary) << "\n";
}


void test1()
{
    std::cout << countSteps("aaaa", "bbbb",
                            Dictionary()) << "\n";
}

void test2()
{
    std::cout << countSteps("aaaa", "mmnn",
                            Dictionary()) << "\n";
}

void test3()
{
    vector<string> forbiddenSequence;
    forbiddenSequence.push_back("bz a a a");
    forbiddenSequence.push_back("a bz a a");
    forbiddenSequence.push_back("a a bz a");
    forbiddenSequence.push_back("a a a bz");

    Dictionary forbiddenDictionary(buildForbiddenDictionary(forbiddenSequence));
    copy(forbiddenDictionary.begin(), forbiddenDictionary.end(),
         ostream_iterator<string>(cout, "\n"));

    std::cout << countSteps("aaaa", "bbbb",
                            forbiddenDictionary) << "\n";
}

int main()
{
    test0();
    test1();
    // test2();
    test3();

    return 0;
}
