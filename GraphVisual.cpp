#include "GraphVisual.h"

GraphVisual::GraphVisual() {

    font = LoadFont("Assets/Fonts/PublicSans-Bold.ttf");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    infoFont = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(infoFont.texture, TEXTURE_FILTER_BILINEAR);
    smallFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(smallFont.texture, TEXTURE_FILTER_BILINEAR);

    Vertices = { {PANEL_WIDTH + 100, 250 + 60, PANEL_WIDTH / 2 - 10, 30}, "Random" };
    Edges = { {PANEL_WIDTH + 100, 250 + 100, PANEL_WIDTH / 2 - 10, 30}, "Random" };

    Vertices.textColor = DARKGRAY;
    Edges.textColor = DARKGRAY;

    Go = { { PANEL_PADDING + 200, 420, 130, 40 }, "Go!" };

    fileMatrix = { { PANEL_PADDING + 200, 510, 130, 40 }, "File Matrix" };
    fileEdges = { { PANEL_PADDING + 200, 570, 130, 40 }, "File Edges" };

    loadFileEdges.width = 600;
    loadFileEdges.height = 300;

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

int GraphVisual::handleEvent() {
    float deltaTime = GetFrameTime();
    Input.update();

    Vertices.update();
    Edges.update();

    if (Input.isCreate) {
        if (Go.update()) {
            G.genRandom((Vertices.text == "Random" ? -1 : Vertices.getDigit()), (Edges.text == "Random" ? -1 : Edges.getDigit()));
            if (G.numNodes >= 0) Vertices.text = to_string(G.numNodes);
            if (G.numEdges >= 0) Edges.text = to_string(G.numEdges);
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
                    int weight = -1e9 - 7 - 2 - 2006;

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

                cout << Vertices.text << ' ' << Edges.text << '\n';
            }
        }
    }

    G.update();
    return Graph_state;
}


void GraphVisual::draw() {
    /// draw info
    int panelMargin = 250; // Khoảng cách lề trên với box
    Color panelColor = isDarkMode ? Color{ 229, 229, 229, 255 } : Color{ 189, 224, 254, 255 }; // Chọn màu theo chế độ

    DrawRectangle(0, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, panelColor);

    // Vẽ nền bảng khu vực panel 2 
    Color panelColorx = isDarkMode ? Color{ 94, 172, 240, 180 } : Color{ 94, 172, 240, 180 }; // Chọn màu theo chế độ

    DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH + 4, Screen_h - 2 * panelMargin, panelColorx);

    Color panelColory = isDarkMode ? Color{ 164, 235, 185, 200 } : Color{ 77, 168, 180, 200 }; // Chọn màu theo chế độ

    int rectWidth = 400;
    int rectHeight = 240;
    int posX = Screen_w - rectWidth;
    int posY = Screen_h - rectHeight;

    DrawRectangle(posX, posY, rectWidth, rectHeight, panelColory);

    // "Operation"
    Color operationColor = isDarkMode ? DARKBLUE : DARKBLUE;
    DrawTextEx(smallFont, "Operations", { PANEL_PADDING + 10, 280 }, 26, 2, operationColor);

    DrawRectangleRec(info, panelColory);

    Color IN4Color = isDarkMode ? Color{ 199, 8, 40, 255 } : Color{ 199, 8, 40, 255 };
    DrawTextEx(infoFont, "[INFO]:", { (float)posX + 10, (float)posY + 10 }, 26, 2, IN4Color);

    Input.draw(smallFont);

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


    if (Input.isCreate) {
        fileEdges.isChose = fileMatrix.isChose = Go.isChose = 0;
        fileMatrix.draw(smallFont);
        fileEdges.draw(smallFont);
        Go.draw(smallFont);

        DrawLineEx(Vector2{ PANEL_WIDTH, 485 }, Vector2{ PANEL_WIDTH * 2 + 4, 485 }, 2, BLACK);
    }

    /// Draw graph
    G.draw(font);
    loadFileEdges.draw(font);
}

GraphVisual::~GraphVisual() {
    UnloadFont(font);
    UnloadFont(infoFont);
    UnloadFont(smallFont);
}