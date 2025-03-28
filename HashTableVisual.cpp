#include "HashTableVisual.h"
#include "Constants.h"


HashTableVisual::HashTableVisual() {
    //// Load font
    //hashTableFont = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
    //SetTextureFilter(hashTableFont.texture, TEXTURE_FILTER_BILINEAR);

    // Load font nhỏ 
    smallFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(smallFont.texture, TEXTURE_FILTER_BILINEAR);

    //// Load font cho [INFO]
    //IN4Font = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    //SetTextureFilter(IN4Font.texture, TEXTURE_FILTER_BILINEAR);

    font = LoadFont("Assets/Fonts/PublicSans-Bold.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    infoFont = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(infoFont.texture, TEXTURE_FILTER_BILINEAR);

    Size = { {270, 350, 90, 30} };

    Value = { {270, 350, 90, 30} };
    valueButton = { { PANEL_PADDING + 100, 440, 80, 30 }, ">Value" };
    valueRect = { -500, -500, 210, 55 };

    Value.fontSize = 22;
    Size.fontSize = 22;

    File = { { PANEL_PADDING + 200, 440, 130, 40 }, "File" };
    Random = { { PANEL_PADDING + 200, 500, 130, 40 }, "Random" };

    int rectWidth = 400;
    int rectHeight = 240;
    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;

    info = { posX * 1.f, posY * 1.f, rectWidth * 1.f, rectHeight * 1.f };

    loadFile.show("File Format Requirement",
        "Please provide an array of positive int,\n"
        "separated by spaces or commas.\n\n"
        "Example:\n "
        " 10, 20, 30, 40. ");

    readFile = FileLoader();

    H = HashTable(40);
    Size.text = to_string(40);

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

int HashTableVisual::handleEvent() {

    if (Input.update()) {
        valueAnimation = true;
        valueTime = 0;

        if (Input.isSearch) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[1].rect.y;
        }
        else if (Input.isInsert) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[2].rect.y;
        }
        else if (Input.isRemove) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[3].rect.y;
        }
    }

    float deltaTime = GetFrameTime();

    H.upd(deltaTime);

    if (Input.isCreate) {
        Size.update();
        if (File.update())
            loadFile.display = true;

        /// read file
        if (loadFile.update()) {
            vector<unsigned char> fileContents = readFile.loadFile();
            H.clear();

            long long num = -1;

            for (unsigned char c : fileContents) {
                if (isdigit(c)) {
                    if (num < 0) num = c + '0';
                    else num = num * 10 + c + '0';
                }
                else {
                    if (num >= 0) H.ins(num);
                    num = -1;
                }
            }
            if (num >= 0) H.ins(num);
        }

        if (Size.isEnter && Size.getDigit() > 0) {
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

        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 295.5, Input.cButton[2].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }

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
        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 295.5, Input.cButton[3].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }

        Value.update();

        if (animation.active == false && Value.isEnter) {
            vector <int> path;
            int final = H.search(Value.getDigit(), path);
            animation.key = Value.getDigit();
            animation.operationSuccess = final >= 0;
            if (final < 0) return HashTable_state;
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
        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 295.5, Input.cButton[1].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }
        if (valueAnimation) {
            valueTime += deltaTime;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }

        Value.update();

        if (animation.active == false && Value.isEnter) {
            vector <int> path;
            animation.key = Value.getDigit();
            int final = H.search(animation.key, path);

            if (final == -1) return HashTable_state;

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

    return HashTable_state;
}


void HashTableVisual::draw() {
    int panelMargin = 250; // Khoảng cách lề trên với box
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 }; // Chọn màu theo chế độ

    // Vẽ nền bảng khu vực panel 2 
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 }; // Chọn màu theo chế độ

    // DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, panelColorx);

    Color panelColory = isDarkMode ? Color{ 189, 211, 206, 255 } : Color{ 112, 140, 105, 100 }; // Chọn màu theo chế độ

    int rectWidth = 400;
    int rectHeight = 240;
    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);

    // "Operation"
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(smallFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    DrawRectangleRec(info, panelColory);

    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(infoFont, "[INFO]:", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);


    File.isChose = Random.isChose = 0;

    if (animation.active == false) H.draw(font);

    if (Input.isCreate) {
        DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, panelColorx);

        int fontSize = 24;
        float spacing = 1.0f;

        string message = "\nConstruct a hash table with Size = " + to_string(H.getSize()) + ".";
        DrawTextEx(smallFont, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, DARKGRAY);


        DrawTextEx(smallFont, "Size: ", { 207, 352 }, fontSize, spacing, BLACK);
        Size.draw();
        File.draw(smallFont);
        Random.draw(smallFont);
    }
    else if (Input.isInsert) {

        string message = "\nInsert a new value to a hash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(smallFont, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, DARKGRAY);

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[animation.currentPathIndex] == i));

        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();


        if (animation.active && !popActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = isDarkMode ? ORANGE : Color{ 189, 211, 206, 255 };

            for (int i = 5; i > 0; i--) {
                DrawCircleV(animation.currentPos, 20 + i * 3, Fade(markerColor, 0.1f * i));
                H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
            }

            H.drawValue(animation.pathIndices[animation.currentPathIndex], font);
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
        Value.bounds.y = Input.cButton[3].rect.y + 5;

        string message = "\nRemove a value in a hash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(smallFont, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, DARKGRAY);

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[animation.currentPathIndex] == i), !(animation.pathIndices.back() == i && shrinkActive));

        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();

        if (animation.active && !shrinkActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = isDarkMode ? ORANGE : Color{ 189, 211, 206, 255 };

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
        Value.bounds.y = Input.cButton[1].rect.y + 5;

        string message = "\nSearch for a value in a hash table (Size =  " + to_string(H.getSize()) + ").";
        DrawTextEx(smallFont, message.c_str(), { info.x + 10, info.y + 10 }, 21, 1, DARKGRAY);

        if (animation.active) H.draw(font);

        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();

        if (animation.active && !searchActive) {
            int fontSize = 20;
            int cur_key = H.getValue(animation.pathIndices[animation.currentPathIndex]);
            std::string keyText = std::to_string(cur_key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1.f).x;
            Color markerColor = isDarkMode ? ORANGE : Color{ 189, 211, 206, 255 };

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

    DrawRectangle(0, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, panelColor);
    Input.draw(smallFont);
    loadFile.draw(font);
}

HashTableVisual::~HashTableVisual() {
    UnloadFont(font);
    UnloadFont(infoFont);
    UnloadFont(smallFont);
}
