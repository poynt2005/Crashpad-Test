#ifndef __EXECUTE_COMMAND_H__
#define __EXECUTE_COMMAND_H__
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <iostream>
#define BUF_SIZE 256

static inline void executeCommand(const std::string &command, std::vector<std::string> &result, bool isPrint)
{
    auto executioner = [&]() -> void
    {
        char buf[BUF_SIZE] = {'\0'};
        FILE *fp = _popen(command.data(), "r");

        while (fgets(buf, BUF_SIZE, fp))
        {
            result.emplace_back(std::string(buf));

            if (isPrint)
            {
                std::cout << "\t" << buf;
            }
        }
    };

    std::thread(executioner).join();
}

#endif