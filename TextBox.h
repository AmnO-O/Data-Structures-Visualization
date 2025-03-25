#pragma once
#include "Constants.h"

struct TextBox {
    Rectangle bounds;      // Text box 's area
    string text = "";           // Content entered
    string outputMessage;

    bool focused;          // is being typed or not
    int pos;               // The position that we want to continue type        

    int framesCounter;     // Blink blink 
    bool isEnter = false;
    bool tt = false;

    Font font = LoadFontEx("Assets/Fonts/PublicSans-bold.ttf", 65, 0, 0);
    int fontSize = 20;

    void update() {
        isEnter = false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
                focused = true;
                Vector2 mousePos = GetMousePosition();
                int relativeX = (int)(mousePos.x - bounds.x - 5);

                int cumWidth = 0;
                pos = 0;

                for (size_t i = 0; i < text.size(); i++) {
                    int charWidth = MeasureTextEx(font, text.substr(i, 1).c_str(), fontSize, 1).x;

                    if (cumWidth + charWidth / 2 >= relativeX) {
                        pos = i;
                        break;
                    }
                    cumWidth += charWidth;
                    pos = i + 1;
                }
            }
            else
                focused = false;
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 126)) {
                    text.insert(pos, 1, (char)key);
                    pos++;
                }
                key = GetCharPressed();
            }

            // Backspace handling: deletes before the pos
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (pos > 0) {
                    text.erase(pos - 1, 1);
                    pos--;
                }
            }
            // Delete handling: deletes right at the pos
            if (IsKeyPressed(KEY_DELETE)) {
                if (pos < text.size()) text.erase(pos, 1);
            }

            if (IsKeyPressed(KEY_LEFT)) {
                if (pos > 0)
                    pos--;
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (pos < (int)text.size())
                    pos++;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                isEnter = true;
                outputMessage = text;
                text = "";
                focused = 0;
                framesCounter = 0;
                return;
            }
        }

        framesCounter++;
    }

    void sss() {
        if (tt == 1) {
            outputMessage = text;
            tt = 0;
        }
    }

    int getDigit() {
        int c = 0;
        for (int i = 0; i < text.size(); i++) {
            c = c * 10 + text[i] - '0';
        }

        return c;
    }

    void draw() {
        Color boxColor = focused ? YELLOW : LIGHTGRAY;
        int textY = bounds.y + (bounds.height - fontSize) / 2;

        DrawRectangleRec(bounds, boxColor);
        DrawRectangleLinesEx(bounds, 1, DARKGRAY);

        // DrawText(text.c_str(), bounds.x + 5, bounds.y + 5, 20, BLACK);
        string displayText = text;
        int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
        if (textWidth > bounds.width - 10) {
            // Duyệt từ cuối về đầu để lấy phần text vừa với chiều rộng
            int start = text.size() - 1;
            int cumWidth = 0;
            for (int i = start; i >= 0; i--) {
                int charWidth = MeasureTextEx(font, text.substr(i, 1).c_str(), fontSize, 1).x;
                if (cumWidth + charWidth > bounds.width - 15) {
                    displayText = text.substr(i + 1);
                    break;
                }
                cumWidth += charWidth;
            }
        }

        DrawTextEx(font, displayText.c_str(), { bounds.x + 5, textY / 1.0f }, fontSize, 1, BLACK);

        if (focused && ((framesCounter / 20) % 2 == 0)) {
            int cursorX = bounds.x + 5 + MeasureTextEx(font, displayText.substr(0, pos).c_str(), fontSize, 1).x;
            DrawLine(cursorX, textY, cursorX, textY + fontSize, BLACK);
        }
    }
};

struct TextBoxCenter { /// put the cursor in the center
    Rectangle bounds;      // Text box 's area
    string text = "";           // Content entered
    string outputMessage;

    bool focused;          // is being typed or not
    int pos;               // The position that we want to continue type        

    int framesCounter;     // Blink blink 
    bool isEnter = false;
    Font font = LoadFontEx("Assets/Fonts/PublicSans-bold.ttf", 65, 0, 0);
    int fontSize = 20;
    Color textColor = BLACK; 

    void update() {
        isEnter = false;
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
                focused = true;
                Vector2 mousePos = GetMousePosition();
                int relativeX = (int)(mousePos.x - bounds.x - 5);

                int cumWidth = 0;
                pos = 0;

                for (size_t i = 0; i < text.size(); i++) {
                    int charWidth = MeasureTextEx(font, text.substr(i, 1).c_str(), fontSize, 1).x;

                    if (cumWidth + charWidth / 2 >= relativeX) {
                        pos = i;
                        break;
                    }
                    cumWidth += charWidth;
                    pos = i + 1;
                }
            }
            else
                focused = false;
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((key >= 32) && (key <= 126)) {
                    text.insert(pos, 1, (char)key);
                    pos++;
                }
                key = GetCharPressed();
            }

            // Backspace handling: deletes before the pos
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (pos > 0) {
                    text.erase(pos - 1, 1);
                    pos--;
                }
            }
            // Delete handling: deletes right at the pos
            if (IsKeyPressed(KEY_DELETE)) {
                if (pos < text.size()) text.erase(pos, 1);
            }

            if (IsKeyPressed(KEY_LEFT)) {
                if (pos > 0)
                    pos--;
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                if (pos < (int)text.size())
                    pos++;
            }

            if (IsKeyPressed(KEY_ENTER)) {
                outputMessage = text;
                focused = 0;
                framesCounter = 0;
                isEnter = 1;
                return;
            }
        }

        framesCounter++;
    }

    int getDigit() {
        int c = 0;
        for (int i = 0; i < text.size(); i++) {
            c = c * 10 + text[i] - '0';
        }

        return c;
    }

    void draw() {
        Color boxColor = focused ? YELLOW : Color{ 244, 162, 83, 210 };

        int textY = bounds.y + (bounds.height - fontSize) / 2;
        // Vẽ ô text box với góc bo
        float roundness = 0.25f;   // Độ bo tròn (0.0 -> 1.0)
        int segments = 6;         // Số segment để vẽ góc cong
        float outlineThickness = 2.0f;

        DrawRectangleRounded(bounds, roundness, segments, boxColor);
        DrawRectangleRoundedLinesEx(bounds, roundness, segments, outlineThickness, DARKGRAY);


        int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
        int textHeight = MeasureTextEx(font, text.c_str(), fontSize, 1).y;
        DrawTextEx(font, text.c_str(), { bounds.x + (bounds.width - textWidth) / 2, bounds.y + (bounds.height - textHeight) / 2 }, fontSize, 1, textColor);

        if (focused && ((framesCounter / 20) % 2 == 0)) {
            std::string textBeforeCaret = text.substr(0, pos);
            int textWidth = MeasureTextEx(font, textBeforeCaret.c_str(), fontSize, 1).x;
            int textFull = MeasureTextEx(font, text.c_str(), fontSize, 1).x;

            DrawLine(bounds.x + bounds.width / 2 - textFull / 2 + 2 + textWidth, textY,
                bounds.x + bounds.width / 2 - textFull / 2 + 2 + textWidth, textY + fontSize, BLACK);
        }
    }
};