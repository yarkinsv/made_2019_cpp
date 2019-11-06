#include "matrix.cpp"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

static const char* test1() {
    Matrix m(5, 2);
    m[1][1] = 10055;
    mu_assert("FAIL: m[1][1] == 10055", m[1][1] == 10055);
    return nullptr;
}

static const char* test2() {
    Matrix m(5, 2);
    m[1][1] = 10055;
    mu_assert("FAIL: m.getColumns() == 2", m.getColumns() == 2);
    mu_assert("FAIL: m.getRows() == 5", m.getRows() == 5);
    return nullptr;
}

static const char* test3() {
    Matrix m(5, 2);
    m[1][1] = 10055;
    m[3][0] = 2;
    m *= 2;
    mu_assert("FAIL: m[1][1] == 20110", m[1][1] == 20110);
    mu_assert("FAIL: m[3][0] == 4", m[3][0] == 4);
    return nullptr;
}

static const char* test4() {
    Matrix m1(5, 2);
    m1[1][1] = 10055;
    m1[3][0] = 2;

    Matrix m2(5, 2);
    m2[1][1] = 10055;
    m2[3][0] = 2;

    Matrix m3(5, 2);
    m3[1][1] = 123;
    m3[3][0] = 2;

    mu_assert("FAIL: m1 == m2", m1 == m2);
    mu_assert("FAIL: m1 != m3", m1 != m3);
    return nullptr;
}

static const char* test5() {
    int* arr = new int[10] { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const Matrix m(5, 2, arr);
    mu_assert("FAIL: m[1][1] == 5", m[1][1] == 3);
    mu_assert("FAIL: m.getColumns() == 2", m.getColumns() == 2);
    mu_assert("FAIL: m.getRows() == 5", m.getRows() == 5);
    // m *= 2 - compilation error, m - const
    //m[1][1] = 4;//  -//-
    return nullptr;
}

static const char* all_tests() {
    mu_run_test(test1);
    mu_run_test(test2);
    mu_run_test(test3);
    mu_run_test(test4);
    mu_run_test(test5);
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
