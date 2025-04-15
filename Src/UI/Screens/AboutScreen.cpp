#include "AboutScreen.h"
#include "../../Constants.h"

void AboutScreen::Init() {
    // Khởi tạo nút "Back"
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Load font
    aboutFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(aboutFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    font = LoadFontEx("Assets/Fonts/PublicSans-bold.ttf", 30, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
}

void AboutScreen::Update(int& state) {
    Vector2 mouse = GetMousePosition();
}

std::vector<std::string> WrapText(const std::string& text, Font font, float maxWidth, int fontSize, float spacing) {
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

void AboutScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);

    // Tiêu đề "About"
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(aboutFont, "About", fontSize, spacing);
    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80;
    //DrawTextEx(aboutFont, "About", { titleX, titleY }, fontSize, spacing, DARKBLUE);

    // Cài đặt văn bản
    int textFontSize = 20;
    int subtitleFontSize = 26;
    float textSpacing = 1.5f;
    float padding = 30.0f;
    Color subtitleColor = Color{ 253, 111, 59, 255 };
    // Danh sách thành viên và thông tin lớp
    std::vector<std::string> teamMembers = {
        "- Pham Huu Nam (Student ID: 24125015) - Leader",
        "- Dang Tran Tuan Khoi (Student ID: 24125034) - UI Designer",
        "- Tran Dang Le Huy (Student ID: 24125057) - Developer",
        "- Vo Quoc Linh (Student ID: 24125065) - Developer"
    };
    std::string classInfo = "All members are from class 24A01, APCS K24, Falcuty of Information and Technology, University of Science, Ho Chi Minh City.";

    // Thông tin ứng dụng và liên hệ
    const char* teamInfo[] = {
        "The app enables users to work with data structures such as Linked Lists, AVL Trees, Hash Tables, and Graphs.",
        "Our goal is to create a user-friendly and intuitive tool that facilitates learning and interacting with algorithms",
        "and data structures.",
        "",
        "Currently, the app is being updated with additional data structures and various improvements. If you have any",
        "suggestions or feedback, please contact us at: khln.apcs@gmail.com"
    };
    int teamInfoCount = sizeof(teamInfo) / sizeof(teamInfo[0]);

    // Tính chiều rộng tối đa
    float maxWidth = 0;
    for (const auto& member : teamMembers) {
        Vector2 textSize = MeasureTextEx(font, member.c_str(), textFontSize, textSpacing);
        if (textSize.x > maxWidth) maxWidth = textSize.x;
    }
    Vector2 classInfoSize = MeasureTextEx(font, classInfo.c_str(), textFontSize, textSpacing);
    if (classInfoSize.x > maxWidth) maxWidth = classInfoSize.x;
    for (int i = 0; i < teamInfoCount; ++i) {
        Vector2 textSize = MeasureTextEx(font, teamInfo[i], textFontSize, textSpacing);
        if (textSize.x > maxWidth) maxWidth = textSize.x;
    }
    Vector2 subtitleSize = MeasureTextEx(aboutFont, "KHLN Team:", subtitleFontSize, textSpacing);
    if (subtitleSize.x > maxWidth) maxWidth = subtitleSize.x;
    subtitleSize = MeasureTextEx(aboutFont, "About the App:", subtitleFontSize, textSpacing);
    if (subtitleSize.x > maxWidth) maxWidth = subtitleSize.x;
    subtitleSize = MeasureTextEx(aboutFont, "Contact:", subtitleFontSize, textSpacing);
    if (subtitleSize.x > maxWidth) maxWidth = subtitleSize.x;

    // Xuống hàng cho teamInfo
    std::vector<std::vector<std::string>> wrappedTeamInfo;
    for (int i = 0; i < teamInfoCount; ++i) {
        if (teamInfo[i][0] != '\0') {
            wrappedTeamInfo.push_back(WrapText(teamInfo[i], font, maxWidth, textFontSize, textSpacing));
        }
        else {
            wrappedTeamInfo.push_back({ "" });
        }
    }

    // Tính tổng chiều cao
    float totalHeight = 0;
    float subtitleHeight = subtitleSize.y;

    // Phần 1: Development Team
    totalHeight += subtitleHeight + 10;
    for (const auto& member : teamMembers) {
        totalHeight += MeasureTextEx(font, member.c_str(), textFontSize, textSpacing).y + 5;
    }
    totalHeight += 10 + classInfoSize.y + 10;

    // Phần 2: About the App
    totalHeight += subtitleHeight + 10;
    for (int i = 0; i < 3; ++i) {
        for (const auto& line : wrappedTeamInfo[i]) {
            totalHeight += MeasureTextEx(font, line.c_str(), textFontSize, textSpacing).y + 5;
        }
        totalHeight += 10;
    }

    // Phần 3: Contact
    totalHeight += subtitleHeight + 10;
    for (int i = 4; i < 5; ++i) {
        for (const auto& line : wrappedTeamInfo[i]) {
            totalHeight += MeasureTextEx(font, line.c_str(), textFontSize, textSpacing).y + 5;
        }
    }

    // Vẽ hình chữ nhật bo góc làm nền
    float initialContentY = titleY + titleSize.y + 100;
    float rectX = (Screen_w - maxWidth) / 2 - padding;
    float rectY = initialContentY - padding;
    float rectWidth = maxWidth + 2 * padding;
    float rectHeight = totalHeight + 2 * padding;
    DrawRectangleRounded({ rectX, rectY, rectWidth, rectHeight }, 0.2f, 4, WHITE);

    // Vẽ nội dung
    float contentY = initialContentY;
    float x = (Screen_w - maxWidth) / 2;

    // Phần 1: Development Team
    DrawTextEx(aboutFont, "KHLN Team:", { x, contentY }, subtitleFontSize, textSpacing, subtitleColor);
    contentY += subtitleHeight + 10;
    for (const auto& member : teamMembers) {
        Vector2 textSize = MeasureTextEx(font, member.c_str(), textFontSize, textSpacing);
        DrawTextEx(font, member.c_str(), { x + 20, contentY }, textFontSize, textSpacing, DARKBLUE);
        contentY += textSize.y + 5;
    }
    contentY += 10;
    DrawTextEx(font, classInfo.c_str(), { x + 20, contentY }, textFontSize, textSpacing, DARKBLUE);
    contentY += classInfoSize.y + 10;

    // Phần 2: About the App
    DrawTextEx(aboutFont, "About the App:", { x, contentY }, subtitleFontSize, textSpacing, subtitleColor);
    contentY += subtitleHeight + 10;
    for (int i = 0; i < 3; ++i) {
        for (const auto& line : wrappedTeamInfo[i]) {
            Vector2 textSize = MeasureTextEx(font, line.c_str(), textFontSize, textSpacing);
            DrawTextEx(font, line.c_str(), { x, contentY }, textFontSize, textSpacing, DARKBLUE);
            contentY += textSize.y + 5;
        }
        contentY += 10;
    }

    // Phần 3: Contact
    DrawTextEx(aboutFont, "Contact:", { x, contentY }, subtitleFontSize, textSpacing, subtitleColor);
    contentY += subtitleHeight + 10;
    for (int i = 4; i <= 5; ++i) {
        for (const auto& line : wrappedTeamInfo[i]) {
            if (line.find("khln.apcs@gmail.com") != std::string::npos) {
                const char* textBefore = "suggestions or feedback, please contact us at: ";
                const char* emailText = "khln.apcs@gmail.com";
                Vector2 textBeforeSize = MeasureTextEx(font, textBefore, textFontSize, textSpacing);
                DrawTextEx(font, textBefore, { x, contentY }, textFontSize, textSpacing, DARKBLUE);
                DrawTextEx(font, emailText, { x + textBeforeSize.x, contentY }, textFontSize, textSpacing, RED);
                contentY += textBeforeSize.y + 5;
            }
            else {
                Vector2 textSize = MeasureTextEx(font, line.c_str(), textFontSize, textSpacing);
                DrawTextEx(font, line.c_str(), { x, contentY }, textFontSize, textSpacing, DARKBLUE);
                contentY += textSize.y + 5;
            }
        }
    }
}


void AboutScreen::Unload() {
    UnloadFont(aboutFont);
}
