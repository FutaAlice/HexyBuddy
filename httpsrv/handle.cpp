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

// HexyBuddy ʵ��
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
 * @brief �ж� HexyBuddy ����Ƿ��ʼ���ɹ�
 * ����ʼ���ɹ����� Hexy �����ں�̨���̣������� 200 OK
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
 * @brief ��ȡ Hexy �����ϵ��������Ӽ�¼
 *  �ظ� JSON ��ʽ���£�
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
    try {
        auto rec = handle.getRec();
        points = { rec.begin(), rec.end() };
    }
    catch (const char *errString) {
        err = { 400, errString };
    }

    Json obj = Json(err);
    auto items = obj.object_items();
    items["records"] = Json(points);
    obj = Json(items);

    res.set_content(obj.dump(), "application/json");
}

/**
 * @brief ���� Hexy ����
 * ���� GET/POST ����
 * ���ӳɹ����� 200 OK �����򷵻ؾ������ԭ��
 */
void HexySet(const Request &req, Response &res)
{
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
    res.set_content(obj.dump(), "application/json");
}

void HexyOrigin(const httplib::Request & req, httplib::Response & res)
{
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
                err = { 400, R"(Param row/col is required.)" };
                break;
            }
            msg = obj["msg"].int_value();
        }
        // GET
        else {
            if (req.params.end() == req.params.find("msg"))
            {
                err = { 400, R"(Param row/col is required.)" };
                break;
            }
            msg = atoi(req.params.find("msg")->second.c_str());
        }

        try {
            handle.postOriginMsg(msg);
        }
        catch (const char *errString) {
            err = { 400, errString };
        }
    } while (0);
    Json obj = Json(err);
    res.set_content(obj.dump(), "application/json");
}

/**
 * @brief ��Ϣ·���޷�ƥ�������Ĵ�����
 */
void Error(const Request& req, Response& res)
{
    Json obj = Json(ErrorStatus(res.status, ""));
    res.set_content(obj.dump(), "application/json");
}

} // namespace httputils
} // namespace handle
