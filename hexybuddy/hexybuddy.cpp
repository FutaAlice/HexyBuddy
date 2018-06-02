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

int HexyHandle::getBoardsize() {
    return pimpl_->getBoardsize();
}

int HexyHandle::getPawnNum() {
    return pimpl_->getPawnNum();
}

bool HexyHandle::getGameOverFlag() {
    return pimpl_->getGameOverFlag();
}

bool HexyHandle::setPiece(const std::tuple<int, int>& pos) {
    return pimpl_->setPiece(pos);
}

void HexyHandle::msgOrigin(Command cmd) {
    pimpl_->msgOrigin(cmd);
}

const std::tuple<int, int>
HexyHandle::setPieceAndWait(const std::tuple<int, int>&pos) {
    return pimpl_->setPieceAndWait(pos);
}

void HexyHandle::msgNewGame() {
    pimpl_->msgOrigin(Command::NewGame);
}

void HexyHandle::msgExit() {
    pimpl_->msgOrigin(Command::Exit);
}

void HexyHandle::msgHexyOn() {
    pimpl_->msgOrigin(Command::HexyOn);
}

void HexyHandle::msgHexyOff() {
    pimpl_->msgOrigin(Command::HexyOff);
}

void HexyHandle::msgSwapOn() {
    pimpl_->msgOrigin(Command::SwapOn);
}

void HexyHandle::msgSwapOff() {
    pimpl_->msgOrigin(Command::SwapOff);
}

void HexyHandle::msgLevelBeginner() {
    pimpl_->msgOrigin(Command::LevelBeginner);
}

void HexyHandle::msgLevelIntermediate() {
    pimpl_->msgOrigin(Command::LevelIntermediate);
}

void HexyHandle::msgLevelAdvanced() {
    pimpl_->msgOrigin(Command::LevelAdvanced);
}

void HexyHandle::msgLevelExpert() {
    pimpl_->msgOrigin(Command::LevelExpert);
}

void HexyHandle::msgSize(int size) {
    if (size < 4 || size > 11) {
        throw "Illegal size! Expect: 4 <= size <= 11.";
    }
    auto size_0 = static_cast<unsigned>(Command::Size_4) - 4;
    pimpl_->msgOrigin(static_cast<Command>(size_0 + size));
}

void HexyHandle::msgShapeDefault() {
    pimpl_->msgOrigin(Command::ShapeFlat_1);
}

void HexyHandle::msgToggleCoord() {
    pimpl_->msgOrigin(Command::Coord);
}

void HexyHandle::msgToggleNumbering() {
    pimpl_->msgOrigin(Command::Numbering);
}

void HexyHandle::msgToggleGuess() {
    pimpl_->msgOrigin(Command::Guess);
}

void HexyHandle::msgToggleSound() {
    pimpl_->msgOrigin(Command::Sound);
}

void HexyHandle::msgToggleRandom() {
    pimpl_->msgOrigin(Command::Random);
}

void HexyHandle::msgToggleClock() {
    pimpl_->msgOrigin(Command::Clock);
}

void HexyHandle::msgMove() {
    pimpl_->msgOrigin(Command::Move);
}

void HexyHandle::msgHint() {
    pimpl_->msgOrigin(Command::Hint);
}

void HexyHandle::msgDemo() {
    pimpl_->msgOrigin(Command::Demo);
}

}
