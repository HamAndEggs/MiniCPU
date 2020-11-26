#ifndef __UTIL_H__
#define __UTIL_H__

#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

typedef std::vector<std::string> StringVec;

inline const std::string ToString(const StringVec& pStringVector)
{
    std::stringstream str;
    for( const auto& s : pStringVector )
    {
        str << "[" << s << "]";
    }
    return str.str();
}

inline std::string TrimWhiteSpace(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
        it++;

    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
        rit++;

    return std::string(it, rit.base());
}

inline StringVec SplitString(const std::string& pString, const char* pSeperator)
{
    StringVec res;
    for (size_t p = 0, q = 0; p != pString.npos; p = q)
    {
        // Make sure it is not just all spaces.
        const std::string s = pString.substr(p + (p != 0), (q = pString.find(pSeperator, p + 1)) - p - (p != 0));
        const std::string cleaned = TrimWhiteSpace(s);
        
        if( s.size() > 0 )
        {
            res.push_back(s);
        }
    }
    return res;
}

inline StringVec SplitInstruction(const std::string& a_InstructionDescription)
{
    if( a_InstructionDescription.size() == 0 )
    {
        throw std::runtime_error("No instruction found for instruction declaration [" + a_InstructionDescription + "]");
    }

    // First remove any comment.
    const std::string cleaned = TrimWhiteSpace(SplitString(a_InstructionDescription,"//")[0]);

    StringVec res = SplitString(cleaned," ");

    if( res.size() == 0 )
    {
        throw std::runtime_error("No instruction found for instruction declaration [" + cleaned + "]");
    }

    if( res.size() != 2 )
    {
        throw std::runtime_error("Bad formatted instruction found, res.size() == " + std::to_string(res.size()) + " [" + cleaned + "] " + ToString(res));
    }

    // Now, if there are paramiters, we need to condition them. Remove start and end space.
    for( auto& s : res )
    {
        s = TrimWhiteSpace(s);
    }

    return res;
}

inline bool CompareNoCase(const std::string& a_String,const char* pB,int pLength = 0)
{
    if( a_String.size() == 0 || pB == nullptr )
    {
        throw std::runtime_error("CompareNoCase passed a null pointer");
    }

    const char* pA = a_String.c_str();

    if( pLength <= 0 )
        pLength = 0x7fff;

    while( (*pA != 0 || *pB != 0) && pLength > 0 )
    {
        // Get here are one of the strings has hit a null then not the same.
        // The while loop condition would not allow us to get here if both are at the null terminator.
        if( *pA == 0 || *pB == 0 )
        {
            return false;
        }

        if( tolower(*pA) != tolower(*pB) )
        {
            return false;
        }

        pA++;
        pB++;
        pLength--;
    };

    // Get here, they are the same.
    return true;
}

#endif // __UTIL_H__
