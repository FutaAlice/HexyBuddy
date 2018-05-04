#pragma once
#include "../cpp-httplib/httplib.h"

namespace httpsrv {
namespace handle {

void HexyInit(const httplib::Request &req, httplib::Response &res);
void HexyRec (const httplib::Request &req, httplib::Response &res);
void HexySet (const httplib::Request &req, httplib::Response &res);
void HexyOrigin(const httplib::Request &req, httplib::Response &res);

void Error   (const httplib::Request &req, httplib::Response &res);

} // namespace httputils
} // namespace handle
