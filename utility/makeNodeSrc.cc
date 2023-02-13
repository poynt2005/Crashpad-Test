#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

void listAllSrcFile(std::vector<std::string> &files)
{
    std::vector<std::string> excludeFiles = {"test.cc"};

    for (auto &&entry : std::filesystem::directory_iterator("./"))
    {
        if (entry.path().extension().string() == ".h" || entry.path().extension().string() == ".cc")
        {
            std::string filename = entry.path().filename().string();

            if (std::find(excludeFiles.begin(), excludeFiles.end(), filename) == excludeFiles.end())
            {
                files.emplace_back(filename);
            }
        }
    }
}

int main()
{
    std::vector<std::string> files;
    listAllSrcFile(files);

    if (std::filesystem::exists("./node/src"))
    {
        std::filesystem::remove_all("./node/src");
    }

    std::filesystem::create_directory("./node/src");

    for (auto &&entry : files)
    {
        std::filesystem::path srcPath = std::filesystem::current_path() / entry;
        std::filesystem::path dstPath = std::filesystem::current_path() / std::filesystem::path("node") / std::filesystem::path("src") / entry;
        std::filesystem::copy_file(srcPath, dstPath);
    }
}