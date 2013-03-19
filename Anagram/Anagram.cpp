#include <string>
#include <vector>
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

int main()
{
    assert( checkAnagramV1("davide", "dviaed") );
    assert( !checkAnagramV1("davde", "dviaed") );
    assert( !checkAnagramV1("aaaa", "aaa") );
    assert( checkAnagramV1("abcd", "dbca") );

    std::cout << " all ok! " << std::endl;
    return 0;
}
