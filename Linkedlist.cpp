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

void Linkedlist::draw2(Vector2 startPos) {
    // Node geometry
    const float nodeWidth = 80;
    const float nodeHeight = 50;
    const float gap = 40;  // gap between nodes

    Nodell* current = root;
    Vector2 currPos = startPos;

    // We can animate the node “pop” if we like:
    static float time = 0.0f;
    time += GetFrameTime();

    int index = 0;

    while (current)
    {
        // A little bounce effect for each node, offset in time by index
        float bounceAnim = sinf(time * 5.0f - index * 0.4f);
        // clamp to [0..1]
        if (bounceAnim < 0) bounceAnim = 0;

        // Draw the node
        current->draw2(currPos, nodeWidth, nodeHeight, bounceAnim);

        // If next is not null, draw an arrow
        if (current->next)
        {
            // Start of arrow = center right of this node
            Vector2 arrowStart = { currPos.x + nodeWidth + 5, currPos.y + nodeHeight / 2 };
            // End of arrow   = left side of next node’s position minus some margin
            Vector2 arrowEnd = { currPos.x + nodeWidth + gap * 0.8f, currPos.y + nodeHeight / 2 };

            DrawThickArrow(arrowStart, arrowEnd, 4.0f, DARKBLUE);
        }
        else
        {
            // For the last node, draw a short arrow + "NULL"
            Vector2 arrowStart = { currPos.x + nodeWidth + 5, currPos.y + nodeHeight / 2 };
            Vector2 arrowEnd = { arrowStart.x + 35, arrowStart.y };
            DrawThickArrow(arrowStart, arrowEnd, 4.0f, RED);

            // Draw "NULL" to the right
            DrawText("NULL", (int)(arrowEnd.x + 8), (int)(arrowEnd.y - 10), 20, RED);
        }

        // Move position to the right for the next node
        currPos.x += (nodeWidth + gap);

        // Next iteration
        current = current->next;
        index++;
    }
}


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
    while (cur) {
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
    }
    else if (Input.isRemove) {

    }
    else if( Input.isSearch){

    }

    return LinkedList_state;
}


void LinkedlistVisual::draw() {
    Input.draw();
    ll.draw2({ 100, 200 });
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

