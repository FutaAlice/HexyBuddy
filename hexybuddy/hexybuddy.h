#pragma once
#include <vector>
#include <tuple>
#include "msgdef.h"
class HexyHandleImpl;
namespace hexybuddy {

class HexyHandle {
    using Points = std::vector<std::tuple<int, int>>;
public:
    HexyHandle();
    ~HexyHandle();
    
    /**
     * @brief ��ʼ����Ѱ���������е� Hexy ʵ��
     * 
     * @return true �ɹ�
     * @return false û���ҵ��������� Hexy ʵ��
     */
    bool init();

    /**
     * @brief ��ȡ Hexy ��ǰ�����ϵ��������Ӽ�¼��δ�ҵ� Hexy ʵ�����׳��쳣��
     * 
     * @return Points ���Ӽ�¼
     */
    Points getRec();

    /**
    * @brief ��ȡ���̳ߴ�
    *
    * @return int ���̳ߴ�
    */
    int getBoardsize();

    /**
    * @brief ��ȡ����������������
    *
    * @return int ������
    */
    int getPawnNum();

    /**
    * @brief ��ȡ����Ƿ����
    *
    * @return bool ���̽������� true ���򷵻� false
    */
    bool getGameOverFlag();

    /**
     * @brief �� Hexy ��ǰ���������ӣ�δ�ҵ� Hexy ʵ�����׳��쳣��
     * 
     * @param �� col �� row ������ɵ� tuple���±�� 0 ����
     * @return true �����ɹ�
     * @return false ʧ�ܣ�����Խ����Ѵ�������
     */
    bool setPiece(const std::tuple<int, int> &);

    /**
    * @brief �� Hexy ���������Ϳ�������
    *
    * @param cmd ������������μ� msgdef.h �ļ���
    */
    void msgOrigin(Command cmd);

    /**
    * @brief �������̲����¿�ʼ
    *
    */
    void msgNewGame();

    /**
    * @brief �˳� Hexy
    *
    */
    void msgExit();

    /**
    * @brief Hexy �Զ����ӿ��ش�
    *
    */
    void msgHexyOn();

    /**
    * @brief Hexy �Զ����ӿ��عر�
    *
    */
    void msgHexyOff();

    /**
    * @brief ���� swap ����
    *
    */
    void msgSwapOn();

    /**
    * @brief ͣ�� swap ����
    *
    */
    void msgSwapOff();

    /**
    * @brief �򵥵� AI
    *
    */
    void msgLevelBeginner();

    /**
    * @brief �еȵ� AI
    *
    */
    void msgLevelIntermediate();

    /**
    * @brief ���ѵ� AI
    *
    */
    void msgLevelAdvanced();

    /**
    * @brief ���˷���� AI
    *
    */
    void msgLevelExpert();

    /**
    * @brief �������̳ߴ�
    *
    * @param size ���̳ߴ磬[4, 11] �����ڵ�����
    */
    void msgSize(int size);

    /**
    * @brief ����������״��FLAT I��
    *
    */
    void msgShapeDefault();

    /**
    * @brief �л�������ʾ����
    *
    */
    void msgToggleCoord();

    /**
    * @brief �л����ӱ����ʾ����
    *
    */
    void msgToggleNumbering();

    /**
    * @brief �л��²���ʾ����
    *
    */
    void msgToggleGuess();

    /**
    * @brief �л�������Ч���ſ���
    *
    */
    void msgToggleSound();

    /**
    * @brief �л������������
    *
    */
    void msgToggleRandom();

    /**
    * @brief �����ʱ����
    *
    */
    void msgToggleClock();

    /**
    * @brief �� Hexy ������һ������
    *
    */
    void msgMove();

    /**
    * @brief �� Hexy ������һ�����Ӳ���ʾ
    *
    */
    void msgHint();

    /**
    * @brief �� Hexy �Լ������ȥ
    *
    */
    void msgDemo();

private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
