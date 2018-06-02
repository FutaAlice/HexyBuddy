#pragma once
#include "../cpp-httplib/httplib.h"

namespace httpsrv {
namespace handle {

void HexyInit               (const httplib::Request &req, httplib::Response &res);

void HexyGetRec             (const httplib::Request &req, httplib::Response &res);
void HexyGetGameoverFlag    (const httplib::Request &req, httplib::Response &res);
void HexyGetPawnNum         (const httplib::Request &req, httplib::Response &res);
void HexyGetBoardSize       (const httplib::Request &req, httplib::Response &res);

void HexySetPiece           (const httplib::Request &req, httplib::Response &res);
void HexySetPieceAndWait    (const httplib::Request &req, httplib::Response &res);

void HexyMsgOrigin          (const httplib::Request &req, httplib::Response &res);
void HexyMsgNewGame         (const httplib::Request &req, httplib::Response &res);
void HexyMsgExit            (const httplib::Request &req, httplib::Response &res);
void HexyMsgHexyOn          (const httplib::Request &req, httplib::Response &res);
void HexyMsgHexyOff         (const httplib::Request &req, httplib::Response &res);
void HexyMsgSwapOn          (const httplib::Request &req, httplib::Response &res);
void HexyMsgSwapOff         (const httplib::Request &req, httplib::Response &res);
void HexyMsgLevelBeginner   (const httplib::Request &req, httplib::Response &res);
void HexyMsgLevelIntermediate(const httplib::Request &req, httplib::Response &res);
void HexyMsgLevelAdvanced   (const httplib::Request &req, httplib::Response &res);
void HexyMsgLevelExpert     (const httplib::Request &req, httplib::Response &res);
void HexyMsgSize            (const httplib::Request &req, httplib::Response &res);
void HexyMsgShapeDefault    (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleCoord     (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleNumbering (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleGuess     (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleSound     (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleRandom    (const httplib::Request &req, httplib::Response &res);
void HexyMsgToggleClock     (const httplib::Request &req, httplib::Response &res);
void HexyMsgMove            (const httplib::Request &req, httplib::Response &res);
void HexyMsgHint            (const httplib::Request &req, httplib::Response &res);
void HexyMsgDemo            (const httplib::Request &req, httplib::Response &res);

void Error                  (const httplib::Request &req, httplib::Response &res);

} // namespace httputils
} // namespace handle
