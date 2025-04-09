#include "Graph.h"

Graph::Graph() {
    numNodes = 0;
    for (int i = 0; i < 14; i++) addNode(i);
    genRandom();
    isDirected = isWeighted = 0;
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
    {79, 195, 247, 255},   // Sky blue
    {77, 182, 172, 255},   // Teal
    LIME,

    };

    numNodes++;
    std::uniform_real_distribution<float> posDist(100.0f, Screen_w - 100.0f);
    std::uniform_real_distribution<float> posDist2(100.0f, Screen_h - 100.0f);
    nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 20.f, palette[random(0, 6)], to_string(id)));
}

void Graph::addEdge(int from, int to, int weight, bool dir) {
    for (const Edge& edge : edges) {
        if ((edge.from == from && edge.to == to) || (edge.from == to && edge.to == from)) {
            return;
        }
    }

    if (from < 0 || from >= numNodes || to < 0 || to >= numNodes) return;
    numEdges = edges.size() + 1;
    edges.push_back(Edge(from, to, isWeighted ? weight : -1e7 - 7 - 2 - 2006, isDarkMode ? WHITE : BLACK, isDirected));
}

void Graph::remEdge(int from, int to, int weight) {
    for (auto it = edges.begin(); it != edges.end(); ++it) {

        if (it->from == from && it->to == to) {
            if (isWeighted) {
                if (it->weight == weight) edges.erase(it);
                return;
            }
            else {
                edges.erase(it);
                return;
            }
        }
        else if (it->from == to && it->to == from && isDirected == 0) {
            if (isWeighted) {
                if (it->weight == weight) edges.erase(it);
                return;
            }
            else {
                edges.erase(it);
                return;
            }
        }
    }
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
        {79, 195, 247, 255},   // Sky blue
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
        numEdges = min(numNodes + 10, random(2, numNodes * (numNodes - 1) / 2));
    else
        numEdges = min(numedges, numNodes * (numNodes - 1) / 2);

    for (int i = 0; i < numNodes; i++) {
        Color nodeColor = palette[i % 7];
        nodes.push_back(GraphNode(posDist(rd), posDist2(rd), 22.0f, nodeColor, to_string(i)));
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

                edges.push_back(Edge(from, to, isWeighted ? random(1, 30) : -1e7 - 7 - 2 - 2006, BLACK, isDirected));
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

    Color displayColor = edge.highlighted ? (isDarkMode ? YELLOW : PURPLE) : edge.color;

    float thickness = edge.highlighted ? 4.0f : edge.thickness;

    if (isDirected)
        DrawThickArrow(adjustedStart, adjustedEnd, thickness, displayColor, edge.highlighted);
    else
        DrawLineEx(adjustedStart, adjustedEnd, thickness, displayColor);

    if (isWeighted) {
        Vector2 midpoint = {
            (adjustedStart.x + adjustedEnd.x) / 2,
            (adjustedStart.y + adjustedEnd.y) / 2
        };

        char weightText[22];
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
        if (nodes[i].position.x < 200 + nodes[i].radius) {
            nodes[i].position.x = 200 + nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.x > Screen_w - 200 - nodes[i].radius) {
            nodes[i].position.x = Screen_w - 200 - nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.y < 60 + nodes[i].radius) {
            nodes[i].position.y = 60 + nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
        if (nodes[i].position.y > Screen_h - 100 - nodes[i].radius) {
            nodes[i].position.y = Screen_h - 100 - nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
    }
}

static Vector2 closestPointOnSegment(Vector2 A, Vector2 B, Vector2 C) {
    Vector2 dirAB = { B.x - A.x, B.y - A.y };
    float lenAB = std::sqrt(dirAB.x * dirAB.x + dirAB.y * dirAB.y);
    if (lenAB < 1e-6f) return A;  // Handle degenerate case where A and B are the same
    Vector2 unitAB = { dirAB.x / lenAB, dirAB.y / lenAB };
    float t = (C.x - A.x) * unitAB.x + (C.y - A.y) * unitAB.y;
    if (t < 0) return A;              // Closest point is A
    else if (t > lenAB) return B;     // Closest point is B
    else return { A.x + t * unitAB.x, A.y + t * unitAB.y };  // Closest point is within segment
}

bool Graph::isConencted() {
    dsu.init(numNodes);

    for (Edge e : edges) {
        dsu.join(e.from, e.to);
    }

    return dsu.nc == 1;
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
        if (nodes[i].position.x < 200 + nodes[i].radius) {
            nodes[i].position.x = 200 + nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.x > Screen_w - 200 - nodes[i].radius) {
            nodes[i].position.x = Screen_w - 200 - nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.y < 50 + nodes[i].radius) {
            nodes[i].position.y = 50 + nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
        if (nodes[i].position.y > Screen_h - 100 - nodes[i].radius) {
            nodes[i].position.y = Screen_h - 100 - nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
    }
}

void Graph::updOld() {
    float c_rep = 3000.0f;
    float c_spring = 0.03f;
    float desiredLen = 200.f;
    float damping = 0.85f;
    float timeStep = 0.82f;

    int n = numNodes;

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

        nodes[i].velocity.x *= damping;
        nodes[i].velocity.y *= damping;

        nodes[i].position.x += nodes[i].velocity.x * timeStep;
        nodes[i].position.y += nodes[i].velocity.y * timeStep;

        float padding = 100.0f;
        if (nodes[i].position.x < 400 + nodes[i].radius) {
            nodes[i].position.x = 400 + nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.x > Screen_w - 200 - nodes[i].radius) {
            nodes[i].position.x = Screen_w - 200 - nodes[i].radius;
            nodes[i].velocity.x *= -0.5f;
        }
        if (nodes[i].position.y < 50 + nodes[i].radius) {
            nodes[i].position.y = 50 + nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
        if (nodes[i].position.y > Screen_h - 100 - nodes[i].radius) {
            nodes[i].position.y = Screen_h - 100 - nodes[i].radius;
            nodes[i].velocity.y *= -0.5f;
        }
    }
}

void Graph::processMST() {
    edgesMST.clear(); curMST.clear();
    inMST.clear();

    inMST = vector<short>(edges.size() + 3, 0);

    for (Edge e : edges) edgesMST.push_back(e);
    sort(edgesMST.begin(), edgesMST.end());
    reverse(edgesMST.begin(), edgesMST.end());
    highlightIndex = -1;

    dsu.init(numNodes);
    isFindMST = true;
    toolbar.isPlaying = toolbar.isOpen = true;
    timeJoin = timeFind = 0;
}

void Graph::update() {
    float deltaTime = GetFrameTime();

    toolbar.Update();
    updOld();

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

    if (isFindMST) {
        if (toolbar.isBack)
            timeFind = timeJoin = 0.0f;

        if (toolbar.isBack && curMST.size()) {
            dsu.roll();

            Edge e = curMST.back();
            e.reset();
            e.directed = isDirected;

            edgesMST.push_back(e);
            curMST.pop_back();

            if (inMST[highlightIndex] == 2)
                MSTweight -= e.weight;
            inMST[highlightIndex] = 0;
            highlightIndex--;
        }

        if (toolbar.isNext) {
            if (curMST.size()) {
                Edge& e = curMST.back();

                if (e.color.r == RED.r && e.color.g == RED.g && e.color.b == RED.b) {
                    dsu.save();

                    if (dsu.join(e.from, e.to) == 0)
                        e.thickness = 0.3f, e.highlighted = 0, e.color = isDarkMode ? WHITE : BLACK, inMST[highlightIndex] = 1;
                    else
                        e.highlighted = true, inMST[highlightIndex] = 2, MSTweight += e.weight;
                }
            }

            if (edgesMST.size()) {
                highlightIndex++;
                if (highlightIndex >= edges.size()) highlightIndex = 0;

                Edge e = edgesMST.back();
                edgesMST.pop_back();

                e.highlighted = false;
                e.color = RED;
                e.thickness = 4.f;
                curMST.push_back(e);
            }

            timeFind = timeJoin = 0.0f;
        }

        if (toolbar.isPlaying) {
            if (timeFind >= 0)
                timeFind += deltaTime * toolbar.speed;
            else {
                timeJoin += deltaTime * toolbar.speed;

                if (timeJoin >= stepJoin) {
                    if (curMST.size()) {
                        Edge& e = curMST.back();

                        if (e.color.r == RED.r && e.color.g == RED.g && e.color.b == RED.b) {
                            dsu.save();

                            // cout << "Nodes: " << e.from << ' ' << e.to << ", Root : " << dsu.root(e.from) << ' ' << dsu.root(e.to) << '\n';

                            if (dsu.join(e.from, e.to) == 0)
                                e.thickness = 0.3f, e.highlighted = 0, e.color = isDarkMode ? WHITE : BLACK, inMST[highlightIndex] = 1;
                            else
                                e.highlighted = true, inMST[highlightIndex] = 2, MSTweight += e.weight;
                        }
                    }

                    timeJoin = timeFind = 0;
                }
            }

            if (timeFind >= stepFind) {
                if (edgesMST.size()) {
                    highlightIndex++;
                    if (highlightIndex >= edges.size()) highlightIndex = 0;

                    Edge e = edgesMST.back();
                    edgesMST.pop_back();

                    e.highlighted = false;
                    e.color = RED;
                    e.thickness = 4.f;
                    curMST.push_back(e);
                }

                timeFind = -1;
                timeJoin = 0;
            }
        }
    }
}

bool Graph::isProcessedMST() {
    return !edgesMST.empty();
}

void Graph::draw(Font& font) {
    toolbar.Draw();

    float deltaTime = GetFrameTime();

    if (isFindMST) {
        for (Edge& edge : edgesMST) {
            edge.color = isDarkMode ? WHITE : BLACK;
            drawEdge(edge, font);
        }

        for (Edge edge : curMST) {
            if (edge.color.r != RED.r || edge.color.g != RED.g || edge.color.b != RED.b)
                edge.color = isDarkMode ? WHITE : BLACK;

            drawEdge(edge, font);
        }

        for (int i = 0; i < nodes.size(); i++) {
            nodes[i].highlighted = false;
            nodes[i].draw(deltaTime, font);
        }

        return;
    }

    for (Edge& edge : edges) {
        edge.color = isDarkMode ? WHITE : BLACK;
        drawEdge(edge, font);
    }

    for (GraphNode& node : nodes) node.draw(deltaTime, font);
}
