#include <string>
#include "tokenizer.hpp"


bool isNumber(const std::string& token) {
    if (token.length() > 10 || token.length() == 0) {
        return false;
    }
    for (char c : token) {
        if (c < '0' || c > '9') {
            return false;
        }
    }
    return !(token.length() == 10 && token > "2147483647");
}

void Tokenizer::parse(const std::string& str) {
    for (const auto& onStartCallback : onStartCallbacks) {
        onStartCallback();
    }

    for (unsigned long start = 0; start < str.length(); start++) {
        while (start < str.length() && isspace(str[start])) {
            start++;
        }
        unsigned long end = start;
        while (end < str.length() && !isspace(str[end])) {
            end++;
        }
        if (start != end) {
            std::string token = str.substr(start, end - start);
            for (const auto& onParseTokenCallback : onParseTokenCallbacks) {
                TokenPredicate predicate;
                OnTokenParsed callback;
                std::tie(predicate, callback) = onParseTokenCallback;
                if (predicate(token)) {
                    callback(token);
                }
            }
        }
        start = end;
    }

    for (const auto& onFinishCallback : onFinishCallbacks) {
        onFinishCallback();
    }
}
