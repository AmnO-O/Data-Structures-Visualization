#pragma once
#include "Constants.h"
#include "Menu.h"
#include "HashTable.h"
#include "LinkedlistScreen.h"
#include "AboutScreen.h"
#include "Helper.h"
#include "HashTableScreen.h"

class Visualizer {
public:
    Visualizer();
    void Process();
private:
    Menu menu;
    LinkedListScreen LinkedListScr;
    AboutScreen AboutScr;
    HelperScreen HelperScr;
    SettingScreen SettingScr;
    AudioSystem audioSys;
    HashTableScreen hashtableScr;
};
