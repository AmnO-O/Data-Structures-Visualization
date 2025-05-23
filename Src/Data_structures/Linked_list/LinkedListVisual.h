﻿#pragma once
#include "raylib.h"
#include "LinkedList.h"
#include "../../UI/Components/Textbox.h"
#include "../../UI/Toolbar/Adjust.h"
#include "../../External/tinyfiledialogs.h"
#include <unordered_set>
#include <string>


enum SelectedButton {
	NONE,
	INSERTHEAD,
	INSERTTAIL,
	INSERTPOS,
	DELETE,
	SEARCH,
	CLEAN,
	OK,
	CREATE,
	FILELINKEDLIST,
	UPDATE,
};

struct Command {
	SelectedButton type;
	int value;
	int position; // Dùng cho InsertAtPosition
	Command(SelectedButton t, int v, int pos = -1) : type(t), value(v), position(pos) {}
};

class LinkedListScreen {
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

	Font linkedListFont;   // Font chữ cho màn hình Linked List
	Font valueFont;
	Font IN4Font;
	float insertOptionsOffset = 0.0f; // Offset để animation trượt xuống
	float insertAnimationTime = 0.0f;  // Offset để animation "Head", "Tail", "Pos" trượt lên

	bool showInsertOptions = false;
	bool insertHovered = false;
	bool deleteHovered = false;
	bool searchHovered = false;
	bool cleanHovered = false;
	bool insertAtHeadHovered = false;
	bool insertAtTailHovered = false;
	bool insertPosHovered = false;
	bool okHovered = false;
	bool createHovered = false;
	bool fileHovered = false;
	bool fileokHovered = false;
	bool updateHovered = false;

	float insertHeadAlpha = 1.0f;  // Alpha của nút Head
	float insertTailAlpha = 1.0f;  // Alpha của nút Tail
	float insertPosAlpha = 1.0f;   // Alpha của nút Pos

	// Vị trí của Panel
	Rectangle updateButton = { PANEL_PADDING + 8, 330, 130, 40 };
	Rectangle createButton = { PANEL_PADDING + 8, 280, 130, 40 };
	Rectangle insertButton = { PANEL_PADDING + 8, 380, 130, 40 };
	Rectangle deleteButton = { PANEL_PADDING + 8, 440, 130, 40 };
	Rectangle searchButton = { PANEL_PADDING + 8, 500, 130, 40 };
	Rectangle cleanButton = { PANEL_PADDING + 8, 560, 130, 40 };

	// Nút con của Insert
	Rectangle insertHeadButton = { PANEL_PADDING + 37, 410, 100, 30 };
	Rectangle insertTailButton = { PANEL_PADDING + 37, 350, 100, 30 };
	Rectangle insertPosButton = { PANEL_PADDING + 37, 490, 100, 30 };

	// Nút OK dưới textBox
	Rectangle okButton = { PANEL_PADDING + 217, 470, 90, 30 };

	Rectangle fileButton = { 270, 400, 90, 30 };
	// Nút undo, redo 
	Vector2 Undoposition = { 700, 840 };
	Vector2 Redoposition = { 800, 840 };

	float Clamp(float value, float minValue, float maxValue);
	float SmoothStep(float a, float b, float t);

	// Kiểm tra trang hiện tại người dùng đang sử dụng
	int linkedlistState;
	int InsertHeadState = 1;
	int InsertPosState = 2;
	int InsertTailState = 3;
	int DeleteState = 4;
	int SearchState = 5;
	int ClearState = 6;
	int UndoState = 7;
	int RedoState = 8;
	int CreateState = 9;
	int FileState = 10;
	int UpdateState = 11;

	Vector2 mouse;

	SelectedButton currentButton; // Xác định operation đang sử dụng

	// TextBox Value và Index 
	TextBox Value;
	TextBox Index;

	TextBoxCenter Nodes;

	int valueNodes;

	bool showFileInfoPopup = false;   // Hiển thị bảng thông báo khi ấn vào Browse File 
	const char* filePath;
	bool cont = false;

	LinkedList linkedList;

	// Kiểm soát đang thực hiện animation nào
	bool isHeadInserting = false;
	bool isTailInserting = false;
	bool isPosInserting = false;
	bool isDeleting = false;
	bool isSearch = false;
	bool isClean = false;
	bool isCreateRandom = false;
	bool isCreateFile = false;
	bool isUpdate = false;

	bool entered = false;
	bool animating = false;   // Đang chạy animation hay không
	float timer = 0.0f;       // Đếm thời gian animation
	float duration = 0.6f;    // Thời gian chạy animation

	int valueInsert;
	int valueDelete;
	int indexInsert;
	int valueSearch;
	int valueUpdate;
	int indexUpdate;

	int indexAnimation;
	int indexSearch;   //  index của Node được search
	int indexDelete;   //  index của Node bị xóa
	LLNode* SearchNode;
	LLNode* currentSearchNode;

	int posX, posY;   // Vị trí của IN4
	string infoMessage = "";   // Lưu dòng chữ INFO
	float fadeProgress = 1.0f;   // Fade linked list khi Clean Linked List 

	Vector2 startPos = { 500, 300 };
	Vector2 startPos2ndrow = { 1500, 400 };
	Vector2 startPos3ndrow = { 500, 500 };
	float nodeSpacing = 100.0f;

	// Hàm reset lại nội dung trong textBox 
	void handleButtonClick(SelectedButton newButton, TextBox& textBox);

	// Undo, Redo
	bool isUndo = false;
	bool isRedo = false;

	std::vector<Command> undoStack; // Lưu trữ các thao tác
	std::vector<Command> redoStack;

	bool stepbystep = false; // use to step by step

	Toolbar toolbar;
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

	Vector2 TargetPos(int index);

	void SaveStateForUndo(SelectedButton type, int value, int position);

	void Undo();
	void Redo();

};