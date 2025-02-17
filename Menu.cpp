#include "Menu.h"
#include "Constants.h"

Menu::Menu() {
    int startX = 440; 
    int startY = 200; 

    for (int i = 0; i < 4; i++) {
        Item item; 
        int row = i / 2; 
        int col = i & 1; 

        float x = startX + col * (Item_w + 50); 
        float y = startY + row * (Item_h + 50); 

        item.name = dsName[i];
        item.bounds = {x, y, Item_w, Item_h};
        item.hovered = 0;

        dsItem.push_back(item); 
    }
}

int Menu :: handleEvent() {
    Vector2 mouse = GetMousePosition();

    for (int id = 0; id < 4; id ++) {
        Item& item = dsItem[id]; 
        if (CheckCollisionPointRec(mouse, item.bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (id == 0) return LinkedList_state; 
                if (id == 1) return HashTable_state; 
                if (id == 2) return Trie_state; 
                return Spanning_state; 
            }

            item.hovered = true;
        }
        else item.hovered = false; 
    }

    return Menu_state; 
}

void Menu::Draw() {
    for(Item item : dsItem) {
        Color boxColor = item.hovered ? LIGHTGRAY : RAYWHITE;

        DrawRectangleRounded(item.bounds, 0.2f, 4, boxColor);
        DrawRectangleRoundedLinesEx(item.bounds, 0.2f, 4, 2.0f, GRAY);

        float centerX = item.bounds.x + Item_w / 2;
        float centerY = item.bounds.y + Item_h / 2;

        int textWidth = MeasureText(item.name.c_str(), 30);
        DrawText(item.name.c_str(), centerX - textWidth / 2, item.bounds.y + Item_h - 40, 30, DARKGRAY);
    }
}
