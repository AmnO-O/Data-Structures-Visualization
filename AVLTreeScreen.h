#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "AVL.h"
#include "TextBox.h"


enum SelectedButtonAVL {
	NONEAVL,
	INSERTAVL,
	DELETEAVL,
	SEARCHAVL,
	CLEANAVL
};

class AVLTreeScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

	Font AVLtreeFont;   // Font chữ cho màn hình AVL
	Font valueFont;
	Font IN4Font;


	bool insertHovered = false;
	bool deleteHovered = false;
	bool searchHovered = false;
	bool cleanHovered = false;


	// Vị trí của Panel
	Rectangle insertButton = { PANEL_PADDING + 8, 330, 130, 40 };
	Rectangle deleteButton = { PANEL_PADDING + 8, 390, 130, 40 };
	Rectangle searchButton = { PANEL_PADDING + 8, 450, 130, 40 };
	Rectangle cleanButton = { PANEL_PADDING + 8, 510, 130, 40 };


	float Clamp(float value, float minValue, float maxValue);
	float SmoothStep(float a, float b, float t);

	// Kiểm tra trang hiện tại người dùng đang sử dụng
	int AVLtreeState;
	int InsertState = 3;
	int DeleteState = 4;
	int SearchState = 5;
	int ClearState = 6;

	SelectedButtonAVL currentButton; // Xác định operation đang sử dụng

	// TextBox Value và Index 
	TextBox Value;

	AVLtree AVLtree;

	// Kiểm soát đang thực hiện animation nào
	bool isInsert = false;
	bool isDeleting = false;
	bool isSearch = false;
	bool isClean = false;

	bool entered = false;
	bool animating = false;   // Đang chạy animation hay không
	float timer = 0.0f;       // Đếm thời gian animation
	float duration = 0.5f;    // Thời gian chạy animation

	int valueInsert;
	int valueDelete;
	int valueSearch;

	AVLNode* SearchNode;
	AVLNode* currentSearchNode;



	int posX, posY;   // Vị trí của IN4
	string infoMessage = "";   // Lưu dòng chữ INFO
	float fadeProgress = 1.0f;   // Fade AVL khi Clean AVL
	// Hàm reset lại nội dung trong textBox 
	void handleButtonClick(SelectedButtonAVL newButton, TextBox& textBox);
public:
	// Hàm khởi tạo màn hình AVL
	void Init();

	// Hàm cập nhật trạng thái của màn hình AVL
	void Update(int& state);

	// Hàm vẽ mành hình AVL
	void Draw();

	// Giải phóng tài nguyên màn hình AVL
	void Unload();

	// Vẽ bảng điều khiển trong AVL Screen
	void DrawOperationsPanel();

	// Vẽ AVLtree lên màn hình
	void drawAVLtree(float animationProgress);
};
