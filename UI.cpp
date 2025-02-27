#include "UI.h"
#include "Menu.h"
#include "HashTable.h"
 
Visualizer::Visualizer() {

}

void Visualizer::Process() {
	InitWindow(Screen_w, Screen_h, "RayViz - Data Visualization");
	SetTargetFPS(60);

	Image icon = LoadImage((pathImage + "IconWin.png").c_str());
	SetWindowIcon(icon);

	/*load all the textutre*/
	
	Texture2D lightIcon = LoadTexture("Assets/Images/lightmode.png");
	Texture2D darkIcon = LoadTexture("Assets/Images/darkmode.png");
	Texture2D titlebar = LoadTexture("Assets/Images/titlebar.png");
	Texture2D backto = LoadTexture("Assets/Images/backto.png");


	Vector2  iconPosition = {Screen_w - 150, 40 }; /// the Position of the lighticon or darkicon
	Rectangle iconRec = {Screen_w - 150, 40, 100, 100}; /// the rect of the lighticon or darkicon

	Vector2 titlePosition = {638, 20}; /// the Position of the title-bar
	Rectangle titleRec = {638, 20, 323, 40}; // the rect of the title-bar

	Vector2 backtoPosition = {580, 20 }; /// the Position of the back-to
	Rectangle backtoRec = {580, 20, 40, 40}; // the rect of the back-to

	font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

	string DataName = ""; 


	auto menu = std::make_unique<Menu>();
	auto hashtable = make_unique<HashTableVisual>();



	while (!WindowShouldClose()) {
		
		if (Program_state == Menu_state)
			Program_state = menu->handleEvent();
		
		if (Program_state == HashTable_state)
			Program_state = hashtable->handleEvent(); 

		/// switch theme
		Vector2 mousePos = GetMousePosition();
		bool isHovering = CheckCollisionPointRec(mousePos, iconRec);

		Color tint = WHITE;
		if (isHovering) tint = { 255, 255, 255, 150 };
		if (isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) isLight = !isLight;

		BeginDrawing();
		if (isLight) ClearBackground(lightmode);
		else ClearBackground(darkmode);


		DrawTextureEx(isLight ? lightIcon : darkIcon, iconPosition, 0.0f, 1.f, tint);

		if (Program_state == Menu_state) {
			Vector2 textSize = MeasureTextEx(font, "RayViz", 90.0f, 2.0f);
			DrawTextEx(font, "RayViz", Vector2{ Screen_w / 2 - textSize.x / 2, 25 }, 90.0f, 2.0f, isLight ? DARKGRAY : WHITE);
			menu->Draw();
		}
		else {
			Vector2 mousePos = GetMousePosition();
			
			bool isHovered = CheckCollisionPointRec(mousePos, Rectangle {580, 20, 40, 40}); /// check if mouse pos is in backto
			DrawRectangleRounded(Rectangle{ 580, 20, 40, 40 }, 0.5f, 6, isHovered ? GRAY : WHITE); /// Draw for back to

			if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) Program_state = Menu_state; 

			DrawTextureEx(backto, backtoPosition, 0.0f, 1.f, BLACK);
			DrawTextureEx(titlebar, titlePosition, 0.0f, 1.f, WHITE);

			Vector2 textSize = MeasureTextEx(font, DataName.c_str(), 25.0f, 2.0f);
			DrawTextEx(font, DataName.c_str(), Vector2{ 800 - textSize.x / 2, 40 - textSize.y / 2 }, 25, 2.0f, DARKGRAY);
		}

		if (Program_state == HashTable_state) hashtable->draw(); 


		if (Program_state == Trie_state) DataName = "Trie";
		else if (Program_state == HashTable_state) DataName = "Hash table: Chainning"; 
		else if (Program_state == LinkedList_state) DataName = "Linked List"; 
		else if (Program_state == Graph_state) DataName = "Graph"; 

		EndDrawing();
	}

	CloseWindow();

	UnloadTexture(lightIcon);
	UnloadTexture(darkIcon);
	UnloadTexture(titlebar);
	UnloadFont(font);
}