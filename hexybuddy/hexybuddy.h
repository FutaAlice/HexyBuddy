#pragma once
#include <vector>
#include <tuple>
class HexyHandleImpl;
namespace hexybuddy {

class HexyHandle
{
public:
    HexyHandle();
    ~HexyHandle();
    bool init();
    std::vector<std::tuple<int, int>> getRec();
    bool setPiece(const std::tuple<int, int> &);
private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
