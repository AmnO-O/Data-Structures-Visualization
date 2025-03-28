#include "Graph.h"

Graph::Graph() {
    numNodes = 0;
    for (int i = 0; i < 14; i++) addNode(i);
    genRandom();
    selectedNode = -1;
}

void Graph::reset() {
    nodes.clear();
    edges.clear();

    numNodes = numEdges = 0;
}

void Graph::addNode(int id) {
    Color palette[] = {
    {41, 98, 255, 255},    // Blue
    {255, 61, 127, 255},   // Pink
    {255, 145, 0, 255},    // Orange
    {98, 205, 255, 255},   // Light blue
    {186, 104, 200, 255},  // Purple
    {79, 195, 247, 255},   // Sky blue
    {255, 213, 79, 255},   // Yellow
    {77, 182, 172, 255},   // Teal
    LIME,

    };

    numNodes++;
    std::uniform_real_distribution<float> posDist(100.0f, Screen_w - 100.0f);
    std::uniform_real_distribution<float> posDist2(100.0f, Screen_h - 100.0f);
    nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 20.f, palette[random(0, 8)], to_string(id)));
}

void Graph::addEdge(int from, int to, int weight, bool dir) {
    for (const Edge& edge : edges) {
        if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
            return;
        }
    }

    if (from < 0 || from >= numNodes || to < 0 || to >= numNodes) return;
    numEdges = edges.size() + 1;
    edges.push_back(Edge(from, to, weight, isDarkMode ? WHITE : BLACK, dir));
}

void Graph::genRandom(int numnodes, int numedges) {
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
        LIME,

    };


    uniform_real_distribution<float> posDist(200.0f, Screen_w - 100.0f);
    uniform_real_distribution<float> posDist2(200.0f, Screen_h - 100.0f);
    uniform_real_distribution<float> weightDist(1.0f, 50.0f);


    if (numnodes == -1)
        numNodes = random(4, 15);
    else
        numNodes = numnodes;

    if (numedges == -1)
        numEdges = random(2, numNodes * (numNodes - 1) / 2);
    else
        numEdges = min(numedges, numNodes * (numNodes - 1) / 2);

    for (int i = 0; i < numNodes; i++) {
        Color nodeColor = palette[i % 9];
        nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 22.0f, nodeColor, to_string(i + 1)));
        forces[i] = { 0, 0 };
    }


    for (int i = 0; i < numEdges; i++) {
        int from = random(0, numNodes - 1);
        int to = random(0, numNodes - 1);

        // Ensure we don't add self-loops or duplicate edges
        if (from != to) {
            bool edgeExists = false;

            for (const Edge& edge : edges) {
                if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
                    edgeExists = true;
                    i--;
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

                edges.push_back(Edge(from, to, random(1, 20), BLACK));
            }
        }
        else i--;
    }
}


void Graph::drawEdge(const Edge& edge, Font& font) {
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

    float thickness = edge.highlighted ? 4.0f : 2.0f;

    if (edge.directed)
        DrawThickArrow(adjustedStart, adjustedEnd, thickness, displayColor, edge.highlighted);
    else
        DrawLineEx(adjustedStart, adjustedEnd, thickness, isDarkMode ? WHITE : BLACK);

    if (edge.weight != -1e9 - 7 - 2 - 2006 && (edge.highlighted || edge.weight > 1.0f)) {
        Vector2 midpoint = {
            (adjustedStart.x + adjustedEnd.x) / 2,
            (adjustedStart.y + adjustedEnd.y) / 2
        };

        char weightText[10];
        sprintf_s(weightText, "%.1f", edge.weight);
        int textWidth = MeasureTextEx(font, weightText, 16, 1).x;

        DrawRectangle(midpoint.x - textWidth / 2 - 4, midpoint.y - 8, textWidth + 8, 16, ColorAlpha(RAYWHITE, 0.7f));
        DrawTextEx(font, weightText, { midpoint.x - textWidth / 2, midpoint.y - 8 }, 16, 1, DARKGRAY);
    }
}

void Graph::updEades() {
    // Constants for optimization
    const float threshold = 5.0f * desiredLen;       // e.g., 1000.0f
    const float thresholdSquared = threshold * threshold;
    const float epsilon = 1e-5f;                     // Small value to prevent division by zero

    // Reset forces and accelerations
    for (int i = 0; i < numNodes; i++) {
        nodes[i].acceleration = { 0, 0 };
        forces[i] = { 0, 0 };
    }
    Vector2 ScreenCenter;
    ScreenCenter = { 100 + Screen_w / 2, Screen_h / 2 };

    // Repulsive forces with distance threshold
    for (int i = 0; i < numNodes; i++) {
        for (int j = i + 1; j < numNodes; j++) {
            Vector2 posA = nodes[i].position;
            Vector2 posB = nodes[j].position;

            Vector2 dir = { posB.x - posA.x, posB.y - posA.y };
            float squaredDistance = dir.x * dir.x + dir.y * dir.y;

            // Skip if nodes are too far apart
            if (squaredDistance >= thresholdSquared) continue;

            // Compute distance with epsilon for stability
            float distance = sqrt(squaredDistance + epsilon);
            float forceMagnitude = c_rep / (distance * distance * distance);
            Vector2 force = { -forceMagnitude * dir.x, -forceMagnitude * dir.y };

            forces[i].x += force.x;
            forces[i].y += force.y;
            forces[j].x -= force.x;
            forces[j].y -= force.y;
        }

        Vector2 dir = { ScreenCenter.x - nodes[i].position.x, ScreenCenter.y - nodes[i].position.y };
        forces[i].x += c_centripetal * dir.x;
        forces[i].y += c_centripetal * dir.y;
    }

    // Attractive (spring) forces for each edge
    for (const auto& edge : edges) {
        Vector2 posA = nodes[edge.from].position;
        Vector2 posB = nodes[edge.to].position;
        Vector2 dir = { posB.x - posA.x, posB.y - posA.y };

        float squaredDistance = dir.x * dir.x + dir.y * dir.y;
        float distance = sqrt(squaredDistance + epsilon);
        float springForce = c_spring * (distance - desiredLen);
        Vector2 unitDir = { dir.x / distance, dir.y / distance };
        Vector2 force = { unitDir.x * springForce, unitDir.y * springForce };

        forces[edge.from].x += force.x;
        forces[edge.from].y += force.y;
        forces[edge.to].x -= force.x;
        forces[edge.to].y -= force.y;
    }

    for (int i = 0; i < numNodes; i++) {
        nodes[i].velocity.x += forces[i].x * timeStep;
        nodes[i].velocity.y += forces[i].y * timeStep;

        nodes[i].velocity.x *= cooling_factor;
        nodes[i].velocity.y *= cooling_factor;

        nodes[i].position.x += nodes[i].velocity.x * timeStep;
        nodes[i].position.y += nodes[i].velocity.y * timeStep;

        float padding = 100.0f;
        if (nodes[i].position.x < padding + 200 + nodes[i].radius) {
            nodes[i].position.x = padding + 200 + nodes[i].radius;
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

void Graph::updGPT() {
    float c_rep = 3000.0f;
    float c_spring = 0.01f;
    float desiredLen = 200.0f;
    float damping = 0.85f;
    float timeStep = 0.82f;
    Vector2 ScreenCenter;
    ScreenCenter = { 100 + Screen_w / 2, Screen_h / 2 };

    for (int i = 0; i < numNodes; i++) {
        nodes[i].acceleration = { 0, 0 };
        forces[i] = { 0, 0 };
    }

    /// repulsive forces
    for (int i = 0; i < numNodes; i++) {
        for (int j = i + 1; j < numNodes; j++) {
            Vector2 posA = nodes[i].position;
            Vector2 posB = nodes[j].position;

            Vector2 dir = { posB.x - posA.x, posB.y - posA.y };
            float distance = sqrt(dir.x * dir.x + dir.y * dir.y);
            if (distance < 1.0f) distance = 1.0f;

            Vector2 unitDir = { dir.x / distance, dir.y / distance };
            float repulsionForce = c_rep / (distance * distance);

            Vector2 force = { -unitDir.x * repulsionForce, -unitDir.y * repulsionForce };

            forces[i].x += force.x;
            forces[i].y += force.y;

            forces[j].x -= force.x;
            forces[j].y -= force.y;
        }

        Vector2 dir = { ScreenCenter.x - nodes[i].position.x, ScreenCenter.y - nodes[i].position.y };
        forces[i].x += c_centripetal * dir.x;
        forces[i].y += c_centripetal * dir.y;
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

    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].velocity.x += forces[i].x * timeStep;
        nodes[i].velocity.y += forces[i].y * timeStep;

        nodes[i].velocity.x *= damping;
        nodes[i].velocity.y *= damping;

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
    numNodes = nodes.size();
    if (numNodes <= 0) return;

    for (int i = 0; i < numNodes; i++) {
        forces[i] = { 0, 0 };
    }

    len_frunch = sqrt(((Screen_w - 200) * (Screen_h - 200)) / numNodes);

    /// repulsive forces
    for (int i = 0; i < numNodes; i++) {
        for (int j = i + 1; j < numNodes; j++) {
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

void Graph::update() {
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

void Graph::draw(Font& font) {

    for (const Edge& edge : edges) {
        drawEdge(edge, font);
    }
    float deltaTime = GetFrameTime();

    for (GraphNode& node : nodes) node.draw(deltaTime, font);
}