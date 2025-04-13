#include "Adjust.h"
#include "../../Constants.h"
#include <iostream>

const int TOOLBAR_WIDTH = 450;
const int TOOLBAR_HEIGHT = 80;
const int BUTTON_SIZE = 60;
const int BUTTON_SPACING = 70;
const int MENU_ICON_SIZE = 50;
const int SLIDE_INCREMENT = 20;
const float MIN_SPEED = 0.25f;
const float MAX_SPEED = 4.0f;
const float SPEED_INCREMENT = 0.25f;
const float DURATION_INCREMENT = 0.09f;


const Color HOVER_COLOR = { 255, 255, 255, 128 };
const Color SELECTED_COLOR = { 0, 0, 0, 128 };
const Color CREAM_COLOR = { 255, 253, 208, 255 };
const Color CREAMY = { 214, 227, 242, 255 };

Toolbar::Toolbar() {
    isOpen = false;
    slidePos = 0.0f;
    speed = 1.0f;
    selectedButtonIndex = -1;
    duration = 0.6f;
    isAVL = false;

    menuIconRect = { 20, 750, MENU_ICON_SIZE, MENU_ICON_SIZE };
    toolbarRect = { -TOOLBAR_WIDTH, 740, TOOLBAR_WIDTH, TOOLBAR_HEIGHT };
    buttons[0] = { -420, 745, BUTTON_SIZE, BUTTON_SIZE }; // Back
    buttons[1] = { -350, 745, BUTTON_SIZE, BUTTON_SIZE }; // Play/Pause
    buttons[2] = { -280, 745, BUTTON_SIZE, BUTTON_SIZE }; // Next
    speedButtons[0] = { -140, 755, 40, 40 }; // Speed-
    speedButtons[1] = { -40, 755, 40, 40 }; // Speed+
    /// 2, 3, 8, 9, 
    textures.resize(16);
    const char* textureFiles[16] = {
        "Assets/Toolbar/menu_light.png",
        "Assets/Toolbar/back_light.png",
        "Assets/Toolbar/play_light.png",
        "Assets/Toolbar/pause_light.png",
        "Assets/Toolbar/next_light.png",
        "Assets/Toolbar/speed_light.png",
        "Assets/Toolbar/menu_dark.png",
        "Assets/Toolbar/back_dark.png",
        "Assets/Toolbar/play_dark.png",
        "Assets/Toolbar/pause_dark.png",
        "Assets/Toolbar/next_dark.png",
        "Assets/Toolbar/speed_dark.png",
        "Assets/Toolbar/add_light.png",
        "Assets/Toolbar/sub_light.png",
        "Assets/Toolbar/add_dark.png",
        "Assets/Toolbar/sub_dark.png"
    };

    for (int i = 0; i < 16; ++i) {
        textures[i] = LoadTexture(textureFiles[i]);
        if (textures[i].id == 0) {
            std::cerr << "Failed to load " << textureFiles[i] << std::endl;
        }
    }

    font = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
}

Toolbar::~Toolbar() {
    for (auto& texture : textures) {
        UnloadTexture(texture);
    }

    UnloadFont(font);
}

void Toolbar::Update() {
    isBack = isNext = false;
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, menuIconRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
    }

    if (isOpen && slidePos < TOOLBAR_WIDTH) slidePos += SLIDE_INCREMENT;
    if (!isOpen && slidePos > 0) slidePos -= SLIDE_INCREMENT;

    toolbarRect.x = -TOOLBAR_WIDTH + slidePos;
    buttons[0].x = 70 + slidePos - TOOLBAR_WIDTH;
    for (int i = 1; i < 3; i++) {
        buttons[i].x = buttons[0].x + i * BUTTON_SPACING;
    }
    speedButtons[0].x = buttons[2].x + BUTTON_SPACING;
    speedButtons[1].x = speedButtons[0].x + 120;

    if (isOpen) {
        for (int i = 0; i < 3; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (selectedButtonIndex == i) {
                    selectedButtonIndex = -1;
                }
                else {
                    selectedButtonIndex = i;
                }
                if (i == 1) {
                    isPlaying = !isPlaying;
                }

                if (i == 0)
                    isBack = true;

                if (i == 2)
                    isNext = true;
            }
        }
    }


    if (CheckCollisionPointRec(mousePos, speedButtons[0]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed > MIN_SPEED) {
        speed -= SPEED_INCREMENT;
        duration += DURATION_INCREMENT;
    }
    float maxspeed = MAX_SPEED;
    if (isAVL == true) maxspeed = 2.5f;
    if (CheckCollisionPointRec(mousePos, speedButtons[1]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed < maxspeed) {
        speed += SPEED_INCREMENT;
        duration -= DURATION_INCREMENT;
    }
}

void Toolbar::Draw() {

    Vector2 mousePos = GetMousePosition();
    Color backgroundColor = isDarkMode ? CREAMY : CREAM_COLOR;
    Color hoverColor = isDarkMode ? SELECTED_COLOR : HOVER_COLOR;
    Color selectedColor = isDarkMode ? HOVER_COLOR : SELECTED_COLOR;

    DrawRectangleRounded({ toolbarRect.x + 5, toolbarRect.y + 5, toolbarRect.width, toolbarRect.height }, 0.2f, 10, { 0, 0, 0, 50 });
    DrawRectangleRounded(toolbarRect, 0.2f, 10, backgroundColor);

    for (int i = 0; i < 3; i++) {
        bool isHovered = CheckCollisionPointRec(mousePos, buttons[i]);
        bool isSelected = (selectedButtonIndex == i);

        if (isSelected) {
            DrawCircle(buttons[i].x + BUTTON_SIZE / 2, buttons[i].y + BUTTON_SIZE / 2, BUTTON_SIZE / 2, selectedColor);
        }
        else if (isHovered) {
            DrawCircle(buttons[i].x + BUTTON_SIZE / 2, buttons[i].y + BUTTON_SIZE / 2, BUTTON_SIZE / 2, hoverColor);
            DrawCircle(buttons[i].x + BUTTON_SIZE / 2, buttons[i].y + BUTTON_SIZE / 2, BUTTON_SIZE / 2 + 10, { 255, 255, 255, 100 });
        }

        int textureIndex;
        if (i == 0) textureIndex = isDarkMode ? 7 : 1; // Back
        else if (i == 1) textureIndex = (isDarkMode ? 8 : 2) + isPlaying; // Play/Pause
        else textureIndex = isDarkMode ? 10 : 4; // Next

        if (textures[textureIndex].id != 0) {
            DrawTexturePro(textures[textureIndex], { 0, 0, (float)textures[textureIndex].width, (float)textures[textureIndex].height },
                { (float)buttons[i].x, (float)buttons[i].y, (float)BUTTON_SIZE, (float)BUTTON_SIZE }, { 0, 0 }, 0, WHITE);
        }
    }

    bool isMenuIconHovered = CheckCollisionPointRec(mousePos, menuIconRect);
    if (isMenuIconHovered) {
        DrawCircle(menuIconRect.x + MENU_ICON_SIZE / 2, menuIconRect.y + MENU_ICON_SIZE / 2, MENU_ICON_SIZE / 2 + 10, { 255, 255, 255, 100 });
    }
    int menuIconIndex = isDarkMode ? 6 : 0;
    DrawRectangleRec({ menuIconRect.x + 3, menuIconRect.y + 3, menuIconRect.width, menuIconRect.height }, { 0, 0, 0, 50 });
    if (textures[menuIconIndex].id != 0) {
        DrawTexturePro(textures[menuIconIndex], { 0, 0, (float)textures[menuIconIndex].width, (float)textures[menuIconIndex].height }, menuIconRect, { 0, 0 }, 0, WHITE);
    }
    else {
        DrawRectangleRec(menuIconRect, backgroundColor);
    }

    for (int i = 0; i < 2; i++) {
        bool isHovered = CheckCollisionPointRec(mousePos, speedButtons[i]);
        if (isHovered) {
            DrawCircle(speedButtons[i].x + speedButtons[i].width / 2, speedButtons[i].y + speedButtons[i].height / 2, speedButtons[i].width / 2 + 10, { 255, 255, 255, 100 });
        }

        int textureIndex = isDarkMode ? (i == 0 ? 15 : 14) : (i == 0 ? 13 : 12);
        if (textures[textureIndex].id != 0) {
            DrawTexturePro(textures[textureIndex], { 0, 0, (float)textures[textureIndex].width, (float)textures[textureIndex].height },
                { (float)speedButtons[i].x, (float)speedButtons[i].y, (float)speedButtons[i].width, (float)speedButtons[i].height }, { 0, 0 }, 0, WHITE);
        }
    }
    DrawTextEx(font, TextFormat("%.2fx", speed), { (speedButtons[0].x + speedButtons[1].x) / 2, speedButtons[0].y + 10 }, 20, 1, BLACK);
}