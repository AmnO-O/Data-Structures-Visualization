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
    bool debugMode = false;

    // Hàm Update() kiểm tra di chuột, bấm chuột...
    // Trả về true nếu người dùng "click" (nhả chuột trong vùng nút)
    bool update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, rect);

        if (isHovered) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                isPressed = true;
            }
            else {
                isPressed = false;
            }
            // Nếu thả chuột ra (sau khi hover), tính là "click"
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // Toggle trạng thái isChose
                isChose = !isChose;
                return true;
            }
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

    void drawName(Font font) {
        int fontSize = 20;
        float spacing = 2;
        Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
        float textX = rect.x + (rect.width - textSize.x) / 2.0f;
        float textY = rect.y + (rect.height - textSize.y) / 2.0f;
        DrawTextEx(font, text.c_str(), { textX, textY }, fontSize, spacing, DARKBLUE);
    }

    void draw(Font font) {
        if (debugMode) {
            std::string info = "x: " + std::to_string(rect.x) +
                ", y: " + std::to_string(rect.y) +
                ", w: " + std::to_string(rect.width) +
                ", h: " + std::to_string(rect.height);
            DrawText(info.c_str(), (int)rect.x, (int)(rect.y - 20), 10, BLACK);
        }

        Color drawColor = normalColor;
        if (isHovered) {
            drawColor = hoverColor;
            if (isPressed) {
                drawColor = clickColor;
            }
        }

        DrawRectangleRounded(rect, 0.3f, 6, drawColor);

        DrawRectangleRoundedLinesEx(rect, 0.3f, 6, 2.0f, GRAY);

        if (isChose)
            DrawRectangleRoundedLinesEx(rect, 0.3f, 6, 4.0f, YELLOW);

        int fontSize = 20;
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
    void draw(Font font);

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

struct Instruction {
    // Content
    std::string title = "Title";
    std::string message = "Message content";
    std::string buttonText = "OK";

    float width = 400.0f;
    float height = 200.0f;
    Color backgroundColor = RAYWHITE;
    Color borderColor = DARKGRAY;
    Color overlayColor = { 0, 0, 0, 128 }; // semi-transparent black
    Color textColor = BLACK;
    int fontSize = 20;
    float textSpacing = 2.0f;
    float modalX = (Screen_w - width) / 2.0f;
    float modalY = (Screen_h - height) / 2.0f;

    bool hovered = false, clicked = false, display = false;
    Button okRect;

    // This function shows the modal with given title/message
    void show(const std::string& newTitle = "", const std::string& newMessage = "") {
        float buttonWidth = 80.0f;
        float buttonHeight = 30.0f;
        float buttonX = modalX + (width - buttonWidth) / 2.0f;
        float buttonY = (modalY + height) - buttonHeight - 10.0f;
        okRect = { buttonX, buttonY, buttonWidth, buttonHeight };

        title = newTitle;
        message = newMessage;
        okRect.text = buttonText;
    }

    bool update() {
        if (!display) return false;

        if (okRect.update()) {
            display = false;
            return true;
        }

        return false;
    }

    bool draw(Font font) {
        if (!display) return false;

        DrawRectangle(0, 0, Screen_w, Screen_h, overlayColor);
        Rectangle modalRect = { modalX, modalY, width, height };

        DrawRectangleRounded(modalRect, 0.1f, 4, backgroundColor);
        DrawRectangleRoundedLines(modalRect, 0.1f, 4, borderColor);

        Vector2 titleSize = MeasureTextEx(font, title.c_str(), 22, textSpacing);
        float titleX = modalX + (width - titleSize.x) / 2.0f;
        float titleY = modalY + 20.0f;

        DrawTextEx(font, title.c_str(), Vector2{ titleX, titleY },
            22, textSpacing, textColor);

        Vector2 messageSize = MeasureTextEx(font, message.c_str(), 20, textSpacing);
        float messageX = modalX + (width - messageSize.x) / 2.0f;
        float messageY = titleY + titleSize.y + 10.0f;

        DrawTextEx(font, message.c_str(), Vector2{ messageX, messageY },
            20, textSpacing, textColor);

        okRect.draw(font);

        return false;
    }
};