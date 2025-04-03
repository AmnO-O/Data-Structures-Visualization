#pragma once
#include "Constants.h"
#include <sstream>
#include <fstream>
struct TextBox {
    Rectangle bounds;      // Text box 's area
    string text = "";           // Content entered
    string outputMessage;

    bool focused;          // is being typed or not
    int pos;               // The position that we want to continue type        

    int framesCounter;     // Blink blink 
    bool isEnter = false;
    bool tt = false;
    bool isClickedEnter = false;

    Font font = LoadFont("Assets/Fonts/PublicSans-bold.ttf");
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

    int getDigit() {
        int c = 0;
        for (int i = 0; i < text.size(); i++) {
            c = c * 10 + text[i] - '0';
        }

        return c;
    }

    void sss() {
        if (tt = 1) {
            outputMessage = text;
            tt = 0;
        }
    }

    void getMessage() {
        if (isClickedEnter) {
            outputMessage = text;
            text = "";
            focused = 0;
            framesCounter = 0;
        }
        return;
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
    Font font = LoadFont("Assets/Fonts/PublicSans-bold.ttf");
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
        Color boxColor = focused ? YELLOW : (isDarkMode ? Color{ 244, 162, 83, 210 } : RAYWHITE);

        if (focused && text == "Random") text = "", pos = 0;

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



struct TextBoxEdge {
    Rectangle bounds;            // Text box's area
    std::string text;            // Content entered (e.g. "0,0" or "0,0,0")
    std::string outputMessage;

    bool focused = false;        // Is the textbox being edited?
    int pos = 0;                 // Current caret position in text (0 <= pos <= text.size())

    int framesCounter = 0;       // For caret blinking
    bool isEnter = false;        // True if Enter is pressed

    Font font = LoadFont("Assets/Fonts/PublicSans-bold.ttf");
    int fontSize = 20;
    Color textColor = BLACK;

    // When weighted is true, we expect three numbers (u,v,w), otherwise only two (u,v).
    bool weighted = false;

    // Set the default text based on the weighted flag.
    void setDefault() {
        if (weighted) {
            text = "0,0,0";
        }
        else {
            text = "0,0";
        }
        pos = 0;
    }

    // Returns the start and end indices of the current segment (i.e. parameter) based on pos.
    void GetSegmentBounds(int& segStart, int& segEnd) {
        segStart = 0;
        segEnd = (int)text.size();
        size_t found = text.rfind(',', pos == 0 ? 0 : pos - 1);
        if (found != std::string::npos) {
            segStart = (int)found + 1;
        }
        found = text.find(',', pos);
        if (found != std::string::npos) {
            segEnd = (int)found;
        }
    }

    vector<int> getValues() const {
        vector<int> values;
        istringstream ss(text);
        string token;
        while (getline(ss, token, ',')) {
            try {
                int value = std::stoi(token);
                values.push_back(value);
            }
            catch (...) {
                values.push_back(0);
            }
        }
        return values;
    }

    // Update processes input and handles caret movement.
    void update() {
        isEnter = false;
        // Mouse click handling: check if inside textbox.
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(GetMousePosition(), bounds)) {
                focused = true;
                // Compute caret position based on click relative to left parenthesis.
                Vector2 mousePos = GetMousePosition();
                // Account for a left margin and the non-editable '('.
                int relativeX = (int)(mousePos.x - bounds.x - 5 - MeasureTextEx(font, "(", fontSize, 1).x);
                int cumWidth = 0;
                pos = 0;
                for (size_t i = 0; i < text.size(); i++) {
                    int charWidth = (int)MeasureTextEx(font, text.substr(i, 1).c_str(), fontSize, 1).x;
                    if (cumWidth + charWidth / 2 >= relativeX) {
                        pos = (int)i;
                        break;
                    }
                    cumWidth += charWidth;
                    pos = (int)i + 1;
                }
            }
            else {
                focused = false;
            }
        }

        if (focused) {
            int key = GetCharPressed();
            while (key > 0) {
                if ((char)key == ',') {
                    // Move caret to after the next comma, if available.
                    size_t commaPos = text.find(',', pos);
                    if (commaPos != std::string::npos) {
                        pos = (int)commaPos + 1;
                    }
                    // Otherwise, do nothing.
                }
                // Accept visible characters except comma.
                else if ((key >= 32) && (key <= 126)) {
                    int segStart, segEnd;
                    GetSegmentBounds(segStart, segEnd);
                    if ((segEnd - segStart) == 1 && text[segStart] == '0') {
                        text.erase(segStart, 1);
                        if (pos > segStart) pos--;
                    }
                    text.insert(pos, 1, (char)key);
                    pos++;
                }
                key = GetCharPressed();
            }

            // Backspace: delete the character before caret.
            if (IsKeyPressed(KEY_BACKSPACE)) {
                if (pos > 0) {
                    if (text[pos - 1] == ',') {
                        // Prevent deleting a comma by resetting to default.
                        setDefault();
                    }
                    else {
                        text.erase(pos - 1, 1);
                        pos--;
                    }
                }
            }
            // Delete: remove character at the caret position.
            if (IsKeyPressed(KEY_DELETE)) {
                if (pos < (int)text.size()) {
                    if (text[pos] == ',') {
                        setDefault();
                    }
                    else {
                        text.erase(pos, 1);
                    }
                }
            }

            // Arrow keys for navigation.
            if (IsKeyPressed(KEY_LEFT)) {
                if (pos > 0)
                    pos--;
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                if (pos < (int)text.size())
                    pos++;
            }
            // Enter key to finalize input.
            if (IsKeyPressed(KEY_ENTER)) {
                outputMessage = text;
                focused = false;
                framesCounter = 0;
                isEnter = true;
                return;
            }
        }

        // Safety: if commas are missing, reset.
        if ((weighted && text.find(",") == std::string::npos) ||
            (!weighted && text.find(",") == std::string::npos))
        {
            setDefault();
        }

        framesCounter++;
    }

    void draw() {
        // Draw the textbox background and border.
        Color boxColor = focused ? YELLOW : Color{ 244, 162, 83, 210 };
        float roundness = 0.25f;
        int segments = 6;
        float outlineThickness = 2.0f;
        DrawRectangleRounded(bounds, roundness, segments, boxColor);
        DrawRectangleRoundedLinesEx(bounds, roundness, segments, outlineThickness, DARKGRAY);

        // Compose the final string with non-editable parentheses.
        std::string displayText = "(" + text + ")";
        // Measure the display text size.
        Vector2 textSize = MeasureTextEx(font, displayText.c_str(), fontSize, 1);
        // Center the display text inside the textbox.
        float textX = bounds.x + (bounds.width - textSize.x) / 2.0f;
        float textY = bounds.y + (bounds.height - textSize.y) / 2.0f;
        DrawTextEx(font, displayText.c_str(), { textX, textY }, fontSize, 1, textColor);

        // If focused, draw the blinking caret.
        if (focused && ((framesCounter / 20) % 2 == 0)) {
            // Build a string containing the non-editable "(" plus the text before the caret.
            std::string caretSubstr = "(" + text.substr(0, pos);
            // Measure its width.
            float caretOffset = MeasureTextEx(font, caretSubstr.c_str(), fontSize, 1).x;
            // The caret's x-position is the centered text start plus the offset.
            float caretX = textX + caretOffset;
            // Draw the caret as a vertical line.
            DrawLine((int)caretX, (int)textY, (int)caretX, (int)(textY + fontSize), BLACK);
        }
    }

};