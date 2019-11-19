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
    // Добавление строки во множество
    bool add(const std::string& str);
    // Удаление строки из множества
    bool remove(const std::string& str);
    // Проверям вхождение элемента во множество
    bool contains(const std::string& str) const;

private:
    size_t limit = 8;
    size_t size = 0;
    std::vector<std::string> buffer = {"", "", "", "", "", "", "", ""};
    // Расширение буфера
    void expand();
    // Поиск индекса, по которому уже распологается или должна будет расположиться строка
    int find(const std::string& str) const;
};

bool StringSet::add(const std::string& str) {
    int index = find(str);
    if (buffer[index].empty()) {
        buffer[index] = str;
        size++;
        if (size > 3 * (limit / 4)) {
            expand();
        }
        return true;
    }
    return false;
}

bool StringSet::remove(const std::string& str) {
    int index = find(str);
    if (!buffer[index].empty()) {
        buffer[index] = "";
        size--;
        return true;
    }
    return false;
}

bool StringSet::contains(const std::string& str) const {
    return !buffer[find(str)].empty();
}

void StringSet::expand() {
    std::vector<std::string> old_buffer = buffer;
    limit *= 2;
    buffer = std::vector<std::string>(limit, "");
    for (int i = 0; i < limit / 2; i++) {
        if (!old_buffer[i].empty()) {
            add(old_buffer[i]);
        }
    }
    std::cout << "";
    return;
}

int StringSet::find(const std::string& str) const {
    int index = hash(str, limit);
    while (!buffer[index].empty() && buffer[index] != str) {
        index++;
    }
    return index;
}


int main() {
    StringSet table;
    char command = ' ';
    for (int i = 0; i < 100000; i++) {
        table.add(std::to_string(i));
    }
    for (int i = 0; i < 100000; i++) {
        table.contains(std::to_string(i));
        table.remove(std::to_string(i));
        table.contains(std::to_string(i));
    }
//    std::string value;
//    while (std::cin >> command >> value) {
//        switch (command) {
//            case '?':
//                std::cout << (table.contains(value) ? "OK" : "FAIL") << std::endl;
//                break;
//            case '+':
//                std::cout << (table.add(value) ? "OK" : "FAIL") << std::endl;
//                break;
//            case '-':
//                std::cout << (table.remove(value) ? "OK" : "FAIL") << std::endl;
//                break;
//        }
//    }
    return 0;
}
