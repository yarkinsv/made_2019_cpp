//
//  main.cpp
//  homework-01
//
//  Created by s.yarkin on 10/10/2019.
//  Copyright © 2019 s.yarkin. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include "parser.hpp"


int main(int argc, const char* argv[]) {
    const char * str = argv[1];
    
    try {
        float x = eval(str);
        printf("%s\n%f\n",str,x);
    } catch(const std::invalid_argument& e) {
        printf("Got error: %s\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
