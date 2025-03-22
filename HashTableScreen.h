#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "HashTable.h"

class HashTableScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề
	HashTable hashTable;

	Font hashTableFont;
	Font IN4Font;
public:
	// Hàm khởi tạo màn hình Linked List
	void Init();

	void Update(int& state);
	void Draw();
	void Unload();

	AnimationData animation;
	CommonButton Input;
	Rectangle border, info;
	Button Random, Empty;
	TextBox Size;
	TextBoxCenter Value;
	HashTable H;
	bool popActive;
	float popElapsedTime;
	float popDuration; // Duration for the pop effect.
	float popScale;    // Current scale factor (from small to normal).

	bool  shrinkActive;
	float shrinkElapsedTime;
	float shrinkDuration; // Duration for removal effect.
	float shrinkScale;    // Scale from 1 to 0.

	bool searchActive;
	float searchElapsedTime;
	float searchDuration; // Duration for search effect.
};