#include <string>
#include <cstddef>
#include <functional>
#include <vector>

using TokenPredicate = std::function<bool (const std::string& token)>;
using OnTokenParsed = std::function<void (const std::string& token)>;
using OnNumberParsed = std::function<void (const long token)>;
using OnStart = std::function<void ()>;
using OnFinish = std::function<void ()>;

bool isNumber(const std::string& token);

// Токенайзер, которому можно зарегистрировать набор callback'ов на разные события.
// На каждое событие можно зарегистрировать ниодного либо сразу множество callback'ов,
// в последнем случае все они вызовутся в очередности их регистрации.
// Токенайзер можно переиспользовать для парсинга другой строки, но удалить уже добавленные callback'и нельзя.
class Tokenizer
{
public:
    // Спарсить строку на токены
    void parse(const std::string& str);
    // Зарегистрировать callback, когда спарсилось число, пусть будет возможность получить в виде текста, либо в виде long'а
    inline void addOnParseNumber(const OnTokenParsed& callback) {
        addOnParseToken(isNumber, callback);
    }
    inline void addOnParseNumber(const std::function<void (const long token)>& callback) {
        addOnParseToken(isNumber, [&callback](const std::string& token) { callback(stol(token)); });
    }
    // Зарегистрировать callback, если спарсилось не число, или слишком длинное число чтобы поместиться в long
    inline void addOnParseText(const OnTokenParsed& callback) {
        addOnParseToken(std::not_fn(isNumber), callback);
    }
    // Возможность загегистрировать более универсальные callback'и
    inline void addOnParseToken(const TokenPredicate& predicate, const OnTokenParsed& callback) {
        onParseTokenCallbacks.emplace_back(predicate, callback);
    }
    // Зарегистрировать callback, который вызовется перед началом парсинга
    inline void addOnStart(const OnStart& callback) {
        onStartCallbacks.push_back(callback);
    }
    // Зарегистрировать callback, который вызовется по завершение парсинга
    inline void addOnFinish(const OnFinish& callback) {
        onFinishCallbacks.push_back(callback);
    }

private:
    std::vector<std::pair<TokenPredicate, OnTokenParsed>> onParseTokenCallbacks;
    std::vector<OnStart> onStartCallbacks;
    std::vector<OnFinish> onFinishCallbacks;
};
