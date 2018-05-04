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
     * @brief Ѱ���������е� Hexy ����
     * 
     * @return �ɹ����� true ������ false
     */
    bool init();

    /**
     * @brief ͬ�� Hexy ��״̬
     * 
     */
    void updateData();

    Points getRec();
    int getSize();
    bool setPiece(const std::tuple<int, int> &);
private:
    /**
     * @brief �ж� Hexy �Ƿ���������
     * 
     * @return true "targetWnd_" �� "targetPID_" �ѱ���ֵ���Ҿ����Ӧ�Ĵ�����Ȼ����
     */
    bool isAlive();

    /**
     * @brief Ѱ���������е� Hexy ���򣬸� "targetWnd_" �� "targetPID_" ��ֵ
     * 
     */
    void findHexy();
private:
    HWND targetWnd_ = 0;    // "Hexy" �Ĵ�����
    DWORD targetPID_ = 0;   // "Hexy" �Ľ��� ID

    int boardSize_;         // ���̳ߴ�
    int globalPosList_[128];
    int pawnNum_;           // ������
    int gameOver_;
    int canvasCenterX_;
    int canvasCenterY_;
    double boardRatio_;
    double hexygonCenterPairList_[13 * 13 * 2];
};