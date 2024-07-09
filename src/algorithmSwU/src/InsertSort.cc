#include <iostream>
#include <vector>
#include "InsertSort.h"
#include "Logging.h"

using namespace std;

void insertionSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 1; i < n; i++) {
        int key = arr[i]; // 当前待插入的元素
        int j = i - 1;

        // 移动已排序部分中较大的元素
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key; // 将当前元素插入到正确的位置
    }
}

void printArrayInInsertSort(vector<int>& arr) {
    for (const auto& num : arr) {
        cout << num << " ";
    }
    cout << endl;
}


void InsertSort::testrun(void)
{
    LOG_INFO << "InsertSort run test";
    vector<int> arr = {7, 2, 5, 9, 1, 4, 6, 3, 8};

    cout << "原始数组：";
    printArrayInInsertSort(arr);
    // LOG_INFO << arr;

    insertionSort(arr);

    cout << "排序后的数组：";
    printArrayInInsertSort(arr);

}

bool runInsertSort()
{
    InsertSort s;
    s.testrun();
    return true;
}

namespace nm_InsertSort{
   // bool _ = runInsertSort();
}
