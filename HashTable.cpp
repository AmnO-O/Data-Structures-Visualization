#include "HashTable.h"
#include <string>

HashTable::HashTable(int x) {
    sz = x; isInitial = true;
    this->AnimationTime = 0.f;
    this-> AnimationDuration = 3.f;

	for (int i = 0; i < sz; i++) htable[i] = -1; /// all slots = -1
}

vector <int> HashTable::getInsertionPath(int val) {
    if (sz <= 0) return vector <int> {};
    vector <int> path; 
    int pos = val % sz; 
    int start = pos; 

    while (htable[pos] >= 0) {
        path.push_back(pos); 
        pos++; 

        if (pos >= sz) pos -= sz; 
        if (pos == start) break; 
    }
    
    path.push_back(pos); 
    return path; 
}

void HashTable::ins(int val) {
    htable[getInsertionPath(val).back()] = val; 
}

void HashTable::ins(int index, int val) {
    htable[index] = val;
}

int HashTable::search(int val) {
    int pos = val % sz; 
    int startIndex = pos;

    int cnt = 0; 

    while (true) {
        if (val == htable[pos]) return pos; 
        pos++; 
        if (pos >= sz) pos -= sz; 
        cnt++; 
        if (cnt >= sz) return -1; 

    }
    return -1; 
}

int HashTable::getSize() const{
    return sz; 
}

void HashTable::rem(int val) {
    int pos = search(val); 
    if (pos != -1) htable[pos] = -1; 
}

Vector2 HashTable::getCenter(int index) {
    int col = index % nodesPerRow;
    int row = index / nodesPerRow;
    float x = startX + col * (slotWidth + margin) + slotWidth / 2.0f;
    float y = startY + row * (slotHeight + margin) + slotHeight / 2.0f;
    return { x, y };
}

int HashTable :: getValue(int pos) const {
    if (pos >= sz) return -1; 
    return htable[pos]; 
}

void HashTable::upd(float delta) {
    if (isInitial) {
        AnimationTime += delta;
        if (AnimationTime >= AnimationDuration) {
            AnimationTime = AnimationDuration; 
            isInitial = 0; 
        }
    }
}


int HashTable::search(int val, vector <int>& path) {
    int pos = val % sz;
    int startIndex = pos;
    int cnt = 0; 

    while (true) {
        path.push_back(pos); 
        if (val == htable[pos]) return pos;
        pos++;
        if (pos >= sz) pos -= sz;
        cnt++; 
        if (cnt >= sz) {
            path.clear();
            return -1; 
        }
    }
    return -1;
}

void HashTable::draw(Font font) {
    float speed = 1.f; 
    if (isInitial) {
        speed = AnimationTime / AnimationDuration; 
        if (speed > 1) speed = 1; 
    }

    for (int i = 0; i < sz; i++) {
        int col = i % nodesPerRow;
        int row = i / nodesPerRow;
        Vector2 finalCenter = getCenter(i);
        Vector2 currentCenter = finalCenter;
        
        Vector2 commonStart = {startX, startY}; 

        if (isInitial) {
            currentCenter = Lerp(commonStart, finalCenter, speed);
        }

        // Create rectangle from center.
        Rectangle rect = { currentCenter.x - slotWidth / 2.0f, currentCenter.y - slotHeight / 2.0f, (float)slotWidth, (float)slotHeight };

        bool occupied = htable[i] >= 0;
        std::string keyText = occupied ? std::to_string(htable[i]) : "";

        drawSlot(rect, i, keyText, font);
    }
}

void HashTable::drawValue(int index, Font font, bool highlight) {
    Vector2 finalCenter = getCenter(index);
    Vector2 currentCenter = finalCenter;

    Rectangle rect = { currentCenter.x - slotWidth / 2.0f, currentCenter.y - slotHeight / 2.0f, (float)slotWidth, (float)slotHeight };
    bool occupied = htable[index] >= 0;
    std::string text = occupied ? std::to_string(htable[index]) : "";

    if (highlight) {
        drawSlot(rect, index, text, font, true);
        return;
    }

    int fontSize = 22;
    int textWidth = MeasureText(text.c_str(), fontSize);
    textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;

    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;
    //DrawText(text.c_str(), textX, textY, fontSize, DARKBLUE);
    DrawTextEx(font, text.c_str(), { textX * 1.f, textY * 1.f }, fontSize, 1, BLACK);
}

void HashTable::drawSlot(int index, Font font, bool highlight, bool Value) {
    Vector2 finalCenter = getCenter(index);
    Vector2 currentCenter = finalCenter;

    Rectangle rect = { currentCenter.x - slotWidth / 2.0f, currentCenter.y - slotHeight / 2.0f, (float)slotWidth, (float)slotHeight };
    bool occupied = htable[index] >= 0;
    std::string text = occupied ? std::to_string(htable[index]) : "";

    drawSlot(rect, index, text, font, highlight, Value);
}

void HashTable::drawSlot(Rectangle rect, int index, const std::string& text, Font font, bool highlight, bool Value) {
    Color baseColor = highlight ? SKYBLUE : LIGHTGRAY;
    float roundness = 0.3f;
    int segments = 8;

    // Drop shadow.
    Rectangle shadowRect = { rect.x + 5, rect.y + 5, rect.width, rect.height };
    DrawRectangleRounded(shadowRect, roundness, segments, Fade(BLACK, 0.3f));

    // Slot background.
    DrawRectangleRounded(rect, roundness, segments, baseColor);
    DrawRectangleRoundedLines(rect, roundness, segments, BLACK);

    // If occupied, draw the key centered.
    if (text.size() && Value) {
        int fontSize = 22;
        int textWidth = MeasureText(text.c_str(), fontSize);
        textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;

        int textX = rect.x + (rect.width - textWidth) / 2;
        int textY = rect.y + (rect.height - fontSize) / 2;
        //DrawText(text.c_str(), textX, textY, fontSize, DARKBLUE);
        DrawTextEx(font, text.c_str(), { textX * 1.f, textY * 1.f }, fontSize, 1, BLACK);
    }

    // Draw a badge with the slot index.
    //int badgeRadius = 12;
    //int badgeX = rect.x + badgeRadius + 5;
    //int badgeY = rect.y + badgeRadius + 5;
    //DrawCircle(badgeX, badgeY, badgeRadius, DARKGRAY);
    //std::string indexStr = std::to_string(index);
    //int idxFontSize = 13;
    //int idxTextWidth = MeasureText(indexStr.c_str(), idxFontSize);
    //
    //idxTextWidth = MeasureTextEx(font, indexStr.c_str(), idxFontSize, 1).x;

    ////DrawText(indexStr.c_str(), badgeX - idxTextWidth / 2, badgeY - idxFontSize / 2, idxFontSize, WHITE);

    //DrawTextEx(font, indexStr.c_str(), { 1.f * badgeX - idxTextWidth / 2, 1.f * badgeY - idxFontSize / 2 }, idxFontSize, 1, WHITE);
    // Number the slot right below it.

    int fontSize = 20;
    std::string indexStr = std::to_string(index);
    int textWidth = MeasureText(indexStr.c_str(), fontSize);
    textWidth = MeasureTextEx(font, indexStr.c_str(), fontSize, 1).x;

    int posX = rect.x + (rect.width - textWidth) / 2;
    int posY = rect.y + rect.height + 5; // 5 pixels below the slot.

    DrawTextEx(font, indexStr.c_str(), { posX * 1.f, posY * 1.f }, fontSize, 1, BLUE);
}


HashTableVisual::HashTableVisual() {
    border = { 166, 500, 166, 210 }; /// orange region
    Size = {{250, 521, 65, 25}};

    info = { 0, 350, 332, 150}; 
  
    Value = { {205, 590, 90, 30} };
    Value.fontSize = 22; 

    Empty = {{210, 600, 90, 30}, "Empty"};
    Random = {{210, 650, 90, 30}, "Random"};

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

int HashTableVisual::handleEvent() {
    Input.update(); 
    float deltaTime = GetFrameTime();

    H.upd(deltaTime);

    if (Input.isCreate) {
        Size.update(); 

        if (Empty.update()) {
            H = HashTable(Size.getDigit());
        }

        if (Random.update()) {
            H = HashTable(Size.getDigit());
            int numOcuppied = random(1,1 + H.getSize() / 2);

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
            int final = H.search(Value.getDigit(), path) ;
            animation.key = Value.getDigit(); 
            animation.operationSuccess = final >= 0; 
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
    Input.draw();
    DrawRectangleRec(border, Color{ 220, 180, 120, 255 });
    DrawRectangleRec(info, DARKGRAY);

    Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

    DrawTextEx(font, "[INFO]:", {info.x + 10, info.y + 10 }, 21, 1, PURPLE);

    if(animation.active == false) H.draw(font);

    if (Input.isCreate) {
        int fontSize = 24;
        float spacing = 1.0f;


        DrawTextEx(font, "Size:", {185, 520}, fontSize, spacing, BLACK);
        Size.draw(); 
        Empty.draw();
        Random.draw(); 
    }
    else if (Input.isInsert) {
        if (animation.active) 
            for (int i = 0; i < H.getSize(); i++) H.drawSlot(i, font, (animation.pathIndices[animation.currentPathIndex] == i));

        int fontSize = 24;
        float spacing = 1.0f;

        int textWidth = MeasureTextEx(font, "Value", fontSize, 1.f).x;
        DrawTextEx(font, "Value", {205 * 1.f + 90 / 2 - textWidth / 2, 555 }, fontSize, spacing, BLACK);
        Value.draw();

        if (animation.active && !popActive) {
            int fontSize = 20;
            int cur_key =  H.getValue(animation.pathIndices[animation.currentPathIndex]);
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
            int textWidth = MeasureTextEx(font,keyText.c_str(), fontSize,1).x;
            // Draw with scaling (popScale varies from 0 to 1).
            float radius = 20 * popScale;
            for (int i = 5; i > 0; i--) {
                DrawCircleV(center, (radius + i * 3), Fade(GREEN, 0.1f * i));
            }
            DrawCircleV(center, radius, GREEN);
            // Scale text as well.
            int scaledFontSize = (int)(fontSize * popScale);
            int scaledTextWidth = MeasureTextEx(font,keyText.c_str(), scaledFontSize, 1).x;
            DrawTextEx(font, keyText.c_str(), {center.x - scaledTextWidth / 2, center.y - scaledFontSize / 2}, scaledFontSize, 1, WHITE);
        }   
    }
    else if (Input.isRemove) {
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
            int scaledTextWidth = MeasureTextEx(font,keyText.c_str(), scaledFontSize, 1).x;

            DrawTextEx(font, keyText.c_str(), { center.x - scaledTextWidth / 2, center.y - scaledFontSize / 2 }, scaledFontSize, 1, Fade(WHITE, shrinkScale));
        }
    }
    else {
        
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

