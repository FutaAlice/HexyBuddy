#include <functional>
#include <memory>
#include <iostream>
#include "hexyhandleimpl.h"
using namespace std;

bool HexyHandleImpl::isAlive()
{
    return targetWnd && targetProcessID && IsWindow(targetWnd);
}

class ScopedGuard
{
public:
    ScopedGuard(std::function<void()> cleanRoutine) :
        func_(cleanRoutine) {}
    ~ScopedGuard()
    {
        func_();
    }
private:
    function<void()> func_;
};

static BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param)
{
    TCHAR wndCaption[MAX_PATH] = TEXT("");
    GetWindowText(hwnd, wndCaption, MAX_PATH);
    if (!wcscmp(wndCaption, TARGET_WINDOW_CAPTION))
    {
        auto pWnd = reinterpret_cast<HWND *>(param);
        *pWnd = hwnd;
        return FALSE;
    }
    return TRUE;
}

void HexyHandleImpl::findHexy()
{
    if (isAlive())
        return;

    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ScopedGuard guard([&] { CloseHandle(hProcess); });
    if (hProcess == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcess, &entry))
        return;
    while (Process32Next(hProcess, &entry))
        if (!wcscmp(TARGET_IAMGE_NAME, entry.szExeFile))
            targetProcessID = entry.th32ProcessID;
    EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(&targetWnd));
}

#define ARG(var) reinterpret_cast<void *>(&var), sizeof(var), var##offset

static bool readRemoteMemory(HANDLE hProc, void *dest, size_t cap, size_t address)
{
    size_t cnt = 0;
    if (!ReadProcessMemory(hProc,
                           reinterpret_cast<LPCVOID>(address),
                           dest,
                           cap,
                           reinterpret_cast<SIZE_T *>(&cnt)) || cnt != cap)
    {
        return false;
    }
    return true;
}

void HexyHandleImpl::updateData()
{
    if (!isAlive())
        return;

    HANDLE hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION,
                               FALSE, targetProcessID);
    ScopedGuard guard([&] {CloseHandle(hProc); });
    if (!hProc)
        return;

    readRemoteMemory(hProc, ARG(pawnNum_));
    readRemoteMemory(hProc, ARG(gameOver_));
    readRemoteMemory(hProc, ARG(boardSize_));
    readRemoteMemory(hProc, ARG(boardRatio_));
    readRemoteMemory(hProc, ARG(canvasCenterX_));
    readRemoteMemory(hProc, ARG(canvasCenterY_));
    readRemoteMemory(hProc, ARG(globalPosList_));
    readRemoteMemory(hProc, ARG(hexygonCenterPairList_));

    for (int i = 0; i < pawnNum_; ++i)
    {
        cout << i + 1 << ": ";
        cout << "col " << globalPosList_[i] % (boardSize_ + 2) - 1 << ", ";
        cout << "row " << globalPosList_[i] / (boardSize_ + 2) - 1 << endl;
    }
}