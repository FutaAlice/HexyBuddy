#pragma once
#include <vector>
#include <tuple>
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

    void postOriginMsg(unsigned);
private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
