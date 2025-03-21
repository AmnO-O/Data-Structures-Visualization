#include "HashTableScreen.h"
#include "Constants.h"
#include "string"

void HashTableScreen::Init() {
    // Khởi tạo nút Back
    backButton = { 20, Screen_h - 60, 150, 40 };

    // Load font
    hashTableFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    SetTextureFilter(hashTableFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    myFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(myFont.texture, TEXTURE_FILTER_BILINEAR);

    border = { 166, 500, 166, 210 }; /// orange region
    Size = { {250, 521, 65, 25} };

    info = { 0, 350, 332, 150 };

    Value = { {205, 590, 90, 30} };
    Value.fontSize = 22;

    Empty = { {210, 600, 90, 30}, "Empty" };
    Random = { {210, 650, 90, 30}, "Random" };

    H = HashTable(40);

    animation.active = false;
    animation.segmentDuration = 1.5f;
    animation.currentPathIndex = 0;
    animation.elapsedTime = 0.0f;
    animation.operationSuccess = false;
    animation.targetIndex = -1;

    popActive = false;
    popElapsedTime = 0.0f;
    popDuration = 1.0f; // 1 second for pop animation.
    popScale = 0.0f;

    shrinkActive = false;
    shrinkElapsedTime = 0.0f;
    shrinkDuration = 1.0f; // 1 second removal effect.
    shrinkScale = 1.0f;

    searchActive = false;
    searchElapsedTime = 0.0f;
    searchDuration = 1.0f; // 1 second search effect.
}

void HashTableScreen::Update(int& state) {
    Input.update();
    float deltaTime = GetFrameTime();

    H.upd(deltaTime);

    Vector2 mouse = GetMousePosition();

    // Kiểm tra hover vào nút "Back"
    backHovered = CheckCollisionPointRec(mouse, backButton);

    // Xử lý sự kiện khi nhấn vào nút "Back"
    if (backHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        state = Menu_state; // Quay lại màn hình Menu
    }

    if (Input.isCreate) {
        Size.update();

        if ((Size.isEnter || Empty.update()) && Size.getDigit() > 0) {
            H = HashTable(Size.getDigit());
        }

        if (Random.update() && Size.getDigit() > 0) {
            H = HashTable(Size.getDigit());
            int numOcuppied = random(1, 1 + H.getSize() / 2);

            for (int i = 0; i < numOcuppied; i++) {
                int val = random(0, 200);
                H.ins(val);
            }
        }
    }
    else if (Input.isInsert) {
        Value.update();
        if (animation.active == false && Value.isEnter) {
            vector <int> path = H.getInsertionPath(Value.getDigit());
            animation.key = Value.getDigit();

            animation.pathIndices = path;
            animation.currentPathIndex = 0;

            if (!path.empty())
                animation.currentPos = H.getCenter(path[0]);

            animation.elapsedTime = 0.0f;
            animation.active = true;
        }

        if (animation.active) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) {
                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);
                animation.elapsedTime += deltaTime;

                float t = animation.elapsedTime / animation.segmentDuration;
                if (t > 1.0f) t = 1.0f;
                float smoothT = t * t * (3 - 2 * t);

                animation.currentPos = Lerp(startPos, endPos, smoothT);
                if (animation.elapsedTime >= animation.segmentDuration) {
                    animation.elapsedTime = 0.0f;
                    animation.currentPathIndex++;
                }
            }
            else {
                if (!popActive) {
                    popActive = true;
                    popElapsedTime = 0.0f;
                    popScale = 0.0f;
                }
            }
        }

        if (popActive) {
            popElapsedTime += deltaTime;
            float t = popElapsedTime / popDuration;
            if (t > 1.0f) t = 1.0f;
            // Using an ease-out effect for the pop (scale from 0 to 1).
            popScale = t * t * (3 - 2 * t);

            if (popElapsedTime >= popDuration) {
                int finalIndex = animation.pathIndices.back();
                H.ins(finalIndex, animation.key);
                popActive = false;
                animation.active = false;
            }
        }
    }
    else if (Input.isRemove) {

        Value.update();

        if (animation.active == false && Value.isEnter) {
            vector <int> path;
            int final = H.search(Value.getDigit(), path);
            animation.key = Value.getDigit();
            animation.operationSuccess = final >= 0;
            if (final < 0) return;
            animation.targetIndex = final;
            animation.currentPathIndex = 0;
            animation.pathIndices = path;
            animation.active = 1;
            if (!path.empty()) animation.currentPos = H.getCenter(path[0]);
        }

        if (animation.active) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) {
                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);

                animation.elapsedTime += deltaTime;

                float t = animation.elapsedTime / animation.segmentDuration;
                if (t > 1.0f) t = 1.0f;
                float smoothT = t * t * (3 - 2 * t);
                animation.currentPos = Lerp(startPos, endPos, smoothT);
                if (animation.elapsedTime >= animation.segmentDuration) {
                    animation.elapsedTime = 0.0f;
                    animation.currentPathIndex++;
                }
            }
            else {
                if (!shrinkActive) {
                    shrinkActive = true;
                    shrinkElapsedTime = 0;
                    shrinkScale = 1;
                }
            }

            if (shrinkActive) {
                shrinkElapsedTime += deltaTime;
                float t = shrinkElapsedTime / shrinkDuration;

                if (t > 1) t = 1;
                shrinkScale = 1 - t;

                if (shrinkElapsedTime >= shrinkDuration) {
                    shrinkActive = 0;
                    animation.active = 0;
                    H.rem(animation.key);
                }
            }
        }
    }
    else {
        Value.update();

        if (animation.active == false && Value.isEnter) {
            vector <int> path;
            animation.key = Value.getDigit();
            int final = H.search(animation.key, path);

            if (final == -1) return;

            animation.operationSuccess = final >= 0;
            animation.pathIndices = path;
            animation.targetIndex = final;
            animation.currentPathIndex = 0;
            animation.active = 1;
            if (path.size()) animation.currentPos = H.getCenter(path[0]);
        }


        if (animation.active) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) {

                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);

                animation.elapsedTime += deltaTime;
                float t = animation.elapsedTime / animation.segmentDuration;
                if (t > 1) t = 1;

                float smoothT = t * t * (3 - 2 * t);
                animation.currentPos = Lerp(startPos, endPos, smoothT);
                if (animation.elapsedTime >= animation.segmentDuration) {
                    animation.elapsedTime = 0.0f;
                    animation.currentPathIndex++;
                }
            }
            else {
                if (searchActive == false) {
                    searchActive = 1;
                    searchElapsedTime = 0;
                }
            }
        }

        if (searchActive) {
            searchElapsedTime += deltaTime;
            if (searchElapsedTime >= searchDuration) {
                searchActive = false;
                animation.active = false;
            }
        }
    }

    return;
}


void HashTableScreen::Draw() {
    Input.draw();
    DrawRectangleRec(border, Color{ 220, 180, 120, 255 });
    DrawRectangleRec(info, DARKGRAY);

    Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

    // Vẽ nút "Back"
    Color backColor = backHovered ? LIGHTGRAY : RAYWHITE;
    DrawRectangleRounded(backButton, 0.2f, 4, backColor);
    DrawRectangleRoundedLinesEx(backButton, 0.2f, 4, 2.0f, GRAY);

    Vector2 textSize = MeasureTextEx(myFont, "<< Back to Menu", 20, 1);
    DrawTextEx(myFont, "<< Back to Menu",
        { backButton.x + (backButton.width - textSize.x) / 2, backButton.y + (backButton.height - textSize.y) / 2 },
        20, 1, DARKBLUE);

    // Hiển thị tiêu đề "How to Use"
    int fontSize = 50;
    float spacing = 3.0f;
    Vector2 titleSize = MeasureTextEx(hashTableFont, "Hash Table", fontSize, spacing);

    float titleX = (Screen_w - titleSize.x) / 2;
    float titleY = 80; // Đưa tiêu đề lên cao hơn

    Color titleColor = isDarkMode ? WHITE : DARKBLUE;
    DrawTextEx(hashTableFont, "Hash Table",
        { titleX, titleY }, fontSize, spacing, titleColor);

    if (animation.active == false) H.draw(font);

    if (Input.isCreate) {
        int fontSize = 24;
        float spacing = 1.0f;

        string message = "[INFO]: Construct a hash table \nwith Size = " + to_string(H.getSize()) + ".";
        DrawTextEx(font, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, PURPLE);


        DrawTextEx(font, "Size:", { 185, 520 }, fontSize, spacing, BLACK);
        Size.draw();
        Empty.draw();
        Random.draw();
    }
    else if (Input.isInsert) {
        string message = "[INFO]: Insert a new value to a \nhash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(font, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, PURPLE);

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[animation.currentPathIndex] == i));

        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(font, "Value", fontSize, 1.f).x;
        DrawTextEx(font, "Value", { 205 * 1.f + 90 / 2 - textWidth / 2, 555 }, fontSize, spacing, BLACK);
        Value.draw();

        if (animation.active && !popActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = ORANGE;

            for (int i = 5; i > 0; i--) {
                DrawCircleV(animation.currentPos, 20 + i * 3, Fade(markerColor, 0.1f * i));
                H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
            }

            H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
            /// DrawTextEx(font,keyText.c_str(), {animation.currentPos.x * 1.f - textWidth / 2, animation.currentPos.y - fontSize / 2}, fontSize, 1.f, WHITE);
        }


        if (popActive) {
            int finalIndex = animation.pathIndices.back();
            Vector2 center = H.getCenter(finalIndex);
            int fontSize = 20;
            std::string keyText = std::to_string(animation.key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1).x;
            // Draw with scaling (popScale varies from 0 to 1).
            float radius = 20 * popScale;
            for (int i = 5; i > 0; i--) {
                DrawCircleV(center, (radius + i * 3), Fade(GREEN, 0.1f * i));
            }
            DrawCircleV(center, radius, GREEN);
            // Scale text as well.
            int scaledFontSize = (int)(fontSize * popScale);
            int scaledTextWidth = MeasureTextEx(font, keyText.c_str(), scaledFontSize, 1).x;
            DrawTextEx(font, keyText.c_str(), { center.x - scaledTextWidth / 2, center.y - scaledFontSize / 2 }, scaledFontSize, 1, WHITE);
        }
    }
    else if (Input.isRemove) {
        string message = "[INFO]: Remove a value in a \nhash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(font, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, PURPLE);

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[animation.currentPathIndex] == i), !(animation.pathIndices.back() == i && shrinkActive));

        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(font, "Value", fontSize, 1.f).x;
        DrawTextEx(font, "Value", { 205 * 1.f + 90 / 2 - textWidth / 2, 555 }, fontSize, spacing, BLACK);
        Value.draw();

        if (animation.active && !shrinkActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = ORANGE;

            for (int i = 5; i > 0; i--) {
                DrawCircleV(animation.currentPos, 20 + i * 3, Fade(markerColor, 0.1f * i));
            }

            H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
            /// DrawTextEx(font,keyText.c_str(), {animation.currentPos.x * 1.f - textWidth / 2, animation.currentPos.y - fontSize / 2}, fontSize, 1.f, WHITE);
        }

        if (shrinkActive) {
            int finalIndex = animation.pathIndices.back();
            Vector2 center = H.getCenter(finalIndex);

            int fontSize = 20;
            std::string keyText = std::to_string(animation.key);
            int textWidth = MeasureText(keyText.c_str(), fontSize);
            float radius = 20 * shrinkScale;
            for (int i = 5; i > 0; i--) {
                DrawCircleV(center, (radius + i * 3), Fade(RED, 0.1f * i));
            }
            DrawCircleV(center, radius, RED);
            int scaledFontSize = (int)(fontSize * shrinkScale);
            int scaledTextWidth = MeasureTextEx(font, keyText.c_str(), scaledFontSize, 1).x;

            DrawTextEx(font, keyText.c_str(), { center.x - scaledTextWidth / 2, center.y - scaledFontSize / 2 }, scaledFontSize, 1, Fade(WHITE, shrinkScale));
        }
    }
    else {
        string message = "[INFO]: Search for a value in a \nhash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(font, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, PURPLE);

        if (animation.active) H.draw(font);

        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(font, "Value", fontSize, 1.f).x;
        DrawTextEx(font, "Value", { 205 * 1.f + 90 / 2 - textWidth / 2, 555 }, fontSize, spacing, BLACK);
        Value.draw();

        if (animation.active && !searchActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = ORANGE;

            for (int i = 5; i > 0; i--) {
                DrawCircleV(animation.currentPos, 20 + i * 3, Fade(markerColor, 0.1f * i));
            }

            H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
        }

        if (searchActive) {
            int finalIndex = animation.pathIndices.back();
            Vector2 center = H.getCenter(finalIndex);
            float pulse = 1.f + 0.4f * sinf((searchElapsedTime / searchDuration) * 2 * PI);
            float ringRadius = 30 * pulse;
            Color ringColor = animation.operationSuccess ? GREEN : RED;

            // Độ dày vòng tròn (điều chỉnh giá trị này)
            float thickness = 5.0f; // độ dày 5 pixel

            // Vẽ vòng tròn rỗng bằng DrawRing
            DrawRing(center, ringRadius, ringRadius + thickness, 0, 360, 100, ringColor); // 100 là số lượng phân đoạn
        }
    }
}

void HashTableScreen::Unload() {
    UnloadFont(hashTableFont);
}
