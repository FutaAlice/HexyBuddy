#include "hexyhandleimpl.h"
#include "hexybuddy.h"

namespace hexybuddy {

HexyHandle::HexyHandle()
    : pimpl_(new HexyHandleImpl)
{
}

HexyHandle::~HexyHandle()
{
    delete pimpl_;
}

bool HexyHandle::isAlive()
{
    return pimpl_->isAlive();
}

void HexyHandle::findHexy()
{
    return pimpl_->findHexy();
}

void HexyHandle::updateData()
{
    return pimpl_->updateData();
}

}
