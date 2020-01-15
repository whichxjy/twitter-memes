#include <algorithm>
#include "SuffixArray.hpp"

// create suffix array from a string
SuffixArray createSuffixArray(const std::string& str) {
    SuffixArray suffix_arr;
    for (int start_idx = 0; start_idx < str.size(); start_idx++) {
        suffix_arr.emplace_back(str.substr(start_idx), start_idx);
    }
    std::sort(suffix_arr.begin(), suffix_arr.end(), [](const Suffix& lhs, const Suffix& rhs) {
        return lhs.str < rhs.str;
    });
    return suffix_arr;
}