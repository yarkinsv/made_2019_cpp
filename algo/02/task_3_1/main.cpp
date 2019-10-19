//3_1. Первые k элементов длинной последовательности
// Дана очень длинная последовательность целых чисел длины n. Требуется вывести в отсортированном виде её
// наименьшие k элементов. Последовательность может не помещаться в память. Время работы O(n * log(k)).
// Доп. память O(k). Использовать слияние.

#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

int* mergeSortTopK(const int* array, const int begin, const int end, const int top_k) {
    assert(end >= begin);
    assert(top_k >= 1);

    // Элементарный случай, когда всего один элемент, то его и возвращаем, так как top_k не может быть 0
    if (begin == end) {
        return new int[1] {array[begin]};
    }

    // Рассчитываем середину отрезка, по которой будем делить массив на две подзадачи
    int middle_index = (end + begin) / 2;

    // Запускаем рекурсию, находя решение левой и правой части
    int* left_sorted_top_k = mergeSortTopK(array, begin, middle_index, top_k);
    int* right_sorted_top_k = mergeSortTopK(array, middle_index + 1, end, top_k);

    // Подготавливаем буфер размера не более чем top_k
    int left_size = middle_index - begin + 1;
    int right_size = end - middle_index;
    int merged_size = min(top_k, left_size + right_size);
    int* merged = new int[merged_size];

    // В подготовленный буфер делаем слияние двух отсортированных массивов, размер которых также не может быть больше top_k
    // Сложность O(n*log(k)) достигается как раз за счет того, что мы останавливаем слияние как только набрали k элементов
    int left_pointer = 0;
    int right_pointer = 0;
    for (int i = 0; i < merged_size; i++) {
        if (left_pointer == left_size) {
            merged[i] = right_sorted_top_k[right_pointer++];
        } else if (right_pointer == right_size) {
            merged[i] = left_sorted_top_k[left_pointer++];
        } else if (left_sorted_top_k[left_pointer] <= right_sorted_top_k[right_pointer]) {
            merged[i] = left_sorted_top_k[left_pointer++];
        } else {
            merged[i] = right_sorted_top_k[right_pointer++];
        }
    }
    free(left_sorted_top_k);
    free(right_sorted_top_k);
    return merged;
}

int main() {
    int array_size = 0;
    int top_k = 0;
    cin >> array_size >> top_k;

    int* array = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        cin >> array[i];
    }

    int* sorted_top_k_elements = mergeSortTopK(array, 0, array_size - 1, top_k);

    for (int i = 0; i < top_k; i++) {
        cout << sorted_top_k_elements[i] << " ";
    }

    free(array);
    free(sorted_top_k_elements);
    return 0;
}
