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
    * @brief 读取棋盘尺寸
    *
    * @return int 棋盘尺寸
    */
    int getBoardsize();

    /**
    * @brief 读取棋盘上现有棋子数
    *
    * @return int 棋子数
    */
    int getPawnNum();

    /**
    * @brief 读取棋局是否结束
    *
    * @return bool 棋盘结束返回 true 否则返回 false
    */
    bool getGameOverFlag();

    /**
     * @brief 在 Hexy 当前棋盘上落子（未找到 Hexy 实例将抛出异常）
     * 
     * @param 由 col 和 row 坐标组成的 tuple，下标从 0 计算
     * @return true 操作成功
     * @return false 失败，坐标越界或已存在棋子
     */
    bool setPiece(const std::tuple<int, int> &);

    /**
    * @brief 向 Hexy 窗体句柄发送控制命令
    *
    * @param cmd 控制命令（命令定义参见 msgdef.h 文件）
    */
    void msgOrigin(Command cmd);

    /**
    * @brief 清理棋盘并重新开始
    *
    */
    void msgNewGame();

    /**
    * @brief 退出 Hexy
    *
    */
    void msgExit();

    /**
    * @brief Hexy 自动落子开关打开
    *
    */
    void msgHexyOn();

    /**
    * @brief Hexy 自动落子开关关闭
    *
    */
    void msgHexyOff();

    /**
    * @brief 启用 swap 规则
    *
    */
    void msgSwapOn();

    /**
    * @brief 停用 swap 规则
    *
    */
    void msgSwapOff();

    /**
    * @brief 简单的 AI
    *
    */
    void msgLevelBeginner();

    /**
    * @brief 中等的 AI
    *
    */
    void msgLevelIntermediate();

    /**
    * @brief 困难的 AI
    *
    */
    void msgLevelAdvanced();

    /**
    * @brief 令人发狂的 AI
    *
    */
    void msgLevelExpert();

    /**
    * @brief 设置棋盘尺寸
    *
    * @param size 棋盘尺寸，[4, 11] 区间内的整数
    */
    void msgSize(int size);

    /**
    * @brief 重置棋盘形状（FLAT I）
    *
    */
    void msgShapeDefault();

    /**
    * @brief 切换坐标显示开关
    *
    */
    void msgToggleCoord();

    /**
    * @brief 切换棋子编号显示开关
    *
    */
    void msgToggleNumbering();

    /**
    * @brief 切换猜测显示开关
    *
    */
    void msgToggleGuess();

    /**
    * @brief 切换落子音效播放开关
    *
    */
    void msgToggleSound();

    /**
    * @brief 切换随机搜索开关
    *
    */
    void msgToggleRandom();

    /**
    * @brief 清理计时开关
    *
    */
    void msgToggleClock();

    /**
    * @brief 由 Hexy 进行下一步落子
    *
    */
    void msgMove();

    /**
    * @brief 由 Hexy 搜索下一步落子并提示
    *
    */
    void msgHint();

    /**
    * @brief 让 Hexy 自己玩儿蛋去
    *
    */
    void msgDemo();

private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
