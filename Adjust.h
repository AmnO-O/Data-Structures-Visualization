#ifndef ADJUST_H
#define ADJUST_H

#include "raylib.h"
#include <vector>

class Toolbar {
private:
    bool isOpen;
    float slidePos;
    float speed;
    int selectedButtonIndex; // Track which button is selected
    Rectangle toolbarRect;
    Rectangle menuIconRect;
    Rectangle buttons[3]; // Back, Play/Pause, Next
    Rectangle speedButtons[2]; // Speed-, Speed+
    std::vector<Texture2D> textures; // Store all textures
    const char* labels[3] = { "Back", "Play", "Next" };

public:
    Toolbar();
    ~Toolbar();
    void Update();
    void Draw();
};

#endif