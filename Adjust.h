#pragma once
#include <string>
#include <vector>

class Adjust
{
public:
    Adjust();
    ~Adjust();

    void CreateToolbar();
    void SetDarkMode(bool enable);
    void LoadIcons(const std::string& iconFolder);

    bool IsDarkMode() const;
    std::vector<std::string> GetIcons() const;

private:
    bool darkMode;
    std::vector<std::string> icons;
};