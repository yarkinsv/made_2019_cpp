#include "threadpool.hpp"
#include <iostream>
#include <sstream>
#include <future>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;


struct A {
    int p;
};

void foo(const A& a) {
    std::cout << "Hello" << a.p;
}


static const char* just_test() {
    auto a = ThreadPool(10);
    auto b = a.exec([]() { return 10; });
    auto c = b.get();

    ThreadPool pool(8);

    auto task1 = pool.exec(foo, A{12});
    task1.get();

    auto task2 = pool.exec([]() { return 1; });
    int x = task2.get();

    assert(x == 1);
    assert(c == 10);

    return nullptr;
}

static const char* all_tests() {
    mu_run_test(just_test);
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
