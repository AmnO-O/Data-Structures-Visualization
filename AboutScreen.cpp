#include "AboutScreen.h"
#include "Constants.h"

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

void AboutScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode); // Sử dụng màu nền theo chế độ hiện tại

    // Hiển thị tiêu đề "About"
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(aboutFont, "About", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    // Thêm nội dung về nhóm và ứng dụng
    int textFontSize = 25;
    float textSpacing = 1.5f;

    // Thông tin nhóm và nội dung ứng dụng
    // Lưu ý: mảng có 9 dòng, trong đó dòng cuối chứa email cần được tách riêng
    const char* teamInfo[] = {
        "This application was developed by the LNKH team, consisting of Pham Huu Nam, Dang Tran Tuan Khoi, Vo Quoc Linh,",
        "and Tran Dang Le Huy from class 24A01, APCS K24, University of Science, Ho Chi Minh City.",
        "",
        "The app enables users to work with data structures such as Linked Lists, AVL Trees, Hash Tables, and Graphs.",
        "Our goal is to create a user-friendly and intuitive tool that facilitates learning and interacting with algorithms",
        "and data structures.",
        "",
        "Currently, the app is being updated with additional data structures and various improvements. If you have any",
        "suggestions or feedback, please contact us at: lnkh.apcs24@gmail.com"
    };
    int teamInfoCount = sizeof(teamInfo) / sizeof(teamInfo[0]);

    // Tìm chiều rộng của dòng văn bản dài nhất
    float maxWidth = 0;
    for (int i = 0; i < teamInfoCount; ++i) {
        // Nếu là dòng chứa email (index 8), tách thành 2 phần
        if (i == 8) {
            const char* textBefore = "suggestions or feedback, please contact us at: ";
            const char* emailText = "lnkh.apcs24@gmail.com";
            Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
            Vector2 emailSize = MeasureTextEx(myFont, emailText, textFontSize, textSpacing);
            float totalWidth = textBeforeSize.x + emailSize.x;
            if (totalWidth > maxWidth) {
                maxWidth = totalWidth;
            }
        }
        else {
            Vector2 textSize = MeasureTextEx(myFont, teamInfo[i], textFontSize, textSpacing);
            if (textSize.x > maxWidth) {
                maxWidth = textSize.x;
            }
        }
    }

    float padding = 20.0f;
    float initialContentY = titleY + titleSize.y + 100;

    float totalTextHeight = 0;
    for (int i = 0; i < teamInfoCount; ++i) {
        float lineHeight;
        if (i == 8) {
            const char* textBefore = "suggestions or feedback, please contact us at: ";
            Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
            lineHeight = textBeforeSize.y;
        }
        else {
            Vector2 textSize = MeasureTextEx(myFont, teamInfo[i], textFontSize, textSpacing);
            lineHeight = textSize.y;
        }
        totalTextHeight += lineHeight + 10;
    }
    totalTextHeight -= 10; // Loại bỏ khoảng cách dư sau dòng cuối cùng

    float rectX = (Screen_w - maxWidth) / 2 - padding;
    float rectY = initialContentY - padding;
    float rectWidth = maxWidth + 2 * padding;
    float rectHeight = totalTextHeight + 2 * padding;

    DrawRectangleRounded({ rectX, rectY, rectWidth, rectHeight }, 0.2f, 4, WHITE);

    float contentY = initialContentY;
    for (int i = 0; i < teamInfoCount; ++i) {
        float lineHeight;
        float x = (Screen_w - maxWidth) / 2; // Căn giữa
        if (i == 8) {
            const char* textBefore = "suggestions or feedback, please contact us at: ";
            const char* emailText = "lnkh.apcs24@gmail.com";
            Vector2 textBeforeSize = MeasureTextEx(myFont, textBefore, textFontSize, textSpacing);
            DrawTextEx(myFont, textBefore, { x, contentY }, textFontSize, textSpacing, DARKBLUE);
            DrawTextEx(myFont, emailText, { x + textBeforeSize.x, contentY }, textFontSize, textSpacing, RED);
            lineHeight = textBeforeSize.y;
        }
        else {
            DrawTextEx(myFont, teamInfo[i], { x, contentY }, textFontSize, textSpacing, DARKBLUE);
            Vector2 textSize = MeasureTextEx(myFont, teamInfo[i], textFontSize, textSpacing);
            lineHeight = textSize.y;
        }
        contentY += lineHeight + 10; // Cập nhật vị trí y cho dòng tiếp theo
    }
}


void AboutScreen::Unload() {
    UnloadFont(aboutFont);
}
