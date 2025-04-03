#pragma once

#include <vector>

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
    int getSize();
    void ClearList();
    LLNode* SearchNode(int value);
    int GetPosition(int value);
    void DeleteAtHead();
    void DeleteAtPosition(int position);
    void DeleteAtTail();
};