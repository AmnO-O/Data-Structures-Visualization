#pragma once
#pragma once
#include "Constants.h"
#include "Button.h"
#include "TextBox.h"


struct Nodell {
    int data;
    Nodell* next;
    Vector2 centerPos;
    int nodeRadius = 50;


    Nodell(int value, Vector2 center)
        : data(value), next(nullptr), centerPos(center) {
    }

   /* Vector2 Lerp(Vector2 start, Vector2 end, float t) {
        return { start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t };
    }*/

    /*void update(float deltaTime, float baseSpeed, float speedMultiplier) {
        currentPosition = Lerp(currentPosition, targetPosition, baseSpeed * speedMultiplier * deltaTime);
    }*/

    void draw() {
        DrawCircleV(centerPos, nodeRadius, BLUE);
        DrawCircleLines(centerPos.x, centerPos.y, nodeRadius, WHITE);
        DrawText(TextFormat("%d", data), centerPos.x - 10, centerPos.y - 10, 20, WHITE);
    }
};

class Linkedlist {
public:
    Linkedlist();
    ~Linkedlist();
    void add(int key);
    //void update(float deltaTime, float baseSpeed, float speedMultiplier);
    void draw();
    Nodell* search(int key);
private:
    Nodell* root;
    int distance = 50;
};

class LinkedlistVisual {
public:
    Linkedlist ll;
    LinkedlistVisual();
    int handleEvent();
    void draw();
private:
    CommonButton Input;
    Rectangle border, info;
    Button Random, File , Enter ;
    TextBox Key;
};
