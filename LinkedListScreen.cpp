#include "LinkedListScreen.h"
#include "Constants.h"

void LinkedListScreen::Init() {
    // Khởi tạo nút Back
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Load font
    linkedListFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(linkedListFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);


    Value = { {270, 350, 90, 30} };
    Index = { {270, 400, 90, 30} };

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
    Vector2 mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Back"
    backHovered = CheckCollisionPointRec(mouse, backButton);

    // Kiểm tra hover vào nút "Insert"
    insertHovered = CheckCollisionPointRec(mouse, insertButton);

    // Kiểm tra hover vào nút "Delete"
    deleteHovered = CheckCollisionPointRec(mouse, deleteButton);

    // Kiểm tra hover vào nút "Reverse"
    reverseHovered = CheckCollisionPointRec(mouse, reverseButton);

    // Kiểm tra hover vào nút "Clean"
    cleanHovered = CheckCollisionPointRec(mouse, cleanButton);

    //Check hover button "insertAtHead"
    insertAtHeadHovered = CheckCollisionPointRec(mouse, insertHeadButton);

    //Check hover button "insertAtTail"
    insertAtTailHovered = CheckCollisionPointRec(mouse, insertTailButton);

    //Check hover button "insertPos"
    insertPosHovered = CheckCollisionPointRec(mouse, insertPosButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }

    // Kiểm tra nút Insert
    if (insertHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        showInsertOptions = !showInsertOptions;
    }

    // Animation:  Điều chỉnh offset khi mở menu Insert
    if (showInsertOptions) {
        // Mở menu, nút Head, Tail, Pos sẽ mở ngay lập tức
        insertHeadAlpha = SmoothStep(insertHeadAlpha, 0.0f, 0.006f);  // Giảm alpha của Head từ từ
        insertTailAlpha = SmoothStep(insertTailAlpha, 0.0f, 0.006f);  // Giảm alpha của Tail từ từ
        insertPosAlpha = SmoothStep(insertPosAlpha, 0.0f, 0.006f);    // Giảm alpha của Pos từ từ
        insertOptionsOffset = SmoothStep(insertOptionsOffset, 100.0f, 0.02f);

        // Cập nhật vị trí của các nút Delete, Reverse, Clean
        insertHeadButton.y = insertButton.y + 45;
        insertTailButton.y = insertButton.y + 80;
        insertPosButton.y = insertButton.y + 115;

        deleteButton.y = 380 + insertOptionsOffset;
        reverseButton.y = 430 + insertOptionsOffset;
        cleanButton.y = 480 + insertOptionsOffset;

        // Nếu Insert đang mở, kiểm tra các nút con
        if (CheckCollisionPointRec(mouse, insertHeadButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertHeadState;
        }
        if (CheckCollisionPointRec(mouse, insertTailButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertTailState;
        }
        if (CheckCollisionPointRec(mouse, insertPosButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            linkedlistState = InsertPosState;
        }
    }
    else {
        insertHeadAlpha = 0.0f;  // Giảm alpha của Head ngay lập tức
        insertTailAlpha = 0.0f;  // Giảm alpha của Tail ngay lập tức
        insertPosAlpha = 0.0f;   // Giảm alpha của Pos ngay lập tức

        // Cập nhật vị trí của các nút khi menu đóng
        insertOptionsOffset = SmoothStep(insertOptionsOffset, 0.0f, 0.02f); // Menu sẽ thu lại khi đóng

        // Nếu menu đã đóng xong, reset lại các nút khác
        deleteButton.y = 380 + insertOptionsOffset;
        reverseButton.y = 430 + insertOptionsOffset;
        cleanButton.y = 480 + insertOptionsOffset;
    }

    // Kiểm tra nút Delete
    if (CheckCollisionPointRec(mouse, deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = DeleteState;
    }

    // Kiểm tra nút Reverse
    if (CheckCollisionPointRec(mouse, reverseButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = ReverseState;
    }

    // Kiểm tra nút Clean 
    if (CheckCollisionPointRec(mouse, cleanButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        linkedlistState = ClearState;
    }

    if (linkedlistState == InsertHeadState) {
        Value.update();
        if (Value.outputMessage != "") {
            linkedList.InsertAtHead(stoi(Value.outputMessage));
            Value.outputMessage = "";
        }
    }
    else if (linkedlistState == InsertHeadState){
        Value.update();
        if (Value.outputMessage != "") {
            linkedList.InsertAtTail(stoi(Value.outputMessage));
            Value.outputMessage = "";
        }
    }
    else if (linkedlistState == InsertPosState) {
        Value.update();
        Index.update();
        if (Value.outputMessage != ""  && Index.outputMessage != "") {
            linkedList.InsertAtPosition(stoi(Value.outputMessage) , stoi(Index.outputMessage));
            Value.outputMessage = "";
            Index.outputMessage = "";
        }
    }
    else if (linkedlistState == DeleteState) {
        Value.update();
        if (Value.outputMessage != "") {
            linkedList.DeleteValue(stoi(Value.outputMessage));
            Value.outputMessage = "";
        }
    }
}

void LinkedListScreen::DrawOperationsPanel() {
    // Vẽ nền bảng
    int panelMargin = 250;  // Khoảng cách lề trên & dưới
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };  // Chọn màu theo chế độ

    DrawRectangle(0, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, Fade(panelColor, 0.8f));

    // Vẽ nền bảng
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };  // Chọn màu theo chế độ

    DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, Fade(panelColorx, 0.8f));

    // Vẽ nền bảng cho [INFO]
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };  // Chọn màu theo chế độ
    int rectWidth = 400;
    int rectHeight = 240;
    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);

    // "[INFO]"
    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(myFont, "[INFO]", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);

    // Tiêu đề
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(myFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

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
        Color InserAtHeadColor = insertAtHeadHovered ? LIGHTGRAY : RAYWHITE;
        Vector2 headSize = MeasureTextEx(myFont, "> Head", fontSize, spacing);
        float headX = insertHeadButton.x + (insertHeadButton.width - headSize.x) / 2;
        float headY = insertHeadButton.y + (insertHeadButton.height - headSize.y) / 2;

        DrawRectangleRounded(insertHeadButton, 0.3f, 6, InserAtHeadColor);
        DrawRectangleRoundedLinesEx(insertHeadButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Head", { headX, headY }, fontSize, spacing, DARKBLUE);

        // Insert Tail
        Color InserAtTailColor = insertAtTailHovered ? LIGHTGRAY : RAYWHITE;
        Vector2 tailSize = MeasureTextEx(myFont, "> Tail", fontSize, spacing);
        float tailX = insertTailButton.x + (insertTailButton.width - tailSize.x) / 2;
        float tailY = insertTailButton.y + (insertTailButton.height - tailSize.y) / 2;

        DrawRectangleRounded(insertTailButton, 0.3f, 6, InserAtTailColor);
        DrawRectangleRoundedLinesEx(insertTailButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Tail", { headX, tailY }, fontSize, spacing, DARKBLUE);

        // Insert Pos
        Color InserPosColor = insertPosHovered ? LIGHTGRAY : RAYWHITE;
        Vector2 posSize = MeasureTextEx(myFont, "> Pos", fontSize, spacing);
        float posX = insertPosButton.x + (insertPosButton.width - posSize.x) / 2;
        float posY = insertPosButton.y + (insertPosButton.height - posSize.y) / 2;

        DrawRectangleRounded(insertPosButton, 0.3f, 6, InserPosColor);
        DrawRectangleRoundedLinesEx(insertPosButton, 0.3f, 6, 2.0f, GRAY);
        DrawTextEx(myFont, "> Pos", { headX, posY }, fontSize, spacing, DARKBLUE);
    }

    // Vẽ nút Delete
    Color DeleteColor = deleteHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(deleteButton, 0.2f, 4, DeleteColor);
    DrawRectangleRoundedLinesEx(deleteButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 deleteSize = MeasureTextEx(myFont, "Delete", 20, 2);
    DrawTextEx(myFont, "Delete",
        { deleteButton.x + (deleteButton.width - deleteSize.x) / 2,
          deleteButton.y + (deleteButton.height - deleteSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Reverse
    Color ReverseColor = reverseHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(reverseButton, 0.2f, 4, ReverseColor);
    DrawRectangleRoundedLinesEx(reverseButton, 0.2f, 4, 2.0f, GRAY);
    Vector2 reverseSize = MeasureTextEx(myFont, "Reverse", 20, 2);
    DrawTextEx(myFont, "Reverse",
        { reverseButton.x + (reverseButton.width - reverseSize.x) / 2,
          reverseButton.y + (reverseButton.height - reverseSize.y) / 2 },
        20, 2, DARKBLUE);

    // Vẽ nút Clean
    Color CleanColor = cleanHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(cleanButton, 0.2f, 4, CleanColor);
    DrawRectangleRoundedLinesEx(cleanButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 cleanSize = MeasureTextEx(myFont, "Clean", 18, 2);
    DrawTextEx(myFont, "Clean",
        { cleanButton.x + (cleanButton.width - cleanSize.x) / 2,
          cleanButton.y + (cleanButton.height - cleanSize.y) / 2 },
        18, 2, DARKBLUE);




    if (linkedlistState == InsertHeadState || linkedlistState == InsertTailState || linkedlistState == DeleteState
        ) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Value", fontSize, 1.f).x;
        DrawTextEx(myFont, "Value", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();
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
    }
    
    else if (linkedlistState == ReverseState) {}
    else if (linkedlistState == ClearState) {}

}

void LinkedListScreen::Draw() {
    ClearBackground(isDarkMode ? darkmode : lightmode);   // Sử dụng màu nền ở chế độ hiện tại 

    // Hiển thị tiêu đề Linked List
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(linkedListFont, "Linked List", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    Color titleColor = isDarkMode ? WHITE : DARKBLUE;
    DrawTextEx(linkedListFont, "Linked List",
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
}

void LinkedListScreen::Unload() {
    UnloadFont(linkedListFont);
}