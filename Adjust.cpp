#include "Adjust.h"
#include <iostream>

Toolbar::Toolbar() {
    isOpen = false;
    isSpeedMenuOpen = false;
    slidePos = 0.0f;
    speed = 1.0f;

    // Bottom-left corner, offset từ cạnh (giả sử cửa sổ 800x600)
    menuIconRect = { 20, 500, 50, 50 }; // Menu icon cố định ở góc dưới trái
    toolbarRect = { -450, 490, 450, 80 }; // Toolbar rộng hơn, ẩn hoàn toàn khi chưa mở
    buttons[0] = { -420, 495, 60, 60 }; // Back
    buttons[1] = { -350, 495, 60, 60 }; // Pause
    buttons[2] = { -280, 495, 60, 60 }; // Play
    buttons[3] = { -210, 495, 60, 60 }; // Next
    buttons[4] = { -140, 495, 60, 60 }; // Speed
    speedButtons[0] = { -140, 425, 40, 40 }; // Speed-
    speedButtons[1] = { -100, 425, 40, 40 }; // Speed+

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
    // Kiểm tra nhấn vào menu icon để mở/đóng toolbar
    if (CheckCollisionPointRec(GetMousePosition(), menuIconRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isOpen = !isOpen;
        if (!isOpen) isSpeedMenuOpen = false; // Đóng speed menu khi toolbar đóng
    }

    // Hiệu ứng trượt: Đảm bảo nút Back thẳng hàng với menuIconRect khi mở
    if (isOpen && slidePos < 470) slidePos += 10; // Dịch đủ để nút Back nằm ngay sau menu icon
    if (!isOpen && slidePos > 0) slidePos -= 10;

    // Cập nhật vị trí toolbar và các nút
    toolbarRect.x = -450 + slidePos; // Toolbar dịch chuyển
    buttons[0].x = 70 + slidePos - 450; // Nút Back bắt đầu ngay sau menuIconRect (20 + 50 = 70)
    for (int i = 1; i < 5; i++) {
        buttons[i].x = buttons[0].x + i * 70; // Các nút khác cách đều 70px
    }
    speedButtons[0].x = buttons[4].x - 10; // Speed- căn sát nút Speed
    speedButtons[1].x = buttons[4].x + 30; // Speed+ căn đối xứng

    // Kiểm tra nhấn vào nút Speed để mở/đóng speed menu
    if (isOpen && CheckCollisionPointRec(GetMousePosition(), buttons[4]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isSpeedMenuOpen = !isSpeedMenuOpen;
    }

    // Xử lý speed menu
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
    // Vẽ bóng cho toolbar
    DrawRectangleRounded({ toolbarRect.x + 5, toolbarRect.y + 5, toolbarRect.width, toolbarRect.height }, 0.2f, 10, { 0, 0, 0, 50 });

    // Vẽ toolbar
    DrawRectangleRounded(toolbarRect, 0.2f, 10, GRAY);

    // Vẽ bóng cho các nút
    for (int i = 0; i < 5; i++) {
        DrawRectangleRec({ buttons[i].x + 3, buttons[i].y + 3, buttons[i].width, buttons[i].height }, { 0, 0, 0, 50 });
    }

    // Vẽ các icon nút nếu đã tải
    for (int i = 0; i < 5; i++) {
        if (textures[i + 1].id != 0) {
            DrawTexturePro(textures[i + 1], { 0, 0, (float)textures[i + 1].width, (float)textures[i + 1].height },
                buttons[i], { 0, 0 }, 0, WHITE);
        }
        else {
            Color colors[5] = { RED, YELLOW, GREEN, BLUE, PURPLE };
            DrawRectangleRec(buttons[i], colors[i]);
        }
    }

    // Vẽ bóng và icon cho menu
    DrawRectangleRec({ menuIconRect.x + 3, menuIconRect.y + 3, menuIconRect.width, menuIconRect.height }, { 0, 0, 0, 50 });
    if (textures[0].id != 0) {
        DrawTexturePro(textures[0], { 0, 0, (float)textures[0].width, (float)textures[0].height }, menuIconRect, { 0, 0 }, 0, WHITE);
    }
    else {
        DrawRectangleRec(menuIconRect, DARKGRAY);
    }

    // Vẽ nhãn dưới các nút với kích thước chữ lớn hơn
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

    // Vẽ speed sub-menu nếu đang mở - Cải thiện giao diện
    if (isSpeedMenuOpen) {
        // Vẽ khung nền cho speed menu
        DrawRectangleRounded({ speedButtons[0].x - 5, speedButtons[0].y - 5, 90, 50 }, 0.2f, 5, { 200, 200, 200, 255 });

        // Vẽ bóng và nút Speed- và Speed+
        DrawRectangleRec({ speedButtons[0].x + 2, speedButtons[0].y + 2, speedButtons[0].width, speedButtons[0].height }, { 0, 0, 0, 50 });
        DrawRectangleRec({ speedButtons[1].x + 2, speedButtons[1].y + 2, speedButtons[1].width, speedButtons[1].height }, { 0, 0, 0, 50 });
        DrawRectangleRec(speedButtons[0], LIGHTGRAY);
        DrawRectangleRec(speedButtons[1], LIGHTGRAY);
        DrawText("-", speedButtons[0].x + 12, speedButtons[0].y + 5, 30, BLACK);
        DrawText("+", speedButtons[1].x + 12, speedButtons[1].y + 5, 30, BLACK);
    }
}