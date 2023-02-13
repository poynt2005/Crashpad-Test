#include "gui.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
int main()
{
    if (InitializeApp())
    {
        // std::function<void()> recur;
        // ShowAndUpdateWindow();
        // unsigned int *nigga;
        // recur = [&]() -> void
        // {
        //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
        //     RenderFrame(nigga);
        //     std::cout << *nigga << '\n';
        //     recur();
        // };

        // recur();
        ShowAndUpdateWindow();
        MainLoop();
    }
    return 0;
}