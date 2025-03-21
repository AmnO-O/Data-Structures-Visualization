#pragma once
#include "raylib.h"
#include "SettingScreen.h"

class HelperScreen {
private:
	Font helperFont;      // Font chữ cho màn hình "How to Use"
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