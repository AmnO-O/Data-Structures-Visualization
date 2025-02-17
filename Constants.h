#pragma once
#include <iostream>
#include <cstring>
#include <raylib.h>
#include <vector>

using namespace std; 

const int Screen_h = 900; 
const int Screen_w = 1600;

const int Item_h = 250; 
const int Item_w = 350; 

const int Menu_state = -1; 
const int LinkedList_state = 0; 
const int HashTable_state = 1; 
const int Trie_state = 2; 
const int Spanning_state = 3; 


const string pathFont = "Assets/Fonts/";
const string pathImage = "Assets/Images/";

const Color lightmode = { 210, 230, 255, 255 };
const Color darkmode =  { 15, 30, 60, 255 };