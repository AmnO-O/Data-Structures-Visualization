#include "Helper.h"
#include "SettingScreen.h"
#include "../../Constants.h"


void HelperScreen::Init() {
    // Khởi tạo nút "Back"
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Load font
    helperFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(helperFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);
}

void HelperScreen::Update(int& state) {
    Vector2 mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Back"
    backHovered = CheckCollisionPointRec(mouse, backButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }
}
std::vector<std::string> HelperScreen::WrapText(const std::string& text, Font font, float maxWidth, int fontSize, float spacing) {
    std::vector<std::string> lines;
    std::string currentLine;
    std::string word;

    for (char c : text) {
        if (c == ' ') {
            Vector2 wordSize = MeasureTextEx(font, (currentLine + word).c_str(), fontSize, spacing);
            if (wordSize.x > maxWidth) {
                lines.push_back(currentLine);
                currentLine = word + " ";
            }
            else {
                currentLine += word + " ";
            }
            word.clear();
        }
        else {
            word += c;
        }
    }

    if (!word.empty()) {
        currentLine += word;
    }
    if (!currentLine.empty()) {
        lines.push_back(currentLine);
    }

    return lines;
}
void HelperScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);

    Color subtitleColor = Color{ 253, 111, 59, 255 };
    Color emailColor = RED;
    Color greenColor = DARKGREEN;

    const char* welcomeMessage = "Welcome to the RayViz Data Structure Visualization!";
    Vector2 welcomeSize = MeasureTextEx(myFont, welcomeMessage, 50, 1.5f);
    float welcomeX = (Screen_w - welcomeSize.x) / 2;
    float welcomeY = 80;

    const char* instructions[] = {
        "The app includes 4 main sections:",
        "Singly Linked List, Hash Table, AVL Tree, and Graph, each allows you to interact with ",
        "and visualize the structure in real time.",
        "Interface Overview:",
        "Center: Displays the selected structure and updates live with each action.",
        "Left Panel: Control buttons with Initialize, Add, Delete, Update, Search.",
        "Right Panel: Shows step-by-step algorithm operations for clear understanding.",
        "Bottom Toolbar: Adjust animation speed, switch between step-by-step or instant run.",
        "Graph Section Special Features:",
        "Drag and drop nodes to reposition freely.",
        "Click nodes/edges to add, remove, or move.",
        "Watch animated algorithm execution with visual cues.",
        "Other Features:",
        "Light/Dark Mode: Easily switch between light and dark themes for a comfortable viewing experience.",
        "Background Music: Turn music on/off to enhance your focus or mood while using the app.",
        "For further assistance, feel free to contact us at: khln.apcs@gmail.com"
    };
    int instructionsCount = sizeof(instructions) / sizeof(instructions[0]);

    int textFontSize = 20;
    float textSpacing = 1.5f;
    float padding = 30.0f;
    float maxWidth = 0;

    for (int i = 0; i < instructionsCount; ++i) {
        Vector2 textSize = MeasureTextEx(myFont, instructions[i], textFontSize, textSpacing);
        if (textSize.x > maxWidth) maxWidth = textSize.x;
    }

    std::vector<std::vector<std::string>> wrappedInstructions;
    for (int i = 0; i < instructionsCount; ++i) {
        if (instructions[i][0] != '\0') {
            wrappedInstructions.push_back(WrapText(instructions[i], myFont, maxWidth, textFontSize, textSpacing));
        }
        else {
            wrappedInstructions.push_back({ "" });
        }
    }

    float totalHeight = 0;
    for (const auto& paragraph : wrappedInstructions) {
        for (const auto& line : paragraph) {
            Vector2 textSize = MeasureTextEx(myFont, line.c_str(), textFontSize, textSpacing);
            totalHeight += textSize.y + 5;
        }
        totalHeight += 10;
    }

    float totalContentHeight = welcomeSize.y + 50 + totalHeight;
    float contentStartY = (Screen_h - totalContentHeight) / 2;

    welcomeY = contentStartY;
    DrawTextEx(myFont, welcomeMessage, { welcomeX, welcomeY }, 50, 1.5f, subtitleColor);

    float contentY = welcomeY + welcomeSize.y + 50;
    float x = (Screen_w - maxWidth) / 2;
    float rectX = x - padding;
    float rectY = contentY - padding;
    float rectWidth = maxWidth + 2 * padding;
    float rectHeight = totalHeight + 2 * padding;

    DrawRectangleRounded({ rectX, rectY, rectWidth, rectHeight }, 0.2f, 4, WHITE);

    for (const auto& paragraph : wrappedInstructions) {
        for (const auto& line : paragraph) {
            if (line == "Interface Overview:" || line == "Graph Section Special Features:" || line == "Other Features:") {
                DrawTextEx(myFont, line.c_str(), { x, contentY - 5 }, textFontSize + 10, textSpacing, subtitleColor);

            }
            else if (line.find("Center:") == 0 || line.find("Left Panel:") == 0 || line.find("Right Panel:") == 0 ||
                line.find("Bottom Toolbar:") == 0 || line.find("Light/Dark Mode:") == 0 || line.find("Background Music:") == 0) {
                std::string prefix;
                if (line.find("Center:") == 0) prefix = "Center:";
                else if (line.find("Left Panel:") == 0) prefix = "Left Panel:";
                else if (line.find("Right Panel:") == 0) prefix = "Right Panel:";
                else if (line.find("Bottom Toolbar:") == 0) prefix = "Bottom Toolbar:";
                else if (line.find("Light/Dark Mode:") == 0) prefix = "Light/Dark Mode:";
                else if (line.find("Background Music:") == 0) prefix = "Background Music:";

                std::string rest = line.substr(prefix.length());
                Vector2 prefixSize = MeasureTextEx(myFont, prefix.c_str(), textFontSize, textSpacing);
                DrawTextEx(myFont, prefix.c_str(), { x, contentY }, textFontSize, textSpacing, greenColor);
                DrawTextEx(myFont, rest.c_str(), { x + prefixSize.x, contentY }, textFontSize, textSpacing, DARKBLUE);
            }
            else if (line.find("Singly Linked List") != std::string::npos) {
                float currentX = x;
                const char* textBefore = "Singly Linked List";
                Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
                DrawTextEx(myFont, textBefore, { currentX, contentY }, textFontSize, textSpacing, greenColor);
                currentX += textBeforeSize.x;

                const char* textMiddle1 = ", ";
                Vector2 textMiddle1Size = MeasureTextEx(myFont, textMiddle1, textFontSize, textSpacing);
                DrawTextEx(myFont, textMiddle1, { currentX, contentY }, textFontSize, textSpacing, DARKBLUE);
                currentX += textMiddle1Size.x;

                const char* textMiddle2 = "Hash Table";
                Vector2 textMiddle2Size = MeasureTextEx(myFont, textMiddle2, textFontSize, textSpacing);
                DrawTextEx(myFont, textMiddle2, { currentX, contentY }, textFontSize, textSpacing, greenColor);
                currentX += textMiddle2Size.x;

                const char* textMiddle3 = ", ";
                Vector2 textMiddle3Size = MeasureTextEx(myFont, textMiddle3, textFontSize, textSpacing);
                DrawTextEx(myFont, textMiddle3, { currentX, contentY }, textFontSize, textSpacing, DARKBLUE);
                currentX += textMiddle3Size.x;

                const char* textMiddle4 = "AVL Tree";
                Vector2 textMiddle4Size = MeasureTextEx(myFont, textMiddle4, textFontSize, textSpacing);
                DrawTextEx(myFont, textMiddle4, { currentX, contentY }, textFontSize, textSpacing, greenColor);
                currentX += textMiddle4Size.x;

                const char* textMiddle5 = ", ";
                Vector2 textMiddle5Size = MeasureTextEx(myFont, textMiddle5, textFontSize, textSpacing);
                DrawTextEx(myFont, textMiddle5, { currentX, contentY }, textFontSize, textSpacing, DARKBLUE);
                currentX += textMiddle5Size.x;

                const char* textAfter = "Graph";
                Vector2 textAfterSize = MeasureTextEx(myFont, textAfter, textFontSize, textSpacing);
                DrawTextEx(myFont, textAfter, { currentX, contentY }, textFontSize, textSpacing, greenColor);
                currentX += textAfterSize.x;

                const char* textFinal = ", each allows you to interact with";
                Vector2 textFinalSize = MeasureTextEx(myFont, textFinal, textFontSize, textSpacing);
                DrawTextEx(myFont, textFinal, { currentX, contentY }, textFontSize, textSpacing, DARKBLUE);
                contentY += textBeforeSize.y + 5;

                const char* nextLine = "and visualize the structure in real time.";
                Vector2 nextLineSize = MeasureTextEx(myFont, nextLine, textFontSize, textSpacing);
                DrawTextEx(myFont, nextLine, { x, contentY }, textFontSize, textSpacing, DARKBLUE);
                contentY += nextLineSize.y + 5;
                continue;
            }
            else if (line.find("and visualize the structure in real time.") != std::string::npos) {
                continue;
            }
            else if (line.find("khln.apcs@gmail.com") != std::string::npos) {
                const char* textBefore = "For further assistance, feel free to contact us at: ";
                const char* emailText = "khln.apcs@gmail.com";
                Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
                DrawTextEx(myFont, textBefore, { x, contentY }, textFontSize, textSpacing, DARKBLUE);
                DrawTextEx(myFont, emailText, { x + textBeforeSize.x, contentY }, textFontSize, textSpacing, emailColor);
                contentY += textBeforeSize.y + 5;
            }
            else {
                DrawTextEx(myFont, line.c_str(), { x, contentY }, textFontSize, textSpacing, DARKBLUE);
            }
            Vector2 textSize = MeasureTextEx(myFont, line.c_str(), textFontSize, textSpacing);
            contentY += textSize.y + 5;
        }
        contentY += 10;
    }

    Color backColor = backHovered ? LIGHTGRAY : RAYWHITE;
    Vector2 textSize = MeasureTextEx(myFont, "<< Back to Menu", 20, 1);
    float backButtonX = (Screen_w - backButton.width) / 2;
    backButton.x = backButtonX;
    DrawRectangleRounded(backButton, 0.2f, 4, backColor);
    DrawRectangleRoundedLinesEx(backButton, 0.2f, 4, 2.0f, GRAY);
    DrawTextEx(myFont, "<< Back to Menu",
        { backButtonX + (backButton.width - textSize.x) / 2, backButton.y + (backButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);
}

void HelperScreen::Unload() {
    UnloadFont(helperFont);
}
