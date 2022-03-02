#include "skeleton.h"

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

using namespace std;
using namespace asio;

void Skeleton::receiveFunctionCall() {
    error_code ec;
    asio::io_context ctx;
    ip::tcp::acceptor acceptor{ctx, my_endpoint};
    ip::tcp::socket socket{ctx};
    acceptor.listen();
    while (true) {
       acceptor.accept(socket);
       ip::tcp::iostream stream{move(socket)};
       if (stream) {
           string data;
           stream >> data;
           stream << "ok.";
       } else {
           spdlog::error("Error while opening connection! Error message: " + stream.error().message());
       }
       stream.close();
    }
}


void Skeleton::printEndpoint() {
    spdlog::info("Server Skeleton: IP-Address: " + 
                 my_endpoint.address().to_string() + 
                 " Port: " + to_string(my_endpoint.port()));
}


Skeleton::Skeleton() : my_endpoint{ip::tcp::v4(), 1113} {
    printEndpoint();
}


Skeleton::Skeleton(unsigned short port) : my_endpoint{ip::tcp::v4(), port} {
    printEndpoint();
}