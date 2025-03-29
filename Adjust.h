#ifndef ADJUST_H
#define ADJUST_H

#include "raylib.h"
#include <vector>

class Toolbar {
private:
    bool isOpen;
    bool isSpeedMenuOpen;
    float slidePos;
    float speed;
    Rectangle toolbarRect;
    Rectangle menuIconRect;
    Rectangle buttons[5]; // Back, Pause, Play, Next, Speed
    Rectangle speedButtons[2]; // Speed-, Speed+
    std::vector<Texture2D> textures; // Store all textures
    const char* labels[5] = { "Back", "Pause", "Play", "Next", "Speed" };

public:
    Toolbar();
    ~Toolbar();
    void Update();
    void Draw();
};

#endif
