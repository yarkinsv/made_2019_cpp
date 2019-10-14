//
//  parser.cpp
//  homework-01
//
//  Created by s.yarkin on 12/10/2019.
//  Copyright © 2019 s.yarkin. All rights reserved.
//

#include <iostream>

using namespace std;

static void skip_spaces(const string& input, int& idx) {
    while (input[idx] == ' ') {
        idx++;
    }
}

static float number(const string& input, int& idx) {
    skip_spaces(input, idx);
    if (input[idx] == '-') {
        idx++;
        return -number(input, idx);
    }
    int start = idx;
    while (input[idx] >= '0' & input[idx] <= '9') {
        idx++;
    }
    if (start == idx) {
        throw invalid_argument(string{ "Expected number, got " } + input[idx] + string{ ", at index " } + to_string(idx));
    }
    int end = idx;
    skip_spaces(input, idx);
    return stof(input.substr(start, end - start));
}

static float term(const string& input, int& idx) {
    float result = number(input, idx);
    while (input[idx] == '/') {
        idx++;
        float divisor = number(input, idx);
        if (divisor == 0) {
            throw invalid_argument("Division by zero");
        }
        result /= divisor;
    }
    if (input[idx] == '*') {
        idx++;
        return result * term(input, idx);
    }
    return result;
}

static float expression(const string& input, int& idx) {
    skip_spaces(input, idx);
    if (input[idx] == '\0') {
        return 0.0;
    }
    float result = term(input, idx);
    if (input[idx] == '+') {
        idx++;
        return result + expression(input, idx);
    } else if (input[idx] == '-') {
        return result + expression(input, idx);
    }
    return result;
}

float eval(const string& input) {
    if (input.length() == 0) {
        throw invalid_argument("Empty string");
    }
    int i = 0;
    float result = expression(input, i);
    if (i != input.length()) {
        throw invalid_argument(
                string{ "Unknown symbol " } + input[i] + string{ ", at index " } + to_string(i) +
                string{ "expected space, \\0 or math symbol" });
    }
    return result;
}
