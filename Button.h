#pragma once
#include "Constants.h"


struct Button {
    Rectangle rect;           // Vị trí & kích thước nút
    string text;         // Nội dung hiển thị
    
    Color normalColor = { 42, 42, 42, 255 };    // Màu bình thường
    Color hoverColor{ 60, 60, 60, 255 };        // Màu khi hover
    Color clickColor = { 90, 90, 90, 255 };      // Màu khi nhấn giữ

    bool isHovered = false;
    bool isPressed = false;
    bool isChose = false; 

    Font font = LoadFontEx("Assets/Fonts/PublicSans-bold.ttf", 65, 0, 0);
    // Hàm Update() kiểm tra di chuột, bấm chuột...
    // Trả về true nếu người dùng "click" (nhả chuột trong vùng nút)

    bool update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, rect);

        if (isHovered) {
            // Nếu chuột đang đè xuống
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                isPressed = true;
            }
            else {
                isPressed = false;
            }
            // Nếu thả chuột ra (sau khi đã hover), tính là "click"
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) return true;
        }
        else {
            isHovered = false;
            isPressed = false;
        }

        return false;
    }
    void drawRect() {
        Color drawColor = normalColor;
        if (isHovered) {
            drawColor = hoverColor;
            if (isPressed) 
                drawColor = clickColor;
        }

        DrawRectangleRounded(rect, 0.2f, 6, drawColor);
    }

    void drawName() {
        int fontSize = 23;
        float spacing = 1.0f;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;
        DrawTextEx(font, text.c_str(), { textX, textY }, fontSize, spacing, RAYWHITE);
    }

    void draw() {
        Color drawColor = normalColor;
        if (isHovered) {
            drawColor = hoverColor;
            if (isPressed) {
                drawColor = clickColor;
            }
        }

        DrawRectangleRounded(rect, 0.2f, 6, drawColor);

        int fontSize = 20;
        float spacing = 1.0f;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;
        DrawTextEx(font, text.c_str(), { textX, textY }, fontSize, spacing, RAYWHITE);
    }
};

class CommonButton {
public:
    CommonButton(); 
    void update(); 
    void draw(); 

    bool isCreate = true;
    bool isInsert = false; 
    bool isRemove = false;
    bool isSearch = false;

private:
    Color btnNormal = { 42, 42, 42, 255 };
    Color btnHover = { 60, 60, 60, 255 };
    Color btnClick = { 90, 90, 90, 255 };

    vector <Button> cButton;
    Rectangle rect;
    Color color = { 244, 162, 97, 255 };
};


