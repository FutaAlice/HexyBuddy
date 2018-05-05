#include "hexyhandleimpl.h"
#include "hexybuddy.h"

namespace hexybuddy {

HexyHandle::HexyHandle() : pimpl_(new HexyHandleImpl) {
}

HexyHandle::~HexyHandle() {
    delete pimpl_;
}

bool HexyHandle::init() {
    return pimpl_->init();
}

HexyHandle::Points HexyHandle::getRec() {
    return pimpl_->getRec();
}

int HexyHandle::getBoardsize()
{
    return pimpl_->getBoardsize();
}

int HexyHandle::getPawnNum()
{
    return pimpl_->getPawnNum();
}

bool HexyHandle::getGameOverFlag()
{
    return pimpl_->getGameOverFlag();
}

bool HexyHandle::setPiece(const std::tuple<int, int>& pos) {
    return pimpl_->setPiece(pos);
}

void HexyHandle::postOriginMsg(unsigned msg) {
    pimpl_->postOriginMsg(msg);
}

}
