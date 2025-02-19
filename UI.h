#pragma once
#include "Constants.h"

class Visualizer{
public : 
	void Process();
private:
	int Program_state = -1;
	bool isLight = true;
};
