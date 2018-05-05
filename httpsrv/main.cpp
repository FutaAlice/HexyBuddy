#include <string>
#include <iostream>

#include "../cpp-httplib/httplib.h"
#include "../json11/json11.hpp"

#include "httputils.h"
#include "handle.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace httplib;
using namespace json11;
using namespace httpsrv;

int main()
{
    Server srv;
    if (!srv.is_valid()) {
        cout << "server has an error..." << endl;
        exit(1);
    }

    srv.Get ("/init",      handle::HexyInit);
    srv.Get ("/rec",       handle::HexyGetRec);
    srv.Get ("/gameover",  handle::HexyGetGameoverFlag);
    srv.Get ("/pawnnum",   handle::HexyGetPawnNum);
    srv.Get ("/boardsize", handle::HexyGetBoardSize);
    srv.Get ("/set",       handle::HexySetPiece);
    srv.Get ("/origin",    handle::HexyOriginMsg);
    srv.Post("/set",       handle::HexySetPiece);
    srv.Post("/origin",    handle::HexyOriginMsg);

    srv.set_error_handler(handle::Error);

    srv.set_logger(httputils::Logger);

    cout << "start listening localhost:8080..." << endl;
    srv.listen("localhost", 8080);
}