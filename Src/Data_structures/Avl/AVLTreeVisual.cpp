
#include "AVLTreeVisual.h"
#include "../../UI/Screens/SettingScreen.h"

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
    ValueAlter = { {270, 450, 90, 30} };
    Nodes = { {270, 350, 90, 30}, "Random" };

    toolbar.isPlaying = true;
    toolbar.isAVL = true;

    sparks.clear();

    myTable.Init(Screen_w - 700, Screen_h - 310, 290, 310);

    infoMessage = "";
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

    myTable.Update();
    toolbar.Update();

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

    // Kiểm tra hover vào nút "Update"
    updateHovered = CheckCollisionPointRec(mouse, updateButton);

    // Kiểm tra hover vào nút "File"
    fileHovered = CheckCollisionPointRec(mouse, fileButton);

    // Kiểm tra hover vào nút "OK"
    okHovered = CheckCollisionPointRec(mouse, okButton);


    if (toolbar.isBack) timer = 1.01f;
    if (toolbar.isNext) timer = 1.01f;

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
    if (clearHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CurrAVLtree.m_root != NULL) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = ClearState;
        MainCaseInfo = ClearCaseInfo;
        currentButton = CLEARAVL;
        isClear = true;
        fadeProgress = 1.0f;
        SearchNode = nullptr;
        infoMessage = "";
    }

    // Kiểm tra nút Update
    if (updateHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isCreateFile = false;
        isCreateRandom = false;
        AVLtreeState = UpdateState;
        MainCaseInfo = UpdateCaseInfo;
        currentButton = UPDATEAVL;
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
            for (auto x : CurrAVLtree.set) {
                if (x->currentstate == 3) {
                    x->currentstate = 1;
                }
            }
            valueInsert = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            if (CurrAVLtree.GetSize() < 20) {
                // Kích hoạt animation sau mỗi lần thay đổi
                isInsert = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "";
            }
            else {
                infoMessage = "The number of nodes has an upper limit\n of 20. Please do it again!";
            }
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                for (auto x : CurrAVLtree.set) {
                    if (x->currentstate == 3) {
                        x->currentstate = 1;
                    }
                }
                valueInsert = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                if (CurrAVLtree.GetSize() < 20) {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    isInsert = true;
                    timer = 0.0f;
                    entered = true;
                    infoMessage = "";
                }
                else {
                    infoMessage = "The number of nodes has an upper limit\n of 20. Please do it again!";
                }

            }
            Value.isClickedEnter = false;
        }
    }
    else if (AVLtreeState == DeleteState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            for (auto x : CurrAVLtree.set) {
                if (x->currentstate == 3) {
                    x->currentstate = 1;
                }
            }
            valueDelete = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            //SearchAnimation
            if (CurrAVLtree.m_root != NULL) {
                ValueSearchAnimation = valueDelete;
            }
            isDeleting = true;
            timer = 0.0f;
            entered = true;
            infoMessage = "";
        }
        else if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                for (auto x : CurrAVLtree.set) {
                    if (x->currentstate == 3) {
                        x->currentstate = 1;
                    }
                }
                valueDelete = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                // SearchAnimation
                if (CurrAVLtree.m_root != NULL) {
                    ValueSearchAnimation = valueDelete;
                }
                // Kích hoạt animation sau mỗi lần thay đổi
                isDeleting = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "";
                Value.isClickedEnter = false;
            }
        }

    }
    else if (AVLtreeState == SearchState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            for (auto x : CurrAVLtree.set) {
                if (x->currentstate == 3) {
                    x->currentstate = 1;
                }
            }
            SubCaseInfo = 0;
            valueSearch = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            // Kích hoạt animation sau mỗi lần thay đổi
            if (SearchNode)SearchNode->currentstate = 1;
            isSearch = true;
            timer = 0.0f;
            entered = true;
            infoMessage = "";
            ValueSearchAnimation = valueSearch;
        }
        else if (Value.isClickedEnter) {
            for (auto x : CurrAVLtree.set) {
                if (x->currentstate == 3) {
                    x->currentstate = 1;
                }
            }
            SubCaseInfo = 0;
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueSearch = stoi(Value.outputMessage);
                Value.isEnter = false; // Reset trạng thái ENTER 
                Value.outputMessage = "";

                // Kích hoạt animation sau mỗi lần thay đổi
                if (SearchNode)SearchNode->currentstate = 1;
                isSearch = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "";
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

            if (valueNodes > 0 && valueNodes < 21) {
                isCreateRandom = true;
                // Kích hoạt animation sau mỗi lần thay đổi
                isInsert = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "";
                //SearchAnimation
                if (CurrAVLtree.m_root != NULL) {
                    ValueSearchAnimation = valueNodes;
                }
            }
            else {
                infoMessage = "The number of nodes has an upper limit\n of 20. Please do it again!";
            }
        }
        else if (Nodes.isClickedEnter) {
            Nodes.getMessage();
            if (Nodes.outputMessage != "") {
                valueNodes = stoi(Nodes.outputMessage);
                Nodes.isEnter = false; // Reset trạng thái ENTER 
                Nodes.outputMessage = "";

                if (valueNodes > 0 && valueNodes < 21) {
                    isCreateRandom = true;
                    // Kích hoạt animation sau mỗi lần thay đổi
                    isInsert = true;
                    timer = 0.0f;
                    entered = true;
                    infoMessage = "";
                    //SearchAnimation
                    if (CurrAVLtree.m_root != NULL) {
                        ValueSearchAnimation = valueNodes;
                    }
                }
                else {
                    infoMessage = "The number of nodes has an upper limit\n of 20. Please do it again!";
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
            infoMessage = "";
        }
    }
    else if (AVLtreeState == ClearState) {}
    else if (AVLtreeState == UpdateState) {
        Value.update();
        ValueAlter.update();
        if (Value.isEnter) {
            ValueAlter.tt = 1;
            ValueAlter.sss();
        }
        if (ValueAlter.isEnter) {
            Value.tt = 1;
            Value.sss();
        }
        if (Value.outputMessage != "" && ValueAlter.outputMessage != "" && (Value.isEnter || ValueAlter.isEnter)) {
            for (auto x : CurrAVLtree.set) {
                if (x->currentstate == 3) {
                    x->currentstate = 1;
                }
            }
            valueAlter = stoi(ValueAlter.outputMessage);
            valueAltered = stoi(Value.outputMessage);
            ValueAlter.isEnter = false; // Reset trạng thái ENTER 
            Value.isEnter = false; // Reset trạng thái ENTER
            ValueAlter.outputMessage = "";
            Value.outputMessage = "";
            isUpdate = true;
            timer = 0.0f;
            entered = true;
        }
    }

    //Cập nhật tiến trình animation
    if (isInsert || isDeleting || isSearch || isClear || isCreateRandom || isCreateFile || !toolbar.isBack || toolbar.isBack) {
        timer += GetFrameTime();
        if (entered && CurrAVLtree.currentanimation == CurrAVLtree.mroot.size() - 1 && ((!toolbar.isPlaying && toolbar.isBack) || (!toolbar.isPlaying && !toolbar.isBack) || (!toolbar.isPlaying && CurrAVLtree.currentanimation == CurrAVLtree.mroot.size() - 1) || !!toolbar.isPlaying || isCreateRandom || isCreateFile)) {
            if (isInsert && AVLtreeState == InsertState) {
                CurrAVLtree.Insert(valueInsert);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }
            else if (isDeleting) {
                CurrAVLtree.Delete(valueDelete);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }
            else if (isSearch) {
                SearchNode = CurrAVLtree.Search(valueSearch);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }
            else if (isCreateRandom) {
                CurrAVLtree.CreateRandomAVL(valueNodes, 1, 100);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }
            else if (isCreateFile) {
                CurrAVLtree.readNumbersFromFile(filePath);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }
            else if (isUpdate) {
                CurrAVLtree.UpdateValue(valueAltered, valueAlter);
                InTraversalValues.clear();
                PreTraversalValues.clear();
                PostTraversalValues.clear();
                CurrAVLtree.InorderTraversal(InTraversalValues);
                CurrAVLtree.PreorderTraversal(PreTraversalValues);
                CurrAVLtree.PostorderTraversal(PostTraversalValues);
            }

            timer = toolbar.duration + 0.1f;
            entered = !entered;
        }
        if (timer >= toolbar.duration) {
            if (toolbar.isBack) {
                if (CurrAVLtree.currentanimation >= 1) {
                    CurrAVLtree.currentanimation--;
                    //stop search animation
                    //undo
                    auto step = CurrAVLtree.mroot[CurrAVLtree.currentanimation];
                    SubCaseInfo = step.second;
                    Animationmroot = step.first;
                    //update Pos
                    CurrAVLtree.UpdateOrderPos(-1);
                    CurrAVLtree.UpdatePos(Animationmroot);
                    //reset time
                    toolbar.isPlaying = false;
                }
                toolbar.isBack = false;
                timer = 0.0f;
            }
            else if (toolbar.isNext) {
                if (CurrAVLtree.currentanimation < CurrAVLtree.mroot.size() - 1) {
                    CurrAVLtree.currentanimation++;
                    //stop search animation
                    //redo
                    auto step = CurrAVLtree.mroot[CurrAVLtree.currentanimation];
                    SubCaseInfo = step.second;
                    Animationmroot = step.first;
                    //update Pos
                    CurrAVLtree.UpdateOrderPos(1);
                    CurrAVLtree.UpdatePos(Animationmroot);
                    //reset time
                    toolbar.isPlaying = false;
                }
                toolbar.isNext = false;
                timer = 0.0f;
            }
            else if (!!toolbar.isPlaying && CurrAVLtree.currentanimation < CurrAVLtree.mroot.size() - 1) {
                ++CurrAVLtree.currentanimation;
                auto step = CurrAVLtree.mroot[CurrAVLtree.currentanimation];
                SubCaseInfo = step.second;
                Animationmroot = step.first;
                CurrAVLtree.UpdateOrderPos(1);
                CurrAVLtree.UpdatePos(Animationmroot);
                timer = 0.0f;
            }
            else if (CurrAVLtree.currentanimation == CurrAVLtree.mroot.size() - 1) {
                SubCaseInfo = -1;
                isInsert = isDeleting = isSearch = isCreateRandom = isUpdate = isCreateFile = false;
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
    Color SearchColor = searchHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == SEARCHAVL) SearchColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(searchButton, 0.2f, 4, SearchColor);
    DrawRectangleRoundedLinesEx(searchButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 searchSize = MeasureTextEx(myFont, "Search", 20, 2);
    DrawTextEx(myFont, "Search",
        { searchButton.x + (searchButton.width - searchSize.x) / 2,
          searchButton.y + (searchButton.height - searchSize.y) / 2 },
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
    // Vẽ nút Update
    Color UpdateColor = updateHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == UPDATEAVL) UpdateColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(updateButton, 0.2f, 4, UpdateColor);
    DrawRectangleRoundedLinesEx(updateButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 updateSize = MeasureTextEx(myFont, "Update", 20, 2);
    DrawTextEx(myFont, "Update",
        { updateButton.x + (updateButton.width - updateSize.x) / 2,
          updateButton.y + (updateButton.height - updateSize.y) / 2 },
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
    else if (AVLtreeState == UpdateState) {
        int fontSize = 20;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Old Val", fontSize, 1.f).x;
        DrawTextEx(myFont, "Old Val", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();

        MeasureTextEx(myFont, "New Val", fontSize, 1.f).x;
        DrawTextEx(myFont, "New Val", { 200, 450 }, fontSize, spacing, BLACK);
        ValueAlter.draw();

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

    int textPadding = 10;
    Vector2 textSize = MeasureTextEx(GetFontDefault(), infoMessage.c_str(), 20, 1);

    // Tọa độ để căn giữa text theo chiều ngang bảng, và nằm phía trên bảng một chút
    int textX = posX + (400 - (int)textSize.x) / 2;
    int textY = posY - (int)textSize.y - textPadding;

    // Vẽ text bằng màu đỏ
    DrawTextEx(myFont, infoMessage.c_str(), { (float)textX, (float)textY - 10 }, 20, 1, RED);

}

void AVLTreeScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);   // Sử dụng màu nền ở chế độ hiện tại 

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
        spark.alpha = spark.lifetime / 0.5f;
        spark.scale = 1.0f - spark.alpha;

        // Vẽ đốm sáng
        int radius = 25 * spark.scale;
        DrawCircleGradient(
            (int)spark.position.x,
            (int)spark.position.y,
            radius,
            { 243, 15, 21, (unsigned char)(255 * spark.alpha) },
            { 243, 15, 21, 0 }
        );
    }

    // Gọi vẽ danh sách với progress từ 0 -> 1
    float animationProgress = timer / toolbar.duration;
    drawAVLtree(animationProgress, Animationmroot);
    finnishAnimation = true;

    myTable.SetData(InTraversalValues, PreTraversalValues, PostTraversalValues);
    myTable.Draw();
    toolbar.Draw();

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
    if (animationProgress >= 0.9f) {
        root->x = root->newx;
        root->y = root->newy;
    }
    // Draw edges
    if (root->left && root->left->currentAnimationNode >= 0) {
        DrawLineEx(Vector2{ (float)root->displayX, (float)root->displayY }, Vector2{ (float)root->left->displayX, (float)root->left->displayY }, 2.0f, BLACK);        
        drawAVLtree(animationProgress, root->left);
    }
    if (root->right && root->right->currentAnimationNode >= 0) {
        DrawLineEx(Vector2{ (float)root->displayX, (float)root->displayY }, Vector2{ (float)root->right->displayX, (float)root->right->displayY }, 2.0f, BLACK);
        drawAVLtree(animationProgress, root->right);
    }

    // Draw node
    if (root->currentstate == 3)DrawCircleV({ root->displayX,root->displayY }, 20, ORANGE);
    else if (root->currentstate == 2)DrawCircleV({ root->displayX,root->displayY }, 20, RED);
    else DrawCircleV({ root->displayX,root->displayY }, 20, WHITE);
    string nodeText = TextFormat("%d", root->val);
    Vector2 textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);

    Vector2 textPos = { root->displayX - textSize.x / 2,  root->displayY - textSize.y / 2 }; // Căn giữa Node 
    DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, fadeProgress));

    if (root && (root->currentstate == 3 || root->currentstate == 2)) {
        nodeText = TextFormat("%d", root->dif);
        textSize = MeasureTextEx(myFont, nodeText.c_str(), 15, 1.25);

        textPos = { root->displayX - textSize.x / 2 - 27,  root->displayY - textSize.y / 2 };
        DrawTextEx(myFont, nodeText.c_str(), textPos, 15, 1.25, Fade(BLACK, fadeProgress));
    }
    if (isClear) {
        fadeProgress -= GetFrameTime();
        if (fadeProgress <= 0.0f) {
            fadeProgress = 1.0f;
            CurrAVLtree.Clear(); // Xóa danh sách khi hiệu ứng kết thúc
            InTraversalValues = PreTraversalValues = PostTraversalValues = {};
            isClear = false;
            Animationmroot = NULL;
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

    int padding = 10;
    int fontSize = 20;
    Color textColor = isDarkMode ? RAYWHITE : RED;

    Vector2 textSize = MeasureTextEx(myFont, infoMessage.c_str(), fontSize, 1);
    Vector2 textPos = {
        posX + (rectWidth - textSize.x) / 2,
        posY - textSize.y - padding
    };

    if (fmod(GetTime(), 1.0) < 0.7)
    {
        DrawTextEx(myFont, infoMessage.c_str(), textPos, fontSize, 1, textColor);
    }

    if (MainCaseInfo == InertionCaseInfo && AVLtreeState == InsertState) {
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

        if (SubCaseInfo == RightLeftRotationCaseInfo) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "       case4: this.right.rotateRight, this.rotateLeft", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == -1) DrawRectangle(posX, posY, rectWidth, 35, RED);
        DrawTextEx(IN4Font, "this is balanced", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);

    }
    else if (MainCaseInfo == DeletionCaseInfo && AVLtreeState == DeleteState) {
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
    else if (MainCaseInfo == SearchCaseInfo && AVLtreeState == SearchState) {
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
    else if (MainCaseInfo == ClearCaseInfo && AVLtreeState == ClearState) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 10;
        DrawTextEx(IN4Font, "Clearing the AVLtree...", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
    }
    else if (MainCaseInfo == UpdateCaseInfo && AVLtreeState == UpdateState) {
        // Hiển thị nội dung
        int textX = posX + 10;
        int textY = posY + 3;
        int lineSpacing = 22; // Tăng khoảng cách dòng để vừa với chiều cao mới

        DrawTextEx(IN4Font, "while loop to find v", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueGreater) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "   if (cur->val < v) ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueGreater) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "       if((cur-alterValue) * (cur-v)<= 0) fail update ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueGreater) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "       else cur = cur->right;", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueSmaller) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "   else if (cur->val > v) ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueSmaller) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "       if((cur-alterValue) * (cur-v)<= 0) fail update ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingValueSmaller) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "       else cur = cur->left; ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingLargest) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "find the largest in leftsubtree ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingLargest) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "   if (largest >= alterValue) fail update ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingSmallest) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "find the smallest in rightsubtree ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == findingSmallest) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "   if (smallest <= alterValue) fail update ", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == alterSuccess) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "success update", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;

        if (SubCaseInfo == unSuccess) DrawRectangle(posX, posY, rectWidth, 30, RED);
        DrawTextEx(IN4Font, "else unsuccess update", { (float)textX, (float)textY }, 20, 2, isDarkMode ? DARKBLUE : WHITE);
        textY += lineSpacing;
        posY += lineSpacing;



        }
}


void AVLTreeScreen::Unload() {
    UnloadFont(AVLtreeFont);
}