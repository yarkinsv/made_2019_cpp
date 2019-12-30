#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <iterator>
#include <utility>
#include <tuple>
#include <memory>


struct Node {
    // Значение в ноде
    char value;
    // Ссылки на следущие ноды
    std::vector<std::shared_ptr<Node>> next;
    // Храним номера паттернов которые оканчиваются в ноде
    std::set<int> finals;
    // Префиксная ссылка
    std::shared_ptr<Node> suffix_ref;

    // Проверка на то, можно ли перейти по символу в следующую ноду, nullptr если нет
    std::shared_ptr<Node> check_next_node(char v);
    // Если перейти в следующую ноду нельзя по символу v, то конструируем ее и добавляем в next
    std::shared_ptr<Node> get_next_node(char v);
    void set_suffix_ref(const std::shared_ptr<Node>& suffixNode);
    bool check_final(int final);
};


std::shared_ptr<Node> Node::check_next_node(char v) {
    for (auto node : next) {
        if (node->value == v) {
            return node;
        }
    }
    return nullptr;
}


std::shared_ptr<Node> Node::get_next_node(char v) {
    std::shared_ptr<Node> nextNode = check_next_node(v);
    if (nextNode == nullptr) {
        nextNode = std::make_shared<Node>(Node{ v });
        next.push_back(nextNode);
    }
    return nextNode;
}


void Node::set_suffix_ref(const std::shared_ptr<Node>& suffixNode) {
    suffix_ref = suffixNode;
    for (int final : suffixNode->finals) {
        finals.insert(final);
    }
}

bool Node::check_final(int final) {
    return !finals.empty() && finals.count(final) > 0;
}


std::shared_ptr<Node> build_trie(const std::vector<std::string>& patterns) {
    std::shared_ptr<Node> root = std::make_shared<Node>(Node { -1 });
    for (size_t i = 0; i < patterns.size(); i++) {
        std::shared_ptr<Node> current = root;
        for (char c : patterns[i]) {
            current = current->get_next_node(c);
        }
        current->finals.insert(i);
    }
    return root;
}


// Возвращаем паттерны и их длины с учетом ?
std::pair<std::vector<std::string>, std::vector<int>> parse_patterns(const std::string& s) {
    std::vector<std::string> result;
    std::vector<int> counts;
    int count = 0;
    int word_count = 0;
    size_t i = 0;
    while (i < s.length()) {
        count = 0;
        while (s[i] == '?' && i < s.length()) {
            count++;
            i++;
        }
        word_count = 0;
        while (s[i] != '?' && i < s.length()) {
            word_count++;
            i++;
        }
        if (word_count > 0) {
            std::string substr = s.substr(i - word_count, word_count);
            result.push_back(substr);
        }
        counts.push_back(count + word_count);
    }
    if (word_count > 0) {
        counts.push_back(0);
    }
    return std::make_pair(result, counts);
}


void set_suffix_ref(const std::shared_ptr<Node>& root, const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& node) {
    std::shared_ptr<Node> previous = parent->suffix_ref;
    while (previous != nullptr) {
        std::shared_ptr<Node> candidate = previous->check_next_node(node->value);
        if (candidate != nullptr) {
            node->set_suffix_ref(candidate);
            return;
        } else {
            previous = previous->suffix_ref;
        }
    }
    std::shared_ptr<Node> candidate = root->check_next_node(node->value);
    if (candidate != nullptr && candidate != node) {
        node->set_suffix_ref(candidate);
    }
}


void set_suffix_refs(const std::shared_ptr<Node>& root) {
    std::queue<std::shared_ptr<Node>> queue;
    queue.push(root);
    while (!queue.empty()) {
        std::shared_ptr<Node> current = queue.front();
        queue.pop();
        for (const std::shared_ptr<Node>& nextNode : current->next) {
            queue.push(nextNode);
            set_suffix_ref(root, current, nextNode);
        }
    }
}


void find_matches(const std::string& input, const std::string& pattern) {
    std::vector<std::string> patterns;
    std::vector<int> counts;
    std::tie(patterns, counts) = parse_patterns(pattern);
    std::shared_ptr<Node> root = build_trie(patterns);
    set_suffix_refs(root);

    if (input.empty() || counts.empty() || pattern.empty() || input.length() < pattern.length()) {
        return;
    }
    if (counts.size() == 1) {
        for (int i = 0; i < input.length() - pattern.length() + 1; i++) {
            std::cout << i << " ";
        }
        return;
    }

    int shift = 0;
    int global_shift = 0;
    // Немного усложненный алгоритм получился в сравнении с тем что обсуждали на лекции
    // Я сохраняю массив из встреченных нод и анализирую его на нахождения паттерна с учетом смещений
    // Но так как целый массив не влазиет, то я использую циклический буфер
    // Когда буфер переполняется, я анализирую его на встеченные паттерны, а потом начинаю перезаписывать его половинку
    std::vector<std::shared_ptr<Node>> found_patterns(2 * pattern.length());
    std::shared_ptr<Node> current = root;
    for (int i = 0; i < input.length() - counts.back(); i++) {

        if (i > pattern.length() && i % pattern.length() == 0) {
            for (int q = 0; q < pattern.length(); q++) {
                int found = true;
                int ind = q - 1 + shift;
                for (int j = 0; j < counts.size() - 1; j++) {
                    ind += counts[j];
                    if (ind >= 2 * pattern.length()) {
                        ind -= 2 * pattern.length();
                    }
                    if (found_patterns[ind] == nullptr || !found_patterns[ind]->check_final(j)) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    std::cout << i - (2 * pattern.length()) + q << " ";
                }
            }
            if (shift == 0) {
                shift = pattern.length();
            } else {
                shift = 0;
            }
            global_shift += pattern.length();
        }

        std::shared_ptr<Node> next = current->check_next_node(input[i]);
        if (next == nullptr) {
            while (current->suffix_ref != nullptr) {
                next = current->suffix_ref->check_next_node(input[i]);
                if (next != nullptr) {
                    break;
                }
                current = current->suffix_ref;
            }
        }
        if (next == nullptr) {
            next = root->check_next_node(input[i]);
        }
        if (next != nullptr) {
            found_patterns[i % (2 * pattern.length())] = next;
            current = next;
        } else {
            current = root;
        }
    }

    for (int q = 0; q < (int) input.length() - global_shift - (int) pattern.length() + 1; q++) {
        int found = true;
        int ind = q - 1 + shift;
        for (int j = 0; j < counts.size() - 1; j++) {
            ind += counts[j];
            if (ind >= 2 * pattern.length()) {
                ind -= 2 * pattern.length();
            }
            if (found_patterns[ind] == nullptr || !found_patterns[ind]->check_final(j)) {
                found = false;
                break;
            }
        }
        if (found) {
            std::cout << global_shift + q << " ";
        }
    }
}


int main() {
    std::string pattern;
    std::string text;
    std::cin >> pattern >> text;

    find_matches(text, pattern);

    return 0;
}
