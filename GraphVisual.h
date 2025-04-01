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
	string warning = "";
	Toolbar toolbar;
	Rectangle iconWeighted, iconDirected;
	GraphButton Input;
	TextBoxCenter Vertices, Edges;
	Font font, infoFont, smallFont;
	Rectangle info;
	Button fileMatrix, fileEdges, Go;
	Graph G;

	Instruction loadFileEdges;
	FileLoader readFileEdges;
	Texture2D switch_off, switch_on;

	Rectangle valueRect;
	Button valueButton;
	TextBoxEdge Value;
	float valueTime = 0;
	float valueDuration = 2.f;
	bool valueAnimation = 0;
};