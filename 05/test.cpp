#include "serializer.hpp"
#include <iostream>
#include <sstream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

struct Data {
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }
};

static const char* just_test() {
    Data x { 1, true, 2 };

    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);

    Data y { 0, false, 0 };

    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);

    assert(err == Error::NoError);

    assert(x.a == y.a);
    assert(x.b == y.b);
    assert(x.c == y.c);

    std::stringstream ss;
    ss << "treu 123 23";
    Deserializer deserializer2(ss);
    const Error err2 = deserializer2.load(y);

    assert(err2 == Error::CorruptedArchive);

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
