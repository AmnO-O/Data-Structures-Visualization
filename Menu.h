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
private:
	Rectangle helpButton; // Nút "How to Use"
	Rectangle aboutButton;  // Nút About
	Rectangle settingsButton;  // Nút Settings

	bool helpHovered;
	bool aboutHovered;
	bool settingsHovered;
public:
	Menu();

	string dsName[4] = {
		"Singly Linked List", "Hash Table", "AVL",  "Graph"
	};

	vector <Item> dsItem;
	Font myFont; // Add new Font for Menu
	Font titleFont;

	~Menu();
	void Init();
	int handleEvent();
	void Draw();
};