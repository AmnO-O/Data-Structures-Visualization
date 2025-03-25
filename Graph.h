#pragma once
#include "Constants.h"

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
		//DrawCircleV(position, radius, color); 
		//DrawCircleLines(position.x, position.y, radius, WHITE);
		//DrawText(label .c_str(),position.x - MeasureText(label.c_str(), 18) / 2,position.y - 9,18,WHITE);

		// Get color variant based on state
		Color displayColor = getColorVariant(color, selected, hovered, highlighted);

		// Drop shadow
		DrawCircleV(Vector2{ position.x + 3, position.y + 3 }, radius, ColorAlpha(DARKGRAY, 0.3f));

		// Outer glow based on state
		if (hovered || selected || highlighted) {
			float glowRadius = radius + (selected ? 8 : (highlighted ? 6 : 4));
			DrawCircleV(position, glowRadius, ColorAlpha(displayColor, 0.3f));
		}

		// Main node body
		DrawCircleV(position, radius, displayColor);

		// Inner highlight for 3D effect
		DrawCircleV(Vector2{ position.x - radius * 0.3f, position.y - radius * 0.3f },
			radius * 0.4f, ColorAlpha(WHITE, 0.3f));

		// Draw node label
		int textWidth = MeasureTextEx(font, label.c_str(), 20, 1).x;
		DrawTextEx(font, label.c_str(), { position.x - textWidth / 2, position.y - 10 }, 20, 1, WHITE);
	}

	void draw(float currentTime, Font& font) {
		//DrawCircleV(position, radius, color); 
		//DrawCircleLines(position.x, position.y, radius, WHITE);
		//DrawText(label .c_str(),position.x - MeasureText(label.c_str(), 18) / 2,position.y - 9,18,WHITE);

		// Calculate display radius with pulsing effect
		float displayRadius = getPulsingRadius(radius, currentTime, selected, hovered, highlighted);

		// Get color variant based on state
		Color displayColor = getColorVariant(color, selected, hovered, highlighted);

		// Drop shadow
		DrawCircleV(Vector2{ position.x + 3, position.y + 3 }, displayRadius, ColorAlpha(DARKGRAY, 0.3f));

		// Outer glow based on state
		if (hovered || selected || highlighted) {
			float glowRadius = displayRadius + (selected ? 8 : (highlighted ? 6 : 4));
			DrawCircleV(position, glowRadius, ColorAlpha(displayColor, 0.3f));
		}

		// Main node body
		DrawCircleV(position, displayRadius, displayColor);

		// Inner highlight for 3D effect
		DrawCircleV(Vector2{ position.x - displayRadius * 0.3f, position.y - displayRadius * 0.3f },
			displayRadius * 0.4f, ColorAlpha(WHITE, 0.3f));

		// Draw node label
		int textWidth = MeasureTextEx(font, label.c_str(), 20, 1).x;
		DrawTextEx(font, label.c_str(), { position.x - textWidth / 2, position.y - 10 }, 20, 1, WHITE);
	}
};

struct Edge {
	int from;
	int to;
	float weight;
	Color color = BLACK;
	bool directed = false;
	bool highlighted = false;

	Edge(int f, int t, float w, Color c, bool dir = false) {
		from = f;
		to = t;
		weight = w;
		color = c;
		directed = dir;
		highlighted = false;
	}

};

class Graph {
public:
	Graph();
	void addNode(int id);
	void addEdge(int from, int to, int weight = 1, bool dir = false);

	void generateRandom();
	void findMST();
	void updEades();
	void updFruchterman();
	void update();
	void draw(Font& font);
private:

	void drawEdge(const Edge& edge, Font& font);
	vector <GraphNode> nodes;
	vector <Edge> edges;

	int matrix[1000][1000];
	Vector2 forces[1000];
	int n;

	int selectedNode = -1;

	const float c_rep = 3000.0f;
	const float c_spring = 0.05f;
	const float desiredLen = 260.0f;
	const float cooling_factor = 0.85f;
	const float timeStep = 0.82f;

	float len_frunch = 35;
	float coolingFactor_fruch = 0.98;

};