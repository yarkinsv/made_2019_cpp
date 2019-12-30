#include <iostream>
#include <vector>
#include <string>


std::vector<int> build_prefix(const std::string& s) {
    std::vector<int> prefix(s.length());
    int j = 0;
    for (size_t i = 1; i < s.length(); i++) {
        while (s[i] != s[j] && j > 0) {
            j = prefix[j - 1];
        }
        if (s[i] == s[j]) {
            j++;
            prefix[i] = j;
        }
    }
    return prefix;
}


std::vector<int> find_substring(const std::string& s, const std::string& pattern) {
    std::vector<int> matches;
    auto prefix = build_prefix(pattern);
    int j = 0;
    for (size_t i = 0; i < s.length(); i++) {
        while (s[i] != pattern[j] && j > 0) {
            j = prefix[j - 1];
        }
        if (s[i] == pattern[j]) {
            j++;
        }
        if (j == pattern.length()) {
            matches.push_back(i - j + 1);
            j = prefix[j - 1];
        }
    }
    return matches;
}


int main() {
    std::string pattern;
    std::string s;
    std::cin >> pattern >> s;
    auto matches = find_substring(s, pattern);
    for (int m : matches) {
        std::cout << m << " ";
    }
    return 0;
}
