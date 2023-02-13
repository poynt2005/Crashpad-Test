#include <Python.h>
#include <filesystem>
#include <vector>
#include <iostream>
#include <string>

void listAllFiles(std::vector<std::string> &files, std::vector<std::string> &path)
{
    PyObject *PyOSModule = PyImport_ImportModule("os");
    PyObject *PyPathModule = PyObject_GetAttrString(PyOSModule, "path");
    PyObject *PyGlobModule = PyImport_ImportModule("glob");
    PyObject *PyGlobMethod = PyObject_GetAttrString(PyGlobModule, "glob");
    std::vector<std::string> exts = {
        "lib",
        "dll",
        "exe",
        "node"};

    std::vector<std::string> dirs = {
        "dump",
        "node_modules",
        "wholeTest",
        "crashpad"};

    for (int i = 0; i < (exts.size() + dirs.size()); i++)
    {
        std::string pathname;
        if (i < exts.size())
        {
            pathname = std::string("**/**." + exts[i]);
        }
        else
        {
            pathname = std::string("**/" + dirs[i - exts.size()]);
        }
        PyObject *PyGlobArgs = Py_BuildValue("(s)", pathname.data());
        PyObject *PyGlobKwargs = PyDict_New();
        PyDict_SetItemString(PyGlobKwargs, "recursive", Py_True);
        PyObject *PyGlobResult = PyObject_Call(PyGlobMethod, PyGlobArgs, PyGlobKwargs);
        if (PyList_Check(PyGlobResult))
        {
            int resultLen = PyList_Size(PyGlobResult);
            for (int j = 0; j < resultLen; j++)
            {
                PyObject *PyRealPath = PyObject_CallMethod(PyPathModule, "realpath", "O", PyList_GetItem(PyGlobResult, j));
                PyObject *PyGlobItemToByte = PyUnicode_AsEncodedString(PyRealPath, "utf-8", NULL);

                std::string globItemStr = std::string(PyBytes_AsString(PyGlobItemToByte));
                if (i < exts.size())
                {
                    if (globItemStr.find("clearAll.exe") == std::string::npos)
                    {
                        files.emplace_back(globItemStr);
                    }
                }
                else
                {
                    path.emplace_back(globItemStr);
                }

                Py_DECREF(PyGlobItemToByte);
                Py_DECREF(PyRealPath);
            }
        }

        Py_DECREF(PyGlobResult);
        Py_DECREF(PyGlobKwargs);
        Py_DECREF(PyGlobArgs);
    }

    Py_DECREF(PyGlobMethod);
    Py_DECREF(PyGlobModule);
    Py_DECREF(PyPathModule);
    Py_DECREF(PyOSModule);
}

void deleter(const bool isDir, const std::string &path)
{
    if (!std::filesystem::exists(path))
    {
        std::cout << "Path: " << path << " is not exists, maybe it is deleted\n";
        return;
    }

    if (isDir)
    {
        if (std::filesystem::remove_all(path))
        {
            std::cout << "Directory path: " << path << " delete success\n";
            return;
        }
    }
    else
    {
        if (std::filesystem::remove(path))
        {
            std::cout << "File path: " << path << " delete success\n";
            return;
        }
    }

    std::cout << "Path: " << path << " delete failed\n";
}

int main()
{
    Py_SetPythonHome(L"");
    Py_Initialize();

    std::vector<std::string> exts;
    std::vector<std::string> paths;
    listAllFiles(exts, paths);

    Py_Finalize();

    for (auto &&path : paths)
    {
        deleter(true, path);
    }

    for (auto &&ext : exts)
    {
        deleter(false, ext);
    }

    if (paths.size() == 0 && exts.size() == 0)
    {
        std::cout << "No files to delete\n";
    }

    return 0;
}