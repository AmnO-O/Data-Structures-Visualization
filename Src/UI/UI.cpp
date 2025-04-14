#include "UI.h"
#include "Toolbar/Adjust.h" // Include the header for Toolbar

Visualizer::Visualizer() {

}

void Visualizer::Process() {
	SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(Screen_w, Screen_h, "RayViz - Data Visualization");
	SetTargetFPS(120); 

	Image icon = LoadImage((pathImage + "IconWin.png").c_str());
	SetWindowIcon(icon);

	/*load all the textutre*/

	Texture2D lightIcon = LoadTexture("Assets/Images/lightmode.png");
	Texture2D darkIcon = LoadTexture("Assets/Images/darkmode.png");
	Texture2D titlebar = LoadTexture("Assets/Images/titlebar.png");
	Texture2D backto = LoadTexture("Assets/Images/backto.png");

	Vector2  iconPosition = { Screen_w - 140, 30 }; /// the Position of the lighticon or darkicon
	Rectangle iconRec = { Screen_w - 140, 30, 80, 80 }; /// the rect of the lighticon or darkicon

	Vector2 titlePosition = { 638, 20 }; /// the Position of the title-bar
	Rectangle titleRec = { 638, 20, 323, 40 }; // the rect of the title-bar

	Vector2 backtoPosition = { 580, 20 }; /// the Position of the back-to
	Rectangle backtoRec = { 580, 20, 40, 40 }; // the rect of the back-to

	//font = LoadFontEx("Assets/Fonts/PublicSans-Bold.ttf", 65, 0, 0);

	font = LoadFontEx("Assets/Fonts/LilitaOne-Regular.ttf", 64, NULL, 0);
	SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
	isDarkMode = false;

	string DataName = "";

	auto menu = std::make_unique<Menu>();
	auto about = make_unique<AboutScreen>();
	auto setting = make_unique< SettingScreen>();
	auto helper = make_unique <HelperScreen>();

	auto hashtable = make_unique<HashTableVisual>();
	auto linkedlist = make_unique<LinkedListScreen>();
	auto graph = make_unique<GraphVisual>();
	auto avl = make_unique<AVLTreeScreen>();

	//Toolbar toolbar; // Instantiate the Toolbar

	about->Init();
	setting->Init();
	helper->Init();
	linkedlist->Init();
	audioSys.Init();
	avl->Init();

	while (!WindowShouldClose()) {
		audioSys.Update();

		if (Program_state == Menu_state)
			Program_state = menu->handleEvent();

		else if (Program_state == HashTable_state)
			Program_state = hashtable->handleEvent();

		else if (Program_state == LinkedList_state)
			linkedlist->Update(Program_state);

		else if (Program_state == Graph_state)
			Program_state = graph->handleEvent();

		else if (Program_state == About_state)
			about->Update(Program_state);

		else if (Program_state == Settings_state)
			setting->Update(Program_state);

		else if (Program_state == HowToUse_state)
			helper->Update(Program_state);

		else if (Program_state == Trie_state)
			avl->Update(Program_state);

		/// switch theme
		Vector2 mousePos = GetMousePosition();
		bool isHovering = CheckCollisionPointRec(mousePos, iconRec);

		Color tint = WHITE;
		if (isHovering) tint = { 255, 255, 255, 150 };
		if (isHovering && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) isDarkMode = !isDarkMode;

		//if (Program_state != Menu_state && Program_state != About_state && Program_state != Settings_state && Program_state != HowToUse_state) {
		//	toolbar.Update(); // Update the Toolbar only on smaller screens
		//}

		BeginDrawing();

		if (!isDarkMode) ClearBackground(lightmode);
		else ClearBackground(darkmode);

		DrawTextureEx(!isDarkMode ? lightIcon : darkIcon, iconPosition, 0.0f, 1.f, tint);

		if (Program_state == Menu_state) {
			Vector2 textSize = MeasureTextEx(font, "RayViz", 70.0f, 2.0f);
			DrawTextEx(font, "RayViz", Vector2{ Screen_w / 2 - textSize.x / 2, 25 }, 70.0f, 2.0f, isDarkMode ? Color{ 244, 162, 88, 255 } : DARKBLUE);
			menu->Draw();
		}
		else {
			Vector2 mousePos = GetMousePosition();

			bool isHovered = CheckCollisionPointRec(mousePos, Rectangle{ 580, 20, 40, 40 }); /// check if mouse pos is in backto
			DrawRectangleRounded(Rectangle{ 580, 20, 40, 40 }, 0.5f, 6, isHovered ? GRAY : WHITE); /// Draw for back to
			DrawRectangleRounded(Rectangle{ 635, 17, 329, 46 }, 2.2f, 25, BLACK); /// Draw for title bar

			if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) Program_state = Menu_state;

			DrawTextureEx(backto, backtoPosition, 0.0f, 1.f, BLACK);
			DrawTextureEx(titlebar, titlePosition, 0.0f, 1.f, WHITE);

			Vector2 textSize = MeasureTextEx(font, DataName.c_str(), 25.0f, 2.0f);
			DrawTextEx(font, DataName.c_str(), Vector2{ 800 - textSize.x / 2, 40 - textSize.y / 2 }, 26, 2.0f, BLACK);
		}

		if (Program_state == HashTable_state) hashtable->draw();
		else if (Program_state == LinkedList_state) linkedlist->Draw();
		else if (Program_state == Graph_state) graph->draw();
		else if (Program_state == Trie_state) avl->Draw();

		else if (Program_state == About_state) about->Draw();
		else if (Program_state == Settings_state) setting->Draw();
		else if (Program_state == HowToUse_state) helper->Draw();

		if (Program_state == Trie_state) DataName = "AVL";
		else if (Program_state == HashTable_state) DataName = "Hash table: Linear Probing";
		else if (Program_state == LinkedList_state) DataName = "Linked List";
		else if (Program_state == Graph_state) DataName = "Graph";
		else if (Program_state == About_state) DataName = "About";
		else if (Program_state == Settings_state) DataName = "Settings";
		else if (Program_state == HowToUse_state) DataName = "How to use";

		//if (Program_state != Menu_state && Program_state != About_state && Program_state != Settings_state && Program_state != HowToUse_state) {
		//	toolbar.Draw(); // Draw the Toolbar only on smaller screens
		//}

		EndDrawing();
	}

	CloseWindow();

	UnloadTexture(lightIcon);
	UnloadTexture(darkIcon);
	UnloadTexture(titlebar);
	UnloadFont(font);

	setting->Unload();  // Giải phóng tài nguyên của màn hình Settings
	about->Unload();    // Giải phóng tài nguyên của màn hình About
	helper->Unload();    // Giải phóng tài nguyên của màn hình Helper
	audioSys.Unload();    // Giải phóng âm thanh 
}
