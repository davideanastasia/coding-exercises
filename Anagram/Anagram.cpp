#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <iostream>
#include <cassert>
#include <algorithm>

bool checkAnagramV1(const std::string& str1, const std::string& str2)
{
    if (str1.length() != str2.length()) return false;

    std::vector<char> temp1(str1.begin(), str1.end());
    std::sort(temp1.begin(), temp1.end());

    std::vector<char> temp2(str2.begin(), str2.end());
    std::sort(temp2.begin(), temp2.end());

    for (size_t idx = 0; idx < temp1.size(); ++idx)
    {
        if ( temp1[idx] != temp2[idx] ) return false;
    }
    return true;
}

typedef std::map<char, size_t> CharMap;
bool checkAnagramV2(const std::string &str1, const std::string &str2)
{
    if (str1.length() != str2.length()) return false;

    CharMap map_str1;
    for (std::string::const_iterator it = str1.begin(), itEnd = str1.end();
         it != itEnd; ++it)
    {
        ++map_str1[*it];
    }

    CharMap map_str2;
    for (std::string::const_iterator it = str2.begin(), itEnd = str2.end();
         it != itEnd; ++it)
    {
        ++map_str2[*it];
    }

    if (map_str1.size() != map_str2.size()) return false;

    CharMap::iterator it1Begin = map_str1.begin();
    CharMap::iterator it1End = map_str1.end();
    CharMap::iterator it2Begin = map_str2.begin();

    for ( ; it1Begin != it1End; ++it1Begin, ++it2Begin)
    {
        if ( it1Begin->first != it2Begin->first ) return false;
        if ( it1Begin->second != it2Begin->second ) return false;
    }
    return true;
}

int main()
{
    assert( checkAnagramV1("davide", "dviaed") );
    assert( !checkAnagramV1("davde", "dviaed") );
    assert( !checkAnagramV1("aaaa", "aaa") );
    assert( checkAnagramV1("abcd", "dbca") );

    assert( checkAnagramV2("davide", "dviaed") );
    assert( !checkAnagramV2("davde", "dviaed") );
    assert( !checkAnagramV2("aaaa", "aaa") );
    assert( checkAnagramV2("abcd", "dbca") );


    std::cout << " all ok! " << std::endl;
    return 0;
}
