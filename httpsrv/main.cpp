#include <string>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <map>
#include <vector>
#include <tuple>

#include "../hexybuddy/hexybuddy.h"
#include "../cpp-httplib/httplib.h"
#include "../json11/json11.hpp"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace httplib;
using namespace json11;
using namespace hexybuddy;

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

struct Point
{
    tuple<int, int> pos;
    Point(tuple<int, int> pos) : pos(pos) {}
    Json to_json() const
    {
        return Json::object {
            {"col", get<0>(pos)}, {"row", get<1>(pos)}
        };
    }
};

int main()
{
    Server srv(HttpVersion::v1_1);

    if (!srv.is_valid())
    {
        cout << "server has an error..." << endl;
        exit(1);
    }

    HexyHandle handle;

    srv.get("/init", [&](const Request &req, Response &res) {
        bool success = handle.init();
        int errCode = success ? 200 : 400;
        string errString = success ? "" : "Fail, Hexy process not found.";
        Json obj = Json::object{
            { "errCode", errCode },
            { "errString", errString },
        };
        res.set_content(obj.dump(), "application/json");
    });

    srv.get("/rec", [&](const Request &req, Response &res) {
        int errCode = 200;
        string errString = "";
        vector<Point> points;
        do {
            if (!handle.init())
            {
                errCode = 400;
                errString = "Hexy process do not exist! Plz open Hexy before query.";
                break;
            }
            auto rec = handle.getRec();
            points = { rec.begin(), rec.end() };
        } while (0);
        Json obj = Json::object({
            { "errCode", errCode },
            { "errString", errString },
            { "record", points },
        });
        res.set_content(obj.dump(), "application/json");
    });
    
    srv.post("/set", [&](const Request &req, Response &res) {
        handle.setPiece(make_tuple(5, 5));
        cout << dump_headers(req.headers) << endl;
        cout << req.body << endl;
        res.set_content("ok", "text/plain");
    });
    srv.listen("localhost", 8080);
}