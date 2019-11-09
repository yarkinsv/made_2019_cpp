// 6. Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
//Рекурсия запрещена
// 6_1. Выведите элементы в порядке pre-order (сверху вниз).

#include <iostream>
#include <vector>
#include <functional>


struct Node {
    int value;
    Node* left_child;
    Node* right_child;
};

void destroy_tree(Node* node) {
    if (node != nullptr) {
        destroy_tree(node->left_child);
        destroy_tree(node->right_child);
        delete node;
    }
}

class BinaryTree {
public:
    // Добавление элемента в дерево
    void add(int value);

    // Обход графа в клубину, на каждый посещенный узел вызов функции callback
    void pre_order(const std::function<void (Node&)>& callback);

    ~BinaryTree() {
        destroy_tree(this->root);
    }

private:
    Node* root = nullptr;
};

void BinaryTree::add(int value) {
    Node* node = root;
    Node* parent = nullptr;
    bool left = true;
    // Сначала находим лист дерева, к которому нужно присоединить новые элемент
    while (node != nullptr) {
        parent = node;
        if (value <= node->value) {
            node = node->left_child;
            left = true;
        } else {
            node = node->right_child;
            left = false;
        }
    }
    // Создаем новый элемент и выстраиваем нужные связи, ссылка на parent для задачи не нужна
    auto* newNode = new Node{value, nullptr, nullptr};
    if (parent != nullptr) {
        if (left) {
            parent->left_child = newNode;
        } else {
            parent->right_child = newNode;
        }
    } else {
        root = newNode;
    }
}

void BinaryTree::pre_order(const std::function<void (Node&)>& callback) {
    // Для обхода в глубину используется стек
    std::vector<Node*> stack;
    stack.push_back(root);
    while (!stack.empty()) {
        Node* node = stack.back();
        stack.pop_back();
        callback(*node);
        if (node->right_child != nullptr) {
            stack.push_back(node->right_child);
        }
        if (node->left_child != nullptr) {
            stack.push_back(node->left_child);
        }
    }
}

int main() {
    BinaryTree tree;

    int n = 0, a = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::cin >> a;
        tree.add(a);
    }

    tree.pre_order([] (Node& node) { std::cout << node.value << " "; });
    return 0;
}
