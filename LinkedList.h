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

    void draw2(Vector2 pos, float width, float height, float bounceAnim) {
        // The bounceAnim can be used to scale or shift the node slightly
        // bounceAnim is expected to be between [0..1], for a subtle pop effect
        float scale = 1.0f + 0.05f * bounceAnim; // up to +5% bigger
        float offset = -(width * (scale - 1.0f)) / 2;  // recenter

        Rectangle r;
        r.x = pos.x + offset;
        r.y = pos.y + offset;

        r.width = width * scale;
        r.height = height * scale;

        float roundness = 0.3f;  // 30% corner roundness
        int   segments = 10;    // # segments for smooth corners

        // 1) Draw shadow
        DrawDropShadowRoundedRect(r, roundness, segments, 5.0f, Fade(BLACK, 0.2f));

        // 2) Draw main rounded rectangle
        //    You could also use DrawRectangleRoundedGradient() for a gradient fill,
        //    but let's do a solid color here:
        Color nodeColor = Color{ 230, 240, 255, 255 }; // a light pastel
        DrawRectangleRounded(r, roundness, segments, nodeColor);

        // 3) Draw an outline
        DrawRectangleRoundedLines(r, roundness, segments, DARKBLUE);

        // 4) Draw the node value
        char buf[64];
        sprintf_s(buf, sizeof(buf), "%d", data);
        int fontSize = 20;
        int textWidth = MeasureText(buf, fontSize);
        int textHeight = fontSize; // quick approximation

        float textX = r.x + (r.width - textWidth) / 2.0f;
        float textY = r.y + (r.height - textHeight) / 2.0f;

        DrawText(buf, (int)textX, (int)textY, fontSize, DARKBLUE);
    }

};

class Linkedlist {
public:
    Linkedlist();
    ~Linkedlist();
    void add(int key);
    //void update(float deltaTime, float baseSpeed, float speedMultiplier);
    void draw();
    void draw2(Vector2 startPos);
    Nodell* search(int key);
private:
    Nodell* root = NULL;
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
