#pragma once
#include "Constants.h"
#include "Button.h"
#include "TextBox.h"


struct Node {
    int data;
    Node* next;
    Vector2 currentPosition;  
    Vector2 targetPosition;   

    Node(int value, Vector2 startPos, Vector2 targetPos)
        : data(value), next(nullptr), currentPosition(startPos), targetPosition(targetPos) {
    }

    Vector2 Lerp(Vector2 start, Vector2 end, float t) {
        return { start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t };
    }

    void update(float deltaTime, float baseSpeed, float speedMultiplier) {
        currentPosition = Lerp(currentPosition, targetPosition, baseSpeed * speedMultiplier * deltaTime);
    }

    void draw() {
        int nodeRadius = 20;
        DrawCircleV(currentPosition, nodeRadius, BLUE);
        DrawCircleLines(currentPosition.x, currentPosition.y, nodeRadius, WHITE);
        DrawText(TextFormat("%d", data), currentPosition.x - 10, currentPosition.y - 10, 20, WHITE);
    }
};

class HashTable {
public:
    HashTable(int x = 0); 
    ~HashTable(); 
    void add(int key, Vector2 startPos, Vector2 targetPos); 
    void update(float deltaTime, float baseSpeed, float speedMultiplier);
    void draw(); 
private:
    Node** htable; 
    int sz;
};

class HashTableVisual {
public:
    HashTableVisual();
    int handleEvent(); 
    void draw(); 
private:
    CommonButton Input; 
    Rectangle border, info; 
    Button Random, File; 
    TextBox Size; 
    HashTable H; 
};