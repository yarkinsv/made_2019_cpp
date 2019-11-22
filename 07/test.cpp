#include <iostream>
#include "format.cpp"

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* test1() {
    auto text = format("{1}+{1} = {0}", 2, "one");
    assert(text == "one+one = 2");
}

static const char* all_tests() {
    mu_run_test(test1);
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
