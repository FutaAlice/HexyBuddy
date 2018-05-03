#include <iostream>

#include "../hexybuddy/hexybuddy.h"
#include "../cpp-httplib/httplib.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main()
{
    auto pHandle = new hexybuddy::HexyHandle;
    pHandle->init();
    auto rec = pHandle->getRec();
    for (auto x : rec)
    {
        cout << "col " << get<0>(x) << ", ";
        cout << "row " << get<1>(x) << endl;
    }
    pHandle->setPiece(make_tuple(5, 5));
}