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

static std::string dump_headers(const Headers& headers)
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

static string log(const Request& req, const Response& res)
{
    string s = "================================\n";
    char buf[BUFSIZ];

    snprintf(buf, sizeof(buf), "%s %s %s",
             req.method.c_str(), req.path.c_str(), req.version.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
        const auto& x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
            (it == req.params.begin()) ? '?' : '&', x.first.c_str(), x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s\n", query.c_str());
    s += buf;
    s += dump_headers(req.headers);
    s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
    s += buf;
    s += dump_headers(res.headers);
    s += "\n";

    if (!res.body.empty())
        s += res.body;
    
    s += "\n";
    return s;
}

struct Point
{
    tuple<int, int> pos;
    Point(tuple<int, int> pos) : pos(pos) {}
    Json to_json() const
    {
        return Json::object {
            { "col", get<0>(pos) }, { "row", get<1>(pos) }
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
    
    Server::Handler setReqHandler = [&](const Request &req, Response &res) {
        int errCode = 200;
        string errString = "";
        do {
            if (!handle.init())
            {
                errCode = 400;
                errString = "Hexy process do not exist! Plz open Hexy before query.";
                break;
            }
            int row, col;
            if ("POST" == req.method)
            {
                Json obj = Json::parse(req.body, errString);
                if (!obj.is_object())
                {
                    errCode = 400;
                    break;
                }
                if (!obj["row"].is_number() || !obj["col"].is_number())
                {
                    errCode = 400;
                    errString = R"(Param row/col is required.)";
                    break;
                }
                row = obj["row"].int_value();
                col = obj["col"].int_value();
            }
            else // GET
            {
                if (req.params.end() == req.params.find("row") ||
                    req.params.end() == req.params.find("col"))
                {
                    errCode = 400;
                    errString = R"(Param row/col is required.)";
                    break;
                }
                row = atoi(req.params.find("row")->second.c_str());
                col = atoi(req.params.find("col")->second.c_str());
            }
            if (!handle.setPiece(make_tuple(col, row)))
            {
                errCode = 400;
                stringstream ss;
                ss << "Position:(col " << col << ", row " << row << ") illegal not empty!";
                errString = ss.str();
                break;
            }
        } while (0);
        Json obj = Json::object({
            { "errCode", errCode },
            { "errString", errString },
        });
        res.set_content(obj.dump(), "application/json");
    };
    srv.post("/set", setReqHandler);
    srv.get("/set", setReqHandler);

    srv.set_error_handler([](const auto& /*req*/, auto& res) {
        const char* fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        char buf[BUFSIZ];
        snprintf(buf, sizeof(buf), fmt, res.status);
        res.set_content(buf, "text/html");
    });

    srv.set_logger([](const auto& req, const auto& res) {
        printf("%s", log(req, res).c_str());
    });

    srv.listen("localhost", 8080);
}