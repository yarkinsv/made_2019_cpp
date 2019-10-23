#include <cstddef>
#include <functional>
#include <vector>

using TokenPredicate = std::function<bool (const std::string& token)>;
using OnTokenParsed = std::function<void (const std::string& token)>;
using OnNumberParsed = std::function<void (const long token)>;
using OnStart = std::function<void ()>;
using OnFinish = std::function<void ()>;

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
    void addOnParseNumber(const OnTokenParsed& callback);
    void addOnParseNumber(const OnNumberParsed& callback);
    // Зарегистрировать callback, если спарсилось не число, или слишком длинное число чтобы поместиться в long
    void addOnParseText(const OnTokenParsed& callback);
    // Возможность загегистрировать более универсальные callback'и
    void addOnParseToken(const TokenPredicate& predicate, const OnTokenParsed& callback);
    // Зарегистрировать callback, который вызовется перед началом парсинга
    void addOnStart(const OnStart& callback);
    // Зарегистрировать callback, который вызовется по завершение парсинга
    void addOnFinish(const OnFinish& callback);

private:
    std::vector<std::pair<TokenPredicate, OnTokenParsed>> onParseTokenCallbacks;
    std::vector<OnStart> onStartCallbacks;
    std::vector<OnFinish> onFinishCallbacks;
};
