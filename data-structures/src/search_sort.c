#include "ds.h"

#include <stdlib.h>

int search_sequential(const int *data, size_t length, int key)
{
    size_t i;

    if (data == NULL && length != 0) {
        return -1;
    }
    for (i = 0; i < length; ++i) {
        if (data[i] == key) {
            return (int)i;
        }
    }
    return -1;
}

int search_binary(const int *data, size_t length, int key)
{
    size_t left = 0;
    size_t right = length;

    if (data == NULL && length != 0) {
        return -1;
    }
    /* [left, right) 左闭右开区间，计算中点可避免 left + right 溢出。 */
    while (left < right) {
        size_t middle = left + (right - left) / 2;
        if (data[middle] < key) {
            left = middle + 1;
        } else if (data[middle] > key) {
            right = middle;
        } else {
            return (int)middle;
        }
    }
    return -1;
}

static bool valid_sort_input(const int *data, size_t length)
{
    return data != NULL || length == 0;
}

static void swap(int *left, int *right)
{
    int temporary = *left;
    *left = *right;
    *right = temporary;
}

bool sort_insertion(int *data, size_t length)
{
    size_t i;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    for (i = 1; i < length; ++i) {
        int key = data[i];
        size_t position = i;
        while (position > 0 && data[position - 1] > key) {
            data[position] = data[position - 1];
            --position;
        }
        data[position] = key;
    }
    return true;
}

bool sort_selection(int *data, size_t length)
{
    size_t i;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    for (i = 0; i < length; ++i) {
        size_t minimum = i;
        size_t j;
        for (j = i + 1; j < length; ++j) {
            if (data[j] < data[minimum]) {
                minimum = j;
            }
        }
        if (minimum != i) {
            swap(&data[i], &data[minimum]);
        }
    }
    return true;
}

bool sort_bubble(int *data, size_t length)
{
    size_t end;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    for (end = length; end > 1; --end) {
        size_t i;
        bool changed = false;
        for (i = 1; i < end; ++i) {
            if (data[i - 1] > data[i]) {
                swap(&data[i - 1], &data[i]);
                changed = true;
            }
        }
        if (!changed) {
            break;
        }
    }
    return true;
}

bool sort_shell(int *data, size_t length)
{
    size_t gap;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    /* 希爾排序按逐步缩小的间隔分组进行直接插入排序。 */
    for (gap = length / 2; gap > 0; gap /= 2) {
        size_t i;
        for (i = gap; i < length; ++i) {
            int key = data[i];
            size_t position = i;
            while (position >= gap && data[position - gap] > key) {
                data[position] = data[position - gap];
                position -= gap;
            }
            data[position] = key;
        }
    }
    return true;
}

static void quick_sort_range(int *data, size_t left, size_t right)
{
    while (left < right) {
        size_t lower = left;
        size_t current = left;
        size_t upper = right;
        size_t left_size;
        size_t right_size;
        int pivot = data[left + (right - left) / 2];

        /* 三路划分把与枢轴相等的元素一次性归位，重复值不会退化。 */
        while (current < upper) {
            if (data[current] < pivot) {
                swap(&data[lower++], &data[current++]);
            } else if (data[current] > pivot) {
                swap(&data[current], &data[--upper]);
            } else {
                ++current;
            }
        }
        left_size = lower - left;
        right_size = right - upper;
        /* 递归较短的一侧，将最坏情况下的调用栈控制在 O(log n)。 */
        if (left_size < right_size) {
            if (left_size > 1) {
                quick_sort_range(data, left, lower);
            }
            if (right_size < 2) {
                return;
            }
            left = upper;
        } else {
            if (right_size > 1) {
                quick_sort_range(data, upper, right);
            }
            if (left_size < 2) {
                return;
            }
            right = lower;
        }
    }
}

bool sort_quick(int *data, size_t length)
{
    if (!valid_sort_input(data, length)) {
        return false;
    }
    if (length > 1) {
        quick_sort_range(data, 0, length);
    }
    return true;
}

static void sift_down(int *data, size_t root, size_t length)
{
    while (root < length / 2) {
        size_t child = root * 2 + 1;
        if (child + 1 < length && data[child] < data[child + 1]) {
            ++child;
        }
        if (data[root] >= data[child]) {
            return;
        }
        swap(&data[root], &data[child]);
        root = child;
    }
}

bool sort_heap(int *data, size_t length)
{
    size_t start;
    size_t end;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    /* 从最后一个非叶结点开始逐个建成大顶堆。 */
    for (start = length / 2; start > 0; --start) {
        sift_down(data, start - 1, length);
    }
    for (end = length; end > 1; --end) {
        swap(&data[0], &data[end - 1]);
        sift_down(data, 0, end - 1);
    }
    return true;
}

static void merge_ranges(int *data, int *temporary, size_t left,
                         size_t middle, size_t right)
{
    size_t first = left;
    size_t second = middle;
    size_t output = left;

    while (first < middle && second < right) {
        if (data[first] <= data[second]) {
            temporary[output++] = data[first++];
        } else {
            temporary[output++] = data[second++];
        }
    }
    while (first < middle) {
        temporary[output++] = data[first++];
    }
    while (second < right) {
        temporary[output++] = data[second++];
    }
    while (left < right) {
        data[left] = temporary[left];
        ++left;
    }
}

static void merge_sort_range(int *data, int *temporary, size_t left,
                             size_t right)
{
    size_t middle;

    if (right - left < 2) {
        return;
    }
    middle = left + (right - left) / 2;
    merge_sort_range(data, temporary, left, middle);
    merge_sort_range(data, temporary, middle, right);
    merge_ranges(data, temporary, left, middle, right);
}

bool sort_merge(int *data, size_t length)
{
    int *temporary;

    if (!valid_sort_input(data, length)) {
        return false;
    }
    if (length < 2) {
        return true;
    }
    if (length > (size_t)-1 / sizeof(*temporary)) {
        return false;
    }
    temporary = (int *)malloc(length * sizeof(*temporary));
    if (temporary == NULL) {
        return false;
    }
    merge_sort_range(data, temporary, 0, length);
    free(temporary);
    return true;
}
