//4. Порядковая статистика
// 4_3. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
// Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

void swap(int* array, int idx1, int idx2) {
    if (idx1 == idx2) {
        return;
    }
    int tmp = array[idx1];
    array[idx1] = array[idx2];
    array[idx2] = tmp;
}

// Процедура partition участка массива от begin до end, в качестве опорного элемента берем элемент с индексом p_index
// Возвращает индекс опорного элемента по окончании процедуры разбиения
int partition(int* array, int begin, int end, int p_index) {
    assert(end >= begin);
    assert(p_index >= begin && p_index <= end);

    // Помещаем опорный элемент в конец массива
    swap(array, p_index, end);

    // Сразу пропустим все элементы, которые уже корректно расположились в левой части
    int left_pointer = begin;
    while (left_pointer < end && array[left_pointer] <= array[end]) {
        left_pointer++;
    }
    // Устанавливаем правый итератор на место
    int right_pointer = left_pointer + 1;
    // Бежим до конца массива двумя итераторами при необходимости меняя элементы местами
    while (right_pointer < end) {
        if (array[right_pointer] <= array[end]) {
            swap(array, left_pointer++, right_pointer++);
        } else {
            right_pointer++;
        }
    }
    // Ставим опорный элемент на место первого элемента, который больше его, там как раз окажется левый итератор
    swap(array, left_pointer, end);
    return left_pointer;
}

// Стратегия выбора опорного элемента “случайный элемент”
int pickPartitionIndex(int begin, int end) {
    assert(end >= begin);
    return end == begin ? end : (rand() % (end - begin)) + begin;
}

int quickSortK(int* array, int begin, int end, int k) {
    assert(end >= begin);
    assert(k >= 0 && k <= end - begin);

    int current_partition_index = -1;
    while (current_partition_index != k) {
        current_partition_index = partition(array, begin, end, pickPartitionIndex(begin, end));
        if (current_partition_index < k) {
            begin = current_partition_index + 1;
        } else if (current_partition_index > k) {
            end = current_partition_index - 1;
        }
    }
    return array[k];
}

int main() {
    int array_size = 0;
    int k = 0;
    cin >> array_size >> k;

    int* array = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        cin >> array[i];
    }

    cout << quickSortK(array, 0, array_size - 1, k);
    return 0;
}
