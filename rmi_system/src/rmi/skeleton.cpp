#include "skeleton.h"

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

using namespace std;
using namespace asio;

void Skeleton::printEndpoint() {
    spdlog::info("Server Skeleton: IP-Address: " + 
                 my_endpoint.address().to_string() + 
                 " Port: " + to_string(my_endpoint.port()));
}

Skeleton::Skeleton() : my_endpoint{ip::tcp::v4(), 1114} {
    printEndpoint();
}


Skeleton::Skeleton(unsigned short port) : my_endpoint{ip::tcp::v4(), port} {
    printEndpoint();
}