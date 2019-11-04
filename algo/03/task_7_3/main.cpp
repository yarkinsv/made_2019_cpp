// 7_3. Порядковые статистики ДД
//Множество натуральных чисел постоянно меняется. Элементы в нем добавляются и удаляются по одному.
// Вычислите указанные порядковые статистики после каждого добавления или удаления.
//Требуемая скорость выполнения запроса - O(log n) в среднем.
//В реализации используйте декартовы деревья.

#include <iostream>
#include <vector>
#include <functional>


struct Node {
    int key; // Значение
    int priority; // Приоритет
    Node* right;
    Node* left;
    int size; // Размер дерева, 1 - если дерево из одной вершины
};


class Treap {
public:
    // Обход графа в клубину, на каждый посещенный узел вызов функции callback
    void pre_order(const std::function<void (Node&)>& callback) {
        // Для обхода в глубину используется очередь
        std::vector<Node*> queue;
        queue.push_back(root);
        while (!queue.empty()) {
            Node* node = queue.back();
            queue.pop_back();
            callback(*node);
            if (node->right != nullptr) {
                queue.push_back(node->right);
            }
            if (node->left != nullptr) {
                queue.push_back(node->left);
            }
        }
    }

    // Разрезание декартового дерева по ключу.
    // В процессе отделения дерева, нужно обновить у того дерева, из которого идет удаление, размер
    static void split(Node* node, int key, Node*& left, Node*& right) {
        if (node == nullptr) {
            left = right = nullptr;
        } else if (node->key <= key) {
            split(node->right, key, node->right, right);
            left = node;
            if (right != nullptr) {
                left->size -= right->size;
            }
        } else {
            split(node->left, key, left, node->left);
            right = node;
            if (left != nullptr) {
                right->size -= left->size;
            }
        }
    }

    // Слияние двух декартовых деревьев.
    // При слиянии мы увеличиваем размер в ветке дерева, к которой идет добавление поддерева
    static Node* merge(Node* left, Node* right) {
        if (!left || !right)
            return left == nullptr ? right : left;
        if (left->priority > right->priority) {
            left->size += right->size;
            left->right = merge(left->right, right);
            return left;
        } else {
            right->size += left->size;
            right->left = merge(left, right->left);
            return right;
        }
    }

    // Вставка по ключу и приоритету
    void insert(int key, int priority) {
        Node* newNode = new Node {key, priority, nullptr, nullptr, 1};
        if (root == nullptr) {
            root = newNode;
            return;
        }
        Node* t1 = nullptr;
        Node* t2 = nullptr;
        split(root, key, t1, t2);
        t1 = merge(t1, newNode);
        root = merge(t1, t2);
    }

    // Удаление по ключу
    void remove(int key) {
        Node* node = root;
        Node* parent = nullptr;
        bool found = false;

        // Бинарный поиск ноды с искомым ключом
        while (node != nullptr && !found) {
            node->size--;
            if (key < node->key) {
                parent = node;
                node = node->left;
            } else if (key > node->key) {
                parent = node;
                node = node->right;
            } else {
                found = true;
            }
        }

        if (!found) {
            throw std::invalid_argument("Key value not found in tree");
        }

        // Сливаем поддеревья удаляемой ноды и восстанавливаем связи
        Node* merged = merge(node->left, node->right);
        if (parent == nullptr) {
            root = merged;
        } else if (parent->left != nullptr && parent->left->key == key) {
            parent->left = merged;
        } else {
            parent->right = merged;
        }

        // Удаленная нода больше не нужна, можно очистить ее из памяти
        delete node;
    }

    // Нахождение k-ой статистики в дереве
    Node* statistic(int k) {
        Node* node = root;
        // Статистику ноды можно определить исходя из размера поддерева текущей ноды
        // Для root'а ранк определить легко по размеру левого дерева, т.к. все элементы в нем будут меньше чем он
        int curK = node->left ? node->left->size : 0;
        while (curK != k) {
            if (curK > k) {
                node = node->left;
                // Если идем в левое поддерево, то ранк ноды нужно уменьшить на размер его правого дерева
                curK = curK - 1 - (node->right ? node->right->size : 0);
            } else {
                node = node->right;
                // Если идем в правое поддерево, то ранк ноды нужно увеличить на размер его левого дерева
                curK = curK + 1 + (node->left ? node->left->size : 0);
            }
        }
        return node;
    }

    void print() {
        pre_order([] (Node& node) { std::cout << "key: " << node.key << " size: " << node.size <<  " priority: " << node.priority
        << " left: " << (node.left ? node.left->key : -1) << " right: " << (node.right ? node.right->key : -1) << "\n"; });
    }

    ~Treap() {
        pre_order([] (Node& node) { delete &node; });
    }

private:
    Node* root = nullptr;
};


int main() {
    Treap tree;

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int value = 0;
        int k = 0;
        std::cin >> value >> k;
        if (value > 0) {
            tree.insert(value, std::rand());
        } else {
            tree.remove(-value);
        }
        std::cout << tree.statistic(k)->key << " ";
    }

    return 0;
}
