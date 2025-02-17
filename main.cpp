#include <iostream>
#include "Constants.h"
#include "Menu.h"

int Program_state = -1; 

int main() {
	try {
		InitWindow(Screen_w, Screen_h, "RayViz - Data Visualization");
		SetTargetFPS(60);
		Image icon = LoadImage((pathImage + "IconWin.png").c_str());
		SetWindowIcon(icon);

		Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 64, 0, 0); 

		Color bgColor = { 210, 230, 255, 255 };
		
		auto menu = std::make_unique<Menu>();

		while (!WindowShouldClose()) {

			if (Program_state == Menu_state) 
				Program_state = menu->handleEvent();

			BeginDrawing();
			ClearBackground(bgColor);

			Vector2 textSize = MeasureTextEx(font, "RayViz", 90.0f, 2.0f);
			Vector2 position = {Screen_w / 2 - textSize.x / 2, 25 };

			DrawTextEx(font, "RayViz", position, 90.0f, 2.0f, DARKGRAY);
			
			if (Program_state == Menu_state) menu->Draw();

			EndDrawing();
		}
		
		UnloadFont(font);
		CloseWindow();

	} catch (std :: exception& e) {
		std :: cout << "\nEXCEPTION: " << e.what() << '\n';
	}
}