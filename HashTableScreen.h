#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "HashTable.h"

class HashTableScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

	Font hashTableFont;   // Font chữ cho màn hình Linked List
	float insertOptionsOffset = 0.0f; // Offset để animation trượt xuống
	float insertAnimationTime = 0.0f;  // Offset để animation "Head", "Tail", "Pos" trượt lên

	bool showInsertOptions = false;
	bool insertHovered = false;
	bool deleteHovered = false;
	bool reverseHovered = false;
	bool cleanHovered = false;
	float insertHeadAlpha = 1.0f;  // Alpha của nút Head
	float insertTailAlpha = 1.0f;  // Alpha của nút Tail
	float insertPosAlpha = 1.0f;   // Alpha của nút Pos

	// Điều chỉnh vị trí bảng sang bên trái
	Rectangle insertButton = { PANEL_PADDING + 8, 330, 130, 40 };
	Rectangle deleteButton = { PANEL_PADDING + 8, 390, 130, 40 };
	Rectangle reverseButton = { PANEL_PADDING + 8, 450, 130, 40 };
	Rectangle cleanButton = { PANEL_PADDING + 8, 510, 130, 40 };

	// Nút con của Insert
	Rectangle insertHeadButton = { PANEL_PADDING + 37, 350, 100, 30 };
	Rectangle insertTailButton = { PANEL_PADDING + 37, 390, 100, 30 };
	Rectangle insertPosButton = { PANEL_PADDING + 37, 430, 100, 30 };
	HashTable hashTable;
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