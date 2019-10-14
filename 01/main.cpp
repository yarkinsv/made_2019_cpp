//
//  main.cpp
//  homework-01
//
//  Created by s.yarkin on 10/10/2019.
//  Copyright © 2019 s.yarkin. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include "parser.hpp"


int main(int argc, const char* argv[]) {
    if (argc < 2) {
        printf("Pass expression as an argument...");
        return EXIT_FAILURE;
    }
    try {
        float x = eval(std::string{argv[1]});
        printf("%s\n%f\n", argv[1], x);
    } catch(const std::invalid_argument& e) {
        printf("Got error: %s\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
