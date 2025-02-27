#pragma once
#include "Constants.h"

class Visualizer{
public : 
	Visualizer();
	void Process();
private:
	int Program_state = -1;
	bool isLight = true;
	Font font;
};
