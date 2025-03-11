#include "Linkedlist.h"
#include <string>

Linkedlist::Linkedlist() {
    root = NULL;
}

void Linkedlist::add(int key) {
    if (!root) {
        root = new Nodell ( key, { 100, 200 });
        return;
    }
    Nodell* cur = root;
    while (cur->next) cur = cur->next;
        cur->next = new Nodell(key, { cur->centerPos.x + 2*cur->nodeRadius + distance , cur->centerPos.y });

}


//void Linkedlist::update(float deltaTime, float baseSpeed, float speedMultiplier) {
//    for (int i = 0; i < sz; i++) {
//        Node* current = htable[i];
//        while (current) {
//            current->update(deltaTime, baseSpeed, speedMultiplier);
//            current = current->next;
//        }
//    }
//}

void Linkedlist::draw() {

    Nodell* cur = Linkedlist::root;

    while (cur) {
        cur->draw();
        if (cur != Linkedlist::root) {
            DrawLine(cur->centerPos.x - cur->nodeRadius - distance, cur->centerPos.y, cur->centerPos.x - cur->nodeRadius , cur->centerPos.y, BLACK);
            DrawTriangle({ cur->centerPos.x - cur->nodeRadius , cur->centerPos.y }, { cur->centerPos.x - cur->nodeRadius - 10 , cur->centerPos.y - 10 }, { cur->centerPos.x - cur->nodeRadius - 10 , cur->centerPos.y + 10 }, BLACK);
        }
        cur = cur->next;
    }
    
}


Linkedlist :: ~Linkedlist() {
    Nodell* cur = root; 
    Nodell* cur2;
    while (cur->next) {
        cur2 = cur->next;
        delete cur; 
        cur = cur2;
    }
    root = NULL;

}


LinkedlistVisual::LinkedlistVisual() {
    border = { 166, 500, 166, 210 };
    Key = { {250, 521, 65, 25} };


    info = { 0, 350, 332, 150 };

    Random = { {210, 600, 90, 30}, "Random" };
    Enter = { {210, 650, 90, 30}, "Enter" };
    File = { {210, 650, 90, 30}, "File" };
}

int LinkedlistVisual::handleEvent() {
    Input.update();

    if (Input.isCreate) {
        File.update();
        Random.update();
    }
    else if (Input.isInsert) {
        Key.update();
        Enter.update();
        if (Key.outputMessage != "") {
            ll.add(stoi(Key.outputMessage));
            Key.outputMessage = "";
        }
        ll.draw();
    }
    else if (Input.isRemove) {

    }
    else if( Input.isSearch){

    }

    return LinkedList_state;
}


void LinkedlistVisual::draw() {
    Input.draw();
    DrawRectangleRec(border, Color{ 220, 180, 120, 255 });
    DrawRectangleRec(info, DARKGRAY);

    Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

    DrawTextEx(font, "[INFO]:", { info.x + 10, info.y + 10 }, 21, 1, PURPLE);

    if (Input.isCreate) {
    }
    else if (Input.isInsert) {
        int fontSize = 24;
        float spacing = 1.0f;
        DrawTextEx(font, "Key:", { 185, 520 }, fontSize, spacing, BLACK);
        Key.draw();
        Enter.draw();
    }
    else if (Input.isRemove) {

    }
    else {

    }
}

