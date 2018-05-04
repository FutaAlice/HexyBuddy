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

namespace {

// HexyBuddy 实例
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

struct ErrorStatus {
    ErrorStatus(int errCode = 200, string errString = "OK") :
        code(errCode), str(errString) {
    }
    Json to_json() const {
        return Json::object{
            { "errCode", code }, { "errString", str }
        };
    }
    int    code;
    string str;
};

} // namespace

/**
 * @brief 判断 HexyBuddy 句柄是否初始化成功
 * 若初始化成功（即 Hexy 存在于后台进程），返回 200 OK
 */
void HexyInit(const Request &req, Response &res) {
    ErrorStatus err;
    if (!handle.init()) {
        err = { 400, "Fail, Hexy process not found." };
    }

    Json obj = Json(err);
    res.set_content(obj.dump(), "application/json");
}

/**
 * @brief 获取 Hexy 棋盘上的所有落子记录
 *  回复 JSON 格式如下：
 *  {
 *      "errCode": 200,
 *      "errString": "OK",
 *      "records": [
 *          {"col": 5, "row": 5},
 *          {"col": 6, "row": 6}
 *      ]
 *  }
 */
void HexyRec(const Request &req, Response &res)
{
    ErrorStatus err;
    vector<Point> points;
    do {
        if (!handle.init()) {
            err = { 400, "Hexy process do not exist! Plz open Hexy before query." };
            break;
        }
        auto rec = handle.getRec();
        points = { rec.begin(), rec.end() };
    } while (0);

    Json obj = Json(err);
    auto items = obj.object_items();
    items["records"] = Json(points);
    obj = Json(items);

    res.set_content(obj.dump(), "application/json");
}

/**
 * @brief 控制 Hexy 落子
 * 接受 GET/POST 请求
 * 若成功向 Hexy 程序窗体句柄 SendMessage 则视为成功（无法指定位置在判断落子前是否已存在棋子）
 */
void HexySet(const Request &req, Response &res)
{
    ErrorStatus err;
    do {
        if (!handle.init()) {
            err = { 400, "Hexy process do not exist! Plz open Hexy before query." };
            break;
        }
        int row, col;
        if ("POST" == req.method) {
            Json obj = Json::parse(req.body, err.str);
            if (!obj.is_object()) {
                err.code = 400;
                break;
            }
            if (!obj["row"].is_number() || !obj["col"].is_number()) {
                err = { 400, R"(Param row/col is required.)" };
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
                err = { 400, R"(Param row/col is required.)" };
                break;
            }
            row = atoi(req.params.find("row")->second.c_str());
            col = atoi(req.params.find("col")->second.c_str());
        }
        if (!handle.setPiece(make_tuple(col, row))) {
            stringstream ss;
            ss << "Position:(col " << col << ", row " << row << ") illegal not empty!";
            err = { 400, ss.str() };
            break;
        }
    } while (0);

    Json obj = Json(err);
    res.set_content(obj.dump(), "application/json");
}

/**
 * @brief 消息路由无法匹配等情况的错误处理
 */
void Error(const Request& req, Response& res)
{
    Json obj = Json({ res.status, "" });
    res.set_content(obj.dump(), "application/json");
}

} // namespace httputils
} // namespace handle
