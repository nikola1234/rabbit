#include <iostream>
#include <vector>
#include "QuickSort.h"
#include "Logging.h"

using namespace std;

// 交换函数
void swap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

// 分区函数
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // 以最后一个元素作为基准
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);

    return i + 1;
}

// 快速排序函数
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

// 打印数组
void printArrayInQuickSort(vector<int>& arr) {
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;
}


void QuickSort::testrun(void)
{
    LOG_INFO << "QuickSort run test";
    vector<int> arr = {7, 2, 5, 9, 1, 4, 6, 3, 8};
    int n = arr.size();

    cout << "原始数组：";
    printArrayInQuickSort(arr);

    quickSort(arr, 0, n - 1);

    cout << "排序后的数组：";
    printArrayInQuickSort(arr);
}

bool runQuickSort()
{
    QuickSort s;
    s.testrun();
    return true;
}

namespace nm_QuickSort{
    // bool _ = runQuickSort();
}
