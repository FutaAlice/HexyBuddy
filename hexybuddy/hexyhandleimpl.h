#pragma once
#include <vector>
#include <tuple>
#include <Windows.h>
#include <TlHelp32.h>

#define TARGET_IAMGE_NAME       L"Hexy.exe"
#define TARGET_WINDOW_CAPTION   L"Hexy"

enum {
    pawnNum_offset              = 0x01B0C5D0,
    boardSize_offset            = 0x0042303C,
    boardRatio_offset           = 0x01B0BFC8,
    gameOver_offset             = 0x01B0CCE0,
    canvasCenterX_offset        = 0x01b0c328,
    canvasCenterY_offset        = 0x01b0c32c,
    globalPosList_offset        = 0x01B073A0,
    hexygonCenterPairList_offset = 0x01B075C0,
};

class HexyHandleImpl {
    using Points = std::vector<std::tuple<int, int>>;
public:
    /**
     * @brief 寻找正在运行的 Hexy 程序
     * 
     * @return 成功返回 true ，否则 false
     */
    bool init();

    /**
     * @brief 同步 Hexy 的状态
     * 
     */
    void updateData();

    Points getRec();
    int getSize();
    bool setPiece(const std::tuple<int, int> &);
private:
    /**
     * @brief 判断 Hexy 是否仍在运行
     * 
     * @return true "targetWnd_" 和 "targetPID_" 已被赋值，且句柄对应的窗口仍然存在
     */
    bool isAlive();

    /**
     * @brief 寻找正在运行的 Hexy 程序，给 "targetWnd_" 和 "targetPID_" 赋值
     * 
     */
    void findHexy();
private:
    HWND targetWnd_ = 0;    // "Hexy" 的窗体句柄
    DWORD targetPID_ = 0;   // "Hexy" 的进程 ID

    int boardSize_;         // 棋盘尺寸
    int globalPosList_[128];
    int pawnNum_;           // 棋子数
    int gameOver_;
    int canvasCenterX_;
    int canvasCenterY_;
    double boardRatio_;
    double hexygonCenterPairList_[13 * 13 * 2];
};