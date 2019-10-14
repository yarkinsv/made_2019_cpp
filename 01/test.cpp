//
//  test.cpp
//  homework-01
//
//  Created by s.yarkin on 11/10/2019.
//  Copyright © 2019 s.yarkin. All rights reserved.
//

#include <iostream>
#include <cmath>
#include "parser.hpp"

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

const float EPSILON = 0.00001;

static bool check(const std::string& str, const float expected) {
    return fabs((eval(str) - expected)) < EPSILON;
}

static const char* test_1() {
    mu_assert("'1+2' = 3 FAIL", check("1+2", 3));
    return 0;
}

static const char* test_2() {
    mu_assert("'1+-2' = -1 FAIL", check("1+-2", -1));
    return 0;
}

static const char* test_3() {
    mu_assert("'1*2/2' = 1 FAIL", check("1*2/2", 1));
    return 0;
}

static const char* test_4() {
    mu_assert("'10 - -1 + 5/8 - 23/4 + 2*76' = 158.625 FAIL", check("10 - -1 + 5/8 - 23/4 + 2*76", 157.875));
    return 0;
}

static const char* test_5() {
    mu_assert("'1*2*3*-4*--5*---6/----5' = 144 FAIL", check("1*2*3*-4*--5*---6/----5", 144));
    return 0;
}

static const char* test_6() {
    mu_assert("'      1    +  -    2    /   4    * 2' = 0 FAIL", check("      1     +  -    2    /   4    * 2", 0));
    return 0;
}

static const char* test_7() {
    mu_assert("'-100' = -100 FAIL", check("-100", -100));
    return 0;
}

static const char* test_8() {
    mu_assert("'-1 - -2 - -3 - -4' = 8 FAIL", check("-1 - -2 - -3 - -4", 8));
    return 0;
}
static const char* test_9() {
    mu_assert("'-11/-3 + 12/33 - 12*3 + 3 - 2*45/32' = -31.7821 FAIL",
              check("-11/-3 + 12/33 - 12*3 + 3 - 2*45/32", -31.78219696969697));
    return 0;
}
 
static const char* all_tests() {
    mu_run_test(test_1);
    mu_run_test(test_2);
    mu_run_test(test_3);
    mu_run_test(test_4);
    mu_run_test(test_5);
    mu_run_test(test_6);
    mu_run_test(test_7);
    mu_run_test(test_8);
    mu_run_test(test_9);
    return 0;
}
 
int main(int argc, const char* argv[]) {
    const char* result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    } else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return 0;
 }
