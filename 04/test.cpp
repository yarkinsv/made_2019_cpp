#include "matrix.cpp"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* just_test() {

    return nullptr;
}


static const char* all_tests() {

    return nullptr;
}
 
//int main(int argc, const char* argv[]) {
//    const char* result = all_tests();
//    if (result != nullptr) {
//        std::cout << result;
//    } else {
//        std::cout << "ALL TESTS PASSED\n";
//    }
//    std::cout << "Tests run: " << tests_run;
//    return 0;
// }
