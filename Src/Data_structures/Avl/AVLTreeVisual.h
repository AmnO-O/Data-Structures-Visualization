﻿#pragma once
#include "raylib.h"
#include "AVL.h"
#include "../../UI/Components/TextBox.h"
#include "../../External/tinyfiledialogs.h"
#include "../../UI/Screens/SettingScreen.h"
#include "Table.h"
#include <stack>
#include <string>



enum SelectedButtonAVL {
	NONEAVL,
	INSERTAVL,
	DELETEAVL,
	SEARCHAVL,
	CLEARAVL,
	UPDATEAVL,
	CREATEAVL,
	FILEAVL,
	OKAVL,
	INORDERAVL,
	PREORDERAVL,
	POSTORDERAVL,
};

struct Spark {
	Vector2 position;
	float alpha;    // Độ trong suốt
	float scale;    // Kích thước
	float lifetime; // Thời gian sống
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
	int UpdateCaseInfo = 5;

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
	int ThisValueCase = -1;
	int SmallerCase = 4;
	int LargerCase = 5;

	int findingValueGreater = 3;
	int findingValueSmaller = 2;
	int findingSmallest = 4;
	int findingLargest = 5;
	int alterSuccess = 7;
	int unSuccess = 1;

	bool insertHovered = false;
	bool deleteHovered = false;
	bool searchHovered = false;
	bool clearHovered = false;
	bool okHovered = false;
	bool createHovered = false;
	bool updateHovered = false;
	bool fileHovered = false;
	bool fileokHovered = false;
	bool inorderHovered = false;
	bool preorderHovered = false;
	bool postorderHovered = false;
	bool undoHovered = false;
	bool redoHovered = false;
	bool pauseHovered = false;

	// Vị trí của Panel
	Rectangle createButton = { PANEL_PADDING + 8, 320, 130, 40 };
	Rectangle insertButton = { PANEL_PADDING + 8, 375, 130, 40 };
	Rectangle deleteButton = { PANEL_PADDING + 8, 430, 130, 40 };
	Rectangle searchButton = { PANEL_PADDING + 8, 485, 130, 40 };
	Rectangle clearButton = { PANEL_PADDING + 8, 540, 130, 40 };
	Rectangle updateButton = { PANEL_PADDING + 8, 595, 130, 40 };
	Rectangle fileButton = { 270, 420, 90, 30 };

	// Nút OK dưới textBox
	Rectangle okButton = { PANEL_PADDING + 197, 500, 130, 40 };

	// Undo, Redo 
	Vector2 Undoposition = { 700, 840 };
	Vector2 Redoposition = { 800, 840 };
	Vector2 Pauseposition = { 750, 840 };
	Rectangle undoRect = { Undoposition.x, Undoposition.y, 50, 50 };
	Rectangle redoRect = { Redoposition.x, Redoposition.y, 50, 50 };
	Rectangle pause = { Pauseposition.x, Pauseposition.y, 50, 50 };

	// Kiểm tra trang hiện tại người dùng đang sử dụng
	int AVLtreeState;
	int InsertState = 3;
	int DeleteState = 4;
	int SearchState = 5;
	int ClearState = 6;
	int CreateState = 7;
	int FileState = 8;
	int UpdateState = 9;


	SelectedButtonAVL currentButton; // Xác định operation đang sử dụng

	// TextBox Value và Index 
	TextBox Value;
	TextBox ValueAlter;
	TextBoxCenter Nodes;

	Vector2 mouse;
	AVLtree CurrAVLtree;

	// Kiểm soát đang thực hiện animation nào
	bool isInsert = false;
	bool isDeleting = false;
	bool isSearch = false;
	bool isClear = false;
	bool isCreateRandom = false;
	bool isCreateFile = false;
	bool isPause = false;
	bool isUndo = false;
	bool isRedo = false;
	bool isUpdate = false;

	bool entered = false;
	bool animating = false;   // Đang chạy animation hay không
	float timer = 0.0f;       // Đếm thời gian animation

	int valueInsert;
	int valueDelete;
	int valueSearch;
	int valueNodes;
	int valueAlter;
	int valueAltered;

	AVLNode* SearchNode;

	bool findingSmallestRightSubTree = false;

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


	// Traversal 
	std::vector<int> InTraversalValues;
	std::vector<int> PreTraversalValues;
	std::vector<int> PostTraversalValues;
	float IntraversalTimer = 0.0f;
	float PretraversalTimer = 0.0f;
	float PosttraversalTimer = 0.0f;
	int currentInIndex = 0, currentPreIndex = 0, currentPostIndex = 0;
	float GetTotalTraversalWidth(const std::vector<int>& values, int currentIndex, Font font, float fontSize, float spacing, float padding);
	float InTraversalBgWidth = 0.0f;
	bool isInTraversalReady = false;
	float PreTraversalBgWidth = 0.0f;
	bool isPreTraversalReady = false;
	float PostTraversalBgWidth = 0.0f;
	bool isPostTraversalReady = false;

	std::vector<Spark> sparks;


	Toolbar toolbar;


	TraversalTable myTable;
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
	void drawAVLtree(float animationProgress, AVLNode* root);

	void DrawInfo();
};


