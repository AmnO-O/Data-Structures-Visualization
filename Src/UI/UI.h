#pragma once
#include "../Constants.h"
#include "Menu.h"
#include "../Data_structures/Hashtable/HashTableVisual.h"
#include "../Data_structures/Linked_list/LinkedlistScreen.h"
#include "../Data_structures/Avl/AVLTreeScreen.h"
#include "../Data_structures/Graph/GraphVisual.h"
#include "Screens/AboutScreen.h"
#include "Screens/Helper.h"


class Visualizer {
public:
	Visualizer();
	void Process();
private:
	int Program_state = -1;
	Font font;
	AudioSystem audioSys;
};
