#include <cstdio>
#include <iostream>
#include "httputils.h"

using namespace std;
using namespace httplib;

namespace httpsrv {
namespace httputils {

string dumpHeaders(const Headers& headers) {
    string s;
    char buf[BUFSIZ];
    for (const auto &x : headers) {
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }
    return s;
}

string toLogStr(const Request& req, const Response& res) {
    string s = "================================\n";
    char buf[BUFSIZ];

    snprintf(buf, sizeof(buf), "%s %s %s",
             req.method.c_str(), req.path.c_str(), req.version.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it) {
        const auto& x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
            (it == req.params.begin()) ? '?' : '&', x.first.c_str(), x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;
    s += dumpHeaders(req.headers);
    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dumpHeaders(res.headers);
    s += "\n";

    if (!res.body.empty())
        s += res.body;

    s += "\n";
    return s;
}

void Logger(const httplib::Request& req, const httplib::Response& res) {
    cout << toLogStr(req, res);
}

} // namespace httputils
} // namespace httpsrv
