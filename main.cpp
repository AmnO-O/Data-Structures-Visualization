#include <iostream>
#include <raylib.h>
#include "Constants.h"

int main() {
	try {
		InitWindow(screen_w, screen_h, "RayViz - Data Visualization");
		SetTargetFPS(60);
		Image icon = LoadImage((pathImage + "IconWin.png").c_str());
		SetWindowIcon(icon);

		Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 64, 0, 0); 

		while (!WindowShouldClose()) {

			BeginDrawing();
			ClearBackground(WHITE);

			Vector2 textSize = MeasureTextEx(font, "RayViz", 90.0f, 2.0f);
			Vector2 position = { screen_w / 2 - textSize.x / 2, 25 };

			DrawTextEx(font, "RayViz", position, 90.0f, 2.0f, BLACK);

			EndDrawing();
		}

		cout << "Test\n here"; 
		
		UnloadFont(font);
		CloseWindow();

	} catch (std :: exception& e) {
		std :: cout << "\nEXCEPTION: " << e.what() << '\n';
	}
}