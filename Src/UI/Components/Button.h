#pragma once
#include "../../Constants.h"
#include "../../External/tinyfiledialogs.h"
#include <sstream>
#include <fstream>

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
    bool isClick = false;


    // Hàm Update() kiểm tra di chuột, bấm chuột...
    // Trả về true nếu người dùng "click" (nhả chuột trong vùng nút)
    bool update() {
        Vector2 mousePos = GetMousePosition();
        isHovered = CheckCollisionPointRec(mousePos, rect);
        isClick = false;

        if (isHovered) {
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                isPressed = true;
            }
            else {
                isPressed = false;
            }
            // Nếu thả chuột ra (sau khi hover), tính là "click"
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                if (isChose == 0) isChose = !isChose;
                isClick = true;

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

        if (isClick)
            DrawRectangleRoundedLinesEx(rect, 0.3f, 6, 4.0f, GRAY);

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
    bool update();
    void draw(Font font);

    bool isCreate = true;
    bool isInsert = false;
    bool isRemove = false;
    bool isSearch = false;
    bool hide = false; 
    vector <Button> cButton;

private:
    Color btnNormal = RAYWHITE;
    Color btnHover = LIGHTGRAY;
    Color btnClick = LIGHTGRAY;

    Rectangle rect;
    Color color;
};

class GraphButton {
public:
    GraphButton();
    bool update();
    void draw(Font font);

    bool isCreate = true;
    bool isAddedge = false;
    bool isRemove = false;
    bool isMst = true;
    bool hide = false; 
    vector <Button> cButton;

private:
    Color btnNormal = RAYWHITE;
    Color btnHover = LIGHTGRAY;
    Color btnClick = LIGHTGRAY;

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
        modalX = (Screen_w - width) / 2.0f;
        modalY = (Screen_h - height) / 2.0f;

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

        // Draw the overlay and modal background
        DrawRectangle(0, 0, Screen_w, Screen_h, overlayColor);
        Rectangle modalRect = { modalX, modalY, width, height };
        DrawRectangleRounded(modalRect, 0.1f, 4, backgroundColor);
        DrawRectangleRoundedLines(modalRect, 0.1f, 4, borderColor);

        // Draw the title
        Vector2 titleSize = MeasureTextEx(font, title.c_str(), 22, textSpacing);
        float titleX = modalX + (width - titleSize.x) / 2.0f;
        float titleY = modalY + 20.0f;
        DrawTextEx(font, title.c_str(), Vector2{ titleX, titleY },
            22, textSpacing, !isDarkMode ? DARKPURPLE : Color{ 255, 0, 0, 255 });

        // Split the message into lines
        std::vector<std::string> lines;
        std::string line;
        std::istringstream iss(message);

        while (getline(iss, line)) {
            lines.push_back(line);
        }

        // Render each line with appropriate styling
        bool inExample = false;
        float currentY = titleY + titleSize.y + 10.0f;
        for (const auto& line : lines) {
            if (line.find("Example:") != std::string::npos) {
                currentY += 10.0f;
                DrawTextEx(font, line.c_str(), Vector2{ modalX + 10.0f, currentY }, 20, textSpacing, isDarkMode ? Color{ 255, 0, 0, 255 } : DARKPURPLE);
                inExample = true;
            }
            else if (inExample) {
                string indentedLine = "    " + line;
                DrawTextEx(font, indentedLine.c_str(), Vector2{ modalX + 10.0f, currentY }, 20, textSpacing, textColor);
            }
            else {
                DrawTextEx(font, line.c_str(), Vector2{ modalX + 10.0f, currentY }, 20, textSpacing, textColor);
            }
            Vector2 lineSize = MeasureTextEx(font, (inExample && line.find("Example:") == std::string::npos ? ("    " + line).c_str() : line.c_str()), 20, textSpacing);
            currentY += lineSize.y + 3.0f;
        }

        okRect.draw(font);

        return false;
    }
};

class FileLoader {
public:
    // Default folder for file dialog (e.g. "Input/")
    string defaultPath = "Input/";

    // Constructor: you can pass a custom default folder, otherwise "Input/" is used.
    FileLoader(const std::string& path = "Input/") : defaultPath(path) {}

    // Loads file data into a vector of unsigned char.
    // Returns an empty vector if loading failed or if user cancels.
    std::vector<unsigned char> loadFile() {
        // Open file dialog with the default folder.
        const char* filePath = tinyfd_openFileDialog(
            "Select file",      // Dialog title
            defaultPath.c_str(),       // Default folder
            0,                         // Number of filter patterns (0 = no filter)
            nullptr,                   // Filter patterns array
            nullptr,                   // Description of filter
            0                          // 0: allow only one file selection
        );

        // If user cancels, return an empty vector.
        if (!filePath) {
            return std::vector<unsigned char>();
        }

        // Load binary file data.
        int bytesRead = 0;  // For raylib, this can be int or unsigned int based on version.
        unsigned char* fileData = LoadFileData(filePath, &bytesRead);

        std::vector<unsigned char> data;
        if (fileData) {
            //std::cout << "Đã load file: " << filePath << "\n";
            //std::cout << "Kích thước: " << bytesRead << " bytes\n";

            // Copy fileData into the vector.
            data.assign(fileData, fileData + bytesRead);
            UnloadFileData(fileData);
        }
        else {
            std::cout << "Can't load file from " << filePath << std::endl;
        }
        return data;
    }

    vector<string> loadFileLines() {
        // Open file dialog with the default folder.
        const char* filePath = tinyfd_openFileDialog(
            "Select file",      // Dialog title
            defaultPath.c_str(), // Default folder
            0,                  // Number of filter patterns (0 = no filter)
            nullptr,            // Filter patterns array
            nullptr,            // Description of filter
            0                   // 0: allow only one file selection
        );

        if (!filePath) {
            return std::vector<string>();
        }

        ifstream file(filePath);
        if (!file.is_open()) {
            std::cout << "Can't open file: " << filePath << std::endl;
            return std::vector<std::string>();
        }

        vector<string> lines;
        string line;

        while (getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return lines;
    }
};