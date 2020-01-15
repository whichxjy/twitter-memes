#include <string>
#include <vector>

struct Suffix {
    // suffix string
    std::string str;
    // where does the suffix start in the original string?
    int start_idx;

    Suffix(const std::string& str, int start_idx) : str(str), start_idx(start_idx) {}
    Suffix& operator=(const Suffix& other) = default;
};

using SuffixArray = std::vector<Suffix>;

// create suffix array from a string
SuffixArray createSuffixArray(const std::string& str);