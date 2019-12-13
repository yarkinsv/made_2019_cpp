#include "myvector.h"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* test1() {
    Vector v = { 1, 2, 3 };
    v[1] = 10;
    mu_assert("v[0] == 1", v[0] == 1);
    mu_assert("v[1] == 2", v[1] == 10);
    mu_assert("v[2] == 3", v[2] == 3);
    return nullptr;
}

static const char* test2() {
    Vector data1 = { 1, 6, 1, 3, 4, 5, 6 };
    int sum = 0;
    std::for_each(data1.begin(), data1.end(), [&sum](int x) { sum += x; });
    mu_assert("sum == 26", sum == 26);

    std::set<int> data2(data1.begin(), data1.end());
    sum = 0;
    std::for_each(data2.begin(), data2.end(), [&sum](int x) { sum += x; });
    mu_assert("sum == 26", sum == 26);

    return nullptr;
}

static const char* test3() {
    Vector<float> v(1);
    v.push_back(1.1);
    v.push_back(10.0);
    v.push_back(3);
    mu_assert("v[0] == 1", v[0] == 1.1);
    mu_assert("v[1] == 2", v[1] == 10);
    mu_assert("v[2] == 3", v[2] == 3);
    mu_assert("v.size() == 3", v.size() == 3);
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
