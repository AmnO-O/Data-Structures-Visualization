#include "Menu.h"
#include "../Constants.h"

Menu::Menu() {
    // Khởi tạo nút "How to Use"
    helpButton = { 20, Screen_h - 60, 150, 40 };
    // Khởi tạo nút "About"
    aboutButton = { 20, Screen_h - 110, 150, 40 };
    // Khởi tạo nút "Settings"
    settingsButton = { 20, Screen_h - 160, 150, 40 };

    helpHovered = aboutHovered = settingsHovered = 0;

    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    int startX = 440;
    int startY = 200;

    for (int i = 0; i < 4; i++) {
        Item item;
        int row = i / 2;
        int col = i & 1;

        float x = startX + col * (Item_w + 50);
        float y = startY + row * (Item_h + 50);

        item.name = dsName[i];
        item.bounds = { x, y, Item_w, Item_h };
        item.hovered = 0;

        dsItem.push_back(item);
    }

    iconImage[0] = LoadImage("Assets/Images/SinglyLinkedList.png");
    iconImage[1] = LoadImage("Assets/Images/HashTable.png");
    iconImage[2] = LoadImage("Assets/Images/AVLTree.png");
    iconImage[3] = LoadImage("Assets/Images/Graph.png");

    iconList[0] = LoadTextureFromImage(iconImage[0]);
    iconList[1] = LoadTextureFromImage(iconImage[1]);
    iconList[2] = LoadTextureFromImage(iconImage[2]);
    iconList[3] = LoadTextureFromImage(iconImage[3]);


    // Làm mịn ảnh bằng bilinear filtering
    for (int i = 0; i < 4; i++) {
        SetTextureFilter(iconList[i], TEXTURE_FILTER_BILINEAR);
    }
}

void Menu::Init() {
    int startX = (Screen_w - 2 * Item_w - Padding) / 2;
    int startY = 350;

    int spacingX = 120;
    int spacingY = 100;

    // Load font from Assets
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    titleFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(titleFont.texture, TEXTURE_FILTER_BILINEAR);

    for (int i = 0; i < 4; i++) {
        Item item;
        int row = i / 2;
        int col = i & 1;

        float x = startX + col * (Item_w + spacingX);
        float y = startY + row * (Item_h + spacingY);

        item.name = dsName[i];
        item.bounds = { x, y, Item_w, Item_h };
        item.hovered = 0;

        dsItem.push_back(item);
    }

    // Khởi tạo nút "How to Use"
    helpButton = { 20, Screen_h - 60, 150, 40 };
    // Khởi tạo nút "About"
    aboutButton = { 20, Screen_h - 110, 150, 40 };
    // Khởi tạo nút "Settings"
    settingsButton = { 20, Screen_h - 160, 150, 40 };

    helpHovered = aboutHovered = settingsHovered = 0;
}

int Menu::handleEvent() {
    Vector2 mouse = GetMousePosition();

    for (int id = 0; id < 4; id++) {
        Item& item = dsItem[id];
        if (CheckCollisionPointRec(mouse, item.bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (id == 0) return LinkedList_state;
                if (id == 1) return HashTable_state;
                if (id == 2) return Trie_state;
                return Graph_state;
            }

            item.hovered = true;
        }
        else item.hovered = false;
    }

    // Xử lý hover cho "How to Use"
    helpHovered = CheckCollisionPointRec(mouse, helpButton);
    if (helpHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return HowToUse_state;
    }

    // Xử lý hover cho "About"
    aboutHovered = CheckCollisionPointRec(mouse, aboutButton);
    if (aboutHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return About_state;
    }

    // Xử lý hover cho "Settings"
    settingsHovered = CheckCollisionPointRec(mouse, settingsButton);
    if (settingsHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        return Settings_state;
    }

    return Menu_state;
}

Menu::~Menu() {
    UnloadFont(myFont);
}

void Menu::Draw() {
    for (Item item : dsItem) {
        Color boxColor = item.hovered ? LIGHTGRAY : RAYWHITE;

        DrawRectangleRounded(item.bounds, 0.2f, 4, boxColor);
        DrawRectangleRoundedLinesEx(item.bounds, 0.2f, 4, 2.0f, isDarkMode ? Color{ 244, 162, 88, 255 } : Color{ 103, 89, 246, 190 });

        float centerX = item.bounds.x + Item_w / 2;
        float centerY = item.bounds.y + Item_h / 2;

        int textWidth = MeasureTextEx(myFont, item.name.c_str(), 30, 1).x;
        DrawTextEx(myFont, item.name.c_str(), { centerX - textWidth / 2, item.bounds.y + Item_h - 40 }, 30, 1, isDarkMode ? Color{ 253, 111, 59, 255 } : DARKBLUE);
    }

    Color helpColor = helpHovered ? LIGHTGRAY : RAYWHITE;
    Color aboutColor = aboutHovered ? LIGHTGRAY : RAYWHITE;
    Color settingsColor = settingsHovered ? LIGHTGRAY : RAYWHITE;
    Color textColor = !isDarkMode ? DARKBLUE : Color{ 253, 111, 59, 255 };
    Color rounded = isDarkMode ? ORANGE : GRAY;

    // Vẽ nút "How to Use"
    DrawRectangleRounded(helpButton, 0.2f, 4, helpColor);
    DrawRectangleRoundedLinesEx(helpButton, 0.2f, 4, 2.0f, rounded);

    Vector2 textSize = MeasureTextEx(myFont, "How to Use", 20, 1);
    DrawTextEx(myFont, "How to Use",
        { helpButton.x + (helpButton.width - textSize.x) / 2, helpButton.y + (helpButton.height - textSize.y) / 2 },
        20, 1, textColor);
    // Vẽ nút "About"
    DrawRectangleRounded(aboutButton, 0.2f, 4, aboutColor);
    DrawRectangleRoundedLinesEx(aboutButton, 0.2f, 4, 2.0f, rounded);

    textSize = MeasureTextEx(myFont, "About", 20, 1);
    DrawTextEx(myFont, "About",
        { aboutButton.x + (aboutButton.width - textSize.x) / 2, aboutButton.y + (aboutButton.height - textSize.y) / 2 },
        20, 1, textColor);
    // Vẽ nút "Settings"
    DrawRectangleRounded(settingsButton, 0.2f, 4, settingsColor);
    DrawRectangleRoundedLinesEx(settingsButton, 0.2f, 4, 2.0f, rounded);

    textSize = MeasureTextEx(myFont, "Settings", 20, 1);
    DrawTextEx(myFont, "Settings",
        { settingsButton.x + (settingsButton.width - textSize.x) / 2, settingsButton.y + (settingsButton.height - textSize.y) / 2 },
        20, 1, textColor);

    for (int i = 0; i < 4; ++i) {
        Item item = dsItem[i];
        Texture2D icon = iconList[i];
        float iconSize;

        if (i == 0) iconSize = 300.0f;
        else if (i == 1) iconSize = 150.0f;
        else if (i == 2) iconSize = 190.0f;
        else if (i == 3) iconSize = 220.0f;

        // Scale ảnh để có kích thước đúng
        float scale = iconSize / std::max(icon.width, icon.height);

        // Tính lại width, height sau khi scale
        float drawWidth = icon.width * scale;
        float drawHeight = icon.height * scale;

        // Căn giữa icon trong bounds của item
        float iconX = item.bounds.x + (item.bounds.width - drawWidth) / 2;
        float iconY = item.bounds.y + (item.bounds.height - drawHeight) / 2 - 10.0f;
        if (i == 3) {
            iconX += 10.0f;
            iconY -= 10.0f;
        }
        else if (i == 2) {
            iconY -= 10.0f;
        }
        else if (i == 0) {
            iconX += 10.0f;
            iconY -= 5.0f;
        }

        DrawTextureEx(icon, { iconX, iconY }, 0.0f, scale, RAYWHITE);
    }


}