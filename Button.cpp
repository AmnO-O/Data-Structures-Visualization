#include "Button.h"
#define PANEL_PADDING 20 // Khoảng cách lề

CommonButton::CommonButton() {
	isInsert = isRemove = isSearch = hide = 0;
	isCreate = 1;

	rect = { PANEL_PADDING + 8, 330, 130, 40 };
	color = DARKBLUE;

	cButton.push_back({ { PANEL_PADDING + 8, 330, 130, 40 }, "Create", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 390, 130, 40 }, "Search", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 450, 130, 40 }, "Insert", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 510, 130, 40 }, "Remove", btnNormal, btnHover, btnClick });


	cButton[0].isChose = 1;
}

bool CommonButton::update() {
	bool ok = false;
	for (int i = 0; i < 4; i++) {
		if (cButton[i].update()) {
			for (int j = 0; j < 4; j++) cButton[j].isChose = 0;
			cButton[i].isChose = 1;
			isInsert = isRemove = isSearch = isCreate = 0;

			if (i == 0) isCreate = 1;
			else if (i == 1) isSearch = 1;
			else if (i == 2) isInsert = 1;
			else isRemove = 1;
			hide = false; 
			ok = true;
		}
	}

	return ok;
}


void CommonButton::draw(Font font) {
	DrawRectangleRec(rect, color);

	for (int i = 0; i < 4; i++) {
		cButton[i].draw(font);
	}
}


GraphButton::GraphButton() {
	isAddedge = isRemove = isMst = hide = 0; 
	isCreate = 1;


	rect = { PANEL_PADDING + 8, 330, 130, 40 };
	color = DARKBLUE;

	cButton.push_back({ { PANEL_PADDING + 8, 330, 130, 40 }, "Create", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 390, 130, 40 }, "Add Edge", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 450, 130, 40 }, "Delete Edge", btnNormal, btnHover, btnClick });
	cButton.push_back({ { PANEL_PADDING + 8, 510, 130, 40 }, "MST(Kruskal)", btnNormal, btnHover, btnClick });


	cButton[0].isChose = 1;
}

bool GraphButton::update() {
	bool ok = false;

	for (int i = 0; i < 4; i++) {
		if (cButton[i].update()) {
			for (int j = 0; j < 4; j++) cButton[j].isChose = 0;
			cButton[i].isChose = 1;
			isAddedge = isRemove = isMst = isCreate = 0;

			if (i == 0) isCreate = 1;
			else if (i == 1) isAddedge = 1;
			else if (i == 2) isRemove = 1;
			else isMst = 1;

			hide = false; 

			ok = true;
		}
	}

	return ok;
}

void GraphButton::draw(Font font) {
	DrawRectangleRec(rect, color);

	for (int i = 0; i < 4; i++) {
		cButton[i].draw(font);

		//if (cButton[i].isChose) {

		//}
		//else if (cButton[i].isHovered) {
		//	cButton[i].drawRect();
		//}

		//cButton[i].drawName(font);
	}
}


