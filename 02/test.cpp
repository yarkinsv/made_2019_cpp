#include "allocator.hpp"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* test_1() {
    LinearAllocator allocator = LinearAllocator(sizeof(int) * 10);
    int* array_2 = (int*) allocator.alloc(sizeof(int) * 2);
    array_2[0] = 10;
    array_2[1] = 123;
    mu_assert("FAIL", array_2 != nullptr);
    return nullptr;
}

static const char* test_2() {
    LinearAllocator allocator = LinearAllocator(sizeof(int) * 10);
    int* array_2 = (int*) allocator.alloc(sizeof(int) * 2);
    int* array_4 = (int*) allocator.alloc(sizeof(int) * 4);
    mu_assert("FAIL", array_4 == array_2 + 2);
    return nullptr;
}

static const char* test_3() {
    LinearAllocator allocator = LinearAllocator(sizeof(int) * 10);
    int* array_2 = (int*) allocator.alloc(sizeof(int) * 2);
    int* array_40 = (int*) allocator.alloc(sizeof(int) * 40);
    mu_assert("FAIL", array_2 != nullptr && array_40 == nullptr);
    return nullptr;
}

static const char* test_4() {
    LinearAllocator allocator = LinearAllocator(sizeof(int) * 10);
    int* array_4 = (int*) allocator.alloc(sizeof(int) * 4);
    int* array_8 = (int*) allocator.alloc(sizeof(int) * 8);
    allocator.reset();
    int* array_8_new = (int*) allocator.alloc(sizeof(int) * 8);
    mu_assert("FAIL", array_4 != nullptr && array_8 == nullptr && array_8_new == array_4);
    return nullptr;
}

static const char* all_tests() {
    mu_run_test(test_1);
    mu_run_test(test_2);
    mu_run_test(test_3);
    mu_run_test(test_4);
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
