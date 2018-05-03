#pragma once
#include <vector>
#include <tuple>
#include <Windows.h>
#include <TlHelp32.h>

#define TARGET_IAMGE_NAME       L"Hexy.exe"
#define TARGET_WINDOW_CAPTION   L"Hexy"

enum
{
    pawnNum_offset              = 0x01B0C5D0,
    boardSize_offset            = 0x0042303C,
    boardRatio_offset           = 0x01B0BFC8,
    gameOver_offset             = 0x01B0CCE0,
    canvasCenterX_offset        = 0x01b0c328,
    canvasCenterY_offset        = 0x01b0c32c,
    globalPosList_offset        = 0x01B073A0,
    hexygonCenterPairList_offset = 0x01B075C0,
};

class HexyHandleImpl
{
public:
    bool init();
    void updateData();
    std::vector<std::tuple<int, int>> getRec();
    bool setPiece(const std::tuple<int, int> &);
private:
    bool isAlive();
    void findHexy();
private:
    HWND targetWnd_ = 0;
    DWORD targetPID_ = 0;

    int boardSize_;
    int globalPosList_[128];
    int pawnNum_;
    int gameOver_;
    int canvasCenterX_;
    int canvasCenterY_;
    double boardRatio_;
    double hexygonCenterPairList_[13 * 13 * 2];
};