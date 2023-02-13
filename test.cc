#include "integrateCrashpad.h"
#include <memory>
#include <iostream>
int main()
{

    IntegrateCrashpad crashpad;

    crashpad = IntegrateCrashpad(L"D:\\dev\\temp\\integratecrashpad\\crashpad\\bin\\crashpad_handler.exe", L"D:\\dev\\temp\\integratecrashpad\\dump");

    crashpad.SetProductName(std::string("MyTestExe"));
    crashpad.SetVersion("1.0.0");
    crashpad.SetProcessType("MicrosoftWindowsExe");
    crashpad.SetServerURL("http://127.0.0.1:7000/crashdump");
    crashpad.SetIsUploadToServer(true);

    std::cout << crashpad.InitializeCrashpad();
    std::cout << crashpad.Start();

    int *p = NULL;
    *p = 1;

    return 0;
}