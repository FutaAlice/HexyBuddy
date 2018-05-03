#include <functional>
#include <memory>
#include "hexyhandleimpl.h"
using namespace std;

bool HexyHandleImpl::init()
{
    findHexy();
    return isAlive();
}

bool HexyHandleImpl::isAlive()
{
    return targetWnd_ && targetPID_ && IsWindow(targetWnd_);
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
            targetPID_ = entry.th32ProcessID;
    EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(&targetWnd_));
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
        throw;
    }
    return true;
}

void HexyHandleImpl::updateData()
{
    if (!isAlive())
        throw;

    const DWORD dwDA = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION;
    HANDLE hProc = OpenProcess(dwDA, FALSE, targetPID_);
    ScopedGuard guard([&] {CloseHandle(hProc); });
    if (!hProc)
        throw;

    readRemoteMemory(hProc, ARG(pawnNum_));
    readRemoteMemory(hProc, ARG(gameOver_));
    readRemoteMemory(hProc, ARG(boardSize_));
    readRemoteMemory(hProc, ARG(boardRatio_));
    readRemoteMemory(hProc, ARG(canvasCenterX_));
    readRemoteMemory(hProc, ARG(canvasCenterY_));
    readRemoteMemory(hProc, ARG(globalPosList_));
    readRemoteMemory(hProc, ARG(hexygonCenterPairList_));
}

std::vector<std::tuple<int, int>> HexyHandleImpl::getRec()
{
    updateData();
    std::vector<std::tuple<int, int>> rec;
    for (int i = 0; i < pawnNum_; ++i)
    {
        int col = globalPosList_[i] % (boardSize_ + 2) - 1;
        int row = globalPosList_[i] / (boardSize_ + 2) - 1;
        rec.push_back(make_tuple(col, row));
    }
    return rec;
}

bool HexyHandleImpl::setPiece(const std::tuple<int, int> &pos)
{
    auto rec = getRec();
    int setCol = get<0>(pos);
    int setRow = get<1>(pos);
    for (auto x : rec)
    {
        if (setCol == get<0>(x) && setRow == get<1>(x))
            return false;
    }

    int index = (setRow + 1) * (boardSize_ + 2) + (setCol + 1);
    double x = canvasCenterX_ + boardRatio_ * hexygonCenterPairList_[2 * index];
    double y = canvasCenterY_ + boardRatio_ * hexygonCenterPairList_[2 * index + 1];
    LPARAM mousePos = short(x) | short(y) << 16;

    SendMessage(targetWnd_, WM_LBUTTONDOWN, 0, mousePos);
    SendMessage(targetWnd_, WM_LBUTTONUP, 0, mousePos);
    return true;
}
