#include <string>
#include <iostream>

#include "../cpp-httplib/httplib.h"
#include "../json11/json11.hpp"

#include "httputils.h"
#include "handle.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace httplib;
using namespace httpsrv;

static void setMessageRouter(Server &srv);

int main()
{
    // 初始化 HTTP 服务实例
    Server server;
    if (!server.is_valid()) {
        cout << "server has an error..." << endl;
        exit(1);
    }

    // 设置消息路由器
    setMessageRouter(server);

    // 设置日志打印函数
    server.set_logger(httputils::Logger);

    // 开启本地服务，监听 8080 端口
    const int port = 8080;
    cout << "start listening localhost:" << port << "..." << endl;
    server.listen("localhost", port);
}

void setMessageRouter(Server &srv) {
    using namespace handle;

    srv.Get ("/init", HexyInit);

    srv.Get ("/get/rec",          HexyGetRec);
    srv.Get ("/get/pawnnum",      HexyGetPawnNum);
    srv.Get ("/get/boardsize",    HexyGetBoardSize);
    srv.Get ("/get/gameoverflag", HexyGetGameoverFlag);

    srv.Get ("/set/piece", HexySetPiece);
    srv.Post("/set/piece", HexySetPiece);
    srv.Get ("/set/wait",  HexySetPieceAndWait);
    srv.Post("/set/wait",  HexySetPieceAndWait);

    srv.Get ("/msg/origin",         HexyMsgOrigin);
    srv.Post("/msg/origin",         HexyMsgOrigin);
    srv.Get ("/msg/newgame",        HexyMsgNewGame);
    srv.Get ("/msg/exit",           HexyMsgExit);
    srv.Get ("/msg/hexy/on",        HexyMsgHexyOn);
    srv.Get ("/msg/hexy/off",       HexyMsgHexyOff);
    srv.Get ("/msg/swap/on",        HexyMsgSwapOn);
    srv.Get ("/msg/swap/off",       HexyMsgSwapOff);
    srv.Get ("/msg/level/beginner", HexyMsgLevelBeginner);
    srv.Get ("/msg/level/intermediate", HexyMsgLevelIntermediate);
    srv.Get ("/msg/level/advanced", HexyMsgLevelAdvanced);
    srv.Get ("/msg/level/expert",   HexyMsgLevelExpert);
    srv.Get ("/msg/size",           HexyMsgSize);
    srv.Post("/msg/size",           HexyMsgSize);
    srv.Get ("/msg/shape/default",  HexyMsgShapeDefault);
    srv.Get ("/msg/toggle/coord",   HexyMsgToggleCoord);
    srv.Get ("/msg/toggle/numbering", HexyMsgToggleNumbering);
    srv.Get ("/msg/toggle/guess",   HexyMsgToggleGuess);
    srv.Get ("/msg/toggle/sound",   HexyMsgToggleSound);
    srv.Get ("/msg/toggle/random",  HexyMsgToggleRandom);
    srv.Get ("/msg/toggle/clock",   HexyMsgToggleClock);
    srv.Get ("/msg/move",           HexyMsgMove);
    srv.Get ("/msg/hint",           HexyMsgHint);
    srv.Get ("/msg/demo",           HexyMsgDemo);

    srv.set_error_handler(Error);
}
