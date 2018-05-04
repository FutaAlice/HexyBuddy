#include <string>
#include <sstream>
#include <tuple>
#include <vector>

#include "../json11/json11.hpp"
#include "../hexybuddy/hexybuddy.h"

#include "httputils.h"

#include "handle.h"

using namespace std;
using namespace httplib;
using namespace json11;

namespace httpsrv {
namespace handle {

inline namespace inner {

hexybuddy::HexyHandle handle;

struct Point {
    tuple<int, int> pos;
    Point(tuple<int, int> pos) : pos(pos) {
    }
    Json to_json() const {
        return Json::object {
            { "col", get<0>(pos) },{ "row", get<1>(pos) }
        };
    }
};

Json makeResp(int errCode, string errString) {
    return Json::object {
        { "errCode", errCode },
        { "errString", errString },
    };
}

}
using namespace inner;

void HexyInit(const Request &req, Response &res) {
    bool success = handle.init();
    int errCode = success ? 200 : 400;
    string errString = success ? "" : "Fail, Hexy process not found.";

    Json obj = makeResp(errCode, errString);
    res.set_content(obj.dump(), "application/json");
}

void HexyRec(const Request &req, Response &res)
{
    int errCode = 200;
    string errString = "";
    vector<Point> points;
    do {
        if (!handle.init()) {
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
}

void HexySet(const Request &req, Response &res)
{
    int errCode = 200;
    string errString = "";
    do {
        if (!handle.init()) {
            errCode = 400;
            errString = "Hexy process do not exist! Plz open Hexy before query.";
            break;
        }
        int row, col;
        if ("POST" == req.method) {
            Json obj = Json::parse(req.body, errString);
            if (!obj.is_object()) {
                errCode = 400;
                break;
            }
            if (!obj["row"].is_number() || !obj["col"].is_number()) {
                errCode = 400;
                errString = R"(Param row/col is required.)";
                break;
            }
            row = obj["row"].int_value();
            col = obj["col"].int_value();
        }
        // GET
        else {
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
        if (!handle.setPiece(make_tuple(col, row))) {
            errCode = 400;
            stringstream ss;
            ss << "Position:(col " << col << ", row " << row << ") illegal not empty!";
            errString = ss.str();
            break;
        }
    } while (0);

    Json obj = makeResp(errCode, errString);
    res.set_content(obj.dump(), "application/json");
}

void Error(const Request& req, Response& res)
{
    Json obj = makeResp(res.status, "");
    res.set_content(obj.dump(), "application/json");
}

} // namespace httputils
} // namespace handle


