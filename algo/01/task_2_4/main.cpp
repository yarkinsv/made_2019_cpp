// 2_4. Скользящий максимум.
// Дан массив целых чисел A[0..n), n не превосходит 10^8. Также задан размер некоторого окна (последовательно
// расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k)
// вывести значение максимума в окне. Скорость работы O(n log n), память O(n).
// Яркин С.В.

#include <iostream>
#include <cassert>

using namespace std;

// Для поддержания порядка следования нам помимо самой кучи нужен связанный список, поэтому тут есть ссылка на
// следующий элемент, а index - для того, чтобы этот элемент можно было найти в куче
struct Node {
    int value;
    int index;
    Node* next;
};

// По-хорошему тут должен быть дженерик класс с элементами, поддерживаеющими интерфейс компаратора,
// но я пока без понятия как все это делается в C++, позже изучу и попробую в других заданиях
// Я сразу сделал интерфейс структуры данных, адаптированный под задачу, т.е. фиксированный размер, и один метод rotate
// Но внутри это все таже куча с просеиваниями вниз-вверх, поэтому думаю это должно быть нормально
class CycleHeap {
public:
    CycleHeap(int size, const int* arr);
    // Добавление элемента в кучу, одновременно удаляя последний
    void rotate(int value);
    // Проверка максимума в куче
    int max();
    // Вывод для отладки
    void print();

    ~CycleHeap();

private:
    // Куча состит из массива ссылок на элементы Node
    Node** heap;
    // Это первый элемент в окне (последний добавленный)
    Node* first;
    // Последний элемент в окне, который удалится при следующем rotate
    Node* last;
    // Количество элементов в куче
    int size;

    // Просеивание элемента вниз
    void sift_down(int idx);
    // Просеивание элемента вверх
    void sift_up(int idx);
    // Поменять два элемента местами
    void swap(Node* node1, Node* node2);
};

CycleHeap::CycleHeap(const int size, const int* arr) {
    assert(size > 0);
    assert(arr != nullptr);

    this->size = size;
    heap = new Node* [size];
    // Заполняем сначала массив как есть, выстраивая все нужные связи между нодами
    for (int i = 0; i < size; i++) {
        heap[i] = new Node{arr[i], i, nullptr};
        if (i == 0) {
            last = heap[0];
        } else {
            first->next = heap[i];
        }
        first = heap[i];
    }
    // Восстанавливаем свойства кучи за O(n)
    for (int i = size / 2 - 1; i >= 0; i--) {
        sift_down(i);
    }
}

void CycleHeap::rotate(int value) {
    // Когда элемент всего один, просто присваиваем ему новое значение
    if (size == 1) {
        first->value = value;
        return;
    }
    // Ноду last нам нужно удалить из кучи, для этого меняем ее с последним элементом
    // После этого нужно восстановить свойства кучи с помощью просеивания, в какую сторону я не знаю, поэтому делаем сразу в обе
    if (last->index < size - 1) {
        Node* side_node = heap[size - 1];
        swap(last, side_node);
        size--;
        sift_up(side_node->index);
        sift_down(side_node->index);
        size++;
    }
    // На последнем месте в куче стоит неиспользуемый элемент, нам нужно просто сдвинуть указатели last и first
    // Новые ноды причем не создаются, мы переиспользуем те, что уже есть, поэтому ничего чистить не нужно
    last = last->next;
    first->next = heap[size - 1];
    first = heap[size - 1];
    first->next = nullptr;
    first->value = value;
    // Осталось только поставить новый элемент на корректную позицию в куче
    sift_up(size - 1);
}

int CycleHeap::max() {
    return heap[0]->value;
}

void CycleHeap::swap(Node* node1, Node* node2) {
    // Проверка на консистентность кучи
    assert(heap[node1->index] == node1);
    assert(heap[node2->index] == node2);

    int tmp = node1->index;
    node1->index = node2->index;
    node2->index = tmp;
    heap[node1->index] = node1;
    heap[node2->index] = node2;
}

void CycleHeap::sift_down(int idx) {
    assert(0 <= idx && idx < size);

    int largest = idx;
    do {
        // Находим самый большой из текущей ноды и ее предков, и меняемся если самым большим окажется один из предков
        idx = largest;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < size && heap[left]->value > heap[largest]->value) {
            largest = left;
        }
        if (right < size && heap[right]->value > heap[largest]->value) {
            largest = right;
        }
        if (largest != idx) {
            swap(heap[idx], heap[largest]);
        }
    } while (largest != idx);
}

void CycleHeap::sift_up(int idx) {
    assert(0 <= idx && idx < size);

    // Меняемся с родителем пока больше чем он
    while (idx != 0 && heap[(idx - 1) / 2]->value < heap[idx]->value) {
        swap(heap[(idx - 1) / 2], heap[idx]);
        idx = (idx - 1) / 2;
    }
}

void CycleHeap::print() {
    printf("\n");
    for (int i=0; i < size; i++) {
        printf("%d->%d ", heap[i]->value, (heap[i]->next == nullptr) ? -1 : (heap[i]->next->value));
    }
    printf(" first %d last %d last->next %d", first->value, last->value, last->next->value);
}

// Почистим тут все объекты, которые насоздавали в конструкторе
CycleHeap::~CycleHeap() {
    for (int i = 0; i < size; i++) {
        delete heap[i];
    }
    delete heap;
}


int main(int argc, const char* argv[]) {
    int n = 0;
    std::cin >> n;
    int buffer[n];
    for (int i = 0; i < n; i++) {
        std::cin >> buffer[i];
    }
    int window_length = 0;
    std::cin >> window_length;
    CycleHeap heap = CycleHeap(window_length, buffer);
    std::cout << heap.max();
    for (int i = window_length; i < n; i++) {
        heap.rotate(buffer[i]);
        std::cout << " " << heap.max();
    }
}

