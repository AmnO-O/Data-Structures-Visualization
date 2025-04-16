#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include <vector>
#include <string>

class HelperScreen {
private:
    Font helperFont;      // Font chữ cho màn hình "How to Use"
    Font myFont;          // Font nhỏ cho nội dung văn bản
    Rectangle backButton; // Nút "Back"
    bool backHovered;     // Trạng thái hover của nút "Back"

    // Hàm hỗ trợ xuống dòng văn bản
    std::vector<std::string> WrapText(const std::string& text, Font font, float maxWidth, int fontSize, float spacing);

public:
    // Hàm khởi tạo màn hình How to Use
    void Init();

    // Hàm cập nhật trạng thái của màn hình How to Use
    void Update(int& state);

    // Hàm vẽ màn hình How to Use
    void Draw();

    // Hàm giải phóng tài nguyên màn hình How to Use
    void Unload();
};
