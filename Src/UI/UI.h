#pragma once
#include "../Constants.h"
#include "Menu.h"
#include "../Data_structures/Hash_table/HashTableVisual.h"
#include "../Data_structures/Linked_list/LinkedlistVisual.h"
#include "../Data_structures/Avl/AVLTreeVisual.h"
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
