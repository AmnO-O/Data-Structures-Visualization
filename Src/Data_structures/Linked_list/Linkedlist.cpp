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

void LinkedList::DeleteValue(int value) {
    if (!head) return;

    // Trường hợp node đầu tiên là node cần xóa
    if (head->value == value) {
        LLNode* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    // Duyệt danh sách để tìm node có giá trị value
    LLNode* current = head;
    while (current->next) {
        if (current->next->value == value) {
            LLNode* toDelete = current->next;
            current->next = current->next->next;
            delete toDelete;
            return;
        }
        current = current->next;
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

// Trả về size của Linked List
int LinkedList::getSize() {
    if (!head) return 0;

    int size = 0;
    LLNode* current = head;
    while (current) {
        size++;
        current = current->next;
    }
    return size;
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

int LinkedList::GetPosition(int value) {
    LLNode* current = head;
    int position = 0;
    while (current) {
        if (current->value == value) {
            return position;
        }
        current = current->next;
        position++;
    }
    return -1; // Không tìm thấy
}

void LinkedList::DeleteAtHead() {
    if (head) {
        LLNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void LinkedList::DeleteAtTail() {
    if (!head) return;
    if (!head->next) {
        delete head;
        head = nullptr;
        return;
    }
    LLNode* current = head;
    while (current->next->next) {
        current = current->next;
    }
    delete current->next;
    current->next = nullptr;
}

void LinkedList::DeleteAtPosition(int position) {
    if (position < 0 || !head) return;

    if (position == 0) {
        DeleteAtHead();
        return;
    }

    LLNode* current = head;
    for (int i = 0; i < position - 1 && current; i++) {
        current = current->next;
    }

    if (current && current->next) {
        LLNode* temp = current->next;
        current->next = temp->next;
        delete temp;
    }
}

void LinkedList::CreateRandom(int count) {
    ClearList(); // Xóa danh sách cũ nếu có
    for (int i = 0; i < count; ++i) {
        int value = GetRandomValue(0, 99); // Hàm có sẵn của Raylib
        InsertAtTail(value); // Bạn có thể InsertAtHead nếu muốn ngược lại
    }
}

bool LinkedList::CreateFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) return false;

    ClearList(); // Xóa danh sách cũ

    int value;
    while (file >> value) {
        InsertAtTail(value); // Chèn vào cuối danh sách
    }

    file.close();
    return true;
}

LLNode* LinkedList::FindFirstNodeWithValue(int value) {
    LLNode* current = head;
    while (current) {
        if (current->value == value) return current;
        current = current->next;
    }
    return nullptr;
}