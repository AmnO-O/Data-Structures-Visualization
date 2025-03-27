#include "Adjust.h"
#include <iostream>

Adjust::Adjust() : darkMode(false) {}

Adjust::~Adjust() {}

void Adjust::CreateToolbar() {
    // Implementation to create a toolbar
    std::cout << "Toolbar created." << std::endl;
}

void Adjust::SetDarkMode(bool enable) {
    darkMode = enable;
    std::cout << "Dark mode " << (enable ? "enabled" : "disabled") << "." << std::endl;
}

void Adjust::LoadIcons(const std::string& iconFolder) {
    // Load icons from the specified folder
    icons = { iconFolder + "/back.png", iconFolder + "/stop.png", iconFolder + "/continue.png", iconFolder + "/next.png" };
    std::cout << "Icons loaded from " << iconFolder << "." << std::endl;
}

bool Adjust::IsDarkMode() const {
    return darkMode;
}

std::vector<std::string> Adjust::GetIcons() const {
    return icons;
}