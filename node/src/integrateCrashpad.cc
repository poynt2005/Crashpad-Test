#include <filesystem>
#include <stdio.h>
#include "integrateCrashpad.h"
#include "infoUtility.h"

IntegrateCrashpad::IntegrateCrashpad()
{
    std::filesystem::path crashpadHandlerPath = std::filesystem::current_path() / std::filesystem::path("crashpad") / std::filesystem::path("bin") / std::filesystem::path("crashpad_handler.exe");
    std::filesystem::path dumpPath = std::filesystem::current_path() / std::filesystem::path("dump");

    if (!std::filesystem::exists(dumpPath))
    {
        std::filesystem::create_directory(dumpPath);
    }

    crashpadExecutablePath = crashpadHandlerPath.wstring();
    dumpfilePath = dumpPath.wstring();

    initializeVars();
}

IntegrateCrashpad::IntegrateCrashpad(const std::wstring &_executable, const std::wstring &_dumpfile)
    : crashpadExecutablePath(_executable), dumpfilePath(_dumpfile)
{
    initializeVars();
}

IntegrateCrashpad::~IntegrateCrashpad() {}

void IntegrateCrashpad::initializeVars()
{
    productName = std::string("UnknownWinProduct");
    version = std::string("1.0.0");
    processType = std::string("WindowsExecutableFile");
    isDevlopement = false;

    isUploadToServer = false;
    serverURL = std::string("");

    crashpadArgs = {
        "--no-rate-limit",
        "--no-upload-gzip"};

    isCrashpadInitialized = false;
    isCrashpadStarted = false;
}

void IntegrateCrashpad::SetProductName(const std::string &str)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    productName = std::move(str);
}

void IntegrateCrashpad::SetIsDevlopement(const bool b)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    isDevlopement = std::move(b);
}

void IntegrateCrashpad::SetVersion(const std::string &str)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    version = std::move(str);
}

void IntegrateCrashpad::SetProcessType(const std::string &str)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    processType = std::move(str);
}

void IntegrateCrashpad::SetServerURL(const std::string &str)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    serverURL = std::move(str);
}

void IntegrateCrashpad::SetIsUploadToServer(const bool b)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot set property after crashpad initialized\n");
        return;
    }
    isUploadToServer = std::move(b);
}

void IntegrateCrashpad::AddCrashpadArgs(const std::string &arg)
{
    if (isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot add arguments after crashpad initialized\n");
        return;
    }
    crashpadArgs.emplace_back(std::move(arg));
}

const bool IntegrateCrashpad::InitializeCrashpad()
{

    db = base::FilePath(dumpfilePath);
    handler = base::FilePath(crashpadExecutablePath);
    datenbank = crashpad::CrashReportDatabase::Initialize(db);

    if (datenbank == nullptr || datenbank->GetSettings() == nullptr)
    {
        fprintf(stderr, "Crash Reporter Initialize Error\n");
        return false;
    }

    if (isUploadToServer)
    {
        datenbank->GetSettings()->SetUploadsEnabled(isUploadToServer);
    }

    annotations.insert(std::pair<std::string, std::string>("format", "minidump"));
    annotations.insert(std::pair<std::string, std::string>("product", productName));
    annotations.insert(std::pair<std::string, std::string>("version", version));
    annotations.insert(std::pair<std::string, std::string>("crashID", GetUUID()));
    annotations.insert(std::pair<std::string, std::string>("date", GetTimestamp()));
    annotations.insert(std::pair<std::string, std::string>("userID", GetCurrentUserName()));
    annotations.insert(std::pair<std::string, std::string>("pid", GetCurrentPID()));
    annotations.insert(std::pair<std::string, std::string>("plat", ""));
    annotations.insert(std::pair<std::string, std::string>("platform", "win32"));
    annotations.insert(std::pair<std::string, std::string>("osarch", GetOsArch()));
    annotations.insert(std::pair<std::string, std::string>("process_type", processType));
    annotations.insert(std::pair<std::string, std::string>("username", GetCurrentUserName()));
    annotations.insert(std::pair<std::string, std::string>("ver", version));
    annotations.insert(std::pair<std::string, std::string>("_productName", productName));
    annotations.insert(std::pair<std::string, std::string>("isDevlopement", isDevlopement ? std::string("true") : std::string("false")));

    isCrashpadInitialized = true;

    return true;
}

const bool IntegrateCrashpad::Start()
{
    if (!isCrashpadInitialized)
    {
        fprintf(stderr, "You cannot start crashpad before you initialized it\n");
        return false;
    }

    bool ret = client.StartHandler(handler,
                                   db,
                                   db,
                                   serverURL,
                                   annotations,
                                   crashpadArgs,
                                   true,
                                   true);
    if (!ret)
    {
        fprintf(stderr, "Cannot Start crashreporter handler\n");
        return false;
    }

    ret = client.WaitForHandlerStart(1e9);

    if (!ret)
    {
        fprintf(stderr, "Crashpad Client start error\n");
        return false;
    }

    isCrashpadStarted = true;

    return true;
}

bool IntegrateCrashpad::GetIsInitialized()
{
    return isCrashpadInitialized;
}
bool IntegrateCrashpad::GetIsStart()
{
    return isCrashpadStarted;
}

std::wstring IntegrateCrashpad::GetCrashpadExecutablePath()
{
    return crashpadExecutablePath;
}
std::wstring IntegrateCrashpad::GetDumpfilePath()
{
    return dumpfilePath;
}

IntegrateCrashpad &IntegrateCrashpad::operator=(IntegrateCrashpad &&rhs)
{
    if (this != &rhs)
    {
        crashpadExecutablePath = rhs.GetCrashpadExecutablePath();
        dumpfilePath = rhs.GetDumpfilePath();
    }

    return *this;
}