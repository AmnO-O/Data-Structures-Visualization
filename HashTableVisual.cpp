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

    File = { { PANEL_PADDING + 200, 450, 130, 40 }, "File" };
    Random = { { PANEL_PADDING + 200, 510, 130, 40 }, "Random" };

    int rectWidth = 400;
    int rectHeight = 300;
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

    frameCnt = 0;

    insertCode = {
        "if Occupied + 1 == htable.size, return;",
        "int slot = value % htable.size;",
        " ",
        "while(htable[slot] != EMPTY)",
        "    slot = (slot + 1) % htable.size;",
        " ",
        "htable[slot] = value, Occupied ++;"
    };

    deleteCode = {
        "if Occupied == 0, return;",
        "int slot = value % htable.size;",
        " ",
        "while(htable[slot] != EMPTY)",
        "    if(htable[slot] != value)",
        "       slot = (slot + 1) % htable.size;",
        "    else",
        "       htable[slot] = EMPTY;",
        "       Occupied --, break;"
    };

    searchCode = {
        "int slot = value % htable.size;",
        " ",
        "while(htable[slot] != EMPTY)",
        "    if(htable[slot] != value)",
        "       slot = (slot + 1) % htable.size;",
        "    else",
        "       return slot; ",
        " ",
        "return Not_Found",
    };

    animation.active = false;
    animation.segmentDuration = 0.8f;
    animation.currentPathIndex = 0;
    animation.elapsedTime = 0.0f;
    animation.operationSuccess = true;
    animation.targetIndex = -1;

    popActive = false;
    popElapsedTime = 0.0f;
    popDuration = 0.65f; // 1 second for pop animation.
    popScale = 0.0f;

    shrinkActive = false;
    shrinkElapsedTime = 0.0f;
    shrinkDuration = 0.65f; // 1 second removal effect.
    shrinkScale = 1.0f;

    searchActive = false;
    searchElapsedTime = 0.0f;
    searchDuration = 0.65f; // 1 second search effect.
}

int HashTableVisual::handleEvent() {
    toolbar.Update();

    if (Input.update()) {
        valueAnimation = true;
        currentLine = valueTime = 0;
        animation.operationSuccess = true;
        warning = "";
        toolbar.isOpen = toolbar.isPlaying = false;


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

        if (Size.isEnter || Random.update()) {
            if (Size.getDigit() < 0) warning = "Size must be a \npositive integer!";
            else warning = "";
        }

        if (Size.isEnter && Size.getDigit() > 0) {

            if (Size.getDigit() > 240) {
                animation.operationSuccess = false;
                return HashTable_state;
            }
            animation.operationSuccess = true;
            H = HashTable(Size.getDigit());
        }

        if (Random.update() && Size.getDigit() > 0) {
            if (Size.getDigit() > 240) {
                animation.operationSuccess = false;
                return HashTable_state;
            }

            animation.operationSuccess = true;

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

        if (Value.isEnter) {
            currentLine = 0;
        }

        if (Value.isEnter) {
            if (Value.getDigit() < 0)
                warning = "Value must be a positive integer!";
            else warning = "", toolbar.isOpen = toolbar.isPlaying = true;
        }


        if (animation.active == false && Value.isEnter && Value.getDigit() >= 0) {
            vector <int> path = H.getInsertionPath(Value.getDigit());
            animation.key = Value.getDigit();

            animation.pathIndices = path;
            animation.currentPathIndex = 0;

            if (!path.empty() && path.size() <= H.getSize())
                animation.currentPos = H.getCenter(path[0]);
            else {
                animation.operationSuccess = false;
                toolbar.isOpen = toolbar.isPlaying = false;
                return HashTable_state;
            }

            animation.operationSuccess = true;
            animation.elapsedTime = 0.0f;
            animation.active = true;
            frameCnt = 0;
        }

        if (animation.active && toolbar.isBack && popActive == false) {
            if (animation.currentPathIndex > 0) {
                animation.currentPathIndex--;
            }

            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
        }

        if (animation.active && toolbar.isNext && popActive == false) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) animation.currentPathIndex++, animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
            else {
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 50) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 2) currentLine++;
                    }
                }
                else if (frameCnt < 50) {
                    frameCnt += toolbar.speed;
                }
                else if (!popActive) {
                    frameCnt = 0;
                    popActive = true;
                    popElapsedTime = 0.0f;
                    popScale = 0.0f;
                }

                animation.currentPos = H.getCenter(animation.pathIndices.back());
            }
        }

        if (toolbar.isBack && popActive == true) {
            if (animation.currentPathIndex > 0)
                animation.currentPathIndex--;

            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
            animation.active = true;
            popActive = false;
            popElapsedTime = 0.0f;
            popScale = 0.0f;
            frameCnt = 0.0f;
        }

        if (animation.active && toolbar.isPlaying) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) {
                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);
                animation.elapsedTime += toolbar.speed * deltaTime;

                float t = animation.elapsedTime / animation.segmentDuration;
                if (t > 1.0f) t = 1.0f;
                float smoothT = t * t * (3 - 2 * t);

                animation.currentPos = Lerp(startPos, endPos, smoothT);
                if (animation.elapsedTime >= animation.segmentDuration) {
                    animation.elapsedTime = 0.0f;
                    animation.currentPathIndex++;
                }

                if (animation.currentPathIndex == 0 && abs(animation.currentPos.x - startPos.x) >= 20) currentLine = 1;
            }
            else {
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 50) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 2) currentLine++;
                    }
                }
                else if (frameCnt < 50) {
                    frameCnt += toolbar.speed;
                }
                else if (!popActive) {
                    frameCnt = 0;
                    popActive = true;
                    popElapsedTime = 0.0f;
                    popScale = 0.0f;
                }
            }
        }

        if (popActive && toolbar.isPlaying) {
            popElapsedTime += deltaTime * toolbar.speed;
            float t = popElapsedTime / popDuration;
            if (t > 1.0f) t = 1.0f;

            popScale = t * t * (3 - 2 * t);

            if (popElapsedTime >= popDuration) {
                int finalIndex = animation.pathIndices.back();
                H.ins(finalIndex, animation.key);
                popActive = false;
                animation.active = false;
                toolbar.isPlaying = false;
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

        if (Value.isEnter) currentLine = 0;

        if (Value.isEnter) {
            if (Value.getDigit() < 0)
                warning = "Value must be a positive integer!";
            else warning = "", toolbar.isOpen = toolbar.isPlaying = true;
        }

        if (animation.active == false && Value.isEnter && Value.getDigit() >= 0) {
            vector <int> path;
            int final = H.search(Value.getDigit(), path);
            animation.key = Value.getDigit();
            animation.operationSuccess = final >= 0;
            if (final < 0) { /// can't remove this number because not in the hashtable
                toolbar.isOpen = toolbar.isPlaying = false;
                return HashTable_state;
            }

            animation.targetIndex = final;
            animation.currentPathIndex = 0;
            animation.pathIndices = path;
            animation.active = 1;
            if (!path.empty()) animation.currentPos = H.getCenter(path[0]);
            frameCnt = 0;
        }

        if (animation.active && toolbar.isBack && shrinkActive == false) {
            if (animation.currentPathIndex > 0) {
                animation.currentPathIndex--;
            }

            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
        }

        if (animation.active && toolbar.isNext && shrinkActive == false) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) animation.currentPathIndex++, animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
            else {
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 80) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 2) currentLine++;
                    }
                }
                else if (frameCnt < 80) {
                    frameCnt += toolbar.speed;
                }
                else if (!shrinkActive) {
                    shrinkActive = true;
                    shrinkElapsedTime = 0;
                    shrinkScale = 1;
                }
                animation.currentPos = H.getCenter(animation.pathIndices.back());
            }
        }

        if (toolbar.isBack && shrinkActive == true) {
            if (animation.currentPathIndex > 0)
                animation.currentPathIndex--;
            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
            animation.active = true;
            shrinkActive = false;
            popElapsedTime = 0.0f;
            popScale = 0.0f;
            frameCnt = 0.0f;
        }


        if (animation.active && toolbar.isPlaying) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) {
                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);

                animation.elapsedTime += deltaTime * toolbar.speed;

                float t = animation.elapsedTime / animation.segmentDuration;
                if (t > 1.0f) t = 1.0f;
                float smoothT = t * t * (3 - 2 * t);
                animation.currentPos = Lerp(startPos, endPos, smoothT);
                if (animation.elapsedTime >= animation.segmentDuration) {
                    animation.elapsedTime = 0.0f;
                    animation.currentPathIndex++;
                }

                if (animation.currentPathIndex == 0 && abs(animation.currentPos.x - startPos.x) >= 20) currentLine = 1;
            }
            else {
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 50) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 2) currentLine++;
                    }
                }
                else if (frameCnt < 50) {
                    frameCnt += toolbar.speed;
                }
                else if (!shrinkActive) {
                    shrinkActive = true;
                    shrinkElapsedTime = 0;
                    shrinkScale = 1;
                }
            }

            if (shrinkActive) {
                shrinkElapsedTime += deltaTime * toolbar.speed;
                float t = shrinkElapsedTime / shrinkDuration;

                if (t > 1) t = 1;
                shrinkScale = 1 - t;

                if (shrinkElapsedTime >= shrinkDuration) {
                    shrinkActive = 0;
                    animation.active = 0;
                    H.rem(animation.key);
                    toolbar.isPlaying = false;
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

        if (Value.isEnter) currentLine = frameCnt = 0;

        if (Value.isEnter) {
            if (Value.getDigit() < 0)
                warning = "Value must be a positive integer!";
            else warning = "", toolbar.isOpen = toolbar.isPlaying = true;
        }

        if (animation.active == false && Value.isEnter && Value.getDigit() >= 0) {
            vector <int> path;
            animation.key = Value.getDigit();
            int final = H.search(animation.key, path);
            animation.operationSuccess = final >= 0;
            animation.pathIndices = path;
            animation.targetIndex = (final >= 0 ? final : animation.key % H.getSize());
            animation.currentPathIndex = 0;
            animation.active = 1;
            if (path.size()) animation.currentPos = H.getCenter(path[0]);
        }

        if (animation.active && toolbar.isBack && searchActive == false) {
            if (animation.currentPathIndex > 0) {
                animation.currentPathIndex--;
            }

            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
        }

        if (animation.active && toolbar.isNext && searchActive == false) {
            if (animation.currentPathIndex < animation.pathIndices.size() - 1) animation.currentPathIndex++, animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
            else {
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 50) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 1) currentLine++;
                    }
                }
                else if (frameCnt < 50) {
                    frameCnt += toolbar.speed;
                }
                else if (searchActive == false) {
                    searchActive = 1;
                    searchElapsedTime = 0;
                }

                animation.currentPos = H.getCenter(animation.pathIndices.back());
            }
        }

        if (toolbar.isBack && searchActive == true) {
            if (animation.currentPathIndex > 0)
                animation.currentPathIndex--;
            animation.currentPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);

            animation.active = 1;
            searchActive = 0;
            searchElapsedTime = 0;
            frameCnt = 0.0f;
        }

        if (animation.active && toolbar.isPlaying) {
            if (animation.pathIndices.size() && animation.currentPathIndex < animation.pathIndices.size() - 1) {

                Vector2 startPos = H.getCenter(animation.pathIndices[animation.currentPathIndex]);
                Vector2 endPos = H.getCenter(animation.pathIndices[animation.currentPathIndex + 1]);

                animation.elapsedTime += deltaTime * toolbar.speed;
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
                if (animation.currentPathIndex < 2) {
                    if (frameCnt < 50) {
                        frameCnt += toolbar.speed;
                    }
                    else {
                        animation.currentPathIndex++;
                        frameCnt = 0;
                        currentLine++;
                        if (currentLine == 1) currentLine++;
                    }
                }
                else if (frameCnt < 50) {
                    frameCnt += toolbar.speed;
                }
                else if (searchActive == false) {
                    searchActive = 1;
                    searchElapsedTime = 0;
                }
            }
        }

        if (searchActive && toolbar.isPlaying) {
            searchElapsedTime += deltaTime * toolbar.speed;
            if (searchElapsedTime >= searchDuration) {
                searchActive = false;
                animation.active = false;
                toolbar.isPlaying = false;
            }
        }
    }

    return HashTable_state;
}

void HashTableVisual::drawBackgroundInfo(int posX, int posY, int width, int height) {
    const Color PANEL_BACKGROUND = { 240, 245, 255, 240 };
    const Color PANEL_BORDER = { 180, 190, 210, 255 };

    DrawRectangle(posX - 5, posY - 5, width + 10, height + 10, { 150, 150, 150, 50 }); // Subtle shadow
    DrawRectangleRounded({ (float)posX, (float)posY, (float)width, (float)height }, 0.1f, 10, PANEL_BACKGROUND);
    DrawRectangleRoundedLinesEx({ (float)posX, (float)posY, (float)width, (float)height }, 0.1f, 10, 1, PANEL_BORDER);
}

void HashTableVisual::drawCode(int posX, int posY, int width, int height, const vector<string>& codeLines) {
    const Color BACKGROUND_COLOR = { 245, 245, 250, 255 };
    const Color CODE_HIGHLIGHT = { 255, 255, 200, 200 };
    const Color EMPTY_SLOT_COLOR = { 230, 230, 230, 255 };
    const Color FILLED_SLOT_COLOR = { 200, 210, 230, 255 };
    const Color CURRENT_SLOT_COLOR = { 255, 240, 150, 255 };
    const Color FOUND_SLOT_COLOR = { 150, 230, 150, 255 };
    const Color NOT_FOUND_COLOR = { 230, 150, 150, 255 };
    const Color TEXT_PRIMARY = { 50, 60, 80, 255 };
    const Color TEXT_SECONDARY = { 100, 110, 130, 255 };
    const Color BUTTON_COLOR = { 210, 220, 240, 255 };
    const Color BUTTON_HOVER = { 190, 200, 220, 255 };
    const Color BUTTON_ACTIVE = { 170, 180, 200, 255 };
    const Color TITLE_COLOR = { 200, 50, 50, 255 }; // Red for title


    const int lineSpacing = 6; // Increased for readability
    const int fontSize = 20;
    const int titleFontSize = 24;
    int yCursor = posY + 15; // Start higher for title

    const char* title = "[INFO]:";
    DrawTextEx(font, title, Vector2{ (float)(posX + 20), (float)yCursor }, titleFontSize, 1, { 255, 0, 0, 255 });
    yCursor += titleFontSize + 15; // Space after title

    // Draw code lines
    for (int i = 0; i < (int)codeLines.size(); i++) {
        string line = codeLines[i];
        int textX = posX + 30;

        // Split code and comments
        size_t commentPos = line.find("//");
        string code = (commentPos != std::string::npos) ? line.substr(0, commentPos) : line;
        string comment = (commentPos != std::string::npos) ? line.substr(commentPos) : "";

        // Draw code with shadow for contrast
        DrawTextEx(font, code.c_str(), Vector2{ (float)textX + 1, (float)yCursor + 1 }, fontSize, 1, { 0, 0, 0, 50 }); // Shadow
        DrawTextEx(font, code.c_str(), Vector2{ (float)textX, (float)yCursor }, fontSize, 1, TEXT_PRIMARY);
        if (!comment.empty()) {
            int commentX = textX + MeasureTextEx(font, code.c_str(), fontSize, 1).x + 10;
            DrawTextEx(font, comment.c_str(), Vector2{ (float)commentX, (float)yCursor }, fontSize, 1, TEXT_SECONDARY);
        }
        yCursor += fontSize + lineSpacing;
    }

    // Draw interactive buttons
    //const int buttonWidth = 80;
    //const int buttonHeight = 30;
    //const int buttonSpacing = 10;
    //int buttonY = posY + height - 50;

    //DrawRectangleRounded({ (float)posX + 20, (float)buttonY, (float)buttonWidth, (float)buttonHeight }, 0.2f, 8, BUTTON_COLOR);
    //DrawTextEx(font, "Execute", Vector2{ (float)posX + 30, (float)buttonY + 5 }, 16, 1, TEXT_PRIMARY);

    //DrawRectangleRounded({ (float)posX + 20 + buttonWidth + buttonSpacing, (float)buttonY, (float)buttonWidth, (float)buttonHeight }, 0.2f, 8, BUTTON_COLOR);
    //DrawTextEx(font, "Explain", Vector2{ (float)posX + 30 + buttonWidth + buttonSpacing, (float)buttonY + 5 }, 16, 1, TEXT_PRIMARY);

    //DrawRectangleRounded({ (float)posX + 20 + 2 * (buttonWidth + buttonSpacing), (float)buttonY, (float)buttonWidth, (float)buttonHeight }, 0.2f, 8, BUTTON_COLOR);
    //DrawTextEx(font, "Lucky", Vector2{ (float)posX + 30 + 2 * (buttonWidth + buttonSpacing), (float)buttonY + 5 }, 16, 1, TEXT_PRIMARY);
}

void HashTableVisual::highlightCode(int posX, int posY, int width, int height, int currentLine) {
    const Color CODE_HIGHLIGHT = { 255, 255, 200, 200 };

    const int lineSpacing = 6; // Increased for readability
    const int fontSize = 20;

    const int titleFontSize = 24;

    int yCursor = posY + 15; // Start higher for title
    yCursor += titleFontSize + 15;

    for (int i = 0; i <= currentLine; i++) {
        if (i == currentLine) {
            DrawRectangleRounded(
                { (float)posX + 10, (float)yCursor - 5, (float)width - 20, (float)fontSize + 10 },
                0.3f, 8, CODE_HIGHLIGHT
            );
            DrawRectangleRoundedLinesEx(
                { (float)posX + 10, (float)yCursor - 5, (float)width - 20, (float)fontSize + 10 },
                0.3f, 8, 1, { 200, 200, 200, 255 }
            );
        }

        yCursor += fontSize + lineSpacing;
    }
}

void HashTableVisual::drawCode() {
    int rectWidth = 450;
    int rectHeight = 300;
    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;
    int panelMargin = 250;

    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };

    DrawRectangleRounded(info, 0.1, 8, panelColory);

    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(infoFont, "[INFO]:", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);
}

void HashTableVisual::draw() {
    int panelMargin = 250;
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };

    toolbar.Draw();
    File.isChose = Random.isChose = 0;

    if (animation.active == false) H.draw(font);

    if (Input.isCreate) {
        DrawRectangleRounded(Rectangle{ PANEL_WIDTH, panelMargin * 1.f, PANEL_WIDTH, Screen_h * 1.f - 2 * panelMargin }, 0.2, 9, panelColorx);
        drawBackgroundInfo(info.x, info.y, info.width, info.height);

        int fontSize = 24;
        float spacing = 1.0f;

        DrawTextEx(font, "Size: ", { 207, 352 }, fontSize, spacing, BLACK);
        Size.draw();
        File.draw(smallFont);
        Random.draw(smallFont);

        if (animation.operationSuccess == 0) {
            string msg = "Size too large! \nMust be <= 240.";
            DrawTextEx(font, msg.c_str(), { Size.bounds.x - 40, Size.bounds.y + 40 }, 18, 1, RED);
        }

        if (warning.size()) {
            DrawTextEx(font, warning.c_str(), { Size.bounds.x - 40, Size.bounds.y + 40 }, 18, 1, RED);
        }
    }
    else if (Input.isInsert) {

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[min(animation.currentPathIndex, (int)animation.pathIndices.size() - 1)] == i));

        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();

        if (animation.active && !popActive && animation.currentPathIndex < animation.pathIndices.size()) { // animation process
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

        drawBackgroundInfo(info.x, info.y, info.width, info.height);

        if (animation.operationSuccess == 0) {
            string msg = "The hashtable is full now, cannot insert more!";
            DrawTextEx(font, msg.c_str(), { valueButton.rect.x, valueButton.rect.y + 50 }, 18, 1, RED);
            highlightCode(info.x, info.y, info.width, info.height, 0);
        }

        if (warning.size()) {
            DrawTextEx(font, warning.c_str(), { valueButton.rect.x, valueButton.rect.y + 50 }, 18, 1, RED);
        }

        if (animation.active) {
            if (!popActive) {
                if (animation.pathIndices.size() == 1 || (animation.currentPathIndex > animation.pathIndices.size() && currentLine < 2)) {
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);
                }
                else if (animation.currentPathIndex > 0) {
                    highlightCode(info.x, info.y, info.width, info.height, 3);
                    if (animation.pathIndices.size() > 1) highlightCode(info.x, info.y, info.width, info.height, 4);
                }
                else
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);

            }
            else {
                highlightCode(info.x, info.y, info.width, info.height, 6);
            }
        }

        drawCode(info.x, info.y, info.width, info.height, insertCode);

        if (popActive) {
            int finalIndex = animation.pathIndices.back();
            Vector2 center = H.getCenter(finalIndex);
            int fontSize = 20;
            std::string keyText = std::to_string(animation.key);
            int textWidth = MeasureTextEx(font, keyText.c_str(), fontSize, 1).x;

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
        drawBackgroundInfo(info.x, info.y, info.width, info.height);

        if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[min(animation.currentPathIndex, (int)animation.pathIndices.size() - 1)] == i), !(animation.pathIndices.back() == i && shrinkActive));

        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();

        if (animation.operationSuccess == 0) {
            string msg = "Number " + to_string(Value.getDigit()) + " was not found in hashtable!";
            DrawTextEx(font, msg.c_str(), { valueButton.rect.x, valueButton.rect.y + 50 }, 18, 1, RED);
            highlightCode(info.x, info.y, info.width, info.height, currentLine);
        }

        if (warning.size()) {
            DrawTextEx(font, warning.c_str(), { valueButton.rect.x, valueButton.rect.y + 50 }, 18, 1, RED);
        }

        if (animation.active && !shrinkActive && animation.currentPathIndex < animation.pathIndices.size()) {
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

        if (animation.active) {
            if (!shrinkActive) {
                if (animation.pathIndices.size() == 1 || (animation.currentPathIndex > animation.pathIndices.size() && currentLine < 2)) {
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);
                    if (currentLine == 3) highlightCode(info.x, info.y, info.width, info.height, 4);
                }
                else if (animation.currentPathIndex > 0) {
                    highlightCode(info.x, info.y, info.width, info.height, 3);
                    highlightCode(info.x, info.y, info.width, info.height, 4);
                    if (animation.pathIndices.size() > 1) highlightCode(info.x, info.y, info.width, info.height, 5);
                }
                else
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);
            }
            else {
                highlightCode(info.x, info.y, info.width, info.height, 7);
                highlightCode(info.x, info.y, info.width, info.height, 8);
            }
        }

        drawCode(info.x, info.y, info.width, info.height, deleteCode);


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
        drawBackgroundInfo(info.x, info.y, info.width, info.height);

        if (animation.active && animation.pathIndices.size())
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[min(animation.currentPathIndex, (int)animation.pathIndices.size() - 1)] == i));
        else if (animation.active)
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.targetIndex == i));


        int fontSize = 24;
        float spacing = 1.0f;

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);

        valueButton.draw(smallFont);
        Value.draw();

        if (warning.size()) {
            DrawTextEx(font, warning.c_str(), { valueButton.rect.x, valueButton.rect.y + 50 }, 18, 1, RED);
        }

        if (animation.active) {
            if (!searchActive) {
                if (animation.pathIndices.size() <= 1 || (animation.currentPathIndex > animation.pathIndices.size() && currentLine < 2)) {
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);
                }
                else if (animation.currentPathIndex > 0) {
                    highlightCode(info.x, info.y, info.width, info.height, 2);
                    highlightCode(info.x, info.y, info.width, info.height, 3);
                    if (animation.pathIndices.size() > 1) highlightCode(info.x, info.y, info.width, info.height, 4);
                }
                else
                    highlightCode(info.x, info.y, info.width, info.height, currentLine);
            }
            else {

                if (animation.operationSuccess) highlightCode(info.x, info.y, info.width, info.height, 6);
                else highlightCode(info.x, info.y, info.width, info.height, 8);
            }
        }

        if (animation.active && !searchActive && animation.pathIndices.size() && animation.currentPathIndex < animation.pathIndices.size()) {
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

        drawCode(info.x, info.y, info.width, info.height, searchCode);


        if (searchActive) {
            int finalIndex = animation.targetIndex;
            Vector2 center = H.getCenter(finalIndex);
            float pulse = 1.f + 0.4f * sinf((searchElapsedTime / searchDuration) * 2 * PI);
            float ringRadius = 30 * pulse;
            Color ringColor = animation.operationSuccess ? GREEN : RED;

            // Độ dày vòng tròn (điều chỉnh giá trị này)
            float thickness = 5.0f; // độ dày 5 pixel

            DrawRing(center, ringRadius, ringRadius + thickness, 0, 360, 100, ringColor); // 100 là số lượng phân đoạn
        }
    }

    DrawRectangleRounded(Rectangle{ 0, panelMargin * 1.f, PANEL_WIDTH, Screen_h * 1.f - 2 * panelMargin }, 0.2, 9, panelColor);
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(smallFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    Input.draw(smallFont);
    loadFile.draw(font);

    frameCnt += toolbar.speed;
}

HashTableVisual::~HashTableVisual() {
    UnloadFont(font);
    UnloadFont(infoFont);
    UnloadFont(smallFont);
}
