#include "Adjust.h"
#include <iostream>

// Constants for toolbar dimensions and positions
const int TOOLBAR_WIDTH = 450;
const int TOOLBAR_HEIGHT = 80;
const int BUTTON_SIZE = 60;
const int BUTTON_SPACING = 70;
const int MENU_ICON_SIZE = 50;
const int SLIDE_INCREMENT = 10;
const float MIN_SPEED = 0.25f;
const float MAX_SPEED = 2.0f;
const float SPEED_INCREMENT = 0.25f;

// Colors for hover and selected states
const Color HOVER_COLOR = { 255, 255, 255, 128 }; // Light effect with transparency
const Color SELECTED_COLOR = { 0, 0, 0, 128 };    // Dark effect with transparency

Toolbar::Toolbar() {
    isOpen = false;
    isSpeedMenuOpen = false;
    slidePos = 0.0f;
    speed = 1.0f;
    selectedButtonIndex = -1; // No button selected initially

    menuIconRect = { 20, 500, MENU_ICON_SIZE, MENU_ICON_SIZE };
    toolbarRect = { -TOOLBAR_WIDTH, 490, TOOLBAR_WIDTH, TOOLBAR_HEIGHT };
    buttons[0] = { -420, 495, BUTTON_SIZE, BUTTON_SIZE }; // Back
    buttons[1] = { -350, 495, BUTTON_SIZE, BUTTON_SIZE }; // Pause
    buttons[2] = { -280, 495, BUTTON_SIZE, BUTTON_SIZE }; // Play
    buttons[3] = { -210, 495, BUTTON_SIZE, BUTTON_SIZE }; // Next
    buttons[4] = { -140, 495, BUTTON_SIZE, BUTTON_SIZE }; // Speed
    speedButtons[0] = { -140, 425, 40, 40 }; // Speed-
    speedButtons[1] = { -100, 425, 40, 40 }; // Speed+

    textures.resize(6);
    const char* textureFiles[6] = {
        "Assets/Toolbar/menu_light.png",
        "Toolbar/back_light.png",
        "Toolbar/pause_light.png",
        "Toolbar/play_light.png",
        "Toolbar/next_light.png",
        "Toolbar/speed_light.png"
    };

    for (int i = 0; i < 6; ++i) {
        textures[i] = LoadTexture(textureFiles[i]);
        if (textures[i].id == 0) {
            std::cerr << "Failed to load " << textureFiles[i] << std::endl;
        }
    }
}

Toolbar::~Toolbar() {
    for (auto& texture : textures) {
        UnloadTexture(texture);
    }
}

void Toolbar::Update() {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, menuIconRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
        if (!isOpen) isSpeedMenuOpen = false;
    }

    if (isOpen && slidePos < TOOLBAR_WIDTH + 20) slidePos += SLIDE_INCREMENT;
    if (!isOpen && slidePos > 0) slidePos -= SLIDE_INCREMENT;

    toolbarRect.x = -TOOLBAR_WIDTH + slidePos;
    buttons[0].x = 70 + slidePos - TOOLBAR_WIDTH;
    for (int i = 1; i < 5; i++) {
        buttons[i].x = buttons[0].x + i * BUTTON_SPACING;
    }
    speedButtons[0].x = buttons[4].x - 10;
    speedButtons[1].x = buttons[4].x + 30;

    // Check hover and click for toolbar buttons
    if (isOpen) {
        for (int i = 0; i < 5; i++) {
            if (CheckCollisionPointRec(mousePos, buttons[i])) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    if (selectedButtonIndex == i) {
                        selectedButtonIndex = -1; // Deselect if already selected
                    }
                    else {
                        selectedButtonIndex = i; // Select the button
                    }
                    if (i == 4) isSpeedMenuOpen = !isSpeedMenuOpen;
                }
            }
        }
    }

    // Handle speed menu
    if (isSpeedMenuOpen) {
        if (CheckCollisionPointRec(mousePos, speedButtons[0]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed > MIN_SPEED) {
            speed -= SPEED_INCREMENT;
        }
        if (CheckCollisionPointRec(mousePos, speedButtons[1]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed < MAX_SPEED) {
            speed += SPEED_INCREMENT;
        }
    }
}

void Toolbar::Draw() {
    Vector2 mousePos = GetMousePosition();
    DrawRectangleRounded({ toolbarRect.x + 5, toolbarRect.y + 5, toolbarRect.width, toolbarRect.height }, 0.2f, 10, { 0, 0, 0, 50 });
    DrawRectangleRounded(toolbarRect, 0.2f, 10, GRAY);

    // Draw buttons with hover and selected effects
    for (int i = 0; i < 5; i++) {
        bool isHovered = CheckCollisionPointRec(mousePos, buttons[i]);
        bool isSelected = (selectedButtonIndex == i);

        if (isSelected) {
            DrawCircle(buttons[i].x + BUTTON_SIZE / 2, buttons[i].y + BUTTON_SIZE / 2, BUTTON_SIZE / 2, SELECTED_COLOR); // Darker overlay when selected
        }
        else if (isHovered) {
            DrawCircle(buttons[i].x + BUTTON_SIZE / 2, buttons[i].y + BUTTON_SIZE / 2, BUTTON_SIZE / 2, HOVER_COLOR); // Lighter overlay when hovered
        }

        if (textures[i + 1].id != 0) {
            DrawTexturePro(textures[i + 1], { 0, 0, (float)textures[i + 1].width, (float)textures[i + 1].height },
                { (float)buttons[i].x, (float)buttons[i].y, (float)BUTTON_SIZE, (float)BUTTON_SIZE }, { 0, 0 }, 0, WHITE);
        }
    }

    DrawRectangleRec({ menuIconRect.x + 3, menuIconRect.y + 3, menuIconRect.width, menuIconRect.height }, { 0, 0, 0, 50 });
    if (textures[0].id != 0) {
        DrawTexturePro(textures[0], { 0, 0, (float)textures[0].width, (float)textures[0].height }, menuIconRect, { 0, 0 }, 0, WHITE);
    }
    else {
        DrawRectangleRec(menuIconRect, DARKGRAY);
    }

    if (isOpen) {
        for (int i = 0; i < 5; i++) {
            if (i == 4) {
                DrawText(TextFormat("%.2fx", speed), buttons[i].x + 5, buttons[i].y + 65, 20, BLACK);
            }
            else {
                DrawText(labels[i], buttons[i].x + (i == 1 ? 5 : 10), buttons[i].y + 65, 20, BLACK);
            }
        }
    }

    if (isSpeedMenuOpen) {
        DrawRectangleRounded({ speedButtons[0].x - 5, speedButtons[0].y - 5, 90, 50 }, 0.2f, 5, { 200, 200, 200, 255 });
        for (int i = 0; i < 2; i++) {
            bool isHovered = CheckCollisionPointRec(mousePos, speedButtons[i]);
            DrawRectangleRec({ speedButtons[i].x + 2, speedButtons[i].y + 2, speedButtons[i].width, speedButtons[i].height }, { 0, 0, 0, 50 });
            DrawRectangleRec(speedButtons[i], LIGHTGRAY);
            if (isHovered) {
                DrawRectangleRec(speedButtons[i], HOVER_COLOR);
            }
            DrawText(i == 0 ? "-" : "+", speedButtons[i].x + 12, speedButtons[i].y + 5, 30, BLACK);
        }
    }
}
