#include "LinkedList.h"
#include "raylib.h"

// Chèn vào đầu
void LinkedList::InsertAtHead(int value) {
    LLNode* newNode = new LLNode{ value, head };
    head = newNode;
}

// Chèn vào cuối
void LinkedList::InsertAtTail(int value) {
    if (!head) {
        InsertAtHead(value);
        return;
    }
    LLNode* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = new LLNode{ value, nullptr };
}

// Chèn vào vị trí chỉ định (0-based index)
void LinkedList::InsertAtPosition(int value, int position) {
    if (position == 0) {
        InsertAtHead(value);
        return;
    }

    LLNode* temp = head;
    for (int i = 0; i < position - 1 && temp; i++) {
        temp = temp->next;
    }

    if (temp) {
        LLNode* newNode = new LLNode{ value, temp->next };
        temp->next = newNode;
    }
}

// Xóa giá trị k nếu có
void LinkedList::DeleteValue(int value) {
    if (!head) return;
    if (head->value == value) {
        LLNode* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    LLNode* temp = head;
    while (temp->next && temp->next->value != value) {
        temp = temp->next;
    }

    if (temp->next) {
        LLNode* toDelete = temp->next;
        temp->next = temp->next->next;
        delete toDelete;
    }
}

// Đảo ngược danh sách liên kết
void LinkedList::ReverseList() {
    LLNode* prev = nullptr;
    LLNode* current = head;
    LLNode* next = nullptr;

    while (current) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head = prev;
}

// Tìm kiếm node có giá trị k
LLNode* LinkedList::SearchNode(int value) {
    LLNode* temp = head;
    while (temp) {
        if (temp->value == value) return temp;
        temp = temp->next;
    }
    return nullptr;
}

// Xóa toàn bộ danh sách
void LinkedList::ClearList() {
    while (head) {
        LLNode* temp = head;
        head = head->next;
        delete temp;
    }
}