#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>

#include "../hexybuddy/hexybuddy.h"
#include "../cpp-httplib/httplib.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace httplib;

std::string dump_headers(const Headers& headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (const auto &x : headers)
    {
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

int main()
{
    Server srv(HttpVersion::v1_1);

    if (!srv.is_valid())
    {
        cout << "server has an error..." << endl;
        exit(1);
    }

    auto pHandle = new hexybuddy::HexyHandle;

    srv.get("/init", [&](const Request &req, Response &res) {
        bool success = pHandle->init();
        string s = success ? "success" : "fail";
        res.set_content(s, "text/plain");
    });

    srv.get("/rec", [&](const Request &req, Response &res) {
        stringstream ss;
        auto rec = pHandle->getRec();
        for (auto x : rec)
        {
            ss << "col " << get<0>(x) << ", ";
            ss << "row " << get<1>(x) << endl;
        }
        res.set_content(ss.str(), "text/plain");
    });
    
    srv.post("/set", [&](const Request &req, Response &res) {
        pHandle->setPiece(make_tuple(5, 5));
        cout << dump_headers(req.headers) << endl;
        cout << req.body << endl;
        res.set_content("ok", "text/plain");
    });
    srv.listen("localhost", 8080);
}