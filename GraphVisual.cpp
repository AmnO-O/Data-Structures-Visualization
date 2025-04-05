#include "GraphVisual.h"
#include "raylib.h"

GraphVisual::GraphVisual() {

    font = LoadFont("Assets/Fonts/PublicSans-Bold.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    infoFont = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(infoFont.texture, TEXTURE_FILTER_BILINEAR);
    smallFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(smallFont.texture, TEXTURE_FILTER_BILINEAR);

    Value = { {270, 350, 105, 30} };
    valueButton = { { PANEL_PADDING + 100, 440, 80, 30 }, "" };
    valueRect = { -500, -500, 225, 55 };
    Value.fontSize = 21;

    Vertices = { {PANEL_WIDTH + 100, 250 + 60, PANEL_WIDTH / 2 - 10, 30}, "Random" };
    Edges = { {PANEL_WIDTH + 100, 250 + 100, PANEL_WIDTH / 2 - 10, 30}, "Random" };

    Vertices.textColor = DARKGRAY;
    Edges.textColor = DARKGRAY;

    Go = { { PANEL_PADDING + 200, 440, 130, 40 }, "Go!" };

    fileMatrix = { { PANEL_PADDING + 200, 530, 130, 40 }, "File Matrix" };
    fileEdges = { { PANEL_PADDING + 200, 590, 130, 40 }, "File Edges" };

    loadFileEdges.width = 600;
    loadFileEdges.height = 300;

    switch_off = LoadTexture("Assets/Images/switch-off.png");
    switch_on = LoadTexture("Assets/Images/switch-on.png");

    iconDirected = { PANEL_PADDING + 100, 565, 50, 45 };
    iconWeighted = { PANEL_PADDING + 100, 620, 50, 50 };

    loadFileEdges.show("File Format Requirement",
        " Every line in the file corresponds to one edge.\n"
        " Each line contains exactly two or three integers.\n"
        " - Two numbers: Represents an edge between two vertices.\n"
        " - Three numbers: Represents an edge with weight\n (the third number).\n\n"
        " Example: \n"
        " 1 2\n"
        " 2 3 2\n"
        " 4 5\n"
        " 5 6 5\n");

    readFileEdges = FileLoader();
}

float calculateTargetScrollOffset(int highlightIndex, float rowHeight, float tableVisibleHeight, int totalRows) {
    float totalContentHeight = totalRows * rowHeight;
    float highlightTop = highlightIndex * rowHeight;
    float target = highlightTop + rowHeight / 2 - tableVisibleHeight / 2;
    if (target < 0) target = 0;
    if (target > totalContentHeight - tableVisibleHeight) target = totalContentHeight - tableVisibleHeight;
    return target;
}

int GraphVisual::handleEvent() {
    float deltaTime = GetFrameTime();

    if (Input.update()) {
        sortingAnimation = false;
        valueAnimation = true;
        valueTime = 0;
        G.isFindMST = 0;
        highlightIndex = 0;
        warning = "";

        if (Input.isAddedge) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[1].rect.y;
        }
        else if (Input.isRemove) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[2].rect.y;
        }
        else if (Input.isMst) {
            Value.bounds.x = -500;
            Value.bounds.y = Input.cButton[3].rect.y;
        }
    }

    Vertices.update();
    Edges.update();

    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, iconDirected) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) G.isDirected ^= 1;
    if (CheckCollisionPointRec(mousePos, iconWeighted) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) G.isWeighted ^= 1, Value.weighted = G.isWeighted, Value.setDefault();


    if (Input.isCreate) {
        Go = { { PANEL_PADDING + 200, 435, 130, 40 }, "Go!" };

        if (Go.update()) {
            if ((Vertices.text != "Random" && Vertices.getDigit() < 0) || (Edges.text != "Random" && Edges.getDigit() < 0)) {
                warning = "Vertex and edge counts \nmust be positive integers!";
            }
            else {
                warning = "";
                G.genRandom((Vertices.text == "Random" ? -1 : Vertices.getDigit()), (Edges.text == "Random" ? -1 : Edges.getDigit()));
                if (G.numNodes >= 0) Vertices.text = to_string(G.numNodes);
                if (G.numEdges >= 0) Edges.text = to_string(G.numEdges);
            }
        }

        fileMatrix.update();

        if (fileEdges.update()) {
            loadFileEdges.display = true;
        }

        if (loadFileEdges.update()) { /// load file Edges
            vector <string> lines = readFileEdges.loadFileLines();

            if (!lines.empty()) {
                G.reset();

                for (string each : lines) {
                    int from = -1, to = -1, digit = -1;
                    int weight = -1e7 - 7 - 2 - 2006;

                    for (int i = 0; i < each.size(); i++) {
                        if (each[i] == '-') {
                            weight = -1;
                            i++;
                        }
                        else if (isdigit(each[i])) {
                            if (digit < 0) digit = each[i] - '0';
                            else {
                                digit = digit * 10 + each[i] - '0';
                            }
                        }
                        else {
                            if (from < 0) from = digit, digit = -1;
                            else if (to < 0) to = digit, digit = -1;
                        }
                    }

                    if (to < 0) to = digit;
                    else if (digit != -1) weight = (weight == -1e9 - 7 - 2 - 2006 ? digit : -digit);

                    while (G.numNodes <= max(from, to)) G.addNode(G.numNodes);
                    if (from >= 0 && to >= 0 && from != to) G.addEdge(from, to, weight);
                }

                if (G.numNodes >= 0) Vertices.text = to_string(G.numNodes);
                if (G.numEdges >= 0) Edges.text = to_string(G.numEdges);
            }
        }
    }
    else if (Input.isAddedge) {
        Go = { { valueRect.x + valueRect.width / 2 - 35, valueRect.height + valueRect.y + 5, 70, 30 }, "Go!" };

        Value.update();

        if (Value.isEnter || Go.update()) {
            vector<int> parameters = Value.getValues();

            if (parameters.size() >= 3)
                G.addEdge(parameters[0], parameters[1], parameters[2], G.isWeighted);
            else if (parameters.size() == 2)
                G.addEdge(parameters[0], parameters[1], -1, G.isWeighted);
        }

        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 295.5, Input.cButton[1].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }

    }
    else if (Input.isRemove) {
        Go = { { valueRect.x + valueRect.width / 2 - 35, valueRect.height + valueRect.y + 5, 70, 30 }, "Go!" };

        Value.update();

        if (Value.isEnter || Go.update()) {
            vector<int> parameters = Value.getValues();

            if (parameters.size() >= 3)
                G.remEdge(parameters[0], parameters[1], parameters[2]);
            else if (parameters.size() == 2)
                G.remEdge(parameters[0], parameters[1], -1);
        }

        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 295.5, Input.cButton[2].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }
    }
    else if (Input.isMst) {

        if (valueAnimation) {
            valueTime += deltaTime;

            float t = valueTime / valueDuration;
            if (t > 1.0f) t = 1.0f;
            float smoothT = t * t * (3 - 2 * t);

            Vector2 move = Lerp({ Value.bounds.x, Value.bounds.y }, { 180, Input.cButton[3].rect.y + 5 }, smoothT);

            Value.bounds.x = move.x;
            Value.bounds.y = move.y;

            valueButton.rect.x = move.x - 95;
            valueButton.rect.y = move.y + 1;

            Go = { {move.x + 28, move.y, 65, 30 }, "Go!" };

            if (valueTime >= valueDuration) {
                valueTime = 0;
                valueAnimation = 0;
                valueAnimation = 0;
            }
        }

        if (sortingAnimation) {
            sortTime += deltaTime / sortDuration;

            if (sortTime >= 1.0f) {
                sortTime = 1.0f;
                sortingAnimation = false;
                highlightIndex = 0;
                G.highlightIndex = 0;
                warning = "", G.processMST();
            }

            for (int i = 0; i < G.edges.size(); i++) {
                float smoothT = sortTime * sortTime * (3 - 2 * sortTime);

                Vector2 transition = Lerp({ 4, displayYs[i] }, { 4, endYs[i] }, smoothT);
                displayYs[i] = transition.y;
            }
        }

        if (Go.update() && G.isFindMST == false) {
            if (G.isDirected) {
                warning = "Graph must be undirected!";
            }
            else if (G.isConencted() == 0) {
                warning = "Graph must be connected!";
            }
            else {
                tmpEdges.clear();
                sortedEdges.clear();

                tmpEdges = G.edges;
                sortedEdges = G.edges;

                sort(sortedEdges.begin(), sortedEdges.end());

                startYs = vector<float>(tmpEdges.size());
                endYs = vector<float>(tmpEdges.size());
                displayYs = vector<float>(tmpEdges.size());

                for (int i = 0; i < tmpEdges.size(); i++) {
                    startYs[i] = i * rowHeight;
                    displayYs[i] = startYs[i];
                }

                warning = "";
                sortTime = 0;
                sortingAnimation = true;

                for (int i = 0; i < tmpEdges.size(); i++) {
                    for (int j = 0; j < sortedEdges.size(); j++) {
                        if (sortedEdges[j].from == tmpEdges[i].from && sortedEdges[j].to == tmpEdges[i].to && sortedEdges[j].weight == tmpEdges[i].weight) {
                            endYs[i] = j * rowHeight;
                            break;
                        }
                    }
                }
            }
        }
    }

    G.update();

    if (sortingAnimation == true) return Graph_state;
    float wheel = GetMouseWheelMove();

    if (wheel != 0)
        currentScrollOffset -= wheel * 20.0f;
    else if (G.isFindMST && G.isProcessedMST()) {
        float targetScrollOffset = calculateTargetScrollOffset(G.highlightIndex, rowHeight, tableVisibleHeight, G.edges.size());
        float delta = (targetScrollOffset - currentScrollOffset) * 0.1f;  // Adjust speed here
        currentScrollOffset += delta;
        timer += GetFrameTime();
    }

    float totalContentHeight = G.edges.size() * rowHeight;
    float maxScroll = totalContentHeight - tableVisibleHeight;
    if (maxScroll < 0) maxScroll = 0;
    if (currentScrollOffset < 0) currentScrollOffset = 0;
    if (currentScrollOffset > maxScroll) currentScrollOffset = maxScroll;

    return Graph_state;
}


void GraphVisual::DrawTextCentered(const string& text, float x, float y, float w, float h, int fontSize, Color color) {
    int textWidth = MeasureTextEx(font, text.c_str(), fontSize, 1).x;
    int textHeight = fontSize;  // Approximate height
    float posX = x + (w - textWidth) * 0.5f;
    float posY = y + (h - textHeight) * 0.5f;
    DrawTextEx(font, text.c_str(), { posX, posY }, fontSize, 1, color);
}

void GraphVisual::DrawEdgeTableDuringAnimation(const vector<Edge>& originalEdges, const std::vector<float>& displayYs, Vector2 position, float width, float rowHeight, float scrollOffset, float tableVisibleHeight) {
    Color tableBg = isDarkMode ? Color{ 127, 205, 255, 255 } : Color{ 253, 196, 182, 255 };
    //Color{ 255,239, 213, 255 };
    Color headerBg = isDarkMode ? Color{ 29, 162, 216, 255 } : Color{ 234, 112, 112, 255 };
    Color highlightBg = { 230, 180, 0, 100 };
    Color textColor = BLACK;
    Color borderColor = DARKGRAY;
    int fontSize = 20;

    float colFromWidth = width * 0.3f;
    float colToWidth = width * 0.3f;
    float colWeightWidth = width * 0.4f;

    DrawRectangle((int)position.x, (int)(position.y + rowHeight), (int)width, (int)tableVisibleHeight, tableBg);

    BeginScissorMode((int)position.x, (int)(position.y + rowHeight), (int)width, (int)tableVisibleHeight);
    for (int i = 0; i < originalEdges.size(); i++) {
        float y = position.y + rowHeight + displayYs[i] - scrollOffset;
        if (y + rowHeight > position.y + rowHeight && y < position.y + rowHeight + tableVisibleHeight) {
            float colX = position.x;
            DrawTextCentered(std::to_string(originalEdges[i].from), colX, y, colFromWidth, rowHeight, fontSize, textColor);
            colX += colFromWidth;
            DrawTextCentered(std::to_string(originalEdges[i].to), colX, y, colToWidth, rowHeight, fontSize, textColor);
            colX += colToWidth;
            DrawTextCentered(G.isWeighted ? to_string((int)originalEdges[i].weight) : "NONE", colX, y, colWeightWidth, rowHeight, fontSize, textColor);
        }
    }
    EndScissorMode();

    DrawRectangle((int)position.x, (int)position.y, (int)width, (int)rowHeight, headerBg);
    float headerX = position.x;
    DrawTextCentered(G.isDirected ? "From" : "U", headerX, position.y, colFromWidth, rowHeight, fontSize + 2, WHITE);
    headerX += colFromWidth;
    DrawTextCentered(G.isDirected ? "To" : "V", headerX, position.y, colToWidth, rowHeight, fontSize + 2, WHITE);
    headerX += colToWidth;
    DrawTextCentered("Weight", headerX, position.y, colWeightWidth, rowHeight, fontSize + 2, WHITE);

    DrawRectangleLinesEx({ position.x, position.y, width, rowHeight + tableVisibleHeight }, 2.0f, borderColor);
    float totalContentHeight = originalEdges.size() * rowHeight;

    if (totalContentHeight > tableVisibleHeight) {
        float thumbHeight = (tableVisibleHeight / totalContentHeight) * tableVisibleHeight;
        float trackHeight = tableVisibleHeight;
        float maxScroll = totalContentHeight - tableVisibleHeight;
        float thumbY = position.y + rowHeight + (scrollOffset / maxScroll) * (trackHeight - thumbHeight);
        DrawRectangle((int)(position.x + width - 10), (int)thumbY, 8, (int)thumbHeight, LIGHTGRAY);
    }

}

void GraphVisual::drawEdgesTable(const vector<Edge>& edges, Vector2 position, int highlightLine, float width, float rowHeight, float scrollOffset, float tableVisibleHeight) {
    Color tableBg = isDarkMode ? Color{ 127, 205, 255, 255 } : Color{ 253, 196, 182, 255 };
    //Color{ 255,239, 213, 255 };
    Color headerBg = isDarkMode ? Color{ 29, 162, 216, 255 } : Color{ 234, 112, 112, 255 };
    Color highlightBg = { 230, 180, 0, 100 };
    Color textColor = BLACK;
    Color borderColor = DARKGRAY;

    int fontSize = 20;

    float totalContentHeight = edges.size() * rowHeight;

    float colFromWidth = width * 0.3f;
    float colToWidth = width * 0.3f;
    float colWeightWidth = width * 0.4f;

    DrawRectangle((int)position.x, (int)(position.y + rowHeight), (int)width, (int)tableVisibleHeight, tableBg);
    BeginScissorMode((int)position.x, (int)(position.y + rowHeight), (int)width, (int)tableVisibleHeight);


    for (int i = 0; i < edges.size(); i++) {
        float rowY = position.y + rowHeight + i * rowHeight - scrollOffset;
        if (rowY + rowHeight > position.y + rowHeight && rowY < position.y + rowHeight + tableVisibleHeight) {
            if (i == highlightLine) {
                DrawRectangleRounded({ position.x, rowY, width, rowHeight }, 0.8, 6, highlightBg);
            }
            Color textCol = textColor;
            if (G.isFindMST) {
                if (G.inMST[i] == 1) textCol = RED;
                if (G.inMST[i] == 2) textCol = DARKGREEN;
            }

            float colX = position.x;
            DrawTextCentered(to_string(edges[i].from), colX, rowY, colFromWidth, rowHeight, fontSize, textCol);
            colX += colFromWidth;
            DrawTextCentered(to_string(edges[i].to), colX, rowY, colToWidth, rowHeight, fontSize, textCol);
            colX += colToWidth;
            DrawTextCentered(G.isWeighted ? to_string((int)edges[i].weight) : "NONE", colX, rowY, colWeightWidth, rowHeight, fontSize, textCol);
        }
    }

    EndScissorMode();

    DrawRectangle((int)position.x, (int)position.y, (int)width, (int)rowHeight, headerBg);
    float headerX = position.x;
    DrawTextCentered(G.isDirected ? "From" : "U", headerX, position.y, colFromWidth, rowHeight, fontSize + 2, WHITE);
    headerX += colFromWidth;
    DrawTextCentered(G.isDirected ? "To" : "V", headerX, position.y, colToWidth, rowHeight, fontSize + 2, WHITE);
    headerX += colToWidth;
    DrawTextCentered("Weight", headerX, position.y, colWeightWidth, rowHeight, fontSize + 2, WHITE);

    DrawRectangleLinesEx({ position.x, position.y, width, rowHeight + tableVisibleHeight }, 2.0f, borderColor);


    if (totalContentHeight > tableVisibleHeight) {
        float thumbHeight = (tableVisibleHeight / totalContentHeight) * tableVisibleHeight;
        float trackHeight = tableVisibleHeight;
        float maxScroll = totalContentHeight - tableVisibleHeight;
        float thumbY = position.y + rowHeight + (scrollOffset / maxScroll) * (trackHeight - thumbHeight);
        DrawRectangle((int)(position.x + width - 10), (int)thumbY, 8, (int)thumbHeight, LIGHTGRAY);
    }
}


void GraphVisual::draw() {
    int panelMargin = 250;
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 };
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 };
    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 };

    valueButton.text = G.isWeighted ? "(u,v,w):" : "(u,v):";

    if (Input.isCreate) {
        DrawRectangleRounded(Rectangle{ PANEL_WIDTH, panelMargin * 1.f, PANEL_WIDTH + 4, Screen_h * 1.f - 2 * panelMargin + 20 }, 0.2, 9, panelColorx);

        if (Vertices.text == "") Vertices.text = "Random";
        if (Edges.text == "") Edges.text = "Random";

        Vertices.draw();
        Edges.draw();
        int fontSize = 22;
        float spacing = 1.0f;

        string text = "Vertices";
        int textwidth = MeasureTextEx(font, text.c_str(), fontSize, spacing).x;
        DrawTextEx(font, text.c_str(), { PANEL_WIDTH * 1.f + 10 + PANEL_WIDTH * 1.f / 4 - 10 - textwidth / 2, 250 + 60 }, fontSize, spacing, BLACK);

        text = "Edges";
        textwidth = MeasureTextEx(font, text.c_str(), fontSize, spacing).x;
        DrawTextEx(font, text.c_str(), { PANEL_WIDTH * 1.f + 10 + PANEL_WIDTH * 1.f / 4 - 10 - textwidth / 2, 250 + 100 }, fontSize, spacing, BLACK);

        // other stuff

        if (warning.size() > 0) {
            DrawTextEx(font, warning.c_str(), { PANEL_WIDTH * 1.f + 5, 250 + 140 }, 16, 1, RED);
        }

        fileEdges.isChose = fileMatrix.isChose = Go.isChose = 0;
        fileMatrix.draw(smallFont);
        fileEdges.draw(smallFont);
        Go.draw(smallFont);

        DrawLineEx(Vector2{ PANEL_WIDTH, 505 }, Vector2{ PANEL_WIDTH * 2 + 4, 505 }, 2, BLACK);
    }
    else if (Input.isAddedge) {
        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);
        Go.isChose = 0;

        if (abs(valueTime - valueDuration) < 1 || valueAnimation == 0) {
            Go.draw(smallFont);
        }

        valueButton.draw(smallFont);
        Value.draw();
    }
    else if (Input.isRemove) {
        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);
        Go.isChose = 0;

        if (abs(valueTime - valueDuration) < 1 || valueAnimation == 0) {
            Go.draw(smallFont);
        }

        valueButton.draw(smallFont);
        Value.draw();
    }
    else if (Input.isMst) {

        valueRect.x = Value.bounds.x - 108;
        valueRect.y = Value.bounds.y - 7 - 5;
        DrawRectangleRounded(valueRect, 0.2, 8, panelColorx);
        Go.isChose = 0;
        Go.draw(smallFont);

        if (warning.size() > 0) {
            DrawTextEx(font, warning.c_str(), { Go.rect.x - 9, valueRect.y + 65 }, 18, 1, RED);
        }
    }

    if (sortingAnimation) {
        DrawEdgeTableDuringAnimation(tmpEdges, displayYs, { 1250, 160 }, 300, rowHeight, currentScrollOffset, tableVisibleHeight);
    }
    else if (G.isFindMST) {
        drawEdgesTable(sortedEdges, { 1250, 160 }, G.highlightIndex, 300, rowHeight, currentScrollOffset, tableVisibleHeight);
    }
    else {
        drawEdgesTable(G.edges, { 1250, 160 }, highlightIndex, 300, rowHeight, currentScrollOffset, tableVisibleHeight);
    }

    DrawRectangleRounded(Rectangle{ 0, panelMargin * 1.f, PANEL_WIDTH, Screen_h * 1.f - 2 * panelMargin + 20 }, 0.2, 9, panelColor);

    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(smallFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);
    Input.draw(smallFont);

    DrawTextEx(font, "Directed: ", { PANEL_PADDING + 5, 583 }, 20, 1, BLACK);
    DrawTextEx(font, "Weighted: ", { PANEL_PADDING + 3, 623 }, 20, 1, BLACK);

    Vector2 mousePos = GetMousePosition();
    bool isHovering = CheckCollisionPointRec(mousePos, iconDirected);
    DrawTextureEx(G.isDirected ? switch_on : switch_off, { PANEL_PADDING + 100, 570 }, 0.0f, 1.f, isHovering ? Color{ 255, 255, 255, 200 } : WHITE);

    isHovering = CheckCollisionPointRec(mousePos, iconWeighted);
    DrawTextureEx(G.isWeighted ? switch_on : switch_off, { PANEL_PADDING + 100, 610 }, 0.0f, 1.f, isHovering ? Color{ 255, 255, 255, 200 } : WHITE);

    /// Draw graph

    G.draw(font);


    loadFileEdges.draw(font);
}


GraphVisual::~GraphVisual() {
    UnloadTexture(switch_off);
    UnloadFont(font);
    UnloadFont(infoFont);
    UnloadFont(smallFont);
}