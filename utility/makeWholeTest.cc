#include "executeCommand.h"
#include <filesystem>
#include <vector>
#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <iostream>

int main()
{
    const std::filesystem::path projectPath = std::filesystem::current_path();
    const std::filesystem::path crashpadNodePath = projectPath / std::filesystem::path("node");
    const std::filesystem::path guiNodePath = projectPath / std::filesystem::path("gui") / std::filesystem::path("node");
    const std::filesystem::path wholeTestPath = projectPath / std::filesystem::path("wholeTest");
    std::vector<std::string> commandResult;

    std::cout << "Step 1:"
              << "Build CrashpadIntegrate node.js module"
              << "\n\n";

    std::cout << "\t***Current work***\t"
              << "Change directory to ./node"
              << "\n\n";

    _chdir(crashpadNodePath.string().data());

    std::cout << "\t***Current work***\t"
              << "Run npm build-native"
              << "\n\n";

    commandResult.empty();
    executeCommand("npm run build-native", commandResult, true);

    std::cout << "Step 2:"
              << "Build simple gui node.js module"
              << "\n\n";

    std::cout << "\t***Current work***\t"
              << "Change directory to ./gui/node"
              << "\n\n";

    _chdir(guiNodePath.string().data());
    std::cout << "\t***Current work***\t"
              << "Run npm build-native"
              << "\n\n";

    commandResult.empty();
    executeCommand("npm run build-native", commandResult, true);

    std::cout << "Step 3:"
              << "Prepare wholeTest directory"
              << "\n\n";

    std::cout << "\t***Current work***\t"
              << "Change directory to ./"
              << "\n\n";
    _chdir(projectPath.string().data());

    std::cout << "\t***Current work***\t"
              << "Make new directory"
              << "\n\n";

    if (std::filesystem::exists(wholeTestPath))
    {
        std::filesystem::remove_all(wholeTestPath);
    }
    std::filesystem::create_directory(wholeTestPath);

    std::cout << "\t***Current work***\t"
              << "Copy files to wholeTest directory"
              << "\n\n";

    std::vector<std::filesystem::path> needFiles = {
        projectPath / "node/CrashpadIntegrate.js",
        projectPath / "node/CrashpadIntegrate.node",
        projectPath / "node/integrateCrashpad.dll",
        projectPath / "gui/node/CrashpadGui.js",
        projectPath / "gui/node/CrashpadGui.node",
        projectPath / "gui/node/gui.dll",
        projectPath / "wholeTest.js"};

    for (auto &&f : needFiles)
    {
        const std::filesystem::path filename = f.filename();

        std::filesystem::copy_file(
            f,
            projectPath / "wholeTest" / filename);
    }

    std::filesystem::rename(
        projectPath / "wholeTest/wholeTest.js",
        projectPath / "wholeTest/test.js");
    return 0;
}