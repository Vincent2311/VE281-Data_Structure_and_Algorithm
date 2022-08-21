#ifndef VE281P1_SORT_HPP
#define VE281P1_SORT_HPP

#include <vector>
#include <stdlib.h>
#include <functional>

template<typename T>
void swap(T *xp, T *yp) {
    T temp = *xp;
    *xp = *yp;
    *yp = temp;
}

template<typename T, typename Compare>
void bubble_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty())
        return;
    for (typename std::vector<T>::size_type i = vector.size() - 1; i > 0; i--) {
        for (typename std::vector<T>::size_type j = 0; j < i; j++) {
            if (comp(vector[j + 1], vector[j])) {
                swap(&vector[j], &vector[j + 1]);
            }
        }
    }
}

template<typename T, typename Compare>
void insertion_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty())
        return;
    T temp;
    int j = 0;
    for (std::vector<int>::size_type i = 1; i <= vector.size() - 1; i++) {
        temp = vector[i];
        j = static_cast<int>(i - 1);
        while (j >= 0 && comp(temp, vector[j])) {

            vector[j + 1] = vector[j];
            j--;
        }
        vector[j + 1] = temp;
    }
}

template<typename T, typename Compare>
void selection_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty())
        return;
    std::vector<int>::size_type index;
    int extremum;
    for (std::vector<int>::size_type i = 0; i < vector.size() - 1; i++) {
        index = i;
        extremum = vector[i];
        for (std::vector<int>::size_type j = i; j < vector.size(); j++) {
            if (comp(vector[j], extremum)) index = j;
        }
        if (index == i) continue;
        swap(&vector[i], &vector[index]);
    }
}

template<typename T, typename Compare>
void merge(std::vector<T> &vector, int left, int mid, int right, Compare comp = std::less<T>()) {
    const int arrayLength1 = mid - left + 1;
    const int arrayLength2 = right - mid;
    auto *leftArr = new T[arrayLength1];
    auto *rightArr = new T[arrayLength2];
    for (int i = 0; i < arrayLength1; i++) {
        leftArr[i] = vector[left + i];
    }
    for (int j = 0; j < arrayLength2; j++) {
        rightArr[j] = vector[mid + 1 + j];
    }
    int i = 0, j = 0;
    int k = left;
    while (i < arrayLength1 && j < arrayLength2) {
        if (!comp(rightArr[j], leftArr[i])) {
            vector[k] = leftArr[i];
            i++;
        } else {
            vector[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (j < arrayLength2) {
        vector[k] = rightArr[j];
        j++;
        k++;
    }
    while (i < arrayLength1) {
        vector[k] = leftArr[i];
        i++;
        k++;
    }
    delete[] leftArr;
    delete[] rightArr;
}

template<typename T, typename Compare>
void merge_help(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    merge_help(vector, left, mid, comp);
    merge_help(vector, mid + 1, right, comp);
    merge(vector, left, mid, right, comp);
}

template<typename T, typename Compare>
void merge_sort(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty()) return;
    int left = 0;
    int right = static_cast<int>(vector.size() - 1);
    merge_help(vector, left, right, comp);
}

template<typename T, typename Compare>
int quick_sort_extra_partition(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    int size = right - left + 1;
    int pivot = rand() % size;
    int leftIndex = left;
    int rightIndex = right;
    auto *sorted_arr = new T[size];
    for (int i = 0; i < size; i++) {
        if (i == pivot) continue;
        if (!comp(vector[left + pivot], vector[left + i])) {
            sorted_arr[leftIndex - left] = vector[left + i];
            leftIndex++;
        } else {
            sorted_arr[rightIndex - left] = vector[left + i];
            rightIndex--;
        }
    }
    sorted_arr[leftIndex - left] = vector[left + pivot];
    for (int i = left; i <= right; i++) {
        vector[i] = sorted_arr[i - left];
    }
    delete[] sorted_arr;
    return leftIndex;
}

template<typename T, typename Compare>
void quick_sort_extra_help(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    if (left >= right) return;
    int leftIndex = quick_sort_extra_partition(vector, left, right, comp);
    quick_sort_extra_help(vector, left, leftIndex - 1, comp);
    quick_sort_extra_help(vector, leftIndex + 1, right, comp);
}

template<typename T, typename Compare>
void quick_sort_extra(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty()) return;
    int left = 0;
    int right = static_cast<int>(vector.size() - 1);
    quick_sort_extra_help(vector, left, right, comp);
}

template<typename T, typename Compare>
int quick_sort_inplace_partition(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    int size = right - left + 1;
    int pivot = rand() % size + left;
    int leftIndex = left;
    int rightIndex = right;
    swap(&vector[pivot], &vector[left]);
    pivot = leftIndex++;
    do {
        while (comp(vector[leftIndex], vector[pivot]) && leftIndex < right) {
            leftIndex++;
        }
        while (!comp(vector[rightIndex], vector[pivot]) && rightIndex > left) {
            rightIndex--;
        }
        if (leftIndex < rightIndex) swap(&vector[leftIndex], &vector[rightIndex]);
    } while (leftIndex < rightIndex);

    swap(&vector[pivot], &vector[rightIndex]);
    return rightIndex;
}

template<typename T, typename Compare>
void quick_sort_inplace_help(std::vector<T> &vector, int left, int right, Compare comp = std::less<T>()) {
    if (left >= right) return;
    int pivotat = quick_sort_inplace_partition(vector, left, right, comp);
    quick_sort_inplace_help(vector, left, pivotat - 1, comp);
    quick_sort_inplace_help(vector, pivotat + 1, right, comp);
}

template<typename T, typename Compare>
void quick_sort_inplace(std::vector<T> &vector, Compare comp = std::less<T>()) {
    if (vector.empty()) return;
    int left = 0;
    int right = static_cast<int>(vector.size() - 1);
    quick_sort_inplace_help(vector, left, right, comp);
}

#endif // VE281P1_SORT_HPP