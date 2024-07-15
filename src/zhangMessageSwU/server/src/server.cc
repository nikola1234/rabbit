#include <iostream>
#include <functional>
#include <unistd.h>

#include "base.pb.h"
#include "zcommon.h"

int main()
{
    O_O::RpcMessageHandler handler;
    std::function<int(std::string, int, std::vector<int>, std::vector<std::string>, bool)> callbackAAA = [](std::string a, int b, std::vector<int> c, std::vector<std::string> d, bool e)
    {
        LOG_INFO << "First: " << a;
        LOG_INFO << "Second: " << b;
        LOG_INFO << "Third: " << c << " - " <<d <<" - "<< e;
        return 123;
    };
    handler.registerCallback("AAA", callbackAAA);
    std::function<std::vector<std::string>(void)> callbackBBB = []()
    {
        LOG_INFO <<"BBB Called";
        return std::vector<std::string>{"AA","BB"};
    };
    handler.registerCallback("BBB", callbackBBB);

    O_O::TcpServer server(8080, std::bind(&O_O::RpcMessageHandler::handleSocketData, &handler, std::placeholders::_1, std::placeholders::_2), std::bind(&O_O::RpcMessageHandler::handleDisconnect, &handler, std::placeholders::_1));
    server.run();

    return 0;
}