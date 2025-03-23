#include "Graph.h"

Graph::Graph() {
    n = 0;
    for (int i = 0; i < 14; i++) addNode(i);
    addEdge(5, 10, 57);
    addEdge(10, 6, 8);
    addEdge(10, 2, 51);
    addEdge(1, 2, 97);
    addEdge(2, 8, 17);
    addEdge(2, 11, 38);
    addEdge(2, 14, 51);
    addEdge(5, 14, 45);
    addEdge(14, 11, 77);
    addEdge(11, 9, 62);
    addEdge(9, 7, 45);
    addEdge(9, 12, 30);
    addEdge(9, 4, 31);
    addEdge(4, 3, 36);
    addEdge(3, 12, 26);
    addEdge(3, 13, 28);
    addEdge(13, 8, 4);
    addEdge(13, 11, 86);

    generateRandom();
    selectedNode = -1;
}

void Graph::addNode(int id) {
    n++;
    std::uniform_real_distribution<float> posDist(100.0f, Screen_w - 100.0f);
    std::uniform_real_distribution<float> posDist2(100.0f, Screen_h - 100.0f);
    nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 20.f, BLACK, to_string(id)));
}

void Graph::addEdge(int from, int to, int weight, bool dir) {
    if (from < 0 || from >= n || to < 0 || to >= n) return;
    edges.push_back(Edge(from, to, weight, WHITE, dir));
}

void Graph::generateRandom() {
    nodes.clear();
    edges.clear();
    // Generate a pleasing color palette

    Color palette[] = {
        {41, 98, 255, 255},    // Blue
        {255, 61, 127, 255},   // Pink
        {255, 145, 0, 255},    // Orange
        {98, 205, 255, 255},   // Light blue
        {186, 104, 200, 255},  // Purple
        {79, 195, 247, 255},   // Sky blue
        {255, 213, 79, 255},   // Yellow
        {77, 182, 172, 255},   // Teal
    };


    std::uniform_real_distribution<float> posDist(100.0f, Screen_w - 100.0f);
    std::uniform_real_distribution<float> posDist2(100.0f, Screen_h - 100.0f);
    std::uniform_real_distribution<float> weightDist(1.0f, 10.0f);

    n = random(3, 15);
    int numedges = random(1, n * (n - 1) / 2);

    for (int i = 0; i < n; i++) {
        Color nodeColor = palette[i % 8];
        nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 22.0f, nodeColor, to_string(i + 1)));
        forces[i] = { 0, 0 };
    }


    for (int i = 0; i < numedges; i++) {
        int from = random(0, n - 1);
        int to = random(0, n - 1);

        // Ensure we don't add self-loops or duplicate edges
        if (from != to) {
            bool edgeExists = false;

            for (const Edge& edge : edges) {
                if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
                    edgeExists = true;
                    break;
                }
            }

            if (!edgeExists) {
                Color edgeColor = {
                    (unsigned char)((nodes[from].color.r + nodes[to].color.r) / 2),
                    (unsigned char)((nodes[from].color.g + nodes[to].color.g) / 2),
                    (unsigned char)((nodes[from].color.b + nodes[to].color.b) / 2),
                    180  // Semi-transparent
                };

                edges.push_back(Edge(from, to, random(1, 20), edgeColor));
            }
        }
    }
}


void Graph::drawEdge(const Edge& edge) {
    Vector2 start = nodes[edge.from].position;
    Vector2 end = nodes[edge.to].position;

    Vector2 dir = { end.x - start.x, end.y - start.y };
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);

    if (length < 0.001f) return;

    dir.x /= length;
    dir.y /= length;

    Vector2 adjustedStart = {
        start.x + dir.x * nodes[edge.from].radius,
        start.y + dir.y * nodes[edge.from].radius
    };

    Vector2 adjustedEnd = {
        end.x - dir.x * nodes[edge.to].radius,
        end.y - dir.y * nodes[edge.to].radius
    };

    Color displayColor = edge.highlighted ?
        Color{
        255, 255, 40, 255
    } :
        edge.color;

    float thickness = edge.highlighted ? 4.0f : 2.5f;

    if (edge.directed)
        DrawThickArrow(adjustedStart, adjustedEnd, thickness, displayColor, edge.highlighted);
    else
        DrawLineEx(adjustedStart, adjustedEnd, thickness, displayColor);

    if (edge.highlighted || edge.weight > 1.0f) {
        Vector2 midpoint = {
            (adjustedStart.x + adjustedEnd.x) / 2,
            (adjustedStart.y + adjustedEnd.y) / 2
        };

        char weightText[10];
        sprintf_s(weightText, "%.1f", edge.weight);
        int textWidth = MeasureText(weightText, 16);

        DrawRectangle(midpoint.x - textWidth / 2 - 4, midpoint.y - 8, textWidth + 8, 16, ColorAlpha(RAYWHITE, 0.7f));
        DrawText(weightText, midpoint.x - textWidth / 2, midpoint.y - 8, 16, DARKGRAY);
    }
}

void Graph::updEades() {
    for (int i = 0; i < n; i++) {
        nodes[i].acceleration = { 0, 0 };
        forces[i] = { 0, 0 };
    }

    /// repulsive forces
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Vector2 posA = nodes[i].position;
            Vector2 posB = nodes[j].position;

            Vector2 dir = { posB.x - posA.x, posB.y - posA.y };
            float distance = sqrt(dir.x * dir.x + dir.y * dir.y);
            if (distance < 1.0f) distance = 1.0f;  // Prevent division by zero

            Vector2 unitDir = { dir.x / distance, dir.y / distance };
            float repulsionForce = c_rep / (distance * distance);

            Vector2 force = { -unitDir.x * repulsionForce, -unitDir.y * repulsionForce };

            forces[i].x += force.x;
            forces[i].y += force.y;

            forces[j].x -= force.x;
            forces[j].y -= force.y;
        }
    }

    // Compute attractive (spring) forces for each edge.
    for (const auto& edge : edges) {
        Vector2 posA = nodes[edge.from].position;
        Vector2 posB = nodes[edge.to].position;
        Vector2 dir = { posB.x - posA.x, posB.y - posA.y };

        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (distance < 1.0f) distance = 1.0f;  // prevent division by zero

        // Normalize the direction vector.
        Vector2 unitDir = { dir.x / distance, dir.y / distance };
        float springForce = c_spring * (distance - desiredLen);
        Vector2 force = { unitDir.x * springForce, unitDir.y * springForce };

        forces[edge.from].x += force.x;
        forces[edge.from].y += force.y;

        forces[edge.to].x -= force.x;
        forces[edge.to].y -= force.y;
    }

    // Update velocities and positions.
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].velocity.x += forces[i].x * timeStep;
        nodes[i].velocity.y += forces[i].y * timeStep;

        nodes[i].velocity.x *= cooling_factor;
        nodes[i].velocity.y *= cooling_factor;

        nodes[i].position.x += nodes[i].velocity.x * timeStep;
        nodes[i].position.y += nodes[i].velocity.y * timeStep;

        float padding = 100.0f;
        if (nodes[i].position.x < padding + nodes[i].radius) {
            nodes[i].position.x = padding + nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.x > Screen_w - padding - nodes[i].radius) {
            nodes[i].position.x = Screen_w - padding - nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.y < padding + nodes[i].radius) {
            nodes[i].position.y = padding + nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
        if (nodes[i].position.y > Screen_h - padding - nodes[i].radius) {
            nodes[i].position.y = Screen_h - padding - nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
    }
}

void Graph::updFruchterman() {
    n = nodes.size();
    if (n <= 0) return;

    for (int i = 0; i < n; i++) {
        forces[i] = { 0, 0 };
    }

    len_frunch = sqrt(((Screen_w - 200) * (Screen_h - 200)) / n);

    /// repulsive forces
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            Vector2 posA = nodes[i].position;
            Vector2 posB = nodes[j].position;

            Vector2 dir = { posB.x - posA.x, posB.y - posA.y };
            float distance = sqrt(dir.x * dir.x + dir.y * dir.y);
            if (distance < 1.0f) distance = 1.0f;  // Prevent division by zero

            Vector2 unitDir = { dir.x / distance, dir.y / distance };
            float repulsionForce = len_frunch * len_frunch / distance;

            Vector2 force = { -unitDir.x * repulsionForce, -unitDir.y * repulsionForce };

            forces[i].x += force.x;
            forces[i].y += force.y;

            forces[j].x -= force.x;
            forces[j].y -= force.y;
        }
    }

    // Compute attractive (spring) forces for each edge.
    for (const auto& edge : edges) {
        Vector2 posA = nodes[edge.from].position;
        Vector2 posB = nodes[edge.to].position;
        Vector2 dir = { posB.x - posA.x, posB.y - posA.y };

        float distance = std::sqrt(dir.x * dir.x + dir.y * dir.y);
        if (distance < 1.0f) distance = 1.0f;  // prevent division by zero

        Vector2 unitDir = { dir.x / distance, dir.y / distance };
        float springForce = distance * distance / len_frunch;
        Vector2 force = { unitDir.x * springForce, unitDir.y * springForce };

        forces[edge.from].x += force.x;
        forces[edge.from].y += force.y;

        forces[edge.to].x -= force.x;
        forces[edge.to].y -= force.y;
    }

    // Update velocities and positions.
    for (size_t i = 0; i < nodes.size(); i++) {
        nodes[i].velocity.x += forces[i].x;
        nodes[i].velocity.y += forces[i].y;

        nodes[i].velocity.x *= coolingFactor_fruch;
        nodes[i].velocity.y *= coolingFactor_fruch;

        nodes[i].position.x += nodes[i].velocity.x;
        nodes[i].position.y += nodes[i].velocity.y;

        // Keep nodes within screen bounds with padding
        float padding = 50.0f;
        if (nodes[i].position.x < padding + nodes[i].radius) {
            nodes[i].position.x = padding + nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.x > Screen_w - padding - nodes[i].radius) {
            nodes[i].position.x = Screen_w - padding - nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.y < padding + nodes[i].radius) {
            nodes[i].position.y = padding + nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
        if (nodes[i].position.y > Screen_h - padding - nodes[i].radius) {
            nodes[i].position.y = Screen_h - padding - nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
    }
}

void Graph::upd() {
    updEades();

    Vector2 mousePos = GetMousePosition();

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].hovered = ((nodes[i].position.x - mousePos.x) * (nodes[i].position.x - mousePos.x) +
            (nodes[i].position.y - mousePos.y) * (nodes[i].position.y - mousePos.y)) <= nodes[i].radius * nodes[i].radius;

        if (nodes[i].hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedNode = i;
            nodes[i].selected = true;
        }
    }

    if (selectedNode >= 0) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            nodes[selectedNode].position = mousePos;
            nodes[selectedNode].velocity = { 0, 0 };
        }
        else {
            nodes[selectedNode].selected = false;
            selectedNode = -1;
        }
    }
}

GraphVisual::GraphVisual() {

}

int GraphVisual::handleEvent() {
    float deltaTime = GetFrameTime();
    G.upd();
    return Graph_state;
}

void Graph::draw() {

    for (const Edge& edge : edges) {
        drawEdge(edge);
    }
    float deltaTime = GetFrameTime();

    for (GraphNode& node : nodes) node.draw2();
}

void GraphVisual::draw() {
    G.draw();
}