#ifndef __INTEGRATE_CRASHPAD_H__
#define __INTEGRATE_CRASHPAD_H__

#include <string>
#include <map>
#include <vector>
#include <client/crash_report_database.h>
#include <client/settings.h>
#include <client/crashpad_client.h>

#ifdef BUILDCPAPI
#define EXPORTCPAPI __declspec(dllexport)
#else
#define EXPORTCPAPI __declspec(dllimport)
#endif

class EXPORTCPAPI IntegrateCrashpad
{
public:
    IntegrateCrashpad();
    IntegrateCrashpad(const std::wstring &, const std::wstring &);

    ~IntegrateCrashpad();

    IntegrateCrashpad &operator=(IntegrateCrashpad &&);

    void SetProductName(const std::string &);
    void SetIsDevlopement(const bool);
    void SetVersion(const std::string &);
    void SetProcessType(const std::string &);
    void SetServerURL(const std::string &);
    void SetIsUploadToServer(const bool);

    void AddCrashpadArgs(const std::string &);

    bool GetIsInitialized();
    bool GetIsStart();

    const bool InitializeCrashpad();
    const bool Start();

    std::wstring GetCrashpadExecutablePath();
    std::wstring GetDumpfilePath();

private:
    std::wstring crashpadExecutablePath;
    std::wstring dumpfilePath;

    /** dumpfile properties*/
    std::string productName;
    std::string version;
    std::string processType;
    bool isDevlopement;

    /** server url setting*/
    bool isUploadToServer;
    std::string serverURL;

    /** variable initialize*/
    void initializeVars();

    std::map<std::string, std::string> annotations;
    std::vector<std::string> crashpadArgs;
    std::unique_ptr<crashpad::CrashReportDatabase> datenbank;

    /** crashpad related*/
    crashpad::CrashpadClient client;
    base::FilePath db;
    base::FilePath handler;

    bool isCrashpadInitialized;
    bool isCrashpadStarted;
};

#endif