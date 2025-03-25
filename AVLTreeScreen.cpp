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

float AVLTreeScreen::Clamp(float value, float minValue, float maxValue) {
    if (value < minValue) return minValue;
    if (value > maxValue) return maxValue;
    return value;
}

float AVLTreeScreen::SmoothStep(float a, float b, float t) {
    t = Clamp(t, 0.0f, 1.0f); // Đảm bảo t trong khoảng [0,1]
    return a + (b - a) * t;
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

    // Kiểm tra hover vào nút "Clean"
    cleanHovered = CheckCollisionPointRec(mouse, cleanButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }

    // Kiểm tra nút Insert
    if (insertHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = InsertState;
        handleButtonClick(INSERTAVL, Value);
        currentButton = INSERTAVL;
    }


    // Kiểm tra nút Delete
    if (CheckCollisionPointRec(mouse, deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = DeleteState;
        handleButtonClick(DELETEAVL, Value);
        currentButton = DELETEAVL;
    }

    // Kiểm tra nút Reverse
    if (CheckCollisionPointRec(mouse, searchButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = SearchState;
        currentButton = SEARCHAVL;
    }

    // Kiểm tra nút Clean 
    if (CheckCollisionPointRec(mouse, cleanButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        AVLtreeState = ClearState;
        currentButton = CLEANAVL;
    }

    if (AVLtreeState == InsertState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueInsert = stoi(Value.outputMessage);
            infoMessage = "Inserting " + to_string(valueInsert) + " into AVL tree.";  // Cập nhật infoMessage
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";

            // Kích hoạt animation sau mỗi lần thay đổi
            isInsert = true;
            timer = 0.0f;
            entered = true;
        }
    }
    else if (AVLtreeState == DeleteState) {
        Value.update();
        if (Value.isEnter && Value.outputMessage != "") {
            valueDelete = stoi(Value.outputMessage);
            Value.isEnter = false; // Reset trạng thái ENTER 
            Value.outputMessage = "";
            if (AVLtree.Search(valueDelete)) {
                // Kích hoạt animation sau mỗi lần thay đổi
                isDeleting = true;
                timer = 0.0f;
                entered = true;
                infoMessage = "Deleting " + to_string(valueDelete) + " of AVL tree.";  // Cập nhật infoMessage
            }
            else {
                infoMessage = "There is no node with the value\n" + to_string(valueDelete) + " in the current AVLtree.";
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
            isSearch = true;
            timer = 0.0f;
            entered = true;
        }
        if (!AVLtree.Search(valueSearch)) {
            infoMessage = "There is no node with the value \n" + to_string(valueSearch) + " in the current AVLtree.";
        }
    }
    else if (AVLtreeState == ClearState) {
        isClean = true;
        infoMessage = "Clearing the AVLtree...";
    }

    if (AVLtreeState != SearchState)   SearchNode = nullptr;

    // Cập nhật tiến trình animation
    //if (isInsert|| isDeleting || isSearch) {
    //    timer += GetFrameTime();
    //    if (entered) {
    //        if (isInsert)  AVLtree.root =AVLtree.insert(AVLtree.root,valueInsert);
    //        else if (isSearch) {
    //            SearchNode = AVLtree.Search(valueSearch);
    //            currentSearchNode = AVLtree.root;
    //        }
    //        entered = !entered;
    //    }
    //    if (timer >= duration) {
    //        if (isDeleting)  AVLtree.DeleteValue(valueDelete);

    //        if (isSearch && currentSearchNode != SearchNode) {
    //            timer = 0.0f;
    //            currentSearchNode = currentSearchNode->next;
    //        }
    //        else {
    //            isInsert = isDeleting = isSearch = false;
    //            timer = 0.0f;  // Reset lại bộ đếm thời gian
    //        }
    //    }
    //}

    if (fadeProgress == 0.0f)   fadeProgress = 1.0f;
}

void AVLTreeScreen::DrawOperationsPanel() {
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
    posX = Screen_w - rectWidth;
    posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);

    // "[INFO]"
    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(myFont, "[INFO]", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);

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

    // Vẽ nút Clean
    Color CleanColor = cleanHovered ? LIGHTGRAY : RAYWHITE;
    if (currentButton == CLEANAVL) CleanColor = { 250, 228, 49, 255 };
    DrawRectangleRounded(cleanButton, 0.2f, 4, CleanColor);
    DrawRectangleRoundedLinesEx(cleanButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 cleanSize = MeasureTextEx(myFont, "Clean", 18, 2);
    DrawTextEx(myFont, "Clean",
        { cleanButton.x + (cleanButton.width - cleanSize.x) / 2,
          cleanButton.y + (cleanButton.height - cleanSize.y) / 2 },
        18, 2, DARKBLUE);

    // Ghi INFO MESSAGE
    DrawTextEx(IN4Font, infoMessage.c_str(), { (float)posX + 10, (float)posY + 40 }, 26, 2, DARKGRAY);

    if ( AVLtreeState == InsertState || AVLtreeState == DeleteState || AVLtreeState == SearchState) {
        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(myFont, "Value", fontSize, 1.f).x;
        DrawTextEx(myFont, "Value", { 200, 350 }, fontSize, spacing, BLACK);
        Value.draw();
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
    //drawAVLtree(animationProgress);
}

//void AVLTreeScreen::drawAVLtree(float animationProgress) {
//    Vector2 startPos = { 500, 300 };
//    float nodeSpacing = 100.0f;
//    AVLNode* current = AVLtree.root;
//
//    int index = 0;
//
//    Vector2 prevPos = { -1, -1 };  // Lưu vị trí của node trước đó
//
//    while (current != nullptr) {
//        Vector2 targetPos = { startPos.x + index * nodeSpacing, startPos.y };
//
//        if (isHeadInserting) {
//            // Node mới chèn vào đầu AVLtree
//            targetPos.x -= (1.0f - animationProgress) * nodeSpacing;
//            if (index == 0) {
//                targetPos.y = startPos.y - nodeSpacing * 2.0f; // Cao hơn bình thường
//                // Node mới sẽ đi xuống
//                targetPos.y = startPos.y - (1.0f - animationProgress) * nodeSpacing;
//            }
//        }
//        else if (isTailInserting && current->next == nullptr) {
//            // Node mới sẽ đi xuống
//            targetPos.x += nodeSpacing * (1.0f - animationProgress); // Dịch từ phải sang trái
//            targetPos.y -= nodeSpacing * (1.0f - animationProgress); // Đi từ trên xuống
//        }
//        else if (isPosInserting && index == indexInsert) {
//            // Node mới chèn vào đúng vị trí indexInsert
//            targetPos.y += (1.0f - animationProgress) * nodeSpacing;
//        }
//        else if (isDeleting && current->value == valueDelete) {
//            DrawCircle(targetPos.x, targetPos.y, 30, Fade(Color{ 246, 50, 130, 255 }, 1.0f - animationProgress));
//            current = current->next;
//            index++;
//            continue;
//        }
//
//        Color nodeColor = (fadeProgress == 1.0f) ? WHITE : Fade(Color{ 246, 50, 130, 255 }, fadeProgress); // Màu mặc định của Node 
//        Color borderColor = Fade(BLACK, fadeProgress); // Viền mặc định của Node
//        // Vẽ viền (Border)
//        DrawCircle(targetPos.x, targetPos.y, 32, borderColor);
//        // Vẽ node chính
//        if (isSearch && current == currentSearchNode && currentSearchNode->value != valueSearch) {
//            DrawCircle(targetPos.x, targetPos.y, 30, Color{ 37, 216, 32, 255 });
//        }
//        else if (AVLtreeState == SearchState && current == currentSearchNode && currentSearchNode->value == valueSearch) {
//            DrawCircle(targetPos.x, targetPos.y, 30, Color{ 246, 50, 130, 255 });
//            infoMessage = "The first node with the value " + to_string(valueSearch) + "\nin the AVLtree (0-indexed) is at \nthe position " + to_string(index) + '.';
//        }
//        else DrawCircle(targetPos.x, targetPos.y, 30, nodeColor);
//
//        // Vẽ giá trị trong node
//        string nodeText = TextFormat("%d", current->value);
//        Vector2 textSize = MeasureTextEx(myFont, nodeText.c_str(), 25, 1.25);
//
//        Vector2 textPos = { targetPos.x - textSize.x / 2, targetPos.y - textSize.y / 2 }; // Căn giữa Node 
//
//        DrawTextEx(myFont, nodeText.c_str(), textPos, 25, 1.25, Fade(BLACK, fadeProgress));
//
//        // Vẽ mũi tên (đường nối) giữa các node
//        if (prevPos.x != -1) {
//            Vector2 startLine = { prevPos.x + 30, prevPos.y }; // Cạnh phải của node trước
//            Vector2 endLine = { targetPos.x - 30, targetPos.y }; // Cạnh trái của node sau
//
//            DrawLineEx(startLine, endLine, 1.5f, Fade(BLACK, fadeProgress));
//
//            // Tính toán góc của đường thẳng
//            float dx = endLine.x - startLine.x;
//            float dy = endLine.y - startLine.y;
//            float angle = atan2(dy, dx);
//
//            // Độ dài đầu mũi tên (có thể điều chỉnh)
//            float arrowLength = 10.0f;
//            float arrowAngle = 25 * DEG2RAD; // Góc nghiêng của mũi tên (25 độ)
//
//            // Tính toán hai điểm cho đầu mũi tên
//            Vector2 arrowPoint1 = {
//                endLine.x - arrowLength * cos(angle + arrowAngle),
//                endLine.y - arrowLength * sin(angle + arrowAngle)
//            };
//            Vector2 arrowPoint2 = {
//                endLine.x - arrowLength * cos(angle - arrowAngle),
//                endLine.y - arrowLength * sin(angle - arrowAngle)
//            };
//
//            // Vẽ hai đoạn thẳng tạo thành đầu mũi tên
//            DrawLineEx(endLine, arrowPoint1, 1.5f, Fade(BLACK, fadeProgress));
//            DrawLineEx(endLine, arrowPoint2, 1.5f, Fade(BLACK, fadeProgress));
//        }
//
//        prevPos = targetPos; // Lưu vị trí của node hiện tại để nối với node tiếp theo
//
//        current = current->next;
//        index++;
//    }
//
//    DrawTextEx(IN4Font, infoMessage.c_str(), { (float)posX + 10, (float)posY + 40 }, 26, 2, DARKGRAY);
//
//    if (isClean) {
//        fadeProgress -= GetFrameTime() / 1.0f;
//        if (fadeProgress <= 0.0f) {
//            fadeProgress = 0.0f;
//            AVLtree.ClearList(); // Xóa danh sách khi hiệu ứng kết thúc
//            isClean = false;
//        }
//    }
//}
void AVLTreeScreen::Unload() {
    UnloadFont(AVLtreeFont);
}