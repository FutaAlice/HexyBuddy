#pragma once
#include <vector>
#include <tuple>
#include <Windows.h>
#include <TlHelp32.h>

#define TARGET_IAMGE_NAME       L"Hexy.exe"
#define TARGET_WINDOW_CAPTION   L"Hexy"

enum {
    pawnNum_offset               = 0x01B0C5D0,
    boardSize_offset             = 0x0042303C,
    boardRatio_offset            = 0x01B0BFC8,
    gameOver_offset              = 0x01B0CCE0,
    canvasCenterX_offset         = 0x01b0c328,
    canvasCenterY_offset         = 0x01b0c32c,
    globalPosList_offset         = 0x01B073A0,
    hexygonCenterPairList_offset = 0x01B075C0,
};

namespace hexybuddy {
enum class Command : unsigned;
}

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
     * @brief ͬ�� Hexy ��״̬��HexyBuddy δ�ɹ���ʼ���� Hexy ���̲����ڽ��׳��쳣��
     */
    void updateData();

    Points getRec();        // ��ȡ���Ӽ�¼
    int getBoardsize();     // ��ȡ���̳ߴ�
    int getPawnNum();       // ��ȡ������
    bool getGameOverFlag(); // �ж�����Ƿ����

    bool setPiece(const std::tuple<int, int> &); // ���ӣ�������� tuple<col, row>, ���� true ��ʾ�ɹ�
    void msgOrigin(hexybuddy::Command cmd);      // �� Hexy ���������Ϳ�������

    const std::tuple<int, int> setPieceAndWait(const std::tuple<int, int> &);
private:
    /**
     * @brief �ж� Hexy �Ƿ���������
     * 
     * @return true "targetWnd_" �� "targetPID_" �ѱ���ֵ���Ҿ����Ӧ�Ĵ�����Ȼ����
     */
    bool isAlive();

    /**
     * @brief Ѱ���������е� Hexy ���򣬸� "targetWnd_" �� "targetPID_" ��ֵ
     */
    void findHexy();
private:
    HWND  targetWnd_ = 0;    // "Hexy" �Ĵ�����
    DWORD targetPID_ = 0;    // "Hexy" �Ľ��� ID

    int boardSize_;          // ���̳ߴ�
    int globalPosList_[128]; // ���Ӽ�¼����ʽΪ val = (row+1)*(size+2)+(col+1)���±�� 0 ����
    int pawnNum_;            // ������
    int gameOver_;           // ����Ƿ��Ѿ�����
    int canvasCenterX_;      // �������� X ����
    int canvasCenterY_;      // �������� Y ����
    double boardRatio_;      // ���ű���
    double hexygonCenterPairList_[13 * 13 * 2]; // �����������������꣨ż��λ X ���꣬����λ Y ���꣩
};