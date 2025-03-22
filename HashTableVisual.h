#pragma once
#include "HashTable.h"
class HashTableVisual {
public:
    HashTableVisual();
    int handleEvent();
    void draw();
    
    Font font, infoFont;

    ~HashTableVisual();

private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

    AnimationData animation;
    CommonButton Input;
    Rectangle info;
    Button Random, Empty;
    TextBox Size;
    TextBoxCenter Value;
    HashTable H;

    bool popActive;
    float popElapsedTime;
    float popDuration; // Duration for the pop effect.
    float popScale;    // Current scale factor (from small to normal).

    bool  shrinkActive;
    float shrinkElapsedTime;
    float shrinkDuration; // Duration for removal effect.
    float shrinkScale;    // Scale from 1 to 0.

    bool searchActive;
    float searchElapsedTime;
    float searchDuration; // Duration for search effect.
};