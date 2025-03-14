#pragma once
#include <iostream>
#include <cstring>
#include <raylib.h>
#include <vector>
#include <random>
#include <chrono>

using namespace std; 

const int Screen_h = 900; 
const int Screen_w = 1600;

const int Item_h = 250; 
const int Item_w = 350; 

const int Menu_state = -1; 
const int LinkedList_state = 0; 
const int HashTable_state = 1; 
const int Trie_state = 2; 
const int Graph_state = 3; 


const string pathFont = "Assets/Fonts/";
const string pathImage = "Assets/Images/";

const Color lightmode = {190, 215, 245, 255};
const Color darkmode = { 15, 30, 60, 255 };

/// Draw arrow from start to end with thickness and color
void DrawThickArrow(Vector2 start, Vector2 end, float thickness, Color color);
void DrawDropShadowRoundedRect(Rectangle rec, float roundness, int segments, float shadowOffset, Color shadowColor);

extern std::mt19937 rd;
int random(int l, int r);
