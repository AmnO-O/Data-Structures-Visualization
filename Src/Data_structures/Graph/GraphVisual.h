#pragma once
#include "Graph.h"
#include "../../UI/Components/Button.h"
#include "../../UI/Components/TextBox.h"

class GraphVisual {
public:
	GraphVisual();
	~GraphVisual();

	int handleEvent();
	void draw();
private:
	void drawMSTWeightPanel(int x, int y, int width, int height, int mstWeight);

	void drawEdgesTable(const std::vector<Edge>& edges, Vector2 position, int highlightLine, float width, float rowHeight, float scrollOffset, float tableVisibleHeight);
	void DrawTextCentered(const string& text, float x, float y, float w, float h, int fontSize, Color color);
	void DrawEdgeTableDuringAnimation(const std::vector<Edge>& originalEdges, const std::vector<float>& displayYs, Vector2 position, float width, float rowHeight, float scrollOffset, float tableVisibleHeight);

	void drawCode(int posX, int posY, int width, int height, const vector<string>& codeLines);
	void highlightCode(int posX, int posY, int width, int height, int currentLine);
	void drawBackgroundInfo(int posX, int posY, int width, int height);

#define PANEL_WIDTH 188  // Độ rộng bảng
#define PANEL_PADDING 20 // Khoảng cách lề

	float timer = 0.0f;
	int highlightIndex = 0;
	float rowHeight = 40.0f;
	float tableVisibleHeight = 400.0f;
	float currentScrollOffset = 0.0f;

	bool sortingAnimation = false;
	float sortTime = 0.0f;
	float sortDuration = 1.0f;

	vector<string> mstCode;

	vector <Edge> tmpEdges, sortedEdges;
	vector<float> startYs;
	vector<float> endYs;
	vector<float> displayYs;

	string warning = "";
	Toolbar toolbar;
	Rectangle iconWeighted, iconDirected, iconStatic;
	GraphButton Input;
	TextBoxCenter Vertices, Edges, Source;
	Font font, infoFont, smallFont;
	Rectangle info;
	Button fileMatrix, fileEdges, Go;
	Graph G;

	Instruction loadFileEdges, loadFileMatrix;
	FileLoader readFileEdges;
	Texture2D switch_off, switch_on;

	Rectangle valueRect;
	Button valueButton;
	TextBoxEdge Value;
	float valueTime = 0;
	float valueDuration = 2.f;
	bool valueAnimation = 0;
};