#ifndef _GUI_H_
#define _GUI_H_

#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)
#include <functional>

#ifdef BUILDGUIAPI
#define EXPORTGUIAPI __declspec(dllexport)
#else
#define EXPORTGUIAPI __declspec(dllimport)
#endif

EXPORTGUIAPI bool InitializeApp();
EXPORTGUIAPI void RegisterButtonCallback(std::function<void()> &);
EXPORTGUIAPI void RenderFrame(int *);
EXPORTGUIAPI void MainLoop();
EXPORTGUIAPI void ShowAndUpdateWindow();

#endif
