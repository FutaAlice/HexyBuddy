#pragma once

class HexyHandleImpl;
namespace hexybuddy {

class HexyHandle
{
public:
    HexyHandle();
    ~HexyHandle();
    bool isAlive();
    void findHexy();
    void updateData();
private:
    HexyHandleImpl *pimpl_ { nullptr };
};

}
