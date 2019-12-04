//Задача 8. Хеш-таблица (10 баллов)
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.

#include <iostream>
#include <vector>
#include <cassert>

// метод Горнера расчета хэша строки
int hash(const std::string& str, int m) {
    int hash = 0;
    for (int i = 0; i < str.length(); i++)
        hash = (hash * 31 + str[i]) % m;
    return hash;
}

class StringSet {
public:
    StringSet() : StringSet(8) {}
    StringSet(size_t n) :
            buffer(std::vector<std::string>(n, "")),
            deleted(std::vector<bool>(n, false)),
            limit(n),
            size(0) {}

    // Добавление строки во множество
    bool add(std::string& str);
    // Удаление строки из множества
    bool remove(const std::string& str);
    // Проверям вхождение элемента во множество
    bool contains(const std::string& str) const;

private:
    size_t limit;
    size_t size;
    std::vector<std::string> buffer;
    std::vector<bool> deleted;
    bool expanding = false;
    // Расширение буфера
    void expand();
    // Поиск индекса, по которому уже распологается или должна будет расположиться строка
    size_t find(const std::string& str) const;
};

bool StringSet::add(std::string& str) {
    size_t index = find(str);
    if (buffer[index].empty()) {
        buffer[index] = std::move(str);
        size++;
        if (size > 3 * (limit / 4)) {
            expand();
        }
        return true;
    } else if (deleted[index]) {
        deleted[index] = false;
        return true;
    }
    return false;
}

bool StringSet::remove(const std::string& str) {
    size_t index = find(str);
    if (!buffer[index].empty() && !deleted[index]) {
        deleted[index] = true;
        return true;
    }
    return false;
}

bool StringSet::contains(const std::string& str) const {
    size_t index = find(str);
    return !buffer[index].empty() && !deleted[index];
}

void StringSet::expand() {
    StringSet stringSet(limit * 2);
    for (size_t i = 0; i < limit; i++) {
        if (!deleted[i] && !buffer[i].empty()) {
            stringSet.add(buffer[i]);
        }
    }
    buffer = std::move(stringSet.buffer);
    deleted = std::move(stringSet.deleted);
    size = stringSet.size;
    limit = stringSet.limit;
}

size_t StringSet::find(const std::string& str) const {
    size_t index = hash(str, limit);
    size_t i = 0;

    while ((!buffer[index].empty() || deleted[index]) && buffer[index] != str) {
        if (++i > limit) {
            throw std::runtime_error("inconsistent state of stringset");
        }
        index = (index + i + 1) % limit;
    }
    return index;
}


int main() {
    StringSet table;
    char command = ' ';
    std::string value;
    while (std::cin >> command >> value) {
        switch (command) {
            case '?':
                std::cout << (table.contains(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.remove(value) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}
