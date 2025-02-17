#include <iostream>
#include "Constants.h"
#include "Menu.h"

int Program_state = -1; 
bool isLight = true;

void Process() {
	InitWindow(Screen_w, Screen_h, "RayViz - Data Visualization");
	SetTargetFPS(60);
	Image icon = LoadImage((pathImage + "IconWin.png").c_str());
	SetWindowIcon(icon);

	Texture2D lightIcon = LoadTexture("Assets/Images/lightmode.png");
	Texture2D darkIcon = LoadTexture("Assets/Images/darkmode.png");

	Vector2  iconPosition = {Screen_w - 150, 50};
	Rectangle iconRec = {Screen_w - 150, 50, 100, 100};

	Font font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 64, 0, 0);

	auto menu = std::make_unique<Menu>();

	while (!WindowShouldClose()) {

		if (Program_state == Menu_state)
			Program_state = menu->handleEvent();


		/// switch theme
		Vector2 mousePos = GetMousePosition();
		bool isHovering = CheckCollisionPointRec(mousePos, iconRec);

		Color tint = WHITE;
		if (isHovering) tint = { 255, 255, 255, 150}; 

		if (isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) isLight = !isLight;

		BeginDrawing();
		if (isLight) ClearBackground(lightmode);
		else ClearBackground(darkmode); 

		Vector2 textSize = MeasureTextEx(font, "RayViz", 90.0f, 2.0f);
		Vector2 position = { Screen_w / 2 - textSize.x / 2, 25 };

		DrawTextEx(font, "RayViz", position, 90.0f, 2.0f, DARKGRAY);


		DrawTextureEx(isLight ? lightIcon : darkIcon, iconPosition, 0.0f, 1.f, tint);


		if (Program_state == Menu_state) menu->Draw();

		EndDrawing();
	}


	UnloadTexture(lightIcon);
	UnloadTexture(darkIcon);
	UnloadFont(font);
	CloseWindow();
}

int main() {
	try {
		Process();
	} catch (std :: exception& e) {
		std :: cout << "\nEXCEPTION: " << e.what() << '\n';
	}
}