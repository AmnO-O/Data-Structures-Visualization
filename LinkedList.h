#pragma once

struct LLNode {
    int value;
    LLNode* next;
};

class LinkedList {
private:
public:
    LLNode* head = nullptr; // Danh sách liên kết chính

    // Các hàm của danh sách liên kết
    void InsertAtHead(int value);
    void InsertAtTail(int value);
    void InsertAtPosition(int value, int position);
    void DeleteValue(int value);
    void ReverseList();
    void ClearList();
    LLNode* SearchNode(int value);
};