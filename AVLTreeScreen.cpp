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

void AVLTreeScreen::handleButtonClick2(SelectedButtonAVL newButton, TextBoxCenter& textBox) {
    if (currentButton != newButton) {
        textBox.text = ""; // Reset lại nội dung trong textBox 
        textBox.pos = 0;
    }
}

void AVLTreeScreen::Init() {
    // Load font
    AVLtreeFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(AVLtreeFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font cho INFO;
    IN4Font = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(IN4Font.texture, TEXTURE_FILTER_BILINEAR);

    font = LoadFont("Assets/Fonts/PublicSans-bold.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    currentButton = NONEAVL;
    Value = { {270, 350, 90, 30} };
    Nodes = { {270, 350, 90, 30}, "Random" };

    // Tải ảnh undo, redo 
    imageRedo = LoadImage("Assets/Images/Redo.png");
    imageUndo = LoadImage("Assets/Images/Undo.png");
    imageRedoHovered = LoadImage("Assets/Images/RedoHovered.png");
    imageUndoHovered = LoadImage("Assets/Images/UndoHovered.png");
    ImageResize(&imageRedo, 50, 50);
    ImageResize(&imageUndo, 50, 50);
    ImageResize(&imageRedoHovered, 50, 50);
    ImageResize(&imageUndoHovered, 50, 50);
    textureRedo = LoadTextureFromImage(imageRedo);
    textureUndo = LoadTextureFromImage(imageUndo);
    textureUndoHovered = LoadTextureFromImage(imageUndoHovered);
    textureRedoHovered = LoadTextureFromImage(imageRedoHovered);

    sparks.clear();
}

void AVLTreeScreen::Update(int& state) {
    mouse = GetMousePosition();

    // Xử lý input để tạo spark mới
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        Spark newSpark = {
            mousePos,    // position
            1.0f,        // alpha
            0.0f,        // scale
            0.5f         // lifetime (0.5 giây)
        };
        sparks.push_back(newSpark);
    }

    // Kiểm tra hover vào nút "Insert"
    insertHovered = CheckCollisionPointRec(mouse, insertButton);

    // Kiểm tra hover vào nút "Delete"
    deleteHovered = CheckCollisionPointRec(mouse, deleteButton);

    // Kiểm tra hover vào nút "Search"
    searchHovered = CheckCollisionPointRec(mouse, searchButton);

    // Kiểm tra hover vào nút "Clear"
    clearHovered = CheckCollisionPointRec(mouse, clearButton);

    // Kiểm tra hover vào nút "Create"
    createHovered = CheckCollisionPointRec(mouse, createButton);

    // Kiểm tra hover vào nút "File"
    fileHovered = CheckCollisionPointRec(mouse, fileButton);

    // Kiểm tra hover vào nút "OK"
    okHovered = CheckCollisionPointRec(mouse, okButton);

    // Kiểm tra hover vào nút "Inorder"
    inorderHovered = CheckCollisionPointRec(mouse, inorderButton);

    // Kiểm tra hover vào nút "Preorder"
    preorderHovered = CheckCollisionPointRec(mouse, preorderButton);

    // Kiểm tra hover vào nút "PostOrder"
    postorderHovered = CheckCollisionPointRec(mouse, postorderButton);

    // Kiểm tra hover vào nút "Undo"
    undoHovered = CheckCollisionPointRec(mouse, undoRect);

    // Kiểm tra hover vào nút "Redo"
    redoHovered = CheckCollisionPointRec(mouse, redoRect);

    if (undoHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        //CurrAVLtree.Undo();
    }

    if (redoHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        //CurrAVLtree.Redo();
    }

    // Kiểm tra nút Inorder
    if (inorderHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isInordered = true;
        InTraversalValues.clear();
        CurrAVLtree.InorderTraversal(InTraversalValues);
        InTraversalBgWidth = 0.0f;
        isInTraversalReady = false;
        currentInIndex = 0;
        IntraversalTimer = 0.0f;
    }

    // Kiểm tra nút Preorder
    if (preorderHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isPreordered = true;
        PreTraversalValues.clear();
        CurrAVLtree.PreorderTraversal(PreTraversalValues);
        PreTraversalBgWidth = 0.0f;
        isPreTraversalReady = false;
        currentPreIndex = 0;
        PretraversalTimer = 0.0f;
    }

    // Kiểm tra nút Postorder
    if (postorderHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isPostordered = true;
        PostTraversalValues.clear();
        CurrAVLtree.PostorderTraversal(PostTraversalValues);
        PostTraversalBgWidth = 0.0f;
        isPostTraversalReady = false;
        currentPostIndex = 0;
        PosttraversalTimer = 0.0f;
    }

    // Kiểm tra nút Create
    if (createHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = CreateState;
        handleButtonClick2(CREATEAVL, Nodes);
        currentButton = CREATEAVL;
        SearchNode = nullptr;
    }

    //  Kiểm tra nút File
    if (fileHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showFileInfoPopup = true; // Hiển thị bảng thông báo
        AVLtreeState = FileState;
        currentButton = FILEAVL;
        SearchNode = nullptr;

    }

    // Kiểm tra nút Insert
    if (insertHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = InsertState;
        handleButtonClick(INSERTAVL, Value);
        currentButton = INSERTAVL;
        MainCaseInfo = InertionCaseInfo;
        SubCaseInfo = 0;
        SearchNode = nullptr;
    }

    // Kiểm tra nút Delete
    if (deleteHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = DeleteState;
        handleButtonClick(DELETEAVL, Value);
        MainCaseInfo = DeletionCaseInfo;
        currentButton = DELETEAVL;
        SearchNode = nullptr;
    }

    // Kiểm tra nút Search
    if (searchHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = SearchState;
        MainCaseInfo = SearchCaseInfo;
        currentButton = SEARCHAVL;
        SearchNode = nullptr;
    }

    // Kiểm tra nút Clear
    if (clearHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = ClearState;
        MainCaseInfo = ClearCaseInfo;
        currentButton = CLEARAVL;
        isClear = true;
        fadeProgress = 1.0f;
        SearchNode = nullptr;
    }

    if (okHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && AVLtreeState != CreateState) {
        Value.isClickedEnter = true;
    }

    if (okHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && AVLtreeState == CreateState) {
        Nodes.isClickedEnter = true;
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
            if (CurrAVLtree.m_root != NULL) {
                currentSearchNode = CurrAVLtree.m_root;
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
                if (CurrAVLtree.m_root != NULL) {
                    currentSearchNode = CurrAVLtree.m_root;
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
            if (CurrAVLtree.m_root != NULL) {
                currentSearchNode = CurrAVLtree.m_root;
                SearchAnimationFinished = false;
                ValueSearchAnimation = valueDelete;
            }

            if (CurrAVLtree.Search(valueDelete)) {
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
                if (CurrAVLtree.m_root != NULL) {
                    currentSearchNode = CurrAVLtree.m_root;
                    SearchAnimationFinished = false;
                    ValueSearchAnimation = valueDelete;
                }
                if (CurrAVLtree.Search(valueDelete)) {
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
            SearchNode = nullptr;
            currentSearchNode = CurrAVLtree.m_root;
            SearchAnimationFinished = false;
            isSearch = true;
            timer = 0.0f;
            entered = true;
            ValueSearchAnimation = valueSearch;
        }
        else if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueSearch = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                // Kích hoạt animation sau mỗi lần thay đổi
                SearchNode = nullptr;
                currentSearchNode = CurrAVLtree.m_root;
                SearchAnimationFinished = false;
                isSearch = true;
                timer = 0.0f;
                entered = true;
                ValueSearchAnimation = valueSearch;
            }
            Value.isClickedEnter = false;
        }
    }
    else if (AVLtreeState == CreateState && !isCreateFile) {
        Nodes.update();
        if (Nodes.isEnter && Nodes.outputMessage != "") {
            valueNodes = stoi(Nodes.outputMessage);
            Nodes.isEnter = false; // Reset trạng thái ENTER 
            Nodes.outputMessage = "";

            if (valueNodes > 0) {
                isCreateRandom = true;
            }

            // Kích hoạt animation sau mỗi lần thay đổi
            isInsert = true;
            timer = 0.0f;
            entered = true;

            //SearchAnimation
            if (CurrAVLtree.m_root != NULL) {
                currentSearchNode = CurrAVLtree.m_root;
                SearchAnimationFinished = false;
                ValueSearchAnimation = valueNodes;
            }
        }
        else if (Nodes.isClickedEnter) {
            Nodes.getMessage();
            if (Nodes.outputMessage != "") {
                valueNodes = stoi(Nodes.outputMessage);
                Nodes.isEnter = false; // Reset trạng thái ENTER 
                Nodes.outputMessage = "";

                if (valueNodes > 0) {
                    isCreateRandom = true;
                }

                // Kích hoạt animation sau mỗi lần thay đổi
                isInsert = true;
                timer = 0.0f;
                entered = true;

                //SearchAnimation
                if (CurrAVLtree.m_root != NULL) {
                    currentSearchNode = CurrAVLtree.m_root;
                    SearchAnimationFinished = false;
                    ValueSearchAnimation = valueNodes;
                }
            }
            Nodes.isClickedEnter = false;
        }
    }
    else if (AVLtreeState == FileState && isCreateFile) {
        if (cont == true) {
            // Kích hoạt animation sau mỗi lần thay đổi
            cont = false;
            isInsert = true;
            timer = 0.0f;
            entered = true;

            //SearchAnimation
            if (CurrAVLtree.m_root != NULL) {
                currentSearchNode = CurrAVLtree.m_root;
                SearchAnimationFinished = false;
            }
        }
    }
    else if (AVLtreeState == ClearState) {}

    //Cập nhật tiến trình animation
    if (isInsert || isDeleting || isSearch || isClear || isCreateRandom || isCreateFile) {
        timer += GetFrameTime();
        if (entered && SearchAnimationFinished) {
            if (isInsert && AVLtreeState == InsertState) {
                CurrAVLtree.Insert(valueInsert);
            }
            else if (isDeleting) {
                CurrAVLtree.Delete(valueDelete);
            }
            else if (isSearch) {
                SearchNode = CurrAVLtree.Search(valueSearch);
            }
            else if (isCreateRandom) {
                CurrAVLtree.CreateRandomAVL(valueNodes, 1, 100);
            }
            else if (isCreateFile) {
                CurrAVLtree.readNumbersFromFile(filePath);
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
            else if (CurrAVLtree.waitinganimation == 0) {
                if (isInsert || isDeleting || isCreateRandom || isCreateFile) SubCaseInfo = -1;
                isInsert = isDeleting = isSearch = false;
                timer = 0.0f;
            }
            else if (CurrAVLtree.waitinganimation > 0) {
                CurrAVLtree.waitinganimation--;
                if (!CurrAVLtree.mroot.empty()) {
                    auto step = CurrAVLtree.mroot.front();
                    SubCaseInfo = step.second;
                    Animationmroot = step.first;
                    CurrAVLtree.mroot.pop_front();
                }
                CurrAVLtree.UpdatePos(Animationmroot);
                timer = 0.0f;
            }
        }
    }
}


void AVLTreeScreen::DrawOperationsPanel() {
    int panelMargin = 250;
    float roundness = 0.15f;  // Độ bo góc
    int segments = 20;        // Độ mượt của bo góc

    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };
    Color panelColorFade = Fade(panelColor, 0.8f);

    Rectangle leftPanel = { 0, (float)panelMargin, (float)PANEL_WIDTH, (float)(Screen_h - 2 * panelMargin) };
    DrawRectangleRounded(leftPanel, roundness, segments, panelColorFade);

    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };
    Color panelColorxFade = Fade(panelColorx, 0.8f);

    Rectangle rightPanel = { (float)PANEL_WIDTH, (float)panelMargin, (float)PANEL_WIDTH, (float)(Screen_h - 2 * panelMargin) };
    DrawRectangleRounded(rightPanel, roundness, segments, panelColorxFade);

    // Tiêu đề
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(myFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    // Vẽ nút Create 
    Color CreateColor = createHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == CREATEAVL) CreateColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(createButton, 0.2f, 4, CreateColor);
    DrawRectangleRoundedLinesEx(createButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 createSize = MeasureTextEx(myFont, "Create", 20, 2);
    DrawTextEx(myFont, "Create",
        { createButton.x + (createButton.width - createSize.x) / 2,
        createButton.y + (createButton.height - createSize.y) / 2 },
        20, 2, DARKBLUE);

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

    Vector2 clearSize = MeasureTextEx(myFont, "Clear", 18, 2);
    DrawTextEx(myFont, "Clear",
        { clearButton.x + (clearButton.width - clearSize.x) / 2,
          clearButton.y + (clearButton.height - clearSize.y) / 2 },
        18, 2, DARKBLUE);

    // Thêm nút Inorder (không viền, bo tròn)
    Color inorderColor = inorderHovered ? Color{ 25, 164, 116, 225 } : Color{ 30, 207, 145, 255 };
    if (currentButton == INORDERAVL) inorderColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(inorderButton, 0.2f, 4, inorderColor);
    Vector2 inorderSize = MeasureTextEx(myFont, "Inorder", 20, 2);
    DrawTextEx(myFont, "Inorder",
        { inorderButton.x + (inorderButton.width - inorderSize.x) / 2,
          inorderButton.y + (inorderButton.height - inorderSize.y) / 2 },
        20, 2, DARKBLUE);

    // Thêm nút Preorder (không viền, bo tròn)
    Color preorderColor = preorderHovered ? Color{ 25, 164, 116, 225 } : Color{ 30, 207, 145, 255 };
    if (currentButton == PREORDERAVL) preorderColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(preorderButton, 0.2f, 4, preorderColor);
    Vector2 preorderSize = MeasureTextEx(myFont, "Preorder", 20, 2);
    DrawTextEx(myFont, "Preorder",
        { preorderButton.x + (preorderButton.width - preorderSize.x) / 2,
          preorderButton.y + (preorderButton.height - preorderSize.y) / 2 },
        20, 2, DARKBLUE);

    // Thêm nút Postorder (không viền, bo tròn)
    Color postorderColor = postorderHovered ? Color{ 25, 164, 116, 225 } : Color{ 30, 207, 145, 255 };
    if (currentButton == POSTORDERAVL) postorderColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(postorderButton, 0.2f, 4, postorderColor);
    Vector2 postorderSize = MeasureTextEx(myFont, "Postorder", 20, 2);
    DrawTextEx(myFont, "Postorder",
        { postorderButton.x + (postorderButton.width - postorderSize.x) / 2,
          postorderButton.y + (postorderButton.height - postorderSize.y) / 2 },
        20, 2, DARKBLUE);

    DrawInfo();

    if (AVLtreeState == InsertState || AVLtreeState == DeleteState || AVLtreeState == SearchState) {
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

        Vector2 okSize = MeasureTextEx(myFont, "GO!", 18, 2);
        DrawTextEx(myFont, "GO!",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (AVLtreeState == CreateState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Nodes", fontSize, 1.f).x;
        DrawTextEx(myFont, "Nodes", { 200, 350 }, fontSize, spacing, BLACK);
        if (Nodes.text == "") Nodes.text = "Random";
        Nodes.draw();

        // Vẽ nút "File"
        int textWidth2 = MeasureTextEx(myFont, "File", fontSize, 1.f).x;
        DrawTextEx(myFont, "File", { 200, 420 }, fontSize, spacing, BLACK);

        Color fileColor = isDarkMode ? Color{ 244, 162, 83, 210 } : RAYWHITE;
        DrawRectangleRounded(fileButton, 0.2f, 4, fileColor);
        DrawRectangleRoundedLinesEx(fileButton, 0.2f, 4, 2.0f, DARKGRAY);

        Vector2 fileSize = MeasureTextEx(myFont, "Browse...", 18, 2);
        DrawTextEx(myFont, "Browse...",
            { fileButton.x + (fileButton.width - fileSize.x) / 2,
              fileButton.y + (fileButton.height - fileSize.y) / 2 },
            18, 2, BLACK);

        // Vẽ nút "OK"
        Color okColor = okHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == OKAVL) okColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(okButton, 0.2f, 4, okColor);
        DrawRectangleRoundedLinesEx(okButton, 0.2f, 4, 2.0f, GRAY);

        Vector2 okSize = MeasureTextEx(myFont, "GO!", 18, 2);
        DrawTextEx(myFont, "GO!",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (AVLtreeState == ClearState) {}

    // Vẽ nút Undo, Redo
    DrawTexture(textureUndo, Undoposition.x, Undoposition.y, WHITE);
    DrawTexture(textureRedo, Redoposition.x, Redoposition.y, WHITE);
}

void AVLTreeScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);   // Sử dụng màu nền ở chế độ hiện tại 

    // INORDER 
    if (isInordered) {
        float x = inorderButton.x + inorderButton.width + 10.0f;
        float padding = 15.0f;
        float fontSize = 25.0f;
        float spacing = 1.0f;
        float textHeight = fontSize;
        float bgPaddingX = 10.0f;
        float bgPaddingY = 5.0f;

        float totalWidth = GetTotalTraversalWidth(InTraversalValues, (int)InTraversalValues.size(), myFont, fontSize, spacing, padding);

        float targetWidth = totalWidth + 2 * bgPaddingX;
        float speed = 400.0f;  // pixels per second
        float deltaTime = GetFrameTime();

        if (InTraversalBgWidth < targetWidth) {
            InTraversalBgWidth += speed * deltaTime;
            if (InTraversalBgWidth >= targetWidth) {
                InTraversalBgWidth = targetWidth;
                isInTraversalReady = true;
            }
        }

        float bgHeight = inorderButton.height;
        float y = inorderButton.y + (inorderButton.height - textHeight) / 2.0f; // Vị trí chữ
        float bgY = inorderButton.y; // Vị trí nền

        float extendLeft = 20.0f; // khoảng muốn nền lùi trái
        Rectangle bgRect = {
            x - bgPaddingX - extendLeft,
            bgY,
            InTraversalBgWidth + extendLeft,
            bgHeight
        };
        DrawRectangleRounded(bgRect, 0.3f, 10, isDarkMode ? Color{ 250, 206, 231, 255 } : Color{ 245, 148, 203, 190 });

        if (isInTraversalReady) {
            IntraversalTimer += deltaTime;
            if (currentInIndex < (int)InTraversalValues.size() && IntraversalTimer >= 0.5f) {
                currentInIndex++;
                IntraversalTimer = 0.0f;
            }
        }

        float drawX = x;
        for (int i = 0; i < currentInIndex; ++i) {
            std::string val = std::to_string(InTraversalValues[i]);
            Vector2 pos = { drawX, y };
            DrawTextEx(myFont, val.c_str(), pos, fontSize, spacing, DARKBLUE);

            Vector2 textSize = MeasureTextEx(myFont, val.c_str(), fontSize, spacing);
            drawX += textSize.x + padding;
        }
    }

    // PREORDER
    if (isPreordered) {
        float x = preorderButton.x + preorderButton.width + 10.0f;
        float padding = 15.0f;
        float fontSize = 25.0f;
        float spacing = 1.0f;
        float textHeight = fontSize;
        float bgPaddingX = 10.0f;
        float bgPaddingY = 5.0f;

        float totalWidth = GetTotalTraversalWidth(PreTraversalValues, (int)PreTraversalValues.size(), myFont, fontSize, spacing, padding);

        float targetWidth = totalWidth + 2 * bgPaddingX;
        float speed = 400.0f;  // pixels per second
        float deltaTime = GetFrameTime();

        if (PreTraversalBgWidth < targetWidth) {
            PreTraversalBgWidth += speed * deltaTime;
            if (PreTraversalBgWidth >= targetWidth) {
                PreTraversalBgWidth = targetWidth;
                isPreTraversalReady = true;
            }
        }

        float bgHeight = preorderButton.height;
        float y = preorderButton.y + (preorderButton.height - textHeight) / 2.0f; // Vị trí chữ
        float bgY = preorderButton.y; // Vị trí nền

        float extendLeft = 20.0f; // khoảng muốn nền lùi trái
        Rectangle bgRect = {
            x - bgPaddingX - extendLeft,
            bgY,
            PreTraversalBgWidth + extendLeft,
            bgHeight
        };
        DrawRectangleRounded(bgRect, 0.3f, 10, isDarkMode ? Color{ 250, 206, 231, 255 } : Color{ 245, 148, 203, 190 });

        if (isPreTraversalReady) {
            PretraversalTimer += deltaTime;
            if (currentPreIndex < (int)PreTraversalValues.size() && PretraversalTimer >= 0.5f) {
                currentPreIndex++;
                PretraversalTimer = 0.0f;
            }
        }

        float drawX = x;
        for (int i = 0; i < currentPreIndex; ++i) {
            std::string val = std::to_string(PreTraversalValues[i]);
            Vector2 pos = { drawX, y };
            DrawTextEx(myFont, val.c_str(), pos, fontSize, spacing, DARKBLUE);

            Vector2 textSize = MeasureTextEx(myFont, val.c_str(), fontSize, spacing);
            drawX += textSize.x + padding;
        }
    }


    // POSTORDER
    if (isPostordered) {
        float x = postorderButton.x + postorderButton.width + 10.0f;
        float padding = 15.0f;
        float fontSize = 25.0f;
        float spacing = 1.0f;
        float textHeight = fontSize;
        float bgPaddingX = 10.0f;
        float bgPaddingY = 5.0f;

        float totalWidth = GetTotalTraversalWidth(PostTraversalValues, (int)PostTraversalValues.size(), myFont, fontSize, spacing, padding);

        float targetWidth = totalWidth + 2 * bgPaddingX;
        float speed = 400.0f;  // pixels per second
        float deltaTime = GetFrameTime();

        if (PostTraversalBgWidth < targetWidth) {
            PostTraversalBgWidth += speed * deltaTime;
            if (PostTraversalBgWidth >= targetWidth) {
                PostTraversalBgWidth = targetWidth;
                isPostTraversalReady = true;
            }
        }

        float bgHeight = postorderButton.height;
        float y = postorderButton.y + (postorderButton.height - textHeight) / 2.0f; // Vị trí chữ
        float bgY = postorderButton.y; // Vị trí nền

        float extendLeft = 20.0f; // khoảng muốn nền lùi trái
        Rectangle bgRect = {
            x - bgPaddingX - extendLeft,
            bgY,
            PostTraversalBgWidth + extendLeft,
            bgHeight
        };
        DrawRectangleRounded(bgRect, 0.3f, 10, isDarkMode ? Color{ 250, 206, 231, 255 } : Color{ 245, 148, 203, 190 });

        if (isPostTraversalReady) {
            PosttraversalTimer += deltaTime;
            if (currentPostIndex < (int)PostTraversalValues.size() && PosttraversalTimer >= 0.5f) {
                currentPostIndex++;
                PosttraversalTimer = 0.0f;
            }
        }

        float drawX = x;
        for (int i = 0; i < currentPostIndex; ++i) {
            std::string val = std::to_string(PostTraversalValues[i]);
            Vector2 pos = { drawX, y };
            DrawTextEx(myFont, val.c_str(), pos, fontSize, spacing, DARKBLUE);

            Vector2 textSize = MeasureTextEx(myFont, val.c_str(), fontSize, spacing);
            drawX += textSize.x + padding;
        }
    }

    // Vẽ bảng điều khiển
    DrawOperationsPanel();

    // Vẽ và cập nhật sparks
    for (int i = sparks.size() - 1; i >= 0; i--) {
        Spark& spark = sparks[i];

        // Cập nhật animation
        spark.lifetime -= GetFrameTime();
        if (spark.lifetime <= 0) {
            sparks.erase(sparks.begin() + i);
            continue;
        }

        // Tính toán alpha và scale dựa trên lifetime
        spark.alpha = spark.lifetime / 0.5f;  // Giảm dần từ 1 về 0
        spark.scale = 1.0f - spark.alpha;     // Tăng từ 0 lên 1

        // Vẽ đốm sáng
        int radius = 25 * spark.scale;
        DrawCircleGradient(
            (int)spark.position.x,
            (int)spark.position.y,
            radius,
            { 243, 15, 21, (unsigned char)(255 * spark.alpha) },
            { 243, 15, 21, 0 }  // Trong suốt ở rìa
        );
    }

    // Gọi vẽ danh sách với progress từ 0 -> 1
    float animationProgress = timer / duration;
    drawAVLtree(animationProgress, Animationmroot);
    finnishAnimation = true;

    mouse = GetMousePosition();
    if (showFileInfoPopup) {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.6f));

        // Kích thước và vị trí popup
        float popupWidth = 600;
        float popupHeight = 300;
        float popupX = (GetScreenWidth() - popupWidth) / 2;  // Căn giữa màn hình
        float popupY = (GetScreenHeight() - popupHeight) / 2;
        Rectangle popupRect = { popupX, popupY, popupWidth, popupHeight };

        // Vẽ nền bảng thông báo với góc bo tròn
        DrawRectangleRounded(popupRect, 0.2, 10, WHITE);
        DrawRectangleRoundedLines(popupRect, 0.2, 10, DARKGRAY);

        // Tiêu đề
        const char* titleText = "File Format Requirement";
        float titleWidth = MeasureTextEx(font, titleText, 27, 2).x;
        Vector2 titlePos = { popupX + (popupWidth - titleWidth) / 2, popupY + 20 };
        DrawTextEx(font, titleText, titlePos, 27, 2, PURPLE);

        // Nội dung
        const char* text =
            "* The file must have a .txt extension.\n"
            "* The numbers on each line must be separated by space.\n"
            "* The numbers represent the nodes to be inserted into \nthe AVL tree in the order they appear in the file.\n\n"
            "Example:\n"
            "   29 4 99 23 100 32 55 34 66 82 42\n"
            "   84 12 11\n";

        Vector2 textPos = { popupRect.x + 20, popupRect.y + 60 };
        DrawTextEx(font, text, textPos, 21, 2, BLACK);

        // Vẽ nút OK 
        // Kích thước và vị trí nút OK
        float buttonWidth = 100;
        float buttonHeight = 40;
        float buttonX = popupRect.x + popupRect.width / 2 - buttonWidth / 2;  // Căn giữa
        float buttonY = popupRect.y + popupRect.height - 60;
        Rectangle fileokButton = { buttonX, buttonY, buttonWidth, buttonHeight };
        float radius = 10.0f;  // Độ bo góc

        // Vẽ nút OK
        fileokHovered = CheckCollisionPointRec(mouse, fileokButton);
        Color fileokColor = fileokHovered ? Color{ 250, 228, 49, 255 } : LIGHTGRAY;
        DrawRectangleRounded(fileokButton, radius, 16, fileokColor);

        // Căn giữa chữ "OK"
        Vector2 fileokTextSize = MeasureTextEx(font, "OK", 22, 2);
        Vector2 fileokTextPos = { fileokButton.x + (fileokButton.width - fileokTextSize.x) / 2, fileokButton.y + (fileokButton.height - fileokTextSize.y) / 2 };
        DrawTextEx(font, "OK", fileokTextPos, 22, 2, BLUE);

        // Xử lý sự kiện khi nhấn OK
        if (fileokHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            showFileInfoPopup = false; // Đóng popup

            // **Mở hộp thoại chọn file**
            const char* filterPatterns[1] = { "*.txt" };
            filePath = tinyfd_openFileDialog("Choose file .txt ", "", 1, filterPatterns, NULL, 0);

            if (filePath) {
                printf("File which was choose: %s\n", filePath);
                isCreateFile = true;
                cont = true;
            }
        }
    }
}

void AVLTreeScreen::drawAVLtree(float animationProgress, AVLNode* root) {
    if (!root) return;
    // Interpolate positions using animation progress

    root->displayX = root->x * (1.0f - animationProgress) + root->newx * animationProgress;
    root->displayY = root->y * (1.0f - animationProgress) + root->newy * animationProgress;
    // Draw edges
    if (root->left) {
        DrawLine(root->displayX, root->displayY, root->left->displayX, root->left->displayY, BLACK);
        drawAVLtree(animationProgress, root->left);
    }
    if (root->right) {
        DrawLine(root->displayX, root->displayY, root->right->displayX, root->right->displayY, BLACK);
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
            CurrAVLtree.Clear(); // Xóa danh sách khi hiệu ứng kết thúc
            isClear = false;
            Animationmroot = NULL;
        }
    }
}

void AVLTreeScreen::SearchAnimation(int key) {
    if (currentSearchNode->val == key || findingSmallestRightSubTree) {
        if (isDeleting && currentSearchNode->val == key &&
            !findingSmallestRightSubTree &&
            currentSearchNode->left && currentSearchNode->right) {
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
        else {
            SearchAnimationFinished = true;
            if (isSearch) SubCaseInfo = 3;
        }
    }
    else {
        if (ValueSearchAnimation < currentSearchNode->val) {
            SubCaseInfo = 1;
            if (!currentSearchNode->left) {
                if (isSearch) SubCaseInfo = NULLCase;
                SearchAnimationFinished = true;
            }
            else {
                if (isSearch) SubCaseInfo = SmallerCase;
                currentSearchNode = currentSearchNode->left;
            }
        }
        else {
            SubCaseInfo = 1;
            if (!currentSearchNode->right) {
                if (isSearch) SubCaseInfo = NULLCase;
                SearchAnimationFinished = true;
            }
            else {
                if (isSearch) SubCaseInfo = LargerCase;
                currentSearchNode = currentSearchNode->right;
            }
        }
    }
}

float AVLTreeScreen::GetTotalTraversalWidth(const std::vector<int>& values, int currentIndex, Font font, float fontSize, float spacing, float padding) {
    float totalWidth = 0.0f;
    for (int i = 0; i < currentIndex; ++i) {
        std::string val = std::to_string(values[i]);
        Vector2 size = MeasureTextEx(font, val.c_str(), fontSize, spacing);
        totalWidth += size.x;
        if (i != currentIndex - 1) totalWidth += padding;
    }
    return totalWidth;
}

void AVLTreeScreen::DrawInfo() {
    int radius = 20; // Bán kính bo góc
    int rectWidth = 400;
    int rectHeight = 300;

    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;

    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };

    // Vẽ phần chính bên phải (không bao gồm góc trái trên)
    DrawRectangle(posX + radius, posY, rectWidth - radius, rectHeight, panelColory);

    // Vẽ phần bên dưới bên trái (tránh bo góc trên)
    DrawRectangle(posX, posY + radius, radius, rectHeight - radius, panelColory);

    // Vẽ góc bo tròn trái trên
    DrawCircleSector(
        {
        (float)(posX + radius), (float)(posY + radius)
        }, // Tâm vòng tròn
        (float)radius,
        180.0f, 270.0f, 30, panelColory
    );

    if (MainCaseInfo == InertionCaseInfo) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        int lineSpacing = 37; // Tăng khoảng cách dòng để vừa với chiều cao mới

        if (SubCaseInfo == FindingtoInsert) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding v", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == InsertingCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "insert v", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        DrawTextEx(IN4Font, "check balance factor of this and its children", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case1: this.rotateRight", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case2: this.left.rotateLeft, this.rotateRight", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case3: this.rotateLeft", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightLeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 20, RED);
        DrawTextEx(IN4Font, "       case4: this.right.rotateRight, this.rotateLeft", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == -1) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "this is balanced", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);

    }
    else if (MainCaseInfo == DeletionCaseInfo) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        int lineSpacing = 32; // Tăng khoảng cách dòng để vừa với chiều cao mới


        if (SubCaseInfo == FindingtoDelete) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding v", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == FindingtoSmallestRightSubTree) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "finding the Smallest in Right Sub Tree", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RomovevCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "delete v", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        DrawTextEx(IN4Font, "check balance factor of this and its children", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case1: this.rotateRight", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRightRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case2: this.left.rotateLeft, this.rotateRight", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case3: this.rotateLeft", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == RightLeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case4: this.right.rotateRight, this.rotateLeft", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == -1) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "this is balanced", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);

    }
    else if (MainCaseInfo == SearchCaseInfo) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        int lineSpacing = 37; // Tăng khoảng cách dòng để vừa với chiều cao mới

        if (SubCaseInfo == NULLCase)DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "If this == NULL", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == NULLCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       return null", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == ThisValueCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "else if this key == search value", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == ThisValueCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       return this ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == SmallerCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "else if this key < search value", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == SmallerCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       search right", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LargerCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "else if this key > search value", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == LargerCase) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       search left", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);

    }
    else if (MainCaseInfo == ClearCaseInfo) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        DrawTextEx(IN4Font, "Clearing the AVLtree...", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
    }
}


void AVLTreeScreen::Unload() {
    UnloadFont(AVLtreeFont);
}