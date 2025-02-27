#include "Button.h"

CommonButton::CommonButton() {
	isInsert = isRemove = isSearch = 0;
	isCreate = 1; 

	rect = {0, 500, 166, 210};
	color = { 220, 180, 120, 255 }; 
	color = { 230, 170, 100, 255 }; 
	color = { 66, 135, 245, 255 };
	color = { 63, 81, 181, 255 }; /// xanh dam dam hon
	color = { 92, 107, 192, 255 };// xanh dam hon chut

///	color = { 159, 168, 218, 255 }; xanh nhat
///	color = { 255, 107, 107, 255 };
///	color = { 200, 70, 70, 255 };


	cButton.push_back({ {8, 515, 150, 45}, "Create", btnNormal, btnHover, btnClick });
	cButton.push_back({ {8, 560, 150, 45}, "Search", btnNormal, btnHover, btnClick });
	cButton.push_back({ {8, 605, 150, 45}, "Insert", btnNormal, btnHover, btnClick });
	cButton.push_back({ {8, 650, 150, 45}, "Remove", btnNormal, btnHover, btnClick });

	cButton[0].isChose = 1; 
}

void CommonButton::update() {
	for (int i = 0; i < 4; i++) {
		if (cButton[i].update()) {
			for (int j = 0; j < 4; j++) cButton[j].isChose = 0; 
			cButton[i].isChose = 1; 
			isInsert = isRemove = isSearch = isCreate = 0; 

			if (i == 0) isCreate = 1;
			else if (i == 1) isSearch = 1;
			else if (i == 2) isInsert = 1;
			else isRemove = 1; 
		}
	}
}


void CommonButton :: draw() {
	DrawRectangleRec(rect, color); 
	Texture2D activated = LoadTexture("Assets/Images/dark-activated.png");

	for (int i = 0; i < 4; i++) {
		if (cButton[i].isChose) {
			DrawTextureEx(activated, { cButton[i].rect.x, cButton[i].rect.y }, 0.0f, 1.f, WHITE);
		}
		else if (cButton[i].isHovered) {
			cButton[i].drawRect();
		}

		cButton[i].drawName();
	}
}

