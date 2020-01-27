#include <iostream>
#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stack>
#include <cxxopts.hpp>
#include "SuffixArray.hpp"

struct Tweet {
    // text in the tweet
    std::string text;
    // end index in the concatenated string
    int end_idx;

    Tweet(const std::string& text, int end_idx) : text(text), end_idx(end_idx) {}
    Tweet& operator=(const Tweet& other) = default;
};

inline bool isPrefix(const std::string& shorter, const std::string& longer) {
    return std::mismatch(shorter.begin(), shorter.end(), longer.begin()).first == shorter.end();
}

int searchTweet(const std::vector<Tweet>& tweets, int left, int right, int start_idx) {
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (tweets[mid].end_idx >= start_idx) {
            right = mid;
        }
        else {
            left = mid + 1;
        }
    }
    return right;
}

int main(int argc, char *argv[]) {
    // path to SQLite database with tweets
    std::string DB_PATH;
    // minimum size of the common substring
    int MIN_COMMOM_SUBSTRING_SIZE = 40;
    // minimum number of suffixes that contains the common substring
    int MIN_COMMOM_NUM = 5;
    // limitation to the number of tweets to search from
    int TWEET_NUM_LIMIT = 1000;

    cxxopts::Options options("meme", "Twitter memes finder");

    options.add_options()
        ("d, db", "Path to SQLite database with tweets", cxxopts::value<std::string>())
        ("s, size", "Minimum size of the common substring (default: " + std::to_string(MIN_COMMOM_SUBSTRING_SIZE) + ")", cxxopts::value<unsigned int>())
        ("n, num", "Minimum number of suffixes that contains the common substring (default: " + std::to_string(MIN_COMMOM_NUM) + ")", cxxopts::value<unsigned int>())
        ("l, limit", "Limitation to the number of tweets to search from (default: " + std::to_string(TWEET_NUM_LIMIT) + ")", cxxopts::value<unsigned int>());

    auto result = options.parse(argc, argv);

    if (result.count("d") == 0) {
        std::cout << options.help() << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        DB_PATH = result["d"].as<std::string>();
    }
    if (result.count("s") > 0) {
        MIN_COMMOM_SUBSTRING_SIZE = result["s"].as<unsigned int>();
    }
    if (result.count("n") > 0) {
        MIN_COMMOM_NUM = result["n"].as<unsigned int>();
    }
    if (result.count("l") > 0) {
        TWEET_NUM_LIMIT = result["l"].as<unsigned int>();
    }

    try {
        SQLite::Database db(DB_PATH);

        SQLite::Statement query(db, "SELECT tweet FROM tweets LIMIT ?");
        query.bind(1, TWEET_NUM_LIMIT);

        std::vector<Tweet> tweets;
        std::string big_str = "";

        while (query.executeStep()) {
            std::string text = query.getColumn(0);
            big_str.append(text);
            int end_idx = big_str.size() - 1;
            tweets.emplace_back(text, end_idx);
        }

        std::stack<int> start_idxs = [=]() -> std::stack<int> {
            // create a suffix array from the big string
            SuffixArray suffix_arr = createSuffixArray(big_str);
            for (SuffixArray::iterator curr_it = suffix_arr.begin(); curr_it != suffix_arr.end(); curr_it++) {
                const std::string& curr_suffix_str = curr_it->str;
                if (curr_suffix_str.size() < MIN_COMMOM_SUBSTRING_SIZE) {
                    continue;
                }
                std::string target = curr_suffix_str.substr(0, MIN_COMMOM_SUBSTRING_SIZE);
                // find the first iter where the target is not a prefix
                SuffixArray::iterator target_end_it = std::find_if_not(curr_it + 1, suffix_arr.end(), [target](const Suffix& suffix) {
                    return suffix.str.size() >= target.size() && isPrefix(target, suffix.str);
                });
                // check if the number of tweets we found is enough
                int tweet_found_num = target_end_it - curr_it;
                if (tweet_found_num < MIN_COMMOM_NUM) {
                    curr_it = target_end_it - 1;
                    continue;
                }
                else {
                    std::cout << "[Common substring: " << target << "]" << std::endl;
                    std::vector<int> start_idxs;
                    for (int i = 0; i < tweet_found_num; i++) {
                        start_idxs.emplace_back((curr_it + i)->start_idx);
                    }
                    std::sort(start_idxs.begin(), start_idxs.end(), std::greater<int>());
                    return std::stack<int>(std::deque<int>(start_idxs.begin(), start_idxs.end()));
                }
            }
            return std::stack<int>();
        }();

        if (start_idxs.empty()) {
            std::cout << "No results found" << std::endl;
        }
        else {
            // print tweets
            int curr_idx = 0;
            while (curr_idx < tweets.size()) {
                int target_idx = searchTweet(tweets, curr_idx, tweets.size() - 1, start_idxs.top());
                std::cout << std::endl
                          << std::string(50, '=') << "[ Tweet ]" << std::string(50, '=') << std::endl
                          << tweets[target_idx].text << std::endl
                          << std::string(109, '=') << std::endl;
                start_idxs.pop();
                // remove repeated tweets
                while (!start_idxs.empty() && tweets[target_idx].end_idx >= start_idxs.top()) {
                    start_idxs.pop();
                }
                if (start_idxs.empty()) {
                    break;
                }
                curr_idx = target_idx + 1;
            }
        }
    }
    catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}