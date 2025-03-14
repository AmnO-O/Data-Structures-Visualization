#include "HashTable.h"
#include <string>

HashTable::HashTable(int x) {
    sz = x; isInitial = true;
    this->AnimationTime = 0.f;
    this-> AnimationDuration = 3.f;

	for (int i = 0; i < sz; i++) htable[i] = -1; /// all slots = -1
}

vector <int> HashTable::getInsertionPath(int val) {
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

int HashTable::search(int val) {
    int pos = val % sz; 
    int startIndex = pos;

    while (true) {
        if (val == htable[pos]) return pos; 
        pos++; 
        if (pos >= sz) pos -= sz; 
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

Vector2 HashTable::Lerp(Vector2 start, Vector2 end, float t) {
    Vector2 result;
    result.x = start.x + (end.x - start.x) * t;
    result.y = start.y + (end.y - start.y) * t;
    return result;
}

Vector2 HashTable::getCenter(int index) {
    int col = index % nodesPerRow;
    int row = index / nodesPerRow;
    float x = startX + col * (slotWidth + margin) + slotWidth / 2.0f;
    float y = startY + row * (slotHeight + margin) + slotHeight / 2.0f;
    return { x, y };
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

void HashTable::drawSlot(Rectangle rect, int index, const std::string& text, Font font, bool highlight) {
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
    if (text.size()) {
        int fontSize = 20;
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
    border = { 166, 500, 166, 210 };
    Size = {{250, 521, 65, 25}};

    info = { 0, 350, 332, 150}; 

    Empty = {{210, 600, 90, 30}, "Empty"};
    Random = {{210, 650, 90, 30}, "Random"};
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
            int numOcuppied = random(1, H.getSize() / 2);

            for (int i = 0; i < numOcuppied; i++) {
                int val = random(0, 200); 
                H.ins(val);
            }
        }
    }
    else if (Input.isInsert) {

    }
    else if (Input.isRemove) {

    }
    else {

    }

    return HashTable_state; 
}


void HashTableVisual::draw() {
    Input.draw();
    DrawRectangleRec(border, Color{ 220, 180, 120, 255 });
    DrawRectangleRec(info, DARKGRAY);

    Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

    DrawTextEx(font, "[INFO]:", {info.x + 10, info.y + 10 }, 21, 1, PURPLE);

    H.draw(font);

    if (Input.isCreate) {
        int fontSize = 24;
        float spacing = 1.0f;


        DrawTextEx(font, "Size:", {185, 520}, fontSize, spacing, BLACK);
        Size.draw(); 
        Empty.draw();
        Random.draw(); 
    }
    else if (Input.isInsert) {

    }
    else if (Input.isRemove) {

    }
    else {

    }
}

