#pragma once
#include <iostream>
#include <cstring>
#include "Constants.h"

using namespace std; 

struct Item {
	std::string name;  
	Rectangle  bounds; 
	bool hovered;
};

class Menu {
public: 
	string dsName[4] = {
		"Singly Linked List", "Hash Table", "Trie",  "Spanning Tree"
	};

	vector <Item> dsItem; 

	Menu(); 
	int handleEvent();
	void Draw(); 
};