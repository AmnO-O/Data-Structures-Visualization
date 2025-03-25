#include "GraphVisual.h"

GraphVisual::GraphVisual() {

    font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);
    infoFont = LoadFont("Assets/Fonts/Acme-Regular.ttf");
    SetTextureFilter(infoFont.texture, TEXTURE_FILTER_BILINEAR);
    smallFont = LoadFont("Assets/Fonts/LilitaOne-Regular.ttf");
    SetTextureFilter(smallFont.texture, TEXTURE_FILTER_BILINEAR);

    Nodes = { {PANEL_WIDTH + 10, 250 + 80, PANEL_WIDTH / 2 - 20, 30} };
    Edges = { {PANEL_WIDTH + PANEL_WIDTH / 2 + 10, 250 + 80, PANEL_WIDTH / 2 - 20, 30} };
}

int GraphVisual::handleEvent() {
    float deltaTime = GetFrameTime();
    Input.update();

    if (Input.isCreate) {
        Nodes.update();
        Edges.update();
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

    DrawRectangle(PANEL_WIDTH, panelMargin, PANEL_WIDTH, Screen_h - 2 * panelMargin, panelColorx);

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

    if (Input.isCreate) {
        int fontSize = 24;
        float spacing = 1.0f;

        Nodes.draw();
        Edges.draw();
        string text = "Nodes";
        int textwidth = MeasureTextEx(font, text.c_str(), fontSize, spacing).x;
        DrawTextEx(font, text.c_str(), { PANEL_WIDTH * 1.f + 10 + PANEL_WIDTH * 1.f / 4 - 10 - textwidth / 2, 300 }, fontSize, spacing, BLACK);

        DrawLine(PANEL_WIDTH + 10 + PANEL_WIDTH / 2 - 10, 250,
            PANEL_WIDTH + 10 + PANEL_WIDTH / 2 - 10, 250 + 200, BLACK);

        text = "Edges";
        textwidth = MeasureTextEx(font, text.c_str(), fontSize, spacing).x;
        DrawTextEx(font, text.c_str(), { PANEL_WIDTH + PANEL_WIDTH / 2 + 10 + PANEL_WIDTH * 1.f / 4 - 10 - textwidth / 2, 300 }, fontSize, spacing, BLACK);
    }

    /// Draw graph
    G.draw(font);


}

GraphVisual::~GraphVisual() {
    UnloadFont(font);
    UnloadFont(infoFont);
    UnloadFont(smallFont);
}