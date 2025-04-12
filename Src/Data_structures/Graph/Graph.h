#pragma once
#include "../../Constants.h"
#include <stack>

struct item {
	int u, v, sz_u, par_u;
};

struct DSU_Roll_Back {
	vector <int> par, sz;
	stack <item> st;
	stack <int> flag;
	int nc;

	void init(const int& t = 0) {
		par = vector <int>(t + 3, 0);
		sz = vector <int>(t + 3, 1);
		for (int i = 0; i <= t; i++) par[i] = i;
		nc = t;
	}

	int root(int i) {
		if (i == par[i])
			return i;
		return root(par[i]);
	}

	bool join(int u, int v) {
		if ((u = root(u)) == (v = root(v))) return 0;
		nc--;
		if (sz[u] > sz[v]) swap(u, v);
		st.push({ u, v, sz[u], par[u] });
		par[u] = v;
		sz[v] += sz[u];
		sz[u] = 0;
		return 1;
	}

	void save() {
		flag.push(st.size());
	}

	void roll_back() {
		item x = st.top();
		int u = x.u;
		int v = x.v;

		sz[u] = x.sz_u;
		sz[v] -= sz[u];
		par[u] = x.par_u;
		nc++;

		st.pop();
	}

	void roll() {
		if (flag.empty()) return;
		int last = flag.top();
		flag.pop();
		while (st.size() != last) roll_back();
	}

};

struct GraphNode {
	Vector2 position, velocity, acceleration;
	float radius;
	Color color = WHITE;
	string label;
	bool selected = false;
	bool hovered = false;
	bool highlighted = false;

	GraphNode(float x, float y, float r, Color c, std::string label) {
		this->label = label;
		position = { x, y };
		velocity = { 0, 0 };
		acceleration = { 0, 0 };
		radius = r;
		color = c;
		selected = hovered = highlighted = false;
	}

	float getPulsingRadius(float baseRadius, float time, bool selected, bool hovered, bool highlighted) {
		if (selected) {
			return baseRadius * (1.0f + 0.2f * sinf(time * 4.0f));
		}
		else if (hovered) {
			return baseRadius * (1.0f + 0.1f * sinf(time * 6.0f));
		}
		else if (highlighted) {
			return baseRadius * (1.0f + 0.15f * sinf(time * 5.0f));
		}
		return baseRadius;
	}

	// Function to get color variant based on state
	Color getColorVariant(Color baseColor, bool selected, bool hovered, bool highlighted) {
		if (selected) {
			return Color{
				(unsigned char)std::min(255, baseColor.r + 40),
					(unsigned char)std::min(255, baseColor.g + 40),
					(unsigned char)std::min(255, baseColor.b + 40),
					255
			};
		}
		else if (highlighted) {
			return Color{
				(unsigned char)std::min(255, baseColor.r + 30),
					(unsigned char)std::min(255, baseColor.g + 30),
					(unsigned char)std::min(255, baseColor.b + 30),
					255
			};
		}
		else if (hovered) {
			return Color{
				(unsigned char)std::min(255, baseColor.r + 20),
					(unsigned char)std::min(255, baseColor.g + 20),
					(unsigned char)std::min(255, baseColor.b + 20),
					255
			};
		}
		return baseColor;
	}

	void draw2(Font& font) {
		Color displayColor = getColorVariant(color, selected, hovered, highlighted);

		DrawCircleV(Vector2{ position.x + 3, position.y + 3 }, radius, ColorAlpha(DARKGRAY, 0.3f));

		if (hovered || selected || highlighted) {
			float glowRadius = radius + (selected ? 8 : (highlighted ? 6 : 4));
			DrawCircleV(position, glowRadius, ColorAlpha(displayColor, 0.3f));
		}

		DrawCircleV(position, radius, displayColor);

		DrawCircleV(Vector2{ position.x - radius * 0.3f, position.y - radius * 0.3f },
			radius * 0.4f, ColorAlpha(WHITE, 0.3f));

		int textWidth = MeasureTextEx(font, label.c_str(), 20, 1).x;
		DrawTextEx(font, label.c_str(), { position.x - textWidth / 2, position.y - 10 }, 20, 1, WHITE);
	}

	void draw(float currentTime, Font& font) {

		float displayRadius = getPulsingRadius(radius, currentTime, selected, hovered, highlighted);

		Color displayColor = getColorVariant(color, selected, hovered, highlighted);

		DrawCircleV(Vector2{ position.x + 3, position.y + 3 }, displayRadius, ColorAlpha(DARKGRAY, 0.3f));

		if (hovered || selected || highlighted) {
			float glowRadius = displayRadius + (selected ? 8 : (highlighted ? 6 : 4));
			DrawCircleV(position, glowRadius, ColorAlpha(displayColor, 0.3f));
		}

		DrawCircleV(position, displayRadius, displayColor);

		DrawCircleV(Vector2{ position.x - displayRadius * 0.3f, position.y - displayRadius * 0.3f },
			displayRadius * 0.4f, ColorAlpha(WHITE, 0.3f));

		int textWidth = MeasureTextEx(font, label.c_str(), 20, 1).x;
		DrawTextEx(font, label.c_str(), { position.x - textWidth / 2, position.y - 10 }, 20, 1, WHITE);
	}
};

struct Edge {
	int from;
	int to;
	float weight = -1e7 - 7 - 2 - 2006;
	Color color = BLACK;
	bool directed = false;
	bool highlighted = false;
	float thickness = 2.12f;

	void reset() {
		color = BLACK;
		directed = highlighted = false;
		thickness = 2.12f;
	}

	Edge(int f, int t, float w, Color c, bool dir = false) {
		from = f;
		to = t;
		weight = w;
		color = c;
		directed = dir;
		highlighted = false;
	}

	bool operator<(const Edge& other) const {
		return weight < other.weight;
	}
};

class Graph {
public:
	Graph();
	void addNode(int id);
	void addEdge(int from, int to, int weight = 1, bool dir = false);
	void remEdge(int from, int to, int weight = 1);

	void genRandom(int nodes = -1, int edges = -1);

	void processMST();

	void updEades();
	void updOld();
	void updGPT();
	void update();
	void draw(Font& font);
	void reset();
	bool isConencted();

	bool isProcessedMST();

	int numNodes, numEdges;
	int highlightIndex = 0;
	bool isDirected = false, isWeighted = false, isStatic = false;
	bool isFindMST = false;
	vector <short> inMST;
	vector <Edge> edges;
	vector <Edge> edgesMST;
	Toolbar toolbar;
	float timeFind = 0.0f;
	float timeJoin = 0.0f;
	float MSTweight = 0.0f;

private:
	void drawEdge(const Edge& edge, Font& font);

	vector <GraphNode> nodes;
	vector <Edge> curMST;

	DSU_Roll_Back dsu;

	int matrix[1000][1000];
	Vector2 forces[1000];

	float stepFind = 1.0f;
	float stepJoin = 1.0f;

	int selectedNode = -1;

	float c_rep = 1000.0f;
	float c_spring = 0.007f;
	float desiredLen = 200.0f;
	float cooling_factor = 0.85f;
	float timeStep = 0.7f;

	float c_centripetal = 0.0002f;

	float len_frunch = 35;
	float coolingFactor_fruch = 0.98;

};