#pragma once
#include "raylib.h"
#include "../../Constants.h"

// Biến kiểm tra hover cho nút Back và Theme
extern bool backHovered;
extern bool themeHovered;

extern Rectangle backButton;

extern Font myFont;

extern bool isMusicOn;
extern Music backgroundMusic;

class SettingScreen {
private:

public:
	// Hàm khởi tạo màn hình Settings
	void Init();

	// Hàm cập nhật trạng thái của màn hình Settings
	void Update(int& state);

	// Hàm vẽ màn hình Settings
	void Draw();

	// Hàm giải phóng tài nguyên màn hình Settings
	void Unload();
};

class AudioSystem {
private:

public:
	// BEGIN (ÂM THANH)
	// Biến lưu trạng thái âm thanh

	// Hàm khởi tạo âm thanh
	void Init();

	// Hàm cập nhật âm thanh (lặp nhạc nền)
	void Update();

	// Hàm dừng âm thanh và giải phóng tài nguyên
	void Unload();
	// END 
};