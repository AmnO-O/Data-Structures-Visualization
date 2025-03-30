#include "AVLTreeScreen.h"
#include "Constants.h"
#include <string>
#include <unordered_set>

void AVLTreeScreen::handleButtonClick(SelectedButtonAVL newButton, TextBox& textBox) {
    if (currentButton != newButton) {
        textBox.text = ""; // Reset lại nội dung trong textBox 
        textBox.pos = 0;
    }
}

void AVLTreeScreen::Init() {
    // Khởi tạo nút Back
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Load font
    AVLtreeFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(AVLtreeFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font cho INFO;
    IN4Font = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(IN4Font.texture, TEXTURE_FILTER_BILINEAR);

    currentButton = NONEAVL;

    Value = { {270, 350, 90, 30} };

}

void AVLTreeScreen::Update(int& state) {
    Vector2 mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Back"
    backHovered = CheckCollisionPointRec(mouse, backButton);

    // Kiểm tra hover vào nút "Insert"
    insertHovered = CheckCollisionPointRec(mouse, insertButton);

    // Kiểm tra hover vào nút "Delete"
    deleteHovered = CheckCollisionPointRec(mouse, deleteButton);

    // Kiểm tra hover vào nút "Search"
    searchHovered = CheckCollisionPointRec(mouse, searchButton);

    // Kiểm tra hover vào nút "Clear"
    clearHovered = CheckCollisionPointRec(mouse, clearButton);
    // Kiểm tra hover vào nút "OK"
    okHovered = CheckCollisionPointRec(mouse, okButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }

    // Kiểm tra nút Insert
    if (insertHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = InsertState;
        handleButtonClick(INSERTAVL, Value);
        currentButton = INSERTAVL;
        MainCaseInfo =InertionCaseInfo ;
        SubCaseInfo = 0;
        SearchNode = nullptr;
    }


    // Kiểm tra nút Delete
    if (CheckCollisionPointRec(mouse, deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = DeleteState;
        handleButtonClick(DELETEAVL, Value);
        MainCaseInfo = DeletionCaseInfo;
        currentButton = DELETEAVL;
        SearchNode = nullptr;
    }

    // Kiểm tra nút Reverse
    if (CheckCollisionPointRec(mouse, searchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = SearchState;
        currentButton = SEARCHAVL;
        SearchNode = nullptr;
    }

    // Kiểm tra nút Clear
    if (CheckCollisionPointRec(mouse, clearButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = ClearState;
        currentButton = CLEARAVL;
        isClear = true;
        fadeProgress = 1.0f;
        SearchNode = nullptr;
    }

    if (okHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Value.isClickedEnter = true;
    }
    
    if (AVLtreeState == InsertState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueInsert = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            // Kích hoạt animation sau mỗi lần thay đổi
            isInsert = true;
            timer = 0.0f;
            entered = true;

			//SearchAnimation
            if (AVLtree.m_root != NULL ) {
                currentSearchNode = AVLtree.m_root;
                SearchAnimationFinished = false;
				ValueSearchAnimation = valueInsert;
            }
			
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueInsert = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                // Kích hoạt animation sau mỗi lần thay đổi
                isInsert = true;
                timer = 0.0f;
                entered = true;

                //SearchAnimation
                if (AVLtree.m_root != NULL) {
                    currentSearchNode = AVLtree.m_root;
                    SearchAnimationFinished = false;
                    ValueSearchAnimation = valueInsert;
                }
            }
            Value.isClickedEnter = false;
        }
    }
    else if (AVLtreeState == DeleteState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueDelete = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            //SearchAnimation
            if (AVLtree.m_root != NULL) {
                currentSearchNode = AVLtree.m_root;
                SearchAnimationFinished = false;
                ValueSearchAnimation = valueDelete;
            }

            if (AVLtree.Search(valueDelete)) {
                // Kích hoạt animation sau mỗi lần thay đổi
                isDeleting = true;
                timer = 0.0f;
                entered = true;	
            }
		}
        else if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueDelete = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

				// SearchAnimation
                if (AVLtree.m_root != NULL) {
                    currentSearchNode = AVLtree.m_root;
                    SearchAnimationFinished = false;
                    ValueSearchAnimation = valueDelete;
                }
                if (AVLtree.Search(valueDelete)) {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    isDeleting = true;
                    timer = 0.0f;
                    entered = true;
                }
                Value.isClickedEnter = false;
            }
        }

    }
    else if (AVLtreeState == SearchState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueSearch = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            // Kích hoạt animation sau mỗi lần thay đổi
            currentSearchNode = AVLtree.m_root;
            SearchAnimationFinished = false;
            isSearch = true;
            timer = 0.0f;
            entered = true;
        }
        else if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueSearch = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                // Kích hoạt animation sau mỗi lần thay đổi
                currentSearchNode = AVLtree.m_root;
                SearchAnimationFinished = false;
                isSearch = true;
                timer = 0.0f;
                entered = true;
            }
            Value.isClickedEnter = false;
        }    
    }
    else if (AVLtreeState == ClearState) {
        infoMessage = "Clearing the AVLtree...";
    }

    if (AVLtree.m_root) DrawCircle(AVLtree.m_root->displayX, AVLtree.m_root->displayY, 50, BLACK);
     //Cập nhật tiến trình animation
    if (isInsert || isDeleting || isSearch || isClear) {
        timer += GetFrameTime();
         if (entered && SearchAnimationFinished) {
            if (isInsert)  AVLtree.Insert(valueInsert);
            else if (isDeleting) {
                AVLtree.Delete(valueDelete);
                SearchNode = nullptr;
            }
            else if (isSearch) {
                SearchNode = AVLtree.Search(valueSearch);
            }
			timer = duration + 0.1f;  
            currentSearchNode = NULL;
            entered = !entered;
         }
         if (timer >= duration) {
            if (!SearchAnimationFinished) {
                SubCaseInfo = 1;
                if (isInsert)  SearchAnimation(valueInsert);
                else if (isDeleting) SearchAnimation(valueDelete);
                else if (isSearch) SearchAnimation(valueSearch);
				timer = 0.0f;
            }
            else if (AVLtree.waitinganimation == 0 ) {
                isInsert = isDeleting = isSearch = false;
                timer = 0.0f;  // Reset lại bộ đếm thời gian
                SubCaseInfo = -1;
            }
            else if (AVLtree.waitinganimation > 0 ) {
                AVLtree.waitinganimation--;
                if (!AVLtree.mroot.empty()) {
                    auto step = AVLtree.mroot.front();
                    SubCaseInfo = step.second;
                    Animationmroot = step.first;
                    AVLtree.mroot.pop_front();
                }
                AVLtree.UpdatePos(Animationmroot);
                timer = 0.0f;
            }
        }
    }
}


void AVLTreeScreen::DrawOperationsPanel() {
    // Vẽ nền bảng
    int panelMargin = 250;  // Khoảng cách lề trên & dưới
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };  // Chọn màu theo chế độ

    DrawRectangle(0, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, Fade(panelColor, 0.8f));

    // Vẽ nền bảng
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };  // Chọn màu theo chế độ

    DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, Fade(panelColorx, 0.8f));

    // Tiêu đề
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(myFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    // Vẽ nút Insert
    Color InsertColor = insertHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == INSERTAVL) InsertColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(insertButton, 0.2f, 4, InsertColor);
    DrawRectangleRoundedLinesEx(insertButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 insertSize = MeasureTextEx(myFont, "Insert", 20, 2);
    DrawTextEx(myFont, "Insert",
        { insertButton.x + (insertButton.width - insertSize.x) / 2,
        insertButton.y + (insertButton.height - insertSize.y) / 2 },
        20, 2, DARKBLUE);

    

    // Vẽ nút Delete
    Color DeleteColor = deleteHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == DELETEAVL) DeleteColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(deleteButton, 0.2f, 4, DeleteColor);
    DrawRectangleRoundedLinesEx(deleteButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 deleteSize = MeasureTextEx(myFont, "Delete", 20, 2);
    DrawTextEx(myFont, "Delete",
        { deleteButton.x + (deleteButton.width - deleteSize.x) / 2,
          deleteButton.y + (deleteButton.height - deleteSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Search
    Color ReverseColor = searchHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == SEARCHAVL) ReverseColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(searchButton, 0.2f, 4, ReverseColor);
    DrawRectangleRoundedLinesEx(searchButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 reverseSize = MeasureTextEx(myFont, "Search", 20, 2);
    DrawTextEx(myFont, "Search",
        { searchButton.x + (searchButton.width - reverseSize.x) / 2,
          searchButton.y + (searchButton.height - reverseSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Clear
    Color ClearColor = clearHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == CLEARAVL) ClearColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(clearButton, 0.2f, 4, ClearColor);
    DrawRectangleRoundedLinesEx(clearButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 clearSize = MeasureTextEx(myFont, "CLear", 18, 2);
    DrawTextEx(myFont, "Clear",
        { clearButton.x + (clearButton.width - clearSize.x) / 2,
          clearButton.y + (clearButton.height - clearSize.y) / 2 },
        18, 2, DARKBLUE);

    DrawInfo();

    if ( AVLtreeState == InsertState || AVLtreeState == DeleteState || AVLtreeState == SearchState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Value", fontSize, 1.f).x;
        DrawTextEx(myFont, "Value", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();
        // Vẽ nút "OK"
        Color okColor = okHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == OKAVL) okColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(okButton, 0.2f, 4, okColor);
        DrawRectangleRoundedLinesEx(okButton, 0.2f, 4, 2.0f, GRAY);

        Vector2 okSize = MeasureTextEx(myFont, "OK", 18, 2);
        DrawTextEx(myFont, "OK",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (AVLtreeState == ClearState) {}
}

void AVLTreeScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);   // Sử dụng màu nền ở chế độ hiện tại 

    // Hiển thị tiêu đề AVLtree
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(AVLtreeFont, "AVLtree", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    Color titleColor = isDarkMode ? WHITE : DARKBLUE;
    DrawTextEx(AVLtreeFont, "AVLtree",
        { titleX, titleY }, fontSize, spacing, titleColor);

    // Vẽ nút "Back"
    Color backColor = backHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(backButton, 0.2f, 4, backColor);
    DrawRectangleRoundedLinesEx(backButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 textSize = MeasureTextEx(myFont, "<< Back to Menu", 20, 1);
    DrawTextEx(myFont, "<< Back to Menu",
        { backButton.x + (backButton.width - textSize.x) / 2, backButton.y + (backButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);

    // Vẽ bảng điều khiển
    DrawOperationsPanel();


    // Gọi vẽ danh sách với progress từ 0 -> 1
    float animationProgress = timer / duration;
    drawAVLtree(animationProgress,Animationmroot);
    finnishAnimation = true;
}

void AVLTreeScreen::drawAVLtree(float animationProgress, AVLNode* root ) {
    if (!root) return;
    // Interpolate positions using animation progress

    root->displayX = root->x * (1.0f - animationProgress) + root->newx * animationProgress;
    root->displayY = root->y * (1.0f - animationProgress) + root->newy * animationProgress;
    // Draw edges
    if (root->left) {
        DrawLine(root->displayX, root->displayY, root->left->displayX, root->left->displayY,BLACK);
        drawAVLtree(animationProgress, root->left);
    }
    if (root->right) {
        DrawLine(root->displayX, root->displayY, root->right->displayX, root->right->displayY,BLACK);
        drawAVLtree(animationProgress, root->right);
    }

    // Draw node
    if (root == SearchNode)DrawCircleV({ root->displayX,root->displayY }, 20, ORANGE);
    else if (root == currentSearchNode)DrawCircleV({ root->displayX,root->displayY }, 20, RED);
    else DrawCircleV({ root->displayX,root->displayY }, 20, WHITE);
    string nodeText = TextFormat("%d", root->val);
    Vector2 textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);

    Vector2 textPos = { root->displayX - textSize.x / 2,  root->displayY - textSize.y / 2 }; // Căn giữa Node 
    DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, fadeProgress));

    if (isClear) {
        fadeProgress -= GetFrameTime();
        if (fadeProgress <= 0.0f) {
            fadeProgress = 1.0f;
            AVLtree.Clear(); // Xóa danh sách khi hiệu ứng kết thúc
            isClear = false;
			Animationmroot = NULL;
        }
    }
}

void AVLTreeScreen::SearchAnimation(int key) {
    if (currentSearchNode->val == key || findingSmallestRightSubTree) {
        if (isDeleting && currentSearchNode->val == key &&
            !findingSmallestRightSubTree &&
            currentSearchNode -> left && currentSearchNode -> right) {
            SubCaseInfo = 6;
            SearchNode = currentSearchNode;
            currentSearchNode = currentSearchNode->right;
            findingSmallestRightSubTree = true;
        }
        else if (isDeleting && findingSmallestRightSubTree) {
            if (currentSearchNode->left) {
                currentSearchNode = currentSearchNode->left;
                SubCaseInfo = 6;
            }
            else {
                SearchAnimationFinished = true;
                findingSmallestRightSubTree = false;
            }
        }
        else SearchAnimationFinished = true;
    }
    else {
        if (ValueSearchAnimation < currentSearchNode->val) {
            SubCaseInfo = 1;
			if (!currentSearchNode->left) {
				SearchAnimationFinished = true;
			}
			else {
				currentSearchNode = currentSearchNode->left;
			}
        }
        else {
            SubCaseInfo = 1;
			if (!currentSearchNode->right) {
				SearchAnimationFinished = true;
			}
			else currentSearchNode = currentSearchNode->right;
        }
    }
}


void AVLTreeScreen::DrawInfo() {
    // Vẽ nền bảng cho [INFO]
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };  // Chọn màu theo chế độ
    int rectWidth = 400;
    int rectHeight = 300;
    posX = Screen_w - rectWidth;
    posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);
    if (MainCaseInfo == InertionCaseInfo) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        int lineSpacing = 37; // Tăng khoảng cách dòng để vừa với chiều cao mới

        if (SubCaseInfo == FindingtoInsert)DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding v", { (float)textX, (float)textY }, 20, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == InsertingCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "insert v", { (float)textX, (float)textY }, 20, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        DrawTextEx(IN4Font, "check balance factor of this and its children", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case1: this.rotateRight", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case2: this.left.rotateLeft, this.rotateRight", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case3: this.rotateLeft", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightLeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case4: this.right.rotateRight, this.rotateLeft", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == -1) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "this is balanced", { (float)textX, (float)textY }, 18, 2, WHITE);

    }
    else if (MainCaseInfo == DeletionCaseInfo){
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        int lineSpacing = 32; // Tăng khoảng cách dòng để vừa với chiều cao mới


        if (SubCaseInfo == FindingtoDelete) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding v", { (float)textX, (float)textY }, 20, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == FindingtoSmallestRightSubTree) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding the Smallest in Right Sub Tree", { (float)textX, (float)textY }, 20, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RomovevCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "delete v", { (float)textX, (float)textY }, 20, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        DrawTextEx(IN4Font, "check balance factor of this and its children", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case1: this.rotateRight", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case2: this.left.rotateLeft, this.rotateRight", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case3: this.rotateLeft", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightLeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case4: this.right.rotateRight, this.rotateLeft", { (float)textX, (float)textY }, 18, 2, WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == -1) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "this is balanced", { (float)textX, (float)textY }, 18, 2, WHITE);

    }
}

void AVLTreeScreen::Unload() {
    UnloadFont(AVLtreeFont);
}