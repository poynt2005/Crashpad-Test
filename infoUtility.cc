#include "infoUtility.h"
#include <iostream>
#include <memory>
#include <windows.h>
#include <Lmcons.h>
#include <time.h>

const std::string GetUUID()
{
    UUID uuid;
    char *crashID;
    RPC_STATUS status = UuidCreate(&uuid);
    UuidToStringA(&uuid, reinterpret_cast<RPC_CSTR *>(&crashID));

    return std::string(crashID);
}

const std::string GetTimestamp()
{
    long long timestamp = static_cast<long long>(time(0));
    return std::to_string(timestamp);
}

const std::string GetOsArch()
{
#if _WIN32 || _WIN64
#if _WIN64
    return std::string("amd64");
#else
#define ENVIRONMENT32
    return std::string("x86");
#endif
#endif
}

const std::string GetCurrentPID()
{
    int pid = GetCurrentProcessId();

    return std::to_string(pid);
}

const std::string GetCurrentUserName()
{
    std::unique_ptr<char[]> username = std::make_unique<char[]>(UNLEN + 1);
    DWORD size = UNLEN + 1;
    GetUserName(username.get(), &size);

    return std::string(username.get());
}