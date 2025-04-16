#include "LinkedListVisual.h"
#include "../../Constants.h"
#include "../../UI/Screens/SettingScreen.h"

#include <string>
#include <unordered_set>


void LinkedListScreen::handleButtonClick(SelectedButton newButton, TextBox& textBox) {
    if (currentButton != newButton) {
        textBox.text = ""; // Reset lại nội dung trong textBox 
        textBox.pos = 0;
    }
}

// Hỗ trợ linked list xuống dòng khi gặp lề màn hình 
Vector2 LinkedListScreen::TargetPos(int index) {
    Vector2 targetPos;
    if (index < 11) {
        // Hàng đầu tiên (từ trái qua phải)
        targetPos = { startPos.x + index * nodeSpacing, startPos.y };
    }
    else if (index >= 11 && index < 22) {
        // Hàng thứ hai (từ phải qua trái)
        int reversedIndex = index - 11; // Đếm số phần tử sau 11
        targetPos = { startPos2ndrow.x - reversedIndex * nodeSpacing, startPos2ndrow.y };
    }
    else if (index >= 22) {
        int reversedIndex = index - 22;
        targetPos = { startPos3ndrow.x + reversedIndex * nodeSpacing, startPos3ndrow.y };
    }
    return targetPos;
}

void LinkedListScreen::SaveStateForUndo(SelectedButton type, int value, int position = -1) {
    undoStack.push_back(Command(type, value, position));
}

void LinkedListScreen::Undo() {
    if (!undoStack.empty()) {
        auto cmd = undoStack.back();
        redoStack.push_back(cmd);
        undoStack.pop_back();

        // Thực hiện thao tác ngược lại
        if (cmd.type == SelectedButton::INSERTHEAD) {
            linkedList.DeleteAtHead(); // Xóa nút đầu
        }
        else if (cmd.type == SelectedButton::INSERTTAIL) {
            linkedList.DeleteAtTail(); // Xóa nút cuối
        }
        else if (cmd.type == SelectedButton::INSERTPOS) {
            linkedList.DeleteAtPosition(cmd.position); // Xóa tại vị trí
        }
        else if (cmd.type == SelectedButton::DELETE) {
            linkedList.InsertAtPosition(cmd.value, cmd.position); // Thêm lại giá trị đã xóa
        }

        infoMessage = "Undo performed.";
        isUndo = true;
        isHeadInserting = isTailInserting = isPosInserting = isDeleting = isSearch = false;
        timer = 0.0f;
    }
    else {
        infoMessage = "Nothing to undo.";
    }
}

void LinkedListScreen::Redo() {
    if (!redoStack.empty()) {
        auto cmd = redoStack.back();
        undoStack.push_back(cmd);
        redoStack.pop_back();

        // Thực hiện lại thao tác
        if (cmd.type == SelectedButton::INSERTHEAD) {
            linkedList.InsertAtHead(cmd.value);
        }
        else if (cmd.type == SelectedButton::INSERTTAIL) {
            linkedList.InsertAtTail(cmd.value);
        }
        else if (cmd.type == SelectedButton::INSERTPOS) {
            linkedList.InsertAtPosition(cmd.value, cmd.position);
        }
        else if (cmd.type == SelectedButton::DELETE) {
            linkedList.DeleteValue(cmd.value);
        }

        infoMessage = "Redo performed.";
        isRedo = true;
        isHeadInserting = isTailInserting = isPosInserting = isDeleting = isSearch = false;
        timer = 0.0f;
    }
    else {
        infoMessage = "Nothing to redo.";
    }
}

void LinkedListScreen::Init() {
    // Load font
    linkedListFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(linkedListFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font cho INFO;
    IN4Font = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(IN4Font.texture, TEXTURE_FILTER_BILINEAR);

    currentButton = NONE;

    Value = { {270, 350, 90, 30} };
    Index = { {270, 400, 90, 30} };

    Nodes = { {270, 350, 90, 30}, "Random" };
    indexAnimation = -1;
    indexSearch = -1;
    indexDelete = -1;
}

float LinkedListScreen::Clamp(float value, float minValue, float maxValue) {
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

float LinkedListScreen::SmoothStep(float a, float b, float t) {
    t = Clamp(t, 0.0f, 1.0f); // Đảm bảo t trong khoảng [0,1]
    return a + (b - a) * t;
}

void LinkedListScreen::Update(int& state) {
    mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Insert"
    insertHovered = CheckCollisionPointRec(mouse, insertButton);

    // Kiểm tra hover vào nút "Delete"
    deleteHovered = CheckCollisionPointRec(mouse, deleteButton);

    // Kiểm tra hover vào nút "Search"
    searchHovered = CheckCollisionPointRec(mouse, searchButton);

    // Kiểm tra hover vào nút "Clean"
    cleanHovered = CheckCollisionPointRec(mouse, cleanButton);

    // Kiểm tra hover vào nút "Insert Head"
    insertAtHeadHovered = CheckCollisionPointRec(mouse, insertHeadButton);

    // Kiểm tra hover vào nút "Insert Tail"
    insertAtTailHovered = CheckCollisionPointRec(mouse, insertTailButton);

    // Kiểm tra hover vào nút "Insert Pos"
    insertPosHovered = CheckCollisionPointRec(mouse, insertPosButton);

    // Kiểm tra hover vào nút "Create"
    createHovered = CheckCollisionPointRec(mouse, createButton);

    // Kiểm tra hover vào nút "File"
    fileHovered = CheckCollisionPointRec(mouse, fileButton);

    // Kiểm tra hover vào nút "OK"
    okHovered = CheckCollisionPointRec(mouse, okButton);

    // Kiểm tra nút Create 
    if (createHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = CreateState;
        currentButton = CREATE;
    }

    // Kiểm tra nút "File"
    if (linkedlistState == CreateState && fileHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showFileInfoPopup = true; // Hiển thị bảng thông báo
        linkedlistState = FileState;
        currentButton = FILELINKEDLIST;
    }

    // Kiểm tra nút Insert
    if (insertHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showInsertOptions = !showInsertOptions;
    }

    // Xử lý sự kiện khi nhấn vào nút "OK"
    if (okHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && linkedlistState != CreateState) {
        Value.isClickedEnter = true;
    }
    if (okHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && linkedlistState == CreateState) {
        Nodes.isClickedEnter = true;
    }

    toolbar.Update();
    if (toolbar.isPlaying && !isHeadInserting && !isTailInserting  && !isCreateRandom && !isCreateFile && linkedlistState != DeleteState && linkedlistState != SearchState && linkedlistState != InsertPosState ) {
        toolbar.isPlaying = false;
    }

    if (toolbar.isBack) {
        if (!toolbar.isPlaying && stepbystep && indexAnimation >= 0 && indexAnimation <= linkedList.getSize() - 1) {
            if (linkedlistState == SearchState) {
                if (indexAnimation <= indexSearch)indexAnimation -= 1;
			}
			else if (linkedlistState == DeleteState) {
                if (indexAnimation == indexDelete)Undo();
				if (indexAnimation <= indexDelete)indexAnimation -= 1;
			}
			else if (linkedlistState == InsertPosState) {
                if (indexAnimation == indexInsert)Undo();
				if (indexAnimation <= indexInsert)indexAnimation -= 1;
			}
            else indexAnimation -= 1;
        }
        else Undo();
    }

    if (toolbar.isNext) {
        if (!toolbar.isPlaying && stepbystep && indexAnimation >= 0 && indexAnimation <= linkedList.getSize() - 1) {
            if (linkedlistState == SearchState) {
                if (indexAnimation < indexSearch)indexAnimation += 1;
            }
			else if (linkedlistState == DeleteState) {
				if (indexAnimation < indexDelete)indexAnimation += 1;
                if (indexAnimation == indexDelete)Redo();

			}
			else if (linkedlistState == InsertPosState) {
                if (indexAnimation <= indexInsert - 1) indexAnimation += 1;
                if (indexAnimation == indexInsert )Redo();
			}
            else indexAnimation += 1;
        }
        else Redo();
    }

    // Animation:  Điều chỉnh offset khi mở menu Insert
    if (showInsertOptions) {
        // Mở menu, nút Head, Tail, Pos sẽ mất ngay lập tức
        insertHeadAlpha = SmoothStep(insertHeadAlpha, 0.0f, 0.06f);  // Giảm alpha của Head từ từ
        insertTailAlpha = SmoothStep(insertTailAlpha, 0.0f, 0.06f);  // Giảm alpha của Tail từ từ
        insertPosAlpha = SmoothStep(insertPosAlpha, 0.0f, 0.06f);    // Giảm alpha của Pos từ từ
        insertOptionsOffset = SmoothStep(insertOptionsOffset, 100.0f, 0.03f);

        // Cập nhật vị trí của các nút Delete, Reverse, Clean
        insertHeadButton.y = insertButton.y + 45;
        insertTailButton.y = insertButton.y + 80;
        insertPosButton.y = insertButton.y + 115;

        deleteButton.y = 430 + insertOptionsOffset;
        searchButton.y = 480 + insertOptionsOffset;
        cleanButton.y = 530 + insertOptionsOffset;

        // Nếu Insert đang mở, kiểm tra các nút con
        if (CheckCollisionPointRec(mouse, insertHeadButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertHeadState;
            handleButtonClick(INSERTHEAD, Value);
            currentButton = INSERTHEAD;
        }
        if (CheckCollisionPointRec(mouse, insertTailButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertTailState;
            handleButtonClick(INSERTTAIL, Value);
            currentButton = INSERTTAIL;
        }
        if (CheckCollisionPointRec(mouse, insertPosButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertPosState;
            handleButtonClick(INSERTPOS, Value);
            handleButtonClick(INSERTPOS, Index);
            currentButton = INSERTPOS;
        }
    }
    else {
        insertHeadAlpha = 0.0f;  // Giảm alpha của Head ngay lập tức
        insertTailAlpha = 0.0f;  // Giảm alpha của Tail ngay lập tức
        insertPosAlpha = 0.0f;   // Giảm alpha của Pos ngay lập tức

        // Cập nhật vị trí của các nút khi menu đóng
        insertOptionsOffset = SmoothStep(insertOptionsOffset, 0.0f, 0.03f); // Menu sẽ thu lại khi đóng

        // Nếu menu đã đóng xong, reset lại các nút khác
        deleteButton.y = 430 + insertOptionsOffset;
        searchButton.y = 480 + insertOptionsOffset;
        cleanButton.y = 530 + insertOptionsOffset;
    }

    // Kiểm tra nút Delete
    if (CheckCollisionPointRec(mouse, deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = DeleteState;
        handleButtonClick(DELETE, Value);
        currentButton = DELETE;
    }

    // Kiểm tra nút Reverse
    if (CheckCollisionPointRec(mouse, searchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = SearchState;
        currentButton = SEARCH;
    }

    // Kiểm tra nút Clean 
    if (CheckCollisionPointRec(mouse, cleanButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = ClearState;
        currentButton = CLEAN;
    }

    if (linkedlistState == InsertHeadState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueInsert = stoi(Value.outputMessage);
            infoMessage = "Inserting " + to_string(valueInsert) + " at Head of Linked List.";  // Cập nhật infoMessage
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            if (linkedList.getSize() == 33) {
                infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
            }
            else {
                // Kích hoạt animation sau mỗi lần thay đổi
                toolbar.isPlaying = true;
                isHeadInserting = true;
                timer = 0.0f;
                entered = true;
            }
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueInsert = stoi(Value.outputMessage);
                infoMessage = "Inserting " + to_string(valueInsert) + " at Head of Linked List.";  // Cập nhật infoMessage
                Value.outputMessage = "";

                if (linkedList.getSize() == 33) {
                    infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
                }
                else {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    toolbar.isPlaying = true;
                    isHeadInserting = true;
                    timer = 0.0f;
                    entered = true;
                }
            }
            Value.isClickedEnter = false;
        }
    }
    else if (linkedlistState == InsertTailState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueInsert = stoi(Value.outputMessage);
            infoMessage = "Inserting " + to_string(valueInsert) + " at Tail of Linked List.";  // Cập nhật infoMessage
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            if (linkedList.getSize() == 33) {
                infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
            }
            else {
                // Kích hoạt animation sau mỗi lần thay đổi
                toolbar.isPlaying = true;
                isTailInserting = true;
                timer = 0.0f;
                entered = true;
            }
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueInsert = stoi(Value.outputMessage);
                infoMessage = "Inserting " + to_string(valueInsert) + " at Tail of Linked List.";  // Cập nhật infoMessage
                Value.outputMessage = "";

                if (linkedList.getSize() == 33) {
                    infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
                }
                else {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    toolbar.isPlaying = true;
                    isTailInserting = true;
                    timer = 0.0f;
                    entered = true;
                }
            }
            Value.isClickedEnter = false;
        }
    }
    else if (linkedlistState == InsertPosState) {
        Value.update();
        Index.update();
        if (Value.isEnter) {
            Index.tt = 1;
            Index.sss();
        }
        if (Index.isEnter) {
            Value.tt = 1;
            Value.sss();
        }
        if (Value.outputMessage != "" && Index.outputMessage != "" && (Value.isEnter || Index.isEnter)) {
            valueInsert = stoi(Value.outputMessage);
            indexInsert = stoi(Index.outputMessage);
            if (indexInsert < linkedList.getSize() + 1) {
                infoMessage = "Inserting " + to_string(valueInsert) + " at Position " + to_string(indexInsert) + " of Linked \nList (0-indexed).";  // Cập nhật infoMessage
                indexAnimation = 0;
            }
            else {
                infoMessage = "The size of Linked List is smaller \nthan the position you need.";
            }
            Value.isEnter = false; // Reset trạng thái ENTER 
            Index.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            Index.outputMessage = "";

            if (linkedList.getSize() == 33) {
                infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
            }
            else {
                // Kích hoạt animation sau mỗi lần thay đổi
                isPosInserting = true;
                timer = 0.0f;
                entered = true;
            } 
        }
        if (Value.isClickedEnter) {
            Index.isClickedEnter = true;
            Value.getMessage();
            Index.getMessage();
            Value.text = "";
            Index.text = "";
            if (Value.outputMessage != "" && Index.outputMessage != "") {
                valueInsert = stoi(Value.outputMessage);
                indexInsert = stoi(Index.outputMessage);

                if (indexInsert < linkedList.getSize() + 1) {
                    indexAnimation = 0;
                    infoMessage = "Inserting " + to_string(valueInsert) + " at Position " + to_string(indexInsert) + " of Linked \nList (0-indexed).";  // Cập nhật infoMessage
                }
                else {
                    infoMessage = "The size of Linked List is smaller \nthan the position you need.";
                }

                Value.outputMessage = "";
                Index.outputMessage = "";

                if (linkedList.getSize() == 33) {
                    infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
                }
                else {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    isPosInserting = true;
                    timer = 0.0f;
                    entered = true;
                }
            }
            Value.isClickedEnter = false;
            Index.isClickedEnter = false;
        }
    }
    else if (linkedlistState == DeleteState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueDelete = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            indexAnimation = 0;
			indexDelete = linkedList.GetPosition2(valueDelete);
            if (linkedList.SearchNode(valueDelete)) {
                // Kích hoạt animation sau mỗi lần thay đổi
                isDeleting = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "Deleting the first node with value\n" + to_string(valueDelete) + " in Linked List.";  // Cập nhật infoMessage
            }
            else {
                isDeleting = true;
                timer = 0.0f;
                infoMessage = "There is no node with the value\n" + to_string(valueDelete) + " in the current Linked List.";
            }
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueDelete = stoi(Value.outputMessage);
                indexAnimation = 0;
                indexDelete = linkedList.GetPosition2(valueDelete);
                if (linkedList.SearchNode(valueDelete)) {
                    // Kích hoạt animation sau mỗi lần thay đổi
                    isDeleting = true;
                    timer = 0.0f;
                    infoMessage = "Deleting the first node with value\n" + to_string(valueDelete) + " in Linked List.";  // Cập nhật infoMessage
                }
                else {
                    isDeleting = true;
                    timer = 0.0f;
                    infoMessage = "There is no node with the value\n" + to_string(valueDelete) + " in the current Linked List.";
                }
                Value.outputMessage = "";
            }
            Value.isClickedEnter = false;
        }
    }
    else if (linkedlistState == SearchState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueSearch = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            indexAnimation = 0;
            indexSearch = linkedList.GetPosition2(valueSearch);

            // Kích hoạt animation sau mỗi lần thay đổi
            isSearch = true;
            timer = 0.0f;
            entered = true;
        }
        if (Value.isClickedEnter) {
            Value.getMessage();
            if (Value.outputMessage != "") {
                valueSearch = stoi(Value.outputMessage);
                Value.outputMessage = "";
                indexAnimation = 0;
                indexSearch = linkedList.GetPosition2(valueSearch);

                // Kích hoạt animation sau mỗi lần thay đổi
                isSearch = true;
                timer = 0.0f;
                entered = true;
            }
            Value.isClickedEnter = false;
        }
        if (!linkedList.SearchNode(valueSearch)) {
            infoMessage = "There is no node with the value \n" + to_string(valueSearch) + " in the current Linked List.";
        }
    }
    else if (linkedlistState == ClearState) {
        toolbar.isPlaying = true;
        isClean = true;
        infoMessage = "Clearing the Linked List...";
    }
    else if (linkedlistState == CreateState && !isCreateFile) {
        Nodes.update();
        if (Nodes.isEnter && Nodes.outputMessage != "") {
            indexAnimation = -1;
            valueNodes = stoi(Nodes.outputMessage);
            Nodes.isEnter = false;
            Nodes.outputMessage = "";

            if (valueNodes > 33) {
                infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
            }

            if (valueNodes > 0 && valueNodes <= 33) {
                isCreateRandom = true;
            }

            // Kích hoạt animation sau mỗi lần thay đổi
            toolbar.isPlaying = true;
            timer = 0.0f;
            entered = true;
        }
        else if (Nodes.isClickedEnter) {
            Nodes.getMessage();
            if (Nodes.outputMessage != "") {
                 indexAnimation = -1;
                valueNodes = stoi(Nodes.outputMessage);
                Nodes.isEnter = false;
                Nodes.outputMessage = "";

                if (valueNodes > 33) {
                    infoMessage = "The number of nodes has an upper limit\n of 33. Please do it again!";
                }

                if (valueNodes > 0 && valueNodes <= 33) {
                    isCreateRandom = true;
                }

                // Kích hoạt animation sau mỗi lần thay đổi
                toolbar.isPlaying = true;
                timer = 0.0f;
                entered = true;
            }
            Nodes.isClickedEnter = false;
        }
    }
    else if (linkedlistState == FileState && isCreateFile) {
        if (cont == true) {
            // Kích hoạt animation sau mỗi lần thay đổi
            toolbar.isPlaying = true;
            cont = false;
            isCreateFile = true;
            timer = 0.0f;
            entered = true;
        }
    }

    if (linkedlistState != SearchState)   SearchNode = nullptr;

    if ((linkedlistState == SearchState || linkedlistState == DeleteState || linkedlistState == InsertPosState) && !toolbar.isPlaying) stepbystep = true;
	else stepbystep = false;

    // Cập nhật tiến trình animation
    if ((isHeadInserting || isTailInserting || isPosInserting || isDeleting || isSearch || isCreateFile || isCreateRandom) && (toolbar.isPlaying||stepbystep)) {
        timer += GetFrameTime();

        if (entered) {
            if (isHeadInserting) {
                SaveStateForUndo(SelectedButton::INSERTHEAD, valueInsert);
                linkedList.InsertAtHead(valueInsert);
            }
            else if (isTailInserting) {
                SaveStateForUndo(SelectedButton::INSERTTAIL, valueInsert);
                linkedList.InsertAtTail(valueInsert);
            }
            else if (isCreateFile) {
                linkedList.CreateFromFile(filePath);
                timer = 0.0f;
            }
            else if (isCreateRandom) {
                linkedList.CreateRandom(valueNodes);
            }

            entered = !entered;
        }
        if (timer >= toolbar.duration) {
            if (isDeleting) {
				if (indexAnimation < indexDelete && indexAnimation < linkedList.getSize()  ) {
					if (!stepbystep) indexAnimation++;
                    timer = 0.0f;
                }
                else if (indexAnimation >= linkedList.getSize()) {
					indexAnimation = -1;
					toolbar.isPlaying = false;
					isDeleting = false;
                }
                else  {
                    SaveStateForUndo(SelectedButton::DELETE, valueDelete, indexDelete);
                    linkedList.DeleteValue(valueDelete);
                    toolbar.isPlaying = false;
					isDeleting = false;
                }
            }
            else if (isPosInserting ) {
				if (indexAnimation < indexInsert - 1 && indexAnimation < linkedList.getSize() && !stepbystep ) {
					indexAnimation++;
					timer = 0.0f;
				}
                else if (indexAnimation == indexInsert - 1 && indexAnimation < linkedList.getSize() ) {
                    SaveStateForUndo(SelectedButton::INSERTPOS, valueInsert, indexInsert);
                    linkedList.InsertAtPosition(valueInsert, indexInsert);
                    indexAnimation++;
                    timer = 0.0f;
                }
				else if (indexAnimation == indexInsert  ) {
                    isPosInserting = false;
					toolbar.isPlaying = false;
					timer = 0.0f;
				}

            }
            else if (isSearch && indexAnimation < indexSearch && !stepbystep) {
                timer = 0.0f;
                indexAnimation++;
            }
            else if (!stepbystep ){
				if (linkedlistState != InsertPosState && linkedlistState != DeleteState && linkedlistState != SearchState) toolbar.isPlaying = false;
                isHeadInserting = isTailInserting = isPosInserting = isDeleting = isSearch = isCreateRandom = isCreateFile = false;
                timer = 0.0f;  // Reset lại bộ đếm thời gian
            }
        }
    }

    if (fadeProgress == 0.0f)   fadeProgress = 1.0f;
}

void LinkedListScreen::DrawOperationsPanel() {
    int panelMargin = 250;
    float offset = 30.0f;
    float roundness = 0.15f;  // Độ bo góc
    int segments = 20;        // Độ mượt của bo góc

    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };
    Color panelColorFade = Fade(panelColor, 0.8f);

    Rectangle leftPanel = { 0, (float)(panelMargin - offset), (float)PANEL_WIDTH, (float)(Screen_h - 2 * panelMargin + offset * 2) };
    DrawRectangleRounded(leftPanel, roundness, segments, panelColorFade);

    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };
    Color panelColorxFade = Fade(panelColorx, 0.8f);

    Rectangle rightPanel = { (float)PANEL_WIDTH, (float)(panelMargin - offset), (float)PANEL_WIDTH, (float)(Screen_h - 2 * panelMargin + offset * 2) };
    DrawRectangleRounded(rightPanel, roundness, segments, panelColorxFade);

    // Vẽ nền bảng cho [INFO]
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };  // Chọn màu theo chế độ
    int rectWidth = 400;
    int rectHeight = 240;
    posX = Screen_w - rectWidth;
    posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);

    // "[INFO]"
    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(myFont, "[INFO]", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);

    // Tiêu đề
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(myFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    // Vẽ nút Create
    Color CreateColor = createHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == CREATE) CreateColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(createButton, 0.2f, 4, CreateColor);
    DrawRectangleRoundedLinesEx(createButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 createSize = MeasureTextEx(myFont, "Create", 20, 2);
    DrawTextEx(myFont, "Create",
        { createButton.x + (createButton.width - createSize.x) / 2,
        createButton.y + (createButton.height - createSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Insert
    Color InsertColor = insertHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(insertButton, 0.2f, 4, InsertColor);
    DrawRectangleRoundedLinesEx(insertButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 insertSize = MeasureTextEx(myFont, "Insert", 20, 2);
    DrawTextEx(myFont, "Insert",
        { insertButton.x + (insertButton.width - insertSize.x) / 2,
        insertButton.y + (insertButton.height - insertSize.y) / 2 },
        20, 2, DARKBLUE);

    // Nếu showInsertOptions = true, hiển thị các nút con
    if (showInsertOptions) {
        int fontSize = 18;  // Kích thước chữ
        int spacing = 2;    // Khoảng cách giữa các ký tự

        // Insert Head
        Vector2 headSize = MeasureTextEx(myFont, "> Head", fontSize, spacing);
        float headX = insertHeadButton.x + (insertHeadButton.width - headSize.x) / 2;
        float headY = insertHeadButton.y + (insertHeadButton.height - headSize.y) / 2;

        Color InsertHeadColor = insertAtHeadHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == INSERTHEAD) InsertHeadColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(insertHeadButton, 0.3f, 6, InsertHeadColor);
        DrawRectangleRoundedLinesEx(insertHeadButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Head", { headX, headY }, fontSize, spacing, DARKBLUE);

        // Insert Tail
        Vector2 tailSize = MeasureTextEx(myFont, "> Tail", fontSize, spacing);
        float tailX = insertTailButton.x + (insertTailButton.width - tailSize.x) / 2;
        float tailY = insertTailButton.y + (insertTailButton.height - tailSize.y) / 2;

        Color InsertTailColor = insertAtTailHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == INSERTTAIL) InsertTailColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(insertTailButton, 0.3f, 6, InsertTailColor);
        DrawRectangleRoundedLinesEx(insertTailButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Tail", { headX, tailY }, fontSize, spacing, DARKBLUE);

        // Insert Pos
        Vector2 posSize = MeasureTextEx(myFont, "> Pos", fontSize, spacing);
        float posX = insertPosButton.x + (insertPosButton.width - posSize.x) / 2;
        float posY = insertPosButton.y + (insertPosButton.height - posSize.y) / 2;

        Color InsertPosColor = insertPosHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == INSERTPOS) InsertPosColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(insertPosButton, 0.3f, 6, InsertPosColor);
        DrawRectangleRoundedLinesEx(insertPosButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Pos", { headX, posY }, fontSize, spacing, DARKBLUE);
    }

    // Vẽ nút Delete
    Color DeleteColor = deleteHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == DELETE) DeleteColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(deleteButton, 0.2f, 4, DeleteColor);
    DrawRectangleRoundedLinesEx(deleteButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 deleteSize = MeasureTextEx(myFont, "Delete", 20, 2);
    DrawTextEx(myFont, "Delete",
        { deleteButton.x + (deleteButton.width - deleteSize.x) / 2,
          deleteButton.y + (deleteButton.height - deleteSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Reverse
    Color ReverseColor = searchHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == SEARCH) ReverseColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(searchButton, 0.2f, 4, ReverseColor);
    DrawRectangleRoundedLinesEx(searchButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 reverseSize = MeasureTextEx(myFont, "Search", 20, 2);
    DrawTextEx(myFont, "Search",
        { searchButton.x + (searchButton.width - reverseSize.x) / 2,
          searchButton.y + (searchButton.height - reverseSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Clean
    Color CleanColor = cleanHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == CLEAN) CleanColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(cleanButton, 0.2f, 4, CleanColor);
    DrawRectangleRoundedLinesEx(cleanButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 cleanSize = MeasureTextEx(myFont, "Clean", 18, 2);
    DrawTextEx(myFont, "Clean",
        { cleanButton.x + (cleanButton.width - cleanSize.x) / 2,
          cleanButton.y + (cleanButton.height - cleanSize.y) / 2 },
        18, 2, DARKBLUE);

    // Ghi INFO MESSAGE
    DrawTextEx(IN4Font, infoMessage.c_str(), { (float)posX + 10, (float)posY + 40 }, 26, 2, DARKGRAY);

    if (linkedlistState == InsertHeadState || linkedlistState == InsertTailState || linkedlistState == DeleteState || linkedlistState == SearchState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Value", fontSize, 1.f).x;
        DrawTextEx(myFont, "Value", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();

        // Vẽ nút "OK"
        Color okColor = okHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == OK) okColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(okButton, 0.2f, 4, okColor);
        DrawRectangleRoundedLinesEx(okButton, 0.2f, 4, 2.0f, GRAY);

        Vector2 okSize = MeasureTextEx(myFont, "GO!", 18, 2);
        DrawTextEx(myFont, "GO!",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (linkedlistState == InsertPosState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Value", fontSize, 1.f).x;
        DrawTextEx(myFont, "Value", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();
        textWidth = MeasureTextEx(myFont, "Index", fontSize, 1.f).x;
        DrawTextEx(myFont, "Index", { 200, 400 }, fontSize, spacing, BLACK);
        Index.draw();

        // Vẽ nút "OK"
        Color okColor = okHovered ? LIGHTGRAY : RAYWHITE;
        if (currentButton == OK) okColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(okButton, 0.2f, 4, okColor);
        DrawRectangleRoundedLinesEx(okButton, 0.2f, 4, 2.0f, GRAY);

        Vector2 okSize = MeasureTextEx(myFont, "GO!", 18, 2);
        DrawTextEx(myFont, "GO!",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (linkedlistState == CreateState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Nodes", fontSize, 1.f).x;
        DrawTextEx(myFont, "Nodes", { 200, 350 }, fontSize, spacing, BLACK);
        if (Nodes.text == "") Nodes.text = "Random";
        Nodes.draw();

        // Vẽ nút "File"
        int textWidth2 = MeasureTextEx(myFont, "File", fontSize, 1.f).x;
        DrawTextEx(myFont, "File", { 200, 400 }, fontSize, spacing, BLACK);

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
        if (currentButton == OK) okColor = { 250, 228, 49, 255 };
        DrawRectangleRounded(okButton, 0.2f, 4, okColor);
        DrawRectangleRoundedLinesEx(okButton, 0.2f, 4, 2.0f, GRAY);

        Vector2 okSize = MeasureTextEx(myFont, "GO!", 18, 2);
        DrawTextEx(myFont, "GO!",
            { okButton.x + (okButton.width - okSize.x) / 2,
              okButton.y + (okButton.height - okSize.y) / 2 },
            18, 2, DARKBLUE);
    }
    else if (linkedlistState == ClearState) {}
}

void LinkedListScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);   // Sử dụng màu nền ở chế độ hiện tại 

    // Vẽ bảng điều khiển
    DrawOperationsPanel();

    toolbar.Draw();
    // Gọi vẽ danh sách với progress từ 0 -> 1
    float animationProgress = timer / toolbar.duration;
    drawLinkedList(animationProgress);

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
        float titleWidth = MeasureTextEx(myFont, titleText, 27, 2).x;
        Vector2 titlePos = { popupX + (popupWidth - titleWidth) / 2, popupY + 20 };
        DrawTextEx(myFont, titleText, titlePos, 27, 2, PURPLE);

        // Nội dung
        const char* text =
            "* The file must have a .txt extension.\n"
            "* The numbers on each line must be separated by space.\n"
            "* The numbers represent the nodes to be inserted into \nthe Linked List in the order they appear in the file.\n\n"
            "Example:\n"
            "   29 4 99 23 100 32 55 34 66 82 42\n"
            "   22 4 32 34 55 77\n";

        Vector2 textPos = { popupRect.x + 20, popupRect.y + 60 };
        DrawTextEx(myFont, text, textPos, 21, 2, BLACK);

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
        Vector2 fileokTextSize = MeasureTextEx(myFont, "OK", 22, 2);
        Vector2 fileokTextPos = { fileokButton.x + (fileokButton.width - fileokTextSize.x) / 2, fileokButton.y + (fileokButton.height - fileokTextSize.y) / 2 };
        DrawTextEx(myFont, "OK", fileokTextPos, 22, 2, BLUE);

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

void LinkedListScreen::drawLinkedList(float animationProgress) {
    Vector2 startPos = { 500, 300 };
    float nodeSpacing = 100.0f;
    LLNode* current = linkedList.head;

    int index = 0;
    int numsDelete = 0;

    Vector2 prevPos = { -1, -1 };  // Lưu vị trí của node trước đó

    while (current != nullptr) {
        Vector2 targetPos = TargetPos(index);

        // Hiệu ứng chụm lại rồi bật ra cho Create Random
        if (isCreateRandom || isCreateFile) {
            Vector2 centerPos = startPos;
            targetPos.x = centerPos.x * (1.0f - animationProgress) + targetPos.x * animationProgress;
            targetPos.y = centerPos.y * (1.0f - animationProgress) + targetPos.y * animationProgress;
        }

        if (isHeadInserting) {
            // Node mới chèn vào đầu linked list
            if (index < 11) targetPos.x -= (1.0f - animationProgress) * nodeSpacing;
            else if (index == 11) targetPos.y -= (1.0f - animationProgress) * nodeSpacing;
            else if (index > 11 && index < 22) targetPos.x += (1.0f - animationProgress) * nodeSpacing;
            else if (index == 22) targetPos.y -= (1.0f - animationProgress) * nodeSpacing;
            else targetPos.x -= (1.0f - animationProgress) * nodeSpacing;

            if (index == 0) {
                targetPos.y = startPos.y - nodeSpacing * 2.0f; // Cao hơn bình thường
                // Node mới sẽ đi xuống
                targetPos.y = startPos.y - (1.0f - animationProgress) * nodeSpacing;
            }
        }
        else if (isTailInserting && current->next == nullptr) {
            if (index < 11) {
                // Node mới sẽ đi xuống
                targetPos.x += nodeSpacing * (1.0f - animationProgress); // Dịch từ phải sang trái
                targetPos.y -= nodeSpacing * (1.0f - animationProgress); // Đi từ trên xuống
            }
            else if (index >= 11 && index < 22) {
                targetPos.x -= nodeSpacing * (1.0f - animationProgress); // Dịch từ phải sang trái
                targetPos.y += nodeSpacing * (1.0f - animationProgress); // Đi từ trên xuống
            }
            else if (index >= 22) {
                targetPos.x += nodeSpacing * (1.0f - animationProgress); // Dịch từ phải sang trái
                targetPos.y += nodeSpacing * (1.0f - animationProgress); // Đi từ trên xuống
            }
        }
        else if (isPosInserting && indexAnimation == indexInsert) {
            if (index == indexInsert) {
                // Node mới chèn vào đúng vị trí indexInsert
                if (index < 11) targetPos.y -= (1.0f - animationProgress) * nodeSpacing;
                else if (index > 11 && index < 22) targetPos.y += (1.0f - animationProgress) * nodeSpacing;
                else if (index > 22) targetPos.y += (1.0f - animationProgress) * nodeSpacing;
            }
            else if (index > indexInsert) {
                if (index < 11) targetPos.x -= (1.0f - animationProgress) * nodeSpacing;
                else if (index == 11) targetPos.y -= (1.0f - animationProgress) * nodeSpacing;
                else if (index > 11 && index < 22) targetPos.x += (1.0f - animationProgress) * nodeSpacing;
                else if (index == 22) targetPos.y -= (1.0f - animationProgress) * nodeSpacing;
                else if (index > 22) targetPos.x -= (1.0f - animationProgress) * nodeSpacing;
            }
        }
        else if (isDeleting && current->value == valueDelete && numsDelete == 0 && indexAnimation == indexDelete) {
            // Đây là node đầu tiên có value == valueDelete
            numsDelete = 1;

            DrawCircle(targetPos.x, targetPos.y, 30, Fade(Color{ 246, 50, 0, 255 }, 1.0f - animationProgress));

            // Vẽ viền
            DrawCircle(targetPos.x, targetPos.y, 32, Fade(BLACK, 1.0f - animationProgress));

            // Vẽ giá trị mờ dần
            string nodeText = TextFormat("%d", current->value);
            Vector2 textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);
            Vector2 textPos = { targetPos.x - textSize.x / 2, targetPos.y - textSize.y / 2 };
            DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, 1.0f - animationProgress));

            current = current->next;
            index++;
            continue;
        }

        else if (isDeleting && index > indexDelete && indexAnimation == indexDelete) {
            // Vị trí ban đầu của node bên phải node bị xóa
            Vector2 initialPos = TargetPos(index);
            // Vị trí của node bị xóa
            Vector2 newPos = TargetPos(index - numsDelete);
            // Đưa node bên phải node bị xóa sang bên trái đến vị trí của node bị xóa để thay thế
            targetPos.x = initialPos.x * (1.0f - animationProgress) + newPos.x * animationProgress;
            targetPos.y = initialPos.y * (1.0f - animationProgress) + newPos.y * animationProgress;
        }


        Color nodeColor = (fadeProgress == 1.0f) ? WHITE : Fade(Color{ 246, 50, 130, 255 }, fadeProgress); // Màu mặc định của Node 
        Color borderColor = Fade(BLACK, fadeProgress); // Viền mặc định của Node
        // Vẽ viền (Border)
        DrawCircle(targetPos.x, targetPos.y, 32, borderColor);
        // Vẽ node chính
        if (linkedlistState == SearchState && indexAnimation == indexSearch && index == indexAnimation) {
            DrawCircle(targetPos.x, targetPos.y, 30, Color{ 246, 50, 130, 255 });
            infoMessage = "The first node with the value " + to_string(valueSearch) + "\nin the Linked List (0-indexed) is at \nthe position " + to_string(index) + '.';
        }
        else if (linkedlistState == DeleteState && index == indexAnimation) {
            if (indexAnimation == indexDelete ) DrawCircle(targetPos.x, targetPos.y, 30, nodeColor);
            else DrawCircle(targetPos.x, targetPos.y, 30, Color{ 37, 216, 32, 255 });
        }
        else if ( index == indexAnimation) {
            DrawCircle(targetPos.x, targetPos.y, 30, Color{ 37, 216, 32, 255 });
        }
        else DrawCircle(targetPos.x, targetPos.y, 30, nodeColor);


        // Vẽ giá trị trong node
        string nodeText = TextFormat("%d", current->value);
        Vector2 textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);

        Vector2 textPos = { targetPos.x - textSize.x / 2, targetPos.y - textSize.y / 2 }; // Căn giữa Node 

        DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, fadeProgress));

		if (isPosInserting && index == indexAnimation) {
             nodeText = TextFormat("%d", index);
             textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);

             textPos = { targetPos.x - textSize.x / 2, targetPos.y - textSize.y / 2 - 40}; // Căn giữa Node 

            DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, fadeProgress));
		}
        // Vẽ mũi tên (đường nối) giữa các node
        if (prevPos.x != -1) {
            if (index < 11) {
                Vector2 startLine = { prevPos.x + 30, prevPos.y }; // Cạnh phải của node trước
                Vector2 endLine = { targetPos.x - 30, targetPos.y }; // Cạnh trái của node sau

                DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));

                // Tính toán góc của đường thẳng
                float dx = endLine.x - startLine.x;
                float dy = endLine.y - startLine.y;
                float angle = atan2(dy, dx);

                // Độ dài đầu mũi tên (có thể điều chỉnh)
                float arrowLength = 10.0f;
                float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)

                // Tính toán hai điểm cho đầu mũi tên
                Vector2 arrowPoint1 = {
                    endLine.x - arrowLength * cos(angle + arrowAngle),
                    endLine.y - arrowLength * sin(angle + arrowAngle)
                };
                Vector2 arrowPoint2 = {
                    endLine.x - arrowLength * cos(angle - arrowAngle),
                    endLine.y - arrowLength * sin(angle - arrowAngle)
                };

                // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
                DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
                DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
            }
            else if (index == 11) {
                Vector2 startLine = { prevPos.x, prevPos.y + 30 }; // Cạnh phải của node trước
                Vector2 endLine = { targetPos.x , targetPos.y - 30 }; // Cạnh trái của node sau

                DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));

                // Tính toán góc của đường thẳng
                float dx = endLine.x - startLine.x;
                float dy = endLine.y - startLine.y;
                float angle = atan2(dy, dx);

                // Độ dài đầu mũi tên (có thể điều chỉnh)
                float arrowLength = 10.0f;
                float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)

                // Tính toán hai điểm cho đầu mũi tên
                Vector2 arrowPoint1 = {
                    endLine.x - arrowLength * cos(angle + arrowAngle),
                    endLine.y - arrowLength * sin(angle + arrowAngle)
                };
                Vector2 arrowPoint2 = {
                    endLine.x - arrowLength * cos(angle - arrowAngle),
                    endLine.y - arrowLength * sin(angle - arrowAngle)
                };

                // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
                DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
                DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
            }
            else if (index > 11 && index < 22) {
                Vector2 startLine = { prevPos.x - 30, prevPos.y }; // Cạnh phải của node trước
                Vector2 endLine = { targetPos.x + 30, targetPos.y }; // Cạnh trái của node sau

                DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));

                // Tính toán góc của đường thẳng
                float dx = endLine.x - startLine.x;
                float dy = endLine.y - startLine.y;
                float angle = atan2(dy, dx);

                // Độ dài đầu mũi tên (có thể điều chỉnh)
                float arrowLength = 10.0f;
                float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)

                // Tính toán hai điểm cho đầu mũi tên
                Vector2 arrowPoint1 = {
                    endLine.x - arrowLength * cos(angle + arrowAngle),
                    endLine.y - arrowLength * sin(angle + arrowAngle)
                };
                Vector2 arrowPoint2 = {
                    endLine.x - arrowLength * cos(angle - arrowAngle),
                    endLine.y - arrowLength * sin(angle - arrowAngle)
                };

                // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
                DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
                DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
            }
            else if (index == 22) {
                Vector2 startLine = { prevPos.x, prevPos.y + 30 }; // Cạnh phải của node trước
                Vector2 endLine = { targetPos.x , targetPos.y - 30 }; // Cạnh trái của node sau

                DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));

                // Tính toán góc của đường thẳng
                float dx = endLine.x - startLine.x;
                float dy = endLine.y - startLine.y;
                float angle = atan2(dy, dx);

                // Độ dài đầu mũi tên (có thể điều chỉnh)
                float arrowLength = 10.0f;
                float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)

                // Tính toán hai điểm cho đầu mũi tên
                Vector2 arrowPoint1 = {
                    endLine.x - arrowLength * cos(angle + arrowAngle),
                    endLine.y - arrowLength * sin(angle + arrowAngle)
                };
                Vector2 arrowPoint2 = {
                    endLine.x - arrowLength * cos(angle - arrowAngle),
                    endLine.y - arrowLength * sin(angle - arrowAngle)
                };

                // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
                DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
                DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
            }
            else if (index > 22) {
                Vector2 startLine = { prevPos.x + 30, prevPos.y }; // Cạnh phải của node trước
                Vector2 endLine = { targetPos.x - 30, targetPos.y }; // Cạnh trái của node sau

                DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));

                // Tính toán góc của đường thẳng
                float dx = endLine.x - startLine.x;
                float dy = endLine.y - startLine.y;
                float angle = atan2(dy, dx);

                // Độ dài đầu mũi tên (có thể điều chỉnh)
                float arrowLength = 10.0f;
                float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)

                // Tính toán hai điểm cho đầu mũi tên
                Vector2 arrowPoint1 = {
                    endLine.x - arrowLength * cos(angle + arrowAngle),
                    endLine.y - arrowLength * sin(angle + arrowAngle)
                };
                Vector2 arrowPoint2 = {
                    endLine.x - arrowLength * cos(angle - arrowAngle),
                    endLine.y - arrowLength * sin(angle - arrowAngle)
                };

                // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
                DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
                DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
            }
        }

        prevPos = targetPos; // Lưu vị trí của node hiện tại để nối với node tiếp theo

        current = current->next;
        index++;
    }

    DrawTextEx(IN4Font, infoMessage.c_str(), { (float)posX + 10, (float)posY + 40 }, 26, 2, DARKGRAY);

    if (isClean) {
        fadeProgress -= GetFrameTime() / 1.0f;
        if (fadeProgress <= 0.0f) {
            fadeProgress = 0.0f;
			redoStack.clear(); // Xóa redo stack
			undoStack.clear(); // Xóa undo stack
            linkedList.ClearList(); // Xóa danh sách khi hiệu ứng kết thúc
            toolbar.isPlaying = false;
            isClean = false;
            linkedlistState = NONE;
        }
    }
}
void LinkedListScreen::Unload() {
    UnloadFont(linkedListFont);
    UnloadFont(myFont);
    UnloadFont(IN4Font);
}
