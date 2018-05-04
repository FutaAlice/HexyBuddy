#pragma once
#include <string>
#include "../cpp-httplib/httplib.h"

namespace httpsrv {
namespace httputils {

std::string dumpHeaders(const httplib::Headers& headers);
std::string toLogStr(const httplib::Request& req, const httplib::Response& res);

void Logger(const httplib::Request& req, const httplib::Response& res);

} // namespace httputils
} // namespace httpsrv
