#include <windows.h>
#include <thread>
#include <future>
#include <iostream>
#include <filesystem>
#include <Python.h>

bool fetchFile(const char *URL, const char *filename)
{
    auto downloader = [&](std::promise<bool> &&returnVal) -> void
    {
        HRESULT hResult = URLDownloadToFile(0, URL, filename, 0, 0);

        if (hResult == S_OK)
        {
            returnVal.set_value(true);
        }
        else
        {
            returnVal.set_value(false);
        }
    };

    std::promise<bool> res;
    std::future<bool> fut = res.get_future();
    std::thread(downloader, std::move(res)).join();

    return fut.get();
}

void extractZip(const char *zipFile, const char *targetDir)
{
    Py_SetPythonHome(L"");
    Py_Initialize();
    PyObject *PyShutilModule = PyImport_ImportModule("shutil");
    PyObject *PyUnpackArchiveResult = PyObject_CallMethod(PyShutilModule, "unpack_archive", "sss", zipFile, targetDir, "zip");
    Py_DECREF(PyUnpackArchiveResult);
    Py_DECREF(PyShutilModule);
    Py_Finalize();
}

int main()
{
    if (!std::filesystem::exists("crashpad") && fetchFile("http://get.backtrace.io/crashpad/builds/crashpad-release-x86-64-latest.zip", "crashpad.zip"))
    {
        extractZip("crashpad.zip", "tempFolder");
        std::filesystem::remove("crashpad.zip");

        for (auto &&entry : std::filesystem::directory_iterator("tempFolder"))
        {
            std::filesystem::path srcDir = "tempFolder" / entry.path().filename();
            std::filesystem::create_directory("crashpad");
            std::filesystem::copy(
                srcDir,
                std::filesystem::path("crashpad"),
                std::filesystem::copy_options::overwrite_existing | std::filesystem::copy_options::recursive);

            std::filesystem::remove_all("tempFolder");
            break;
        }
    }
    return 0;
}
