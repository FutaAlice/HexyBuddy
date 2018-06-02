#include <functional>
#include <memory>
#include <chrono>
#include <thread>
#include "hexyhandleimpl.h"
#include "msgdef.h"
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
 * @brief ���� EnumWindows �Ļص�����
 * 
 * @param hwnd ���α������ж��㴰�ڵľ��
 * @param param ���ڽ��ս���� HWND *
 * @return BOOL ��������FALSE����ֹͣö�٣����������
 */
static BOOL __stdcall findTargetWindow(HWND hwnd, LPARAM param) {
    TCHAR wndCaption[MAX_PATH] = TEXT("");
    GetWindowText(hwnd, wndCaption, MAX_PATH);
    // ���������Ϊ "Hexy"�����¼��ֹͣö��
    if (!wcscmp(wndCaption, TARGET_WINDOW_CAPTION)) {
        *reinterpret_cast<HWND *>(param) = hwnd;
        return FALSE;
    }
    return TRUE;
}

void HexyHandleImpl::findHexy() {
    if (isAlive())
        return;

    // ��ȡϵͳ�����н��̵Ŀ���
    HANDLE hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    ScopedGuard guard([&] { CloseHandle(hProcess); });
    if (hProcess == INVALID_HANDLE_VALUE)
        return;

    // ���������Ϊ "Hexy.exe" �Ľ��̣���ֵ�� targetPID_
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    if (!Process32First(hProcess, &entry))
        return;
    while (Process32Next(hProcess, &entry))
        if (!wcscmp(TARGET_IAMGE_NAME, entry.szExeFile))
            targetPID_ = entry.th32ProcessID;

    // ������ڱ�����Ϊ "Hexy" �Ķ��㴰�ڣ��򽫴�������ֵ�� targetWnd_
    EnumWindows(findTargetWindow, reinterpret_cast<LPARAM>(&targetWnd_));
}

/**
 * @brief ���ڸ�����ȡ�ڴ�ĺ�
 */
#define ARG(var) reinterpret_cast<void *>(&var), sizeof(var), var##offset

/**
 * @brief ��ȡ�ڴ�ƫ���е�����
 * 
 * @param hProc Hexy ���̶���ľ��
 * @param dest ��ȡ�ɹ�����λ��
 * @param cap ��ȡ���ֽ���
 * @param address ��ȡĿ����ڴ��ַ
 * @return bool �ɹ����� true �������׳��쳣
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
    return boardSize_;
}

int HexyHandleImpl::getPawnNum()
{
    updateData();
    return pawnNum_;
}

bool HexyHandleImpl::getGameOverFlag()
{
    updateData();
    return gameOver_ == 1;
}

bool HexyHandleImpl::setPiece(const std::tuple<int, int> &pos) {
    if (getGameOverFlag())
        throw "Game is over. Plz try again after start a new game :)";

    auto rec = getRec();

    int targetCol = get<0>(pos), targetRow = get<1>(pos);
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

    POINT pt;
    GetCursorPos(&pt);

    SendMessage(targetWnd_, WM_LBUTTONDOWN, 0, mousePos);
    SendMessage(targetWnd_, WM_LBUTTONUP, 0, mousePos);

    SetCursorPos(pt.x, pt.y);

    return true;
}

void HexyHandleImpl::msgOrigin(hexybuddy::Command cmd) {
    updateData();

    POINT pt;
    GetCursorPos(&pt);

    SendMessage(targetWnd_, WM_COMMAND, static_cast<unsigned>(cmd), 0);

    SetCursorPos(pt.x, pt.y);
}

const std::tuple<int, int> HexyHandleImpl::setPieceAndWait(const std::tuple<int, int> &pos) {
    using namespace hexybuddy;
    msgOrigin(Command::SwapOff);
    msgOrigin(Command::HexyOff);
    msgOrigin(Command::Score);

    auto pawnNum = getPawnNum();
    if (!setPiece(pos)) {
        throw "position illegal not empty!";
    }
    pawnNum++;
    msgOrigin(Command::Move);
    
    const auto timeLimit = chrono::seconds(8);
    auto timeCost = chrono::milliseconds(0);
    while (pawnNum == getPawnNum()) {
        auto timeSleepFor = chrono::milliseconds(200);
        this_thread::sleep_for(timeSleepFor);
        timeCost += timeSleepFor;
        if (timeCost > timeLimit)
            throw "timeout!";
    }

    auto rec = getRec();
    return rec[rec.size() - 1];
}
