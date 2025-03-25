#pragma once
#include "Graph.h"
#include "Button.h"
#include "TextBox.h"

class GraphVisual {
public:
	GraphVisual();
	~GraphVisual();

	int handleEvent();
	void draw();
private:
#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề
	GraphButton Input;
	TextBoxCenter Nodes, Edges;
	Font font, infoFont, smallFont;
	Rectangle info;
	Graph G;
};