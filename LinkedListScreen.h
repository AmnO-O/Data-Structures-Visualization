#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "LinkedList.h"

class LinkedListScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

	Font linkedListFont;   // Font chữ cho màn hình Linked List
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
	float Clamp(float value, float minValue, float maxValue);
	float SmoothStep(float a, float b, float t);

	LinkedList linkedList;

	float alphaVelocity = 3.0f;   // Tốc độ fade in/out
	float offsetVelocity = 200.0f; // Tốc độ di chuyển menu
	float deltaTime = GetFrameTime();
public:
	// Hàm khởi tạo màn hình Linked List
	void Init();

	// Hàm cập nhật trạng thái của màn hình Linked List
	void Update(int& state);

	// Hàm vẽ mành hình Linked List
	void Draw();

	// Giải phóng tài nguyên màn hình Linked List
	void Unload();

	// Vẽ bảng điều khiển trong Linked List Screen
	void DrawOperationsPanel();
};