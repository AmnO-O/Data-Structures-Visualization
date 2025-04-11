#include "Table.h"
#include <algorithm>

void TraversalTable::Init(float x, float y, float width, float height) {
    scrollY = 0;
    targetScrollY = 0;
    scrollSpeed = 30; // Tăng tốc độ cuộn
    smoothScrollSpeed = 10.0f; // Tốc độ làm mượt
    rowHeight = 34;
    visibleRows = (int)(height / rowHeight);
    visible = false;
    tableArea = { x, y, width, height };

    // Nút ở phía trên bên phải bảng
    toggleButton = { x + width - 140, y - 50, 140, 40 };

    myFont = LoadFont("Assets/Fonts/PublicSans-bold.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    scrollbarHovered = false;
    scrollbarDragged = false;
}

Rectangle TraversalTable::GetTableArea() const { return tableArea; }

void TraversalTable::SetData(std::vector<int> in, std::vector<int> pre, std::vector<int> post) {
    inorder = in;
    preorder = pre;
    postorder = post;
}

void TraversalTable::ToggleVisibility() {
    visible = !visible;

    if (visible) {
        animationActive = true;
        animationProgress = 0.0f;
    }
}

bool TraversalTable::IsVisible() const {
    return visible;
}

void TraversalTable::Update() {
    if (animationActive) {
        animationProgress += GetFrameTime();
        if (animationProgress >= animationDuration) {
            animationProgress = animationDuration;
            animationActive = false;
        }
    }

    if (CheckCollisionPointRec(GetMousePosition(), toggleButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        ToggleVisibility();
    }

    if (!visible) return;

    int numRows = inorder.size();
    float headerHeight = 40;
    float contentHeight = numRows * rowHeight + headerHeight;
    maxScroll = std::max(0.0f, contentHeight - tableArea.height);
    Vector2 mouse = GetMousePosition();

    // Kiểm tra xem chuột có trong vùng bảng không
    bool mouseInTable = CheckCollisionPointRec(mouse, tableArea);

    // Xử lý cuộn bằng chuột
    if (mouseInTable) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            targetScrollY -= wheel * scrollSpeed;
            targetScrollY = Clamp(targetScrollY, 0.0f, maxScroll);
        }
    }

    if (fabs(scrollY - targetScrollY) > 0.5f) {
        scrollY = Lerp(scrollY, targetScrollY, smoothScrollSpeed * GetFrameTime());
    }
    else {
        scrollY = targetScrollY;
    }

    // Xử lý thanh cuộn
    if (contentHeight > tableArea.height) {
        float scrollbarWidth = 10;
        float scrollbarX = tableArea.x + tableArea.width - scrollbarWidth;
        float scrollbarY = tableArea.y + headerHeight;
        float scrollbarHeight = tableArea.height - headerHeight;

        float thumbHeight = (tableArea.height / contentHeight) * scrollbarHeight;
        thumbHeight = Clamp(thumbHeight, 20.0f, scrollbarHeight);

        float thumbY = scrollbarY + (scrollY / maxScroll) * (scrollbarHeight - thumbHeight);
        Rectangle thumbRect = { scrollbarX, thumbY, scrollbarWidth, thumbHeight };

        // Kiểm tra hover
        scrollbarHovered = CheckCollisionPointRec(GetMousePosition(), thumbRect);

        // Xử lý kéo thanh cuộn
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (scrollbarHovered || scrollbarDragged) {
                scrollbarDragged = true;

                // Tính toán vị trí mới dựa trên vị trí chuột
                float newThumbY = GetMousePosition().y - (thumbHeight / 2);
                newThumbY = Clamp(newThumbY, scrollbarY, scrollbarY + scrollbarHeight - thumbHeight);

                // Cập nhật targetScrollY
                targetScrollY = ((newThumbY - scrollbarY) / (scrollbarHeight - thumbHeight)) * maxScroll;
                targetScrollY = Clamp(targetScrollY, 0.0f, maxScroll);
                scrollY = targetScrollY;
            }
        }
        else {
            scrollbarDragged = false;
        }
    }

    scrollY = Clamp(scrollY, 0.0f, maxScroll);
    targetScrollY = Clamp(targetScrollY, 0.0f, maxScroll);
}

void TraversalTable::Draw() {
    DrawToggleButton();
    if (!visible) return;
    DrawTable();
}

void TraversalTable::DrawToggleButton() {
    Vector2 mousePos = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mousePos, toggleButton);
    Color buttonColor = isHovered ? Color{ 180, 200, 255, 255 } : Color{ 220, 230, 255, 255 };
    Color borderColor = isHovered ? Color{ 100, 100, 200, 255 } : Color{ 160, 160, 200, 255 };

    float roundness = 0.5f;
    int segments = 16;

    // Đổ bóng phía sau nút
    Rectangle shadowRect = toggleButton;
    shadowRect.x += 3;
    shadowRect.y += 3;
    DrawRectangleRounded(shadowRect, roundness, segments, Fade(BLACK, 0.2f));

    // Nút chính
    DrawRectangleRounded(toggleButton, roundness, segments, buttonColor);
    DrawRectangleRoundedLines(toggleButton, roundness, segments, borderColor);

    // Text
    const char* buttonText = "Traversal";
    float fontSize = 20;
    Vector2 textSize = MeasureTextEx(myFont, buttonText, fontSize, 1);
    float textX = toggleButton.x + (toggleButton.width - textSize.x) / 2;
    float textY = toggleButton.y + (toggleButton.height - textSize.y) / 2;

    // Đổ bóng chữ (hiệu ứng 3D nhẹ)
    DrawTextEx(myFont, buttonText, { textX + 1, textY + 1 }, fontSize, 1, Fade(DARKGRAY, 0.5f));

    // Chữ chính
    DrawTextEx(myFont, buttonText, { textX, textY }, fontSize, 1, BLACK);
}



void TraversalTable::DrawTable() {
    // Tính phần đã hiện
    float t = animationProgress / animationDuration;
    if (!animationActive) t = 1.0f;
    t = t * t * (3 - 2 * t);

    // Tính alpha và chiều cao bảng đã xuất hiện
    unsigned char alpha = (unsigned char)(t * 255);
    float currentHeight = tableArea.height * t;

    BeginScissorMode(tableArea.x, tableArea.y, tableArea.width, currentHeight);

    int numRows = inorder.size();
    float colWidth = tableArea.width / 3;
    float headerHeight = 40;
    float contentHeight = numRows * rowHeight + headerHeight;
    maxScroll = std::max(0.0f, contentHeight - tableArea.height);

    // shadow bảng
    Rectangle shadowRect = tableArea;
    shadowRect.x += 4;
    shadowRect.y += 4;
    DrawRectangleRounded(shadowRect, 0.05f, 10, Fade(BLACK, 0.15f));

    // Nền bảng
    DrawRectangleRounded(tableArea, 0.05f, 10, Color{ 250, 250, 250, alpha });
    DrawRectangleRoundedLines(tableArea, 0.05f, 10, Color{ 200, 200, 200, alpha });

    // Header gradient
    Rectangle headerRect = { tableArea.x, tableArea.y, tableArea.width, headerHeight };
    DrawRectangleGradientV(headerRect.x, headerRect.y, headerRect.width, headerRect.height,
        Color{ 200, 220, 255, alpha }, Color{ 180, 200, 255, alpha });
    DrawRectangleLinesEx(headerRect, 1, Color{ 160, 160, 200, alpha });

    // Header text
    const float fontSize = 20;
    Vector2 headerInPos = MeasureTextEx(myFont, "Inorder", fontSize, 1);
    Vector2 headerPrePos = MeasureTextEx(myFont, "Preorder", fontSize, 1);
    Vector2 headerPostPos = MeasureTextEx(myFont, "Postorder", fontSize, 1);

    DrawTextEx(myFont, "Inorder", {
        tableArea.x + (colWidth - headerInPos.x) / 2,
        tableArea.y + (headerHeight - headerInPos.y) / 2
        }, fontSize, 1, DARKGRAY);

    DrawTextEx(myFont, "Preorder", {
        tableArea.x + colWidth + (colWidth - headerPrePos.x) / 2,
        tableArea.y + (headerHeight - headerPrePos.y) / 2
        }, fontSize, 1, DARKGRAY);

    DrawTextEx(myFont, "Postorder", {
        tableArea.x + 2 * colWidth + (colWidth - headerPostPos.x) / 2,
        tableArea.y + (headerHeight - headerPostPos.y) / 2
        }, fontSize, 1, DARKGRAY);

    BeginScissorMode((int)tableArea.x, (int)(tableArea.y + headerHeight),
        (int)tableArea.width, (int)(tableArea.height - headerHeight));

    // Vẽ hàng dữ liệu
    for (int i = 0; i < numRows; i++) {
        float y = tableArea.y + headerHeight + i * rowHeight - scrollY;

        if (y + rowHeight < tableArea.y || y > tableArea.y + tableArea.height) continue;

        // Màu hàng xen kẽ
        Color rowColor = i % 2 == 0 ? Color{ 240, 245, 255, alpha } : Color{ 255, 255, 255, alpha };
        DrawRectangle(tableArea.x, y, tableArea.width, rowHeight, rowColor);
        DrawLine(tableArea.x, y, tableArea.x + tableArea.width, y, Color{ 220, 220, 220, alpha });

        std::string inText = TextFormat("%d", inorder[i]);
        std::string preText = TextFormat("%d", preorder[i]);
        std::string postText = TextFormat("%d", postorder[i]);

        Vector2 inSize = MeasureTextEx(myFont, inText.c_str(), fontSize, 1);
        Vector2 preSize = MeasureTextEx(myFont, preText.c_str(), fontSize, 1);
        Vector2 postSize = MeasureTextEx(myFont, postText.c_str(), fontSize, 1);

        DrawTextEx(myFont, inText.c_str(), {
            tableArea.x + (colWidth - inSize.x) / 2,
            y + (rowHeight - inSize.y) / 2
            }, fontSize, 1, BLACK);

        DrawTextEx(myFont, preText.c_str(), {
            tableArea.x + colWidth + (colWidth - preSize.x) / 2,
            y + (rowHeight - preSize.y) / 2
            }, fontSize, 1, BLACK);

        DrawTextEx(myFont, postText.c_str(), {
            tableArea.x + 2 * colWidth + (colWidth - postSize.x) / 2,
            y + (rowHeight - postSize.y) / 2
            }, fontSize, 1, BLACK);
    }

    EndScissorMode();

    // Scrollbar
    if (contentHeight > tableArea.height) {
        float scrollbarWidth = 12;
        float scrollbarX = tableArea.x + tableArea.width - scrollbarWidth;
        float scrollbarY = tableArea.y + headerHeight;
        float scrollbarHeight = tableArea.height - headerHeight;

        DrawRectangle(scrollbarX, scrollbarY, scrollbarWidth, scrollbarHeight, Color{ 230, 230, 230, alpha });

        float thumbHeight = (tableArea.height / contentHeight) * scrollbarHeight;
        thumbHeight = Clamp(thumbHeight, 20.0f, scrollbarHeight);
        float maxScroll = contentHeight - tableArea.height;
        float thumbY = scrollbarY + (scrollY / maxScroll) * (scrollbarHeight - thumbHeight);

        Color thumbColor = scrollbarDragged ? DARKGRAY :
            (scrollbarHovered ? Color{ 100, 100, 100, alpha } : Color{ 140, 140, 140, alpha });

        DrawRectangleRounded(Rectangle{ scrollbarX, thumbY, scrollbarWidth, thumbHeight },
            0.5f, 8, thumbColor);
    }
}

float TraversalTable::Lerp(float start, float end, float t) {
    // Đảm bảo t nằm trong khoảng [0, 1]
    t = t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t);
    return start + t * (end - start);
}

float TraversalTable::Clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}