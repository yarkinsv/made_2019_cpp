#include <iostream>
#include <string>
#include <sstream>


template<typename T>
void format_n(int n, std::stringstream& out, T&& t) {
    out << t;
}


template<typename T, typename... Args>
void format_n(int n, std::stringstream& out, T&& t, Args&&... args) {
    if (n == 0) {
        out << t;
    } else {
        format_n(n - 1, out, std::forward<Args>(args)...);
    }
}

template<typename T, typename... Args>
std::string format(const std::string& str, T&& t, Args&&... args) {
    std::stringstream out;
    size_t args_num = sizeof...(args) + 1;
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
                format_n(arg_num, out, t, std::forward<Args>(args)...);
                found_argument = false;
                break;
            }
            default:
                if (found_argument) {
                    if (!std::isdigit(c)) {
                        throw std::runtime_error("Unknown symbol as argument number: " + std::string{c});
                    }
                } else {
                    out << c;
                }
                break;
        }
    }
    return out.str();
}
