#pragma once
#include "raylib.h"
#include "SettingScreen.h"
#include "AVL.h"
#include "TextBox.h"
#include "tinyfiledialogs.h"

enum SelectedButtonAVL {
	NONEAVL,
	INSERTAVL,
	DELETEAVL,
	SEARCHAVL,
	CLEARAVL,
	CREATEAVL,
	FILEAVL,
	OKAVL,
};

class AVLTreeScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề
	Font AVLtreeFont;   // Font chữ cho màn hình AVL
	Font valueFont;
	Font IN4Font;
	Font font;
	bool finnishAnimation = false;

	int MainCaseInfo;
	int SubCaseInfo;

	int InertionCaseInfo = 1;
	int DeletionCaseInfo = 2;
	int SearchCaseInfo = 3;
	int ClearCaseInfo = 4;

	int FindingtoInsert = 1;
	int InsertingCaseInfo = 7;
	int RightRotationCaseInfo = 2;
	int LeftRotationCaseInfo = 4;
	int RightLeftRotationCaseInfo = 5;
	int LeftRightRotationCaseInfo = 3;
	int FinishInsertionCaseInfo = -1;

	int FindingtoDelete = 1;
	int FindingtoSmallestRightSubTree = 6;
	int RomovevCaseInfo = 7;

	int NULLCase = 2;
	int ThisValueCase = 3;
	int SmallerCase = 4;
	int LargerCase = 5;

	bool insertHovered = false;
	bool deleteHovered = false;
	bool searchHovered = false;
	bool clearHovered = false;
	bool okHovered = false;
	bool createHovered = false;
	bool fileHovered = false;
	bool fileokHovered = false;

	// Vị trí của Panel
	Rectangle createButton = { PANEL_PADDING + 8, 330, 130, 40 };
	Rectangle insertButton = { PANEL_PADDING + 8, 390, 130, 40 };
	Rectangle deleteButton = { PANEL_PADDING + 8, 450, 130, 40 };
	Rectangle searchButton = { PANEL_PADDING + 8, 510, 130, 40 };
	Rectangle clearButton = { PANEL_PADDING + 8, 570, 130, 40 };;
	Rectangle fileButton = { 270, 420, 90, 30 };

	// Nút OK dưới textBox
	Rectangle okButton = { PANEL_PADDING + 217, 500, 90, 30 };

	// Kiểm tra trang hiện tại người dùng đang sử dụng
	int AVLtreeState;
	int InsertState = 3;
	int DeleteState = 4;
	int SearchState = 5;
	int ClearState = 6;
	int CreateState = 7;
	int FileState = 8;

	SelectedButtonAVL currentButton; // Xác định operation đang sử dụng

	// TextBox Value và Index 
	TextBox Value;
	TextBoxCenter Nodes;

	AVLtree AVLtree;

	// Kiểm soát đang thực hiện animation nào
	bool isInsert = false;
	bool isDeleting = false;
	bool isSearch = false;
	bool isClear = false;
	bool isCreateRandom = false;
	bool isCreateFile = false;

	bool entered = false;
	bool animating = false;   // Đang chạy animation hay không
	float timer = 0.0f;       // Đếm thời gian animation
	float duration = 0.6f;    // Thời gian chạy animation

	int valueInsert;
	int valueDelete;
	int valueSearch;
	int valueNodes;

	AVLNode* SearchNode;
	AVLNode* currentSearchNode;

	bool findingSmallestRightSubTree = false;

	bool SearchAnimationFinished = true;
	int ValueSearchAnimation;

	bool showFileInfoPopup = false;   // Hiển thị bảng thông báo khi ấn vào Browse File 
	const char* filePath;
	bool cont = false;

	AVLNode* Animationmroot;

	int posX, posY;   // Vị trí của IN4
	string infoMessage = "";   // Lưu dòng chữ INFO
	float fadeProgress = 1.0f;   // Fade AVL khi Clean AVL
	// Hàm reset lại nội dung trong textBox 
	void handleButtonClick(SelectedButtonAVL newButton, TextBox& textBox);
	void handleButtonClick2(SelectedButtonAVL newButton, TextBoxCenter& textBox);
public:
	// Hàm khởi tạo màn hình AVL
	void Init();

	// Hàm cập nhật trạng thái của màn hình AVL
	void Update(int& state);

	// Hàm vẽ mành hình AVL
	void Draw();

	// Giải phóng tài nguyên màn hình AVL
	void Unload();

	void SearchAnimation(int key);

	// Vẽ bảng điều khiển trong AVL Screen
	void DrawOperationsPanel();

	// Vẽ AVLtree lên màn hình
	void drawAVLtree(float animationProgress, AVLNode* root);

	void DrawInfo();
};
