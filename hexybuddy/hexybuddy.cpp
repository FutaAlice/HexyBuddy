#include <functional>

#include <Windows.h>
#include <TlHelp32.h>

#include "define.h"
#include "hexybuddy.h"

namespace hexybuddy {

HWND targetWnd = 0;
DWORD targetProcessID = 0;

struct scopedGuard
{
public:
    scopedGuard(std::function<void()> cleanRoutine) :
        func(cleanRoutine)
    {
    }
    ~scopedGuard()
    {
        func();
    }
private:
    std::function<void()> func;
};

static BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param)
{
    TCHAR wndCaption[MAX_PATH] = TEXT("");
    GetWindowText(hwnd, wndCaption, MAX_PATH);
    if (!wcscmp(wndCaption, TARGET_WINDOW_CAPTION))
    {
        targetWnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

void foo()
{
    if (targetWnd && targetProcessID && IsWindow(targetWnd))
        return;

    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    scopedGuard guard([&]() { CloseHandle(hProcess); });
    if (hProcess == INVALID_HANDLE_VALUE)
    {
        return;
    }
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcess, &entry))
    {
        return;
    }
    while (Process32Next(hProcess, &entry))
    {
        if (!wcscmp(TARGET_IAMGE_NAME, entry.szExeFile))
        {
            targetProcessID = entry.th32ProcessID;
        }
    }
    EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(targetWnd));

}

}
