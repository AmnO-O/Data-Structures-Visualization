#pragma once
#include "Constants.h"
#include "Menu.h"
#include "HashTableVisual.h"
#include "LinkedlistScreen.h"
#include "AboutScreen.h"
#include "Helper.h"
#include "GraphVisual.h"
#include "AVLTreeScreen.h"

class Visualizer {
public:
	Visualizer();
	void Process();
private:
	int Program_state = -1;
	Font font;
	AudioSystem audioSys;
};
