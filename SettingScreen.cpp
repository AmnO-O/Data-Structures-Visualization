#include "SettingScreen.h"

bool isDarkMode = false; // Mặc định là Light Mode
bool backHovered = false;
bool themeHovered = false;
bool soundHovered = false; // Mặc định sound 

bool isMusicOn = true; // Mặc định bật nhạc

Rectangle backButton; // Nút "Back"
Rectangle themeButton; // Nút "Theme"
Font settingsFont;    // Font chữ cho màn hình Settings
Font myFont;   // Font nhỏ 
Rectangle soundButton; // Nút âm thanh

Music backgroundMusic;

void SettingScreen::Init() {
    // Khởi tạo nút "Back"
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Khởi tạo nút "Theme"
    themeButton = { (Screen_w - 200) / 2, 450, 200, 40 };

    // Load font
    settingsFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(settingsFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    // Khởi tạo nút "Sound"
    soundButton = { (Screen_w - 200) / 2, 370, 200, 40 }; // Nút nằm dưới "Theme"
}

void SettingScreen::Update(int& state) {
    Vector2 mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Back"
    backHovered = CheckCollisionPointRec(mouse, backButton);

    // Kiểm tra hover vào nút "Theme"
    themeHovered = CheckCollisionPointRec(mouse, themeButton);

    // Kiểm tra hover vào nút "Sound"
    soundHovered = CheckCollisionPointRec(mouse, soundButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }

    // Xử lý sự kiện khi nhấn vào nút "Theme"
    if (themeHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isDarkMode = !isDarkMode;
    }

    // Xử lý sự kiện khi nhấn vào nút "Sound"
    if (soundHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isMusicOn = !isMusicOn;
        if (isMusicOn) {
            ResumeMusicStream(backgroundMusic);
        }
        else {
            PauseMusicStream(backgroundMusic);
        }
    }
}

void SettingScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode); // Sử dụng màu nền theo chế độ hiện tại

    // Hiển thị tiêu đề "Settings"
    int fontSize = 60;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(settingsFont, "Settings", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    Color titleColor = isDarkMode ? WHITE : DARKBLUE;
    DrawTextEx(settingsFont, "Settings",
        { titleX, titleY }, fontSize, spacing, titleColor);

    // Vẽ nút "Back"
    Color backColor = backHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(backButton, 0.2f, 4, backColor);
    DrawRectangleRoundedLinesEx(backButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 textSize = MeasureTextEx(myFont, "<< Back to Menu", 20, 1);
    DrawTextEx(myFont, "<< Back to Menu",
        { backButton.x + (backButton.width - textSize.x) / 2, backButton.y + (backButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);

    // Vẽ nút "Theme"
    Color themeColor = themeHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(themeButton, 0.2f, 4, themeColor);
    DrawRectangleRoundedLinesEx(themeButton, 0.2f, 4, 2.0f, GRAY);

    const char* themeText = isDarkMode ? "Light Mode" : "Dark Mode";
    textSize = MeasureTextEx(myFont, themeText, 20, 1);
    DrawTextEx(myFont, themeText,
        { themeButton.x + (themeButton.width - textSize.x) / 2, themeButton.y + (themeButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);

    // Vẽ nút bật/tắt âm thanh
    Color soundColor = soundHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(soundButton, 0.2f, 4, soundColor);
    DrawRectangleRoundedLinesEx(soundButton, 0.2f, 4, 2.0f, GRAY);

    const char* soundText = isMusicOn ? "Sound: ON" : "Sound: OFF";
    textSize = MeasureTextEx(myFont, soundText, 20, 1);
    DrawTextEx(myFont, soundText,
        { soundButton.x + (soundButton.width - textSize.x) / 2, soundButton.y + (soundButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);
}

void SettingScreen::Unload() {
    UnloadFont(settingsFont);
}
void AudioSystem::Init() {
    InitAudioDevice(); // Khởi tạo hệ thống âm thanh
    backgroundMusic = LoadMusicStream("Assets/Sounds/background_music.mp3");
    PlayMusicStream(backgroundMusic);
    backgroundMusic.looping = true; // Lặp vô hạn
}

void AudioSystem::Update() {
    if (isMusicOn) {
        UpdateMusicStream(backgroundMusic); // Cập nhật luồng nhạc
    }
}

void AudioSystem::Unload() {
    StopMusicStream(backgroundMusic);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice(); // Đóng hệ thống âm thanh
}
