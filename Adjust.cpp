#include "Adjust.h"
#include <iostream>

Toolbar::Toolbar() {
    isOpen = false;
    isSpeedMenuOpen = false;
    slidePos = 0.0f;
    speed = 1.0f;
    // Bottom-left corner, offset from edge (assuming 800x600 window)
    menuIconRect = { 20, 550, 30, 30 };
    toolbarRect = { -300, 540, 300, 50 };
    buttons[0] = { -280, 545, 40, 40 }; // Back
    buttons[1] = { -230, 545, 40, 40 }; // Pause
    buttons[2] = { -180, 545, 40, 40 }; // Play
    buttons[3] = { -130, 545, 40, 40 }; // Next
    buttons[4] = { -80, 545, 40, 40 };  // Speed
    speedButtons[0] = { -80, 495, 20, 20 }; // Speed-
    speedButtons[1] = { -60, 495, 20, 20 }; // Speed+

    // Load PNG icons into vector
    textures.resize(6); // 6 textures: menu, back, pause, play, next, speed
    textures[0] = LoadTexture("Assets/Toolbar/menu_light.png");
    if (textures[0].id == 0) std::cout << "Failed to load Toolbar/menu.png" << std::endl;
    textures[1] = LoadTexture("Assets/Toolbar/back_light.png");
    if (textures[1].id == 0) std::cout << "Failed to load Toolbar/back.png" << std::endl;
    textures[2] = LoadTexture("Assets/Toolbar/pause_light.png");
    if (textures[2].id == 0) std::cout << "Failed to load Toolbar/pause.png" << std::endl;
    textures[3] = LoadTexture("Assets/Toolbar/play_light.png");
    if (textures[3].id == 0) std::cout << "Failed to load Toolbar/play.png" << std::endl;
    textures[4] = LoadTexture("Assets/Toolbar/next_light.png");
    if (textures[4].id == 0) std::cout << "Failed to load Toolbar/next.png" << std::endl;
    textures[5] = LoadTexture("Assets/Toolbar/speed_light.png");
    if (textures[5].id == 0) std::cout << "Failed to load Toolbar/speed.png" << std::endl;
}

Toolbar::~Toolbar() {
    for (auto& texture : textures) {
        UnloadTexture(texture);
    }
}

void Toolbar::Update() {
    if (CheckCollisionPointRec(GetMousePosition(), menuIconRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
        if (!isOpen) isSpeedMenuOpen = false; // Close speed menu when toolbar closes
    }

    if (isOpen && slidePos < 300) slidePos += 10;
    if (!isOpen && slidePos > 0) slidePos -= 10;

    toolbarRect.x = -300 + slidePos;
    for (int i = 0; i < 5; i++) buttons[i].x = -280 + slidePos + i * 50;
    speedButtons[0].x = -80 + slidePos;
    speedButtons[1].x = -60 + slidePos;

    if (isOpen && CheckCollisionPointRec(GetMousePosition(), buttons[4]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isSpeedMenuOpen = !isSpeedMenuOpen;
    }

    if (isSpeedMenuOpen) {
        if (CheckCollisionPointRec(GetMousePosition(), speedButtons[0]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed > 0.25f) {
            speed -= 0.25f;
        }
        if (CheckCollisionPointRec(GetMousePosition(), speedButtons[1]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && speed < 2.0f) {
            speed += 0.25f;
        }
    }
}

void Toolbar::Draw() {
    // Draw shadow for toolbar
    DrawRectangleRounded({ toolbarRect.x + 5, toolbarRect.y + 5, toolbarRect.width, toolbarRect.height }, 0.2f, 10, { 0, 0, 0, 50 });

    // Draw toolbar
    DrawRectangleRounded(toolbarRect, 0.2f, 10, GRAY);

    // Draw shadows for buttons
    for (int i = 0; i < 5; i++) {
        DrawRectangleRec({ buttons[i].x + 3, buttons[i].y + 3, buttons[i].width, buttons[i].height }, { 0, 0, 0, 50 });
    }

    // Draw button icons if loaded
    for (int i = 0; i < 5; i++) {
        if (textures[i + 1].id != 0) {
            DrawTexturePro(textures[i + 1], { 0, 0, (float)textures[i + 1].width, (float)textures[i + 1].height }, buttons[i], { 0, 0 }, 0, WHITE);
        }
        else {
            Color colors[5] = { RED, YELLOW, GREEN, BLUE, PURPLE };
            DrawRectangleRec(buttons[i], colors[i]);
        }
    }

    // Draw menu icon shadow and icon
    DrawRectangleRec({ menuIconRect.x + 3, menuIconRect.y + 3, menuIconRect.width, menuIconRect.height }, { 0, 0, 0, 50 });
    if (textures[0].id != 0) {
        DrawTexturePro(textures[0], { 0, 0, (float)textures[0].width, (float)textures[0].height }, menuIconRect, { 0, 0 }, 0, WHITE);
    }
    else {
        DrawRectangleRec(menuIconRect, DARKGRAY);
    }

    // Draw labels below buttons
    if (isOpen) {
        for (int i = 0; i < 5; i++) {
            if (i == 4) {
                DrawText(TextFormat("%.2fx", speed), buttons[i].x - 5, buttons[i].y + 45, 10, BLACK);
            }
            else {
                DrawText(labels[i], buttons[i].x + (i == 1 ? 2 : 5), buttons[i].y + 45, 10, BLACK);
            }
        }
    }

    // Draw speed sub-menu if open
    if (isSpeedMenuOpen) {
        DrawRectangleRounded({ speedButtons[0].x, speedButtons[0].y, 40, 20 }, 0.2f, 5, GRAY);
        DrawRectangleRec({ speedButtons[0].x + 3, speedButtons[0].y + 3, speedButtons[0].width, speedButtons[0].height }, { 0, 0, 0, 50 });
        DrawRectangleRec({ speedButtons[1].x + 3, speedButtons[1].y + 3, speedButtons[1].width, speedButtons[1].height }, { 0, 0, 0, 50 });
        DrawText("-", speedButtons[0].x + 5, speedButtons[0].y, 20, BLACK);
        DrawText("+", speedButtons[1].x + 5, speedButtons[1].y, 20, BLACK);
    }
}
