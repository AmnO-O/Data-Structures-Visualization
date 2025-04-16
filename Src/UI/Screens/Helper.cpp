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
    ClearBackground(isDarkMode ? darkmode : lightmode); // Sử dụng màu nền theo chế độ hiện tại

    // Hiển thị tiêu đề "How to Use"
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(helperFont, "How to Use", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    Color titleColor = isDarkMode ? WHITE : DARKBLUE;
    Color subtitleColor = Color{ 253, 111, 59, 255 }; // Highlight color for subtitles
    Color emailColor = RED;                          // Highlight color for email

    DrawTextEx(helperFont, "How to Use",
        { titleX, titleY }, fontSize, spacing, titleColor);

    // Highlight and center the welcome message
    const char* welcomeMessage = "Welcome to the RayViz Data Structure Visualization!";
    Vector2 welcomeSize = MeasureTextEx(myFont, welcomeMessage, 30, 1.5f); // Adjust font size and spacing
    float welcomeX = (Screen_w - welcomeSize.x) / 2; // Center horizontally
    float welcomeY = titleY + titleSize.y + 30;      // Adjusted position to move it upward
    DrawTextEx(myFont, welcomeMessage, { welcomeX, welcomeY }, 30, 1.5f, subtitleColor); // Highlighted in subtitle color

    // Nội dung hướng dẫn
    const char* instructions[] = {
        "The app includes 4 main sections:",
        "Singly Linked List, Hash Table, AVL Tree, and Graph, each allows you to interact with ",
        "and visualize the structure in real time.",
        "Interface Overview:",
        "Center: Displays the selected structure and updates live with each action.",
        "Left Panel: Control buttons – Initialize, Add, Delete, Update, Search.",
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

    // Tính toán chiều rộng tối đa
    int textFontSize = 20;
    float textSpacing = 1.5f;
    float padding = 30.0f;
    float maxWidth = 0;

    for (int i = 0; i < instructionsCount; ++i) {
        Vector2 textSize = MeasureTextEx(myFont, instructions[i], textFontSize, textSpacing);
        if (textSize.x > maxWidth) maxWidth = textSize.x;
    }

    // Xuống hàng cho nội dung
    std::vector<std::vector<std::string>> wrappedInstructions;
    for (int i = 0; i < instructionsCount; ++i) {
        if (instructions[i][0] != '\0') {
            wrappedInstructions.push_back(WrapText(instructions[i], myFont, maxWidth, textFontSize, textSpacing));
        }
        else {
            wrappedInstructions.push_back({ "" });
        }
    }

    // Tính tổng chiều cao của nội dung
    float totalHeight = 0;
    for (const auto& paragraph : wrappedInstructions) {
        for (const auto& line : paragraph) {
            Vector2 textSize = MeasureTextEx(myFont, line.c_str(), textFontSize, textSpacing);
            totalHeight += textSize.y + 5;
        }
        totalHeight += 10; // Khoảng cách giữa các đoạn
    }

    // Vẽ hình chữ nhật bo góc làm nền
    float contentY = welcomeY + welcomeSize.y + 50; // Start below the welcome message
    float x = (Screen_w - maxWidth) / 2;
    float rectX = x - padding;
    float rectY = contentY - padding;
    float rectWidth = maxWidth + 2 * padding;
    float rectHeight = totalHeight + 2 * padding;

    DrawRectangleRounded({ rectX, rectY, rectWidth, rectHeight }, 0.2f, 4, WHITE);

    // Vẽ nội dung
    for (const auto& paragraph : wrappedInstructions) {
        for (const auto& line : paragraph) {
            // Highlight specific sections
            if (line == "Interface Overview:" || line == "Graph Section Special Features:" || line == "Other Features:") {
                DrawTextEx(myFont, line.c_str(), { x, contentY }, textFontSize, textSpacing, subtitleColor); // Highlight subtitles
            }
            else if (line.find("khln.apcs@gmail.com") != std::string::npos) {
                const char* textBefore = "For further assistance, feel free to contact us at: ";
                const char* emailText = "khln.apcs@gmail.com";
                Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
                DrawTextEx(myFont, textBefore, { x, contentY }, textFontSize, textSpacing, DARKBLUE);
                DrawTextEx(myFont, emailText, { x + textBeforeSize.x, contentY }, textFontSize, textSpacing, emailColor); // Highlight email
                contentY += textBeforeSize.y + 5;
            }
            else {
                DrawTextEx(myFont, line.c_str(), { x, contentY }, textFontSize, textSpacing, DARKBLUE);
            }
            Vector2 textSize = MeasureTextEx(myFont, line.c_str(), textFontSize, textSpacing);
            contentY += textSize.y + 5;
        }
        contentY += 10; // Khoảng cách giữa các đoạn
    }

    // Vẽ nút "Back"
    Color backColor = backHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(backButton, 0.2f, 4, backColor);
    DrawRectangleRoundedLinesEx(backButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 textSize = MeasureTextEx(myFont, "<< Back to Menu", 20, 1);
    DrawTextEx(myFont, "<< Back to Menu",
        { backButton.x + (backButton.width - textSize.x) / 2, backButton.y + (backButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);
}




void HelperScreen::Unload() {
    UnloadFont(helperFont);
}
