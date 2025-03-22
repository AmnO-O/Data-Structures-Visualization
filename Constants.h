#pragma once
#include <iostream>
#include <cstring>
#include <raylib.h>
#include <vector>
#include <random>
#include <string>
#include <chrono>

using namespace std;

const int Screen_h = 900;
const int Screen_w = 1600;

const int Item_h = 200;
const int Item_w = 350;
const int Padding = 50;

const int Settings_state = -3;
const int About_state = -2;
const int Menu_state = -1;
const int LinkedList_state = 0;
const int HashTable_state = 1;
const int Trie_state = 2;
const int Graph_state = 3;
const int HowToUse_state = 4;


const string pathFont = "Assets/Fonts/";
const string pathImage = "Assets/Images/";

const Color lightmode = { 205, 201, 250, 255 };
const Color darkmode = { 138, 136, 164, 255 };
const Color btnHoverColor = { 100, 150, 255, 255 };
const Color btnNormalColor = { 200, 200, 200, 255 };

/// Draw arrow from start to end with thickness and color
void DrawThickArrow(Vector2 start, Vector2 end, float thickness, Color color, bool highlighted = false);
void DrawDropShadowRoundedRect(Rectangle rec, float roundness, int segments, float shadowOffset, Color shadowColor);
Vector2 Lerp(Vector2 start, Vector2 end, float t);

extern std::mt19937 rd;
int random(int l, int r);

extern bool isDarkMode;


/*
lavander: 189, 224, 254
*/