#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "LinkedList.h"
#include "TextBox.h"


enum SelectedButton {
	NONE,
	INSERTHEAD,
	INSERTTAIL,
	INSERTPOS,
	DELETE,
	REVERSE,
	CLEAN
};

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
	bool insertAtHeadHovered = false;
	bool insertAtTailHovered = false;
	bool insertPosHovered = false;

	float insertHeadAlpha = 1.0f;  // Alpha của nút Head
	float insertTailAlpha = 1.0f;  // Alpha của nút Tail
	float insertPosAlpha = 1.0f;   // Alpha của nút Pos

	// Vị trí của Panel
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

	// Kiểm tra trang hiện tại người dùng đang sử dụng
	int linkedlistState;
	int InsertHeadState = 1;
	int InsertPosState = 2;
	int InsertTailState = 3;
	int DeleteState = 4;
	int ReverseState = 5;
	int ClearState = 6;

	SelectedButton currentButton; // Xác định operation đang sử dụng

	// TextBox Value và Index 
	TextBox Value;
	TextBox Index;

	LinkedList linkedList;

	// Kiểm soát đang thực hiện animation nào
	bool isHeadInserting = false;
	bool isTailInserting = false;
	bool isPosInserting = false;
	bool isDeleting = false;
	bool isReverse = false;
	bool isClean = false;

	bool entered = false;
	bool animating = false;   // Đang chạy animation hay không
	float timer = 0.0f;       // Đếm thời gian animation
	float duration = 0.5f;    // Thời gian chạy animation

	int valueInsert;
	int valueDelete;
	int indexInsert;

	// Hàm reset lại nội dung trong textBox 
	void handleButtonClick(SelectedButton newButton, TextBox& textBox);
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

	// Vẽ Linked List lên màn hình
	void drawLinkedList(float animationProgress);
};