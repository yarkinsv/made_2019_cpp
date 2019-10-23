#include "tokenizer.hpp"
#include <iostream>

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { const char* message = test(); tests_run++; \
                               if (message) return message; } while (0)

int tests_run = 0;

const std::string TEST_TEXT { "Все будут видеть друг-друга, то есть это не простая трансляция, а именно телемост, участники в\n"
                                 "Москве будут видеть участников в Санкт-Петербурге и Новосибирске. Было 700 человек в главном зале, а станет 1200 " };

static const char* just_test() {
    Tokenizer tokenizer;
    tokenizer.addOnStart([]() { std::cout << "Start 1!\n"; });
    tokenizer.addOnStart([]() { std::cout << "Start 2!\n"; });
    tokenizer.addOnStart([]() { std::cout << "Start 3!\n"; });

    tokenizer.addOnParseText([](const std::string& token) { std::cout << token << ' '; });

    long total = 0;
    tokenizer.addOnParseNumber([&total](const long token) { total += token; });
    tokenizer.addOnParseNumber([&total](const std::string& token) { std::cout << "\ntotal updated: " << total << "\n"; });

    tokenizer.addOnFinish([]() { std::cout << "\nFinish 1!\n"; });
    tokenizer.addOnFinish([]() { std::cout << "Finish 2!\n"; });
    tokenizer.addOnFinish([]() { std::cout << "Finish 3!\n"; });
    tokenizer.parse("hello world!! hi 123 and 143");
    return nullptr;
}

static const char* test_numbers() {
    Tokenizer tokenizer;

    long total_sum = 0;
    long total_mul = 1;
    long total_sum_squares = 0;

    tokenizer.addOnParseNumber([&total_sum](const long token) { total_sum += token; });
    tokenizer.addOnParseNumber([&total_mul](const long token) { total_mul *= token; });
    tokenizer.addOnParseNumber([&total_sum_squares](const long token) { total_sum_squares += token * token; });

    tokenizer.parse("1 2 3 4 5");

    assert(total_sum == 15);
    assert(total_mul == 120);
    assert(total_sum_squares == 55);
    return nullptr;
}

static const char* test_words() {
    Tokenizer tokenizer;

    std::string text_wo_spaces;
    std::string text_w_double_spaces;

    tokenizer.addOnParseText([&text_wo_spaces](const std::string& token) { text_wo_spaces += token; });
    tokenizer.addOnParseText([&text_w_double_spaces](const std::string& token) { text_w_double_spaces += token + "  "; });

    tokenizer.parse("hello world it's time to ");

    assert(text_wo_spaces == "helloworldit'stimeto");
    assert(text_w_double_spaces == "hello  world  it's  time  to  ");
    return nullptr;
}

static const char* test_count_tokens() {
    Tokenizer tokenizer;

    long count = 0;
    long count_numbers = 0;
    long count_text_tokens = 0;

    tokenizer.addOnParseToken([](const std::string& token) { return true; }, [&count](const std::string& token) { count++; });
    tokenizer.addOnParseNumber([&count_numbers](const std::string& token) { count_numbers++; });
    tokenizer.addOnParseText([&count_text_tokens](const std::string& token) { count_text_tokens++; });

    tokenizer.parse(TEST_TEXT);

    assert(count == 32);
    assert(count_numbers == 2);
    assert(count_text_tokens == 30);
    return nullptr;
}

static const char* test_count_tokens_started_with_t() {
    Tokenizer tokenizer;

    long count_t = 0;
    long count_t_other_way = 0;

    tokenizer.addOnParseToken([](const std::string& token) { return token.rfind("т", 0) == 0; },
            [&count_t](const std::string& token) { count_t++; });
    tokenizer.addOnParseText([&count_t_other_way](const std::string& token) { if (token.rfind("т", 0) == 0) count_t_other_way++; });

    tokenizer.parse(TEST_TEXT);

    assert(count_t == 3);
    assert(count_t_other_way == 3);
    return nullptr;
}

static const char* test_start_end_callbacks__(const std::string& text) {
    Tokenizer tokenizer;

    bool check_started = false;
    bool check_finished = false;

    tokenizer.addOnStart([&check_started]() { check_started = true; });
    tokenizer.addOnFinish([&check_finished]() { check_finished = true; });

    tokenizer.parse(text);

    assert(check_started);
    assert(check_finished);
    return nullptr;
}

static const char* test_start_end_callbacks() {
    return test_start_end_callbacks__(TEST_TEXT);
}

static const char* test_empty() {
    return test_start_end_callbacks__("");
}

static const char* all_tests() {
//    mu_run_test(just_test);
    mu_run_test(test_numbers);
    mu_run_test(test_words);
    mu_run_test(test_count_tokens);
    mu_run_test(test_count_tokens_started_with_t);
    mu_run_test(test_start_end_callbacks);
    mu_run_test(test_empty);
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
