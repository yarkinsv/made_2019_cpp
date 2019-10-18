// 1_3. Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
// Яркин С.В.

#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

class Stack {
public:
    // Проверка стека на пустоту
    bool empty() const;
    // Добавление элемента
    void push(int value);
    // Извлечение элемента
    int pop();
    // Добавляем элемент, если есть свободный слот в буфере
    bool try_push(int value);
    // Нужно освободить буфер при удалении стека
    ~Stack();

private:
    // Пусть начальный размер буфера будет непустым, чтобы не обрабатывать граничный кейс
    int* buffer = (int*) malloc(sizeof(int));
    // Размер выделленого буфера
    int size = 1;
    // Индекс вершины стека, -1 означает что стек пустой
    int head = -1;
    // Заполнен ли буфер
    bool full();
    // Увеличить буфер в два раза
    void grow_buffer();
};

class Queue {
public:
    // Проверка очереди на пустоту
    bool empty() const;
    // Добавление элемента
    void push(int value);
    // Извлечение
    int pop();

private:
    // Стек в который будем направлять входной поток значений
    Stack input_stack = Stack{};
    // Стек из которого будем читать очередь
    Stack output_stack = Stack{};
};


bool Stack::empty() const {
    return head == -1;
}

bool Stack::full() {
    return head == size - 1;
}

int Stack::pop() {
    assert(!empty());
    return buffer[head--];
}

void Stack::push(int value) {
    // Если места нет, удваиваем буфер
    if (full()) {
        grow_buffer();
    }
    buffer[++head] = value;
}

bool Stack::try_push(int value) {
    if (full()) {
        return false;
    }
    push(value);
    return true;
}

void Stack::grow_buffer() {
    // Просто удваиваем размер буфера, копируем в новый все данные текущего и удаляем его
    size *= 2;
    int* buffer_tmp = (int*) malloc(size * sizeof(int));
    memcpy(buffer_tmp, buffer, (head + 1) * sizeof(int));
    free(buffer);
    buffer = buffer_tmp;
}

Stack::~Stack() {
    free(buffer);
}

bool Queue::empty() const {
    return input_stack.empty() && output_stack.empty();
}

void Queue::push(int value) {
    // Привожу вариант с оптимизацией, было интересно поиграться со стратегиями расширения буфера.
    // try_push() вообще говоря нестандартный метод для стека, поэтому если к этому есть претензии
    // можно в этом методе оставить просто input_stack.push(value); и все будет работать точно также
    // С этой оптимизацией я пытаюсь вместо очередного расширения стека входящих чисел, переложить их в выходной стек
    // оказалось, что на примерах из контеста это работает: без оптимизации имеем время работы 0.543s и память 9.74Mb
    // с данной оптимизацией получилось 0.532s и 4.11Mb, т.е. мы действительно сэкономили памяти, причем в два раза
    if (!input_stack.try_push(value)) {
        if (output_stack.empty()) {
            output_stack.push(value);
            while (!input_stack.empty()) {
                output_stack.push(input_stack.pop());
            }
        } else {
            input_stack.push(value);
        }
    }
}

int Queue::pop() {
    assert(!empty());
    // Чтобы сформировать очередь из стека, перекидываем элементы из влохдного потока в выходной
    if (output_stack.empty()) {
        while (!input_stack.empty()) {
            output_stack.push(input_stack.pop());
        }
    }
    return output_stack.pop();
}

int main(int argc, const char* argv[]) {
    int commands_count = 0;
    std::cin >> commands_count;
    Queue queue;
    for (int i = 0; i < commands_count; i++) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        if (command == 3) {
            queue.push(value);
        } else if (command == 2) {
            if (queue.empty()) {
                if (value != -1) {
                    std::cout << "NO";
                    return 0;
                }
            } else if (queue.pop() != value) {
                std::cout << "NO";
                return 0;
            }
        }
    }
    std::cout << "YES";
}

