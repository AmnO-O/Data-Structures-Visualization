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

struct AnimationData {
    int key;
    std::vector<int> pathIndices;
    int currentPathIndex;
    Vector2 currentPos;
    float elapsedTime;
    float segmentDuration;
    bool active;
    bool operationSuccess;
    int targetIndex;
};



class HashTable {
public:
    HashTable(int x = 0);
    vector <int> getInsertionPath(int val);
    int getSize() const;
    int getValue(int pos) const;

    int search(int val);
    int search(int val, vector <int>& path);

    void ins(int val);
    void ins(int index, int val);
    void rem(int val);
    void draw(Font font);
    void drawValue(int index, Font font, bool highlight = false);
    void upd(float delta);
    Vector2 getCenter(int index);
    void drawSlot(int index, Font font, bool highlight = false, bool Value = true);
    void clear();

private:
    void drawSlot(Rectangle rect, int index, const std::string& text, Font font, bool highlight = false, bool Value = true);

    bool isInitial = false;

    float AnimationTime = 0.f;
    float AnimationDuration = 0.2f;

    int htable[800];
    int sz;
    int nodesPerRow = 10;
    int slotWidth = 70;
    int slotHeight = 70;
    int margin = 30;
    int startX = 480;
    int startY = 150;
};