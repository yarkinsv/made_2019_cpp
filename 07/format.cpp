#include <iostream>
#include <string>
#include <sstream>


template<int N, typename T>
void format_n(const int n, std::stringstream& out, T t) {
    if (n == N) {
        out << t;
    } else {
        throw std::runtime_error("Argument number " + std::to_string(n) + " was not provided");
    }
}

template<int N, typename T, typename... Args>
void format_n(const int n, std::stringstream& out, T t, Args... args) {
    if (n == N) {
        out << t;
    } else {
        format_n<N + 1>(n, out, args...);
    }
}

template<typename T, typename... Args>
std::string format(const std::string& str, T t, Args... args) {
    std::stringstream out;
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
                int arg_num = std::stoi(str.substr(start_argument + 1, i - start_argument - 1));
                format_n<0>(arg_num, out, t, args...);
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
