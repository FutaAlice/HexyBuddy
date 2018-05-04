#include <functional>
#include <memory>
#include "hexyhandleimpl.h"
using namespace std;

bool HexyHandleImpl::init() {
    findHexy();
    return isAlive();
}

bool HexyHandleImpl::isAlive() {
    return targetWnd_ && targetPID_ && IsWindow(targetWnd_);
}

class ScopedGuard {
public:
    ScopedGuard(std::function<void()> cleanRoutine) :
        func_(cleanRoutine) {}
    ~ScopedGuard() {
        func_();
    }
private:
    function<void()> func_;
};

/**
 * @brief 用于 EnumWindows 的回调函数
 * 
 * @param hwnd 依次遍历所有顶层窗口的句柄
 * @param param 用于接收结果的 HWND *
 * @return BOOL 函数返回FALSE，则停止枚举；否则继续。
 */
static BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param) {
    TCHAR wndCaption[MAX_PATH] = TEXT("");
    GetWindowText(hwnd, wndCaption, MAX_PATH);
    // 若窗体标题为 "Hexy"，则记录并停止枚举
    if (!wcscmp(wndCaption, TARGET_WINDOW_CAPTION)) {
        *reinterpret_cast<HWND *>(param) = hwnd;
        return FALSE;
    }
    return TRUE;
}

void HexyHandleImpl::findHexy() {
    if (isAlive())
        return;

    // 获取系统中所有进程的快照
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ScopedGuard guard([&] { CloseHandle(hProcess); });
    if (hProcess == INVALID_HANDLE_VALUE)
        return;

    // 如果存在名为 "Hexy.exe" 的进程，则赋值给 targetPID_
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcess, &entry))
        return;
    while (Process32Next(hProcess, &entry))
        if (!wcscmp(TARGET_IAMGE_NAME, entry.szExeFile))
            targetPID_ = entry.th32ProcessID;

    // 如果存在标题名为 "Hexy" 的顶层窗口，则将窗体句柄赋值给 targetWnd_
    EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(&targetWnd_));
}

/**
 * @brief 用于辅助读取内存的宏
 */
#define ARG(var) reinterpret_cast<void *>(&var), sizeof(var), var##offset

/**
 * @brief 读取内存偏移中的数据
 * 
 * @param hProc Hexy 进程对象的句柄
 * @param dest 读取成功后存放位置
 * @param cap 读取的字节数
 * @param address 读取目标的内存地址
 * @return bool 成功返回 true ，否则抛出异常
 */
static bool readRemoteMemory(HANDLE hProc, void *dest, size_t cap, size_t address) {
    size_t cnt = 0;
    if (!ReadProcessMemory(hProc,
                           reinterpret_cast<LPCVOID>(address),
                           dest,
                           cap,
                           reinterpret_cast<SIZE_T *>(&cnt)) || cnt != cap) {
        throw "Fatal error at readRemoteMemory() !";
    }
    return true;
}

void HexyHandleImpl::updateData() {
    if (!isAlive())
        throw "Hexy process not exist, or HexyBuddy not initialized!";

    const DWORD dwDA = PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_QUERY_LIMITED_INFORMATION;
    HANDLE hProc = OpenProcess(dwDA, FALSE, targetPID_);
    ScopedGuard guard([&] {CloseHandle(hProc); });
    if (!hProc)
        throw "Error occurred at OpenProcess()! Plz init HexyBuddy and try again.";

    readRemoteMemory(hProc, ARG(pawnNum_));
    readRemoteMemory(hProc, ARG(gameOver_));
    readRemoteMemory(hProc, ARG(boardSize_));
    readRemoteMemory(hProc, ARG(boardRatio_));
    readRemoteMemory(hProc, ARG(canvasCenterX_));
    readRemoteMemory(hProc, ARG(canvasCenterY_));
    readRemoteMemory(hProc, ARG(globalPosList_));
    readRemoteMemory(hProc, ARG(hexygonCenterPairList_));
}

HexyHandleImpl::Points HexyHandleImpl::getRec() {
    updateData();
    Points rec;
    for (int i = 0; i < pawnNum_; ++i) {
        int col = globalPosList_[i] % (boardSize_ + 2) - 1;
        int row = globalPosList_[i] / (boardSize_ + 2) - 1;
        rec.push_back(make_tuple(col, row));
    }
    return rec;
}

int HexyHandleImpl::getBoardsize() {
    updateData();
    return pawnNum_;
}

bool HexyHandleImpl::setPiece(const std::tuple<int, int> &pos) {
    auto rec = getRec();
    int targetCol = get<0>(pos);
    int targetRow = get<1>(pos);
    if (0 > targetCol || boardSize_ <= targetCol ||
        0 > targetRow || boardSize_ <= targetRow)
        return false;

    for (auto x : rec) {
        if (targetCol == get<0>(x) && targetRow == get<1>(x))
            return false;
    }

    int index = (targetRow + 1) * (boardSize_ + 2) + (targetCol + 1);
    double x = canvasCenterX_ + boardRatio_ * hexygonCenterPairList_[2 * index];
    double y = canvasCenterY_ + boardRatio_ * hexygonCenterPairList_[2 * index + 1];
    LPARAM mousePos = short(x) | short(y) << 16;

    SendMessage(targetWnd_, WM_LBUTTONDOWN, 0, mousePos);
    SendMessage(targetWnd_, WM_LBUTTONUP, 0, mousePos);
    return true;
}
