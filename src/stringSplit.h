#ifndef _STRING_SPLIT_H_
#define _STRING_SPLIT_H_

#include <string>
#include <iterator>
#include <set>
#include <algorithm>
#include <sstream>


template <typename C>
size_t split(const std::basic_string<char> & s,
             C &container,
             char const delimiter,
             size_t maxelems = 0,
             bool keepBlankFields = true) {

    size_t n = 0;
    std::basic_string<char>::const_iterator it;
    it = s.begin();
    std::basic_string<char>::const_iterator end = s.end(), first;


    for (first = it; it != end; ++it) {
        // Examine each character and if it matches the delimiter
        if (delimiter == *it) {
            if (keepBlankFields || first != it) {
                // extract the current field from the string and
                // append the current field to the given container
                
                if (n + 1 == maxelems) {
                    container.push_back(std::basic_string<char>(first, end));
                    return maxelems;
                    }

                container.push_back(std::basic_string<char>(first, it));
                ++n;
                
                // skip the delimiter
                first = it + 1;
                }
            else  {
                ++first;
                }
            }
        }

    if (keepBlankFields || first != it) {
        // extract the last field from the string and
        // append the last field to the given container
        container.push_back(std::basic_string<char>(first, it));
        ++n;
        }

    return n;
}

template <typename C>
size_t split(const std::basic_string<char> & s,
             C &container,
             const std::set<char> &delimiters,
             size_t maxelems = 0,
             bool keepBlankFields = true) {

    size_t n = 0;
    std::basic_string<char>::const_iterator it;
    it = s.begin();
    std::basic_string<char>::const_iterator end = s.end(), first;


    for (first = it; it != end; ++it) {
        // Examine each character and if it matches the delimiter
        //if (delimiter == *it) {
        if (delimiters.find(*it) != delimiters.end()) {
            if (keepBlankFields || first != it) {
                // extract the current field from the string and
                // append the current field to the given container
                
                if (n + 1 == maxelems) {
                    container.push_back(std::basic_string<char>(first, end));
                    return maxelems;
                    }

                container.push_back(std::basic_string<char>(first, it));
                ++n;
                
                // skip the delimiter
                first = it + 1;
                }
            else  {
                ++first;
                }
            }
        }

    if (keepBlankFields || first != it) {
        // extract the last field from the string and
        // append the last field to the given container
        container.push_back(std::basic_string<char>(first, it));
        ++n;
        }

    return n;
}
/*
std::string string_replace(const std::string &str, \
                            const std::string &what, \
                            const std::string &with) { 

    std::string s = str;
    std::string::iterator i = s.begin();

    while(i != s.end()) { 
        i = std::search(i, s.end(), what.begin(), what.end());

        if (i != s.end()) { 
            i  = s.erase(i, i + what.length());
            
            //Buggy!
            //s.insert(i, with.begin(), with.end());
            
            //FIXME: Do something useful
            for (std::string::const_iterator j = with.begin(); j != with.end(); j++) { 
                i = s.insert(i, *j) + 1;
                }
            }
        }

    return s;
}
*/
#include "StringUtils.h"

#endif /* _STRING_SPLIT_H_ */
