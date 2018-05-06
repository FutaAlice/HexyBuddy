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

/**
* @brief 向 Json 对象中添加新的 field
*/
#define JsonAddItem(obj, k, v) do {  \
    auto items = obj.object_items(); \
    items[k] = Json(v);              \
    obj = Json(items);               \
} while (0)

/**
* @brief 向 Json 对象写入 Response 返回
*/
#define JsonSetResp(obj, rep) do {                   \
    if ("POST" == req.method)                        \
        res.set_header("Cache-Control", "no-cache"); \
    res.set_content(obj.dump(), "application/json"); \
} while (0)

/**
* @brief try 执行指定语句。成功，返回 200 OK；失败，返回异常内容。
*/
#define TryAndReturn(func) do {     \
    ErrorStatus err;                \
    try {                           \
        func();                     \
    }                               \
    catch (const char *errString) { \
        err = { 400, errString };   \
    }                               \
    Json obj = Json(err);           \
    JsonSetResp(obj, res);          \
} while (0)

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
    ErrorStatus(int errCode = 200, string errString = "OK")
        : code(errCode), str(errString) {
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
    JsonSetResp(obj, res);
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
void HexyGetRec(const Request &req, Response &res) {
    ErrorStatus err;
    vector<Point> points;
    try {
        auto rec = handle.getRec();
        points = { rec.begin(), rec.end() };
    }
    catch (const char *errString) {
        err = { 400, errString };
    }

    Json obj = Json(err);
    JsonAddItem(obj, "records", points);
    JsonSetResp(obj, res);
}

/**
* @brief 获取 Hexy 棋局是否结束
*  回复 JSON 格式如下：
*  {
*      "errCode": 200,
*      "errString": "OK",
*      "gameover": true/false
*  }
*/
void HexyGetGameoverFlag(const Request &req, Response &res) {
    ErrorStatus err;
    bool gameover = false;
    try {
        gameover = handle.getGameOverFlag();
    }
    catch (const char *errString) {
        err = { 400, errString };
    }

    Json obj = Json(err);
    JsonAddItem(obj, "gameover", gameover);
    JsonSetResp(obj, res);
}

/**
 * @brief 获取 Hexy 棋盘上的棋子数
 *  回复 JSON 格式如下：
 *  {
 *      "errCode": 200,
 *      "errString": "OK",
 *      "num": 21
 *  }
*/
void HexyGetPawnNum(const Request &req, Response &res) {
    ErrorStatus err;
    int num = 0;
    try {
        num = handle.getPawnNum();
    }
    catch (const char *errString) {
        err = { 400, errString };
    }

    Json obj = Json(err);
    JsonAddItem(obj, "num", num);
    JsonSetResp(obj, res);
}

/**
 * @brief 获取 Hexy 棋盘尺寸
 *  回复 JSON 格式如下：
 *  {
 *      "errCode": 200,
 *      "errString": "OK",
 *      "boardsize": 6
 *  }
*/
void HexyGetBoardSize(const Request &req, Response &res) {
    ErrorStatus err;
    int boardsize = 0;
    try {
        boardsize = handle.getBoardsize();
    }
    catch (const char *errString) {
        err = { 400, errString };
    }

    Json obj = Json(err);
    JsonAddItem(obj, "boardsize", boardsize);
    JsonSetResp(obj, res);
}

/**
 * @brief 控制 Hexy 落子
 * 接受 GET/POST 请求
 * 落子成功返回 200 OK ，否则返回具体错误原因
 */
void HexySetPiece(const Request &req, Response &res) {
    ErrorStatus err;
    do {
        int row, col;
        if ("POST" == req.method) {
            Json obj = Json::parse(req.body, err.str);
            if (!obj.is_object()) {
                err.code = 400;
                break;
            }
            if (!obj["row"].is_number() || !obj["col"].is_number()) {
                err = { 400, R"(Param: row/col is required.)" };
                break;
            }
            row = obj["row"].int_value();
            col = obj["col"].int_value();
        }
        // GET
        else {
            if (req.params.end() == req.params.find("row") ||
                req.params.end() == req.params.find("col")) {
                err = { 400, R"(Param: row/col is required.)" };
                break;
            }
            row = atoi(req.params.find("row")->second.c_str());
            col = atoi(req.params.find("col")->second.c_str());
        }

        try {
            bool success = handle.setPiece(make_tuple(col, row));
            if (!success) {
                stringstream ss;
                ss << "Position:(col " << col << ", row " << row << ") illegal not empty!";
                err = { 400, ss.str() };
            }
        }
        catch (const char *errString) {
            err = { 400, errString };
        }

    } while (0);

    Json obj = Json(err);
    JsonSetResp(obj, res);
}

void HexyMsgOrigin(const Request &req, Response &res) {
    ErrorStatus err;
    do {
        unsigned int msg;
        if ("POST" == req.method) {
            Json obj = Json::parse(req.body, err.str);
            if (!obj.is_object()) {
                err.code = 400;
                break;
            }
            if (!obj["msg"].is_number()) {
                err = { 400, R"(Param: "msg" is required.)" };
                break;
            }
            msg = obj["msg"].int_value();
        }
        // GET
        else {
            if (req.params.end() == req.params.find("msg")) {
                err = { 400, R"(Param: "msg" is required.)" };
                break;
            }
            msg = atoi(req.params.find("msg")->second.c_str());
        }

        try {
            handle.msgOrigin(static_cast<hexybuddy::Command>(msg));
        }
        catch (const char *errString) {
            err = { 400, errString };
        }
    } while (0);

    Json obj = Json(err);
    JsonSetResp(obj, res);
}

void HexyMsgNewGame(const Request &req, Response &res) {
    TryAndReturn(handle.msgNewGame);
}

void HexyMsgExit(const Request &req, Response &res) {
    TryAndReturn(handle.msgExit);
}

void HexyMsgHexyOn(const Request &req, Response &res) {
    TryAndReturn(handle.msgHexyOn);
}

void HexyMsgHexyOff(const Request &req, Response &res) {
    TryAndReturn(handle.msgHexyOff);
}

void HexyMsgSwapOn(const Request &req, Response &res) {
    TryAndReturn(handle.msgSwapOn);
}

void HexyMsgSwapOff(const Request &req, Response &res) {
    TryAndReturn(handle.msgSwapOff);
}

void HexyMsgLevelBeginner(const Request &req, Response &res) {
    TryAndReturn(handle.msgLevelBeginner);
}

void HexyMsgLevelIntermediate(const Request &req, Response &res) {
    TryAndReturn(handle.msgLevelIntermediate);
}

void HexyMsgLevelAdvanced(const Request &req, Response &res) {
    TryAndReturn(handle.msgLevelAdvanced);
}

void HexyMsgLevelExpert(const Request &req, Response &res) {
    TryAndReturn(handle.msgLevelExpert);
}

void HexyMsgSize(const Request &req, Response &res) {
    ErrorStatus err;
    do {
        int size;
        if ("POST" == req.method) {
            Json obj = Json::parse(req.body, err.str);
            if (!obj.is_object()) {
                err.code = 400;
                break;
            }
            if (!obj["size"].is_number()) {
                err = { 400, R"(Param: "size" is required.)" };
                break;
            }
            size = obj["size"].int_value();
        }
        // GET
        else {
            if (req.params.end() == req.params.find("size")) {
                err = { 400, R"(Param: "size" is required.)" };
                break;
            }
            size = atoi(req.params.find("size")->second.c_str());
        }

        try {
            handle.msgSize(size);
        }
        catch (const char *errString) {
            err = { 400, errString };
        }
    } while (0);

    Json obj = Json(err);
    JsonSetResp(obj, res);
}

void HexyMsgShapeDefault(const Request &req, Response &res) {
    TryAndReturn(handle.msgShapeDefault);
}

void HexyMsgToggleCoord(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleCoord);
}

void HexyMsgToggleNumbering(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleNumbering);
}

void HexyMsgToggleGuess(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleGuess);
}

void HexyMsgToggleSound(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleSound);
}

void HexyMsgToggleRandom(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleRandom);
}

void HexyMsgToggleClock(const Request &req, Response &res) {
    TryAndReturn(handle.msgToggleClock);
}

void HexyMsgMove(const Request &req, Response &res) {
    TryAndReturn(handle.msgMove);
}

void HexyMsgHint(const Request &req, Response &res) {
    TryAndReturn(handle.msgHint);
}

void HexyMsgDemo(const Request &req, Response &res) {
    TryAndReturn(handle.msgDemo);
}

/**
 * @brief 消息路由无法匹配等情况的错误处理
 */
void Error(const Request& req, Response& res) {
    Json obj = Json(ErrorStatus(res.status, ""));
    JsonSetResp(obj, res);
}

} // namespace httputils
} // namespace handle
