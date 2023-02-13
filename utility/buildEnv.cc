#include "executeCommand.h"
#include <filesystem>
#include <iostream>
#include <direct.h>
int main()
{
    std::cout << "Install Node Modules...\n\n";
    std::filesystem::path projectDir = std::filesystem::current_path();
    std::filesystem::path crashpadDir = projectDir / "node";
    std::filesystem::path guiDir = projectDir / "gui/node";

    std::vector<std::string> result;

    _chdir(crashpadDir.string().data());
    executeCommand("npm i --ignore-scripts", result, true);
    result.empty();

    _chdir(guiDir.string().data());
    executeCommand("npm i --ignore-scripts", result, true);
    result.empty();

    std::cout << "Install Node Modules successfully\n\n";
    return 0;
}