#ifndef UTIL_H
#define UTIL_H

#include <string>

std::string uniq(const std::string &s) {
    std::string r = "";
    for(auto i : s) {
        if (r.find(i) == std::string::npos) {
            r += i;
        }
    }
    return r;
}

#endif // UTIL_H
