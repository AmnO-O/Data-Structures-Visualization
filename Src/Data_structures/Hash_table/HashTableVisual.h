#pragma once
#include "HashTable.h"

class HashTableVisual {
public:
    HashTableVisual();
    int handleEvent();
    void draw();
    void drawCode();

    Font font, infoFont, smallFont;

    ~HashTableVisual();

private:
    void drawCode(int posX, int posY, int width, int height, const vector<string>& codeLines);
    void highlightCode(int posX, int posY, int width, int height, int currentLine);
    void drawBackgroundInfo(int posX, int posY, int width, int height);

#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

    vector <string> insertCode, deleteCode, searchCode;

    AnimationData animation;
    CommonButton Input;
    Rectangle info, valueRect;
    Button Random, File;
    TextBoxCenter Size, Value;
    Instruction loadFile;
    FileLoader readFile;
    HashTable H;
    Toolbar toolbar;
    string warning = "";

    Button valueButton;
    float valueTime = 0;
    float valueDuration = 2.f;
    bool valueAnimation = 0;

    float frameCnt = 0;
    int currentLine = 0;

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