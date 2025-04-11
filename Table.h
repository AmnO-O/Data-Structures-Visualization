#pragma once
#include "raylib.h"
#include <vector>
#include <string>

class TraversalTable {
public:
    TraversalTable() = default; // Dùng default constructor

    void Init(float x, float y, float width, float height);

    void SetData(std::vector<int> in, std::vector<int> pre, std::vector<int> post);
    void Update();
    void Draw();

    void ToggleVisibility();
    bool IsVisible() const;
    Rectangle GetTableArea() const;
private:
    std::vector<int> inorder, preorder, postorder;
    bool scrollbarDragged = false;
    bool scrollbarHovered = false;
    float targetScrollY;
    float smoothScrollSpeed;
    float dragOffsetY = 0;

    Font myFont;

    Rectangle tableArea{};
    float scrollY = 0;
    float scrollSpeed = 20;
    float maxScroll;
    int rowHeight = 30;
    int visibleRows = 0;

    float animationProgress = 0.0f;
    bool animationActive = false;
    float animationDuration = 0.4f;

    bool visible = false;

    Rectangle toggleButton{};

    void DrawTable();
    void DrawToggleButton();
    float Lerp(float start, float end, float t);
    float Clamp(float value, float min, float max);
};
