/*
author: Watz Emil
class:  5AHIF
file:   remoteFunctionCaller.cpp
*/

#include "remoteFunctionCaller.h"

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

using namespace std;
using namespace asio;

void RemoteFunctionCaller::printEndpoint() {
    spdlog::info("IP-Address: " + server_endpoint.address().to_string() + 
                 " Port: " + to_string(server_endpoint.port()));
}


RemoteFunctionCaller::RemoteFunctionCaller() :
  server_endpoint{ip::make_address("127.0.0.1"), 1113} {
    printEndpoint();
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address) :
  server_endpoint{ip::make_address(dest_ip_address), 1113} {
    printEndpoint();
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address, 
                                           unsigned short port) :
        server_endpoint{ip::make_address(dest_ip_address), port} {
    printEndpoint();
}