//
//  parser.cpp
//  homework-01
//
//  Created by s.yarkin on 12/10/2019.
//  Copyright © 2019 s.yarkin. All rights reserved.
//

#include <iostream>
#include <stdlib.h>


static void skipSpaces(const char* input, int* idx) {
    while (input[*idx] == ' ') {
        (*idx)++;
    }
}

static float number(const char* input, int* idx) {
    skipSpaces(input, idx);
    if (input[*idx] == '-') {
        (*idx)++;
        return -number(input, idx);
    }
    int start = *idx;
    while (input[*idx] >= '0' & input[*idx] <= '9') {
        (*idx)++;
    }
    if (start == *idx) {
        char* message = (char*)malloc(100 * sizeof(char));
        sprintf(message, "Expected number, got '%c', at index %d", input[*idx], *idx);
        throw std::invalid_argument(message);
    }
    char* buffer = new char;
    int x;
    strncpy(buffer, input + start, *idx - start);
    sscanf(buffer, "%d", &x);
    skipSpaces(input, idx);
    return (float) x;
}

static float term(const char* input, int* idx) {
    float result = number(input, idx);
    while (input[*idx] == '/') {
        (*idx)++;
        float divisor = number(input, idx);
        if (divisor == 0) {
            throw std::invalid_argument("Division by zero");
        }
        result /= divisor;
    }
    if (input[*idx] == '*') {
        (*idx)++;
        return result * term(input, idx);
    }
    return result;
}

static float expression(const char* input, int* idx) {
    skipSpaces(input, idx);
    if (input[*idx] == '\0') {
        return 0.0;
    }
    float result = term(input, idx);
    if (input[*idx] == '+') {
        (*idx)++;
        return result + expression(input, idx);
    } else if (input[*idx] == '-') {
        return result + expression(input, idx);
    }
    return result;
}

float eval(const char* input) {
    int i = 0;
    float result = expression(input, &i);
    if (i != strlen(input)) {
        char* message = (char*)malloc(100 * sizeof(char));
        sprintf(message, "Unknown symbol '%c', at index %d, expected space, \\0 or math symbol", input[i], i);
        throw std::invalid_argument(message);
    }
    return result;
}
