
#include "DivideLinkedList.h"
#include "Logging.h"

namespace{

struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
 };

ListNode* buildListFromVector(const std::vector<int>& arr, size_t index = 0) {
    if (index < arr.size()) {
        return new ListNode(arr[index], buildListFromVector(arr, index + 1));
    }
    return nullptr;
}

 ListNode* partition(ListNode* head, int x)
 {
    ListNode* small = new ListNode(0);
    ListNode* smallHead = small;
    ListNode* large = new ListNode(0);
    ListNode* largeHead = large;
    while (head != nullptr) {
        if (head->val < x) {
            small->next = head;
            small = small->next;
        } else {
            large->next = head;
            large = large->next;
        }
        head = head->next;
    }
    large->next = nullptr;
    small->next = largeHead->next;
    return smallHead->next;
}

}
void DivideLinkedList::testrun(void)
{
    LOG_INFO << "DivideLinkedList run test";

    std::vector<int> listData = {1, 1};
    ListNode* head = buildListFromVector(listData);

    partition(head, 2);
/*
    ListNode* current = head;
    while (current != nullptr) {
        std::cout << current->val << " ";
        current = current->next;
    }
    std::cout << std::endl;
*/
    LOG_INFO << "partition done";
    // 清理内存
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

bool runDivideLinkedListTest()
{
    DivideLinkedList s;
    s.testrun();
    return true;
}

namespace nm_DivideLinkedListTest{
   // bool _ = runDivideLinkedListTest();
}
