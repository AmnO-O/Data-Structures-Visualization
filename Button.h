#pragma once
#include "Constants.h"


struct Button {
    Rectangle rect;           // Vị trí & kích thước nút
    string text;         // Nội dung hiển thị

    Color normalColor = RAYWHITE;    // Màu bình thường
    Color hoverColor = LIGHTGRAY;        // Màu khi hover
    Color clickColor = LIGHTGRAY;      // Màu khi nhấn giữ

    bool isHovered = false;
    bool isPressed = false;
    bool isChose = false;

    Font font = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
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

        DrawRectangleRounded(rect, 0.3, 6, drawColor);
        DrawRectangleRoundedLinesEx(rect, 0.3f, 6, 2.0f, GRAY);
    }

    void drawName() {
        int fontSize = 18;
        float spacing = 2;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;
        DrawTextEx(font, text.c_str(), { textX, textY }, fontSize, spacing, DARKBLUE);
    }

    void draw() {
        Color drawColor = normalColor;
        if (isHovered) {
            drawColor = hoverColor;
            if (isPressed) {
                drawColor = clickColor;
            }
        }

        DrawRectangleRounded(rect, 0.3, 6, drawColor);
        DrawRectangleRoundedLinesEx(rect, 0.3f, 6, 2.0f, GRAY);

        int fontSize = 18;
        float spacing = 2.0f;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;
        DrawTextEx(font, text.c_str(), { textX, textY }, fontSize, spacing, DARKBLUE);
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
    Color btnNormal = RAYWHITE;
    Color btnHover = LIGHTGRAY;
    Color btnClick = LIGHTGRAY;

    vector <Button> cButton;
    Rectangle rect;
    Color color;
};

