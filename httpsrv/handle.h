#pragma once
#include "../cpp-httplib/httplib.h"

namespace httpsrv {
namespace handle {

void HexyInit           (const httplib::Request &req, httplib::Response &res);

void HexyGetRec         (const httplib::Request &req, httplib::Response &res);
void HexyGetGameoverFlag(const httplib::Request &req, httplib::Response &res);
void HexyGetPawnNum     (const httplib::Request &req, httplib::Response &res);
void HexyGetBoardSize   (const httplib::Request &req, httplib::Response &res);

void HexySetPiece       (const httplib::Request &req, httplib::Response &res);
void HexyOriginMsg      (const httplib::Request &req, httplib::Response &res);

void Error   (const httplib::Request &req, httplib::Response &res);

} // namespace httputils
} // namespace handle
