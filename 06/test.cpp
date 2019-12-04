#include "bigint.hpp"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* test1() {
    BigInt a = 1;
    BigInt b = a;
    BigInt c = a + b + 2;
    assert(a == b);
    assert(a == 1);
    assert(b == 1);
    assert(c == 4);
    return nullptr;
}

static const char* test2() {
    BigInt a = 1;
    BigInt b = a;
    BigInt c = a + b - 2;
    assert(-a == -b);
    assert(-a == -1);
    assert(-b == -1);
    assert(c == 0);
    return nullptr;
}

static const char* test3() {
    BigInt a = BIG_INT_MAX_BUFFER_VALUE + 1;
    BigInt b = BIG_INT_MAX_BUFFER_VALUE - 1;
    BigInt c = BIG_INT_MAX_BUFFER_VALUE;

    assert(a == BIG_INT_MAX_BUFFER_VALUE + 1);
    assert(b == BIG_INT_MAX_BUFFER_VALUE - 1);
    assert(c == BIG_INT_MAX_BUFFER_VALUE);
    assert(a+b == BIG_INT_MAX_BUFFER_VALUE + BIG_INT_MAX_BUFFER_VALUE);
    assert(a-b == 2);
    assert(b-a == -2);
    assert(a+b+c == BIG_INT_MAX_BUFFER_VALUE + BIG_INT_MAX_BUFFER_VALUE + BIG_INT_MAX_BUFFER_VALUE);
    return nullptr;
}


static const char* all_tests() {
    mu_run_test(test1);
    mu_run_test(test2);
    mu_run_test(test3);
    return nullptr;
}
 
int main(int argc, const char* argv[]) {
    const char* result = all_tests();
    if (result != nullptr) {
        std::cout << result;
    } else {
        std::cout << "ALL TESTS PASSED\n";
    }
    std::cout << "Tests run: " << tests_run;
    return 0;
 }
