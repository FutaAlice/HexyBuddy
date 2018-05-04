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
     * @brief 初始化并寻找正在运行的 Hexy 实例
     * 
     * @return true 成功
     * @return false 没有找到正在运行 Hexy 实例
     */
    bool init();

    /**
     * @brief 获取 Hexy 当前棋盘上的所有落子记录（未找到 Hexy 实例将抛出异常）
     * 
     * @return Points 落子记录
     */
    Points getRec();

    /**
     * @brief 在 Hexy 当前棋盘上落子（未找到 Hexy 实例将抛出异常）
     * 
     * @param 由 col 和 row 坐标组成的 tuple，下标从 0 计算
     * @return true 操作成功
     * @return false 失败，坐标越界或已存在棋子
     */
    bool setPiece(const std::tuple<int, int> &);

private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
