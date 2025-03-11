#include "HashTable.h"
#include <string>

HashTable::HashTable(int x) {
    sz = x; 
	htable = new Node * [sz]; 
	for (int i = 0; i < sz; i++) htable[i] = NULL; 
}

void HashTable :: add(int key, Vector2 startPos, Vector2 targetPos) {
	int index = key % sz; 
    Node* newNode = new Node(key, startPos, targetPos);
    newNode->next = htable[index];
    htable[index] = newNode;
}

void HashTable :: update(float deltaTime, float baseSpeed, float speedMultiplier) {
    for (int i = 0; i < sz; i++) {
        Node* current = htable[i];
        while (current) {
            current->update(deltaTime, baseSpeed, speedMultiplier);
            current = current->next;
        }
    }
}

void HashTable:: draw() {
    int startX = 50;
    int startY = 50;
    int bucketSpacing = 80;    
    int nodeSpacing = 80;      

    for (int i = 0; i < sz; i++) {
        int posY = startY + i * bucketSpacing;
        DrawText(TextFormat("Bucket %d:", i), startX, posY, 20, WHITE);

        Node* current = htable[i];
        while (current) {
            current->draw();    
            current = current->next;
        }
    }
}


HashTable :: ~HashTable() {
	for (int i = 0; i < sz; i++) delete[]htable[i]; 
}


HashTableVisual::HashTableVisual() {
    border = { 166, 500, 166, 210 };
    Size = {{250, 521, 65, 25}};

    info = { 0, 350, 332, 150}; 

    Random = {{210, 600, 90, 30}, "Random"};
    File = {{210, 650, 90, 30}, "File"};
}

int HashTableVisual::handleEvent() {
    Input.update(); 

    if (Input.isCreate) {
        Size.update(); 
        File.update();
        Random.update();

        if (Random.isPressed) {
            H = HashTable(Size.getDigit());
        }
    }
    else if (Input.isInsert) {

    }
    else if (Input.isRemove) {

    }
    else {

    }

    return HashTable_state; 
}


void HashTableVisual::draw() {
    Input.draw();
    DrawRectangleRec(border, Color{ 220, 180, 120, 255 });
    DrawRectangleRec(info, DARKGRAY);

    Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

    DrawTextEx(font, "[INFO]:", {info.x + 10, info.y + 10 }, 21, 1, PURPLE);

    H.draw(); 

    if (Input.isCreate) {
        int fontSize = 24;
        float spacing = 1.0f;


        DrawTextEx(font, "Size:", {185, 520}, fontSize, spacing, BLACK);
        Size.draw(); 
        File.draw(); 
        Random.draw(); 
    }
    else if (Input.isInsert) {

    }
    else if (Input.isRemove) {

    }
    else {

    }
}

