#include <string>
#include "tokenizer.hpp"


static bool isNumber(const std::string& token) {
    bool check = true;
    try {
        stol(token);
    } catch (const std::invalid_argument& e) {
        check = false;
    } catch (const std::out_of_range& e) {
        check = false;
    }
    return check;
}

static bool isNotNumber(const std::string& token) {
    return !isNumber(token);
}

static bool isDelimiter(const char& c) {
    return c == ' ' || c == '\t' || c == '\n';
}

void Tokenizer::parse(const std::string& str) {
    for (const auto& onStartCallback : onStartCallbacks) {
        onStartCallback();
    }

    for (unsigned long start = 0; start < str.length(); start++) {
        while (start < str.length() && isDelimiter(str[start])) {
            start++;
        }
        unsigned long end = start;
        while (end < str.length() && !isDelimiter(str[end])) {
            end++;
        }
        if (start != end) {
            std::string token = str.substr(start, end - start);
            for (const auto& onParseTokenCallback : onParseTokenCallbacks) {
                auto predicate = onParseTokenCallback.first;
                auto callback = onParseTokenCallback.second;
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

void Tokenizer::addOnParseNumber(const OnTokenParsed& callback) {
    addOnParseToken(isNumber, callback);
}

void Tokenizer::addOnParseNumber(const OnNumberParsed& callback) {
    addOnParseToken(isNumber, [&callback](const std::string& token) { callback(stol(token)); });
}

void Tokenizer::addOnParseText(const OnTokenParsed& callback) {
    addOnParseToken(isNotNumber, callback);
}

void Tokenizer::addOnParseToken(const TokenPredicate& predicate, const OnTokenParsed& callback) {
    onParseTokenCallbacks.emplace_back(predicate, callback);
}

void Tokenizer::addOnStart(const OnStart& callback) {
    onStartCallbacks.push_back(callback);
}

void Tokenizer::addOnFinish(const OnFinish& callback) {
    onFinishCallbacks.push_back(callback);
}
