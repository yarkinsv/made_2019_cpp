#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>


template<typename T>
std::string to_str(T&& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}


template<typename... Args>
std::string format(const std::string& str, Args&&... args) {
    std::string out;
    std::vector<std::string> args_string{ to_str(std::forward<Args>(args))... };
    size_t args_num = sizeof...(args);
    size_t start_argument = 0;
    bool found_argument = false;
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        switch (c) {
            case '{': {
                if (found_argument) {
                    throw std::runtime_error("wrong symbol after {: "+ std::string{c});
                }
                start_argument = i;
                found_argument = true;
                break;
            }
            case '}': {
                if (!found_argument) {
                    throw std::runtime_error("found } that doesn't correspond to any {");
                }
                size_t arg_num = std::stol(str.substr(start_argument + 1, i - start_argument - 1));
                if (arg_num >= args_num) {
                    throw std::runtime_error("Argument number " + std::to_string(arg_num) + " was not provided");
                }
                out += args_string[arg_num];
                found_argument = false;
                break;
            }
            default:
                if (found_argument) {
                    if (!std::isdigit(c)) {
                        throw std::runtime_error("Unknown symbol as argument number: " + std::string{c});
                    }
                } else {
                    out += c;
                }
                break;
        }
    }
    return out;
}
