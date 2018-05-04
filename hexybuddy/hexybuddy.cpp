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

bool HexyHandle::setPiece(const std::tuple<int, int>& pos) {
    return pimpl_->setPiece(pos);
}

}
