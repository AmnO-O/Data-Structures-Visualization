#pragma once
#include "raylib.h"
#include "SettingScreen.h"

class AboutScreen {
private:
	Font aboutFont;      // Font chữ cho màn hình "How to Use"
	Font font;
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