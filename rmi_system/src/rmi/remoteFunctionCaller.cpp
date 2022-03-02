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

void RemoteFunctionCaller::sendFunctionCall(string name) {
    if (endpoint_error.value() != 0) {
        spdlog::warn("Send function call cancelled because endpoint is not set");
        return;
    }
    ip::tcp::iostream stream{server_endpoint};
    if (stream) {
        stream << name << endl;
        string data;
        getline(stream, data);
        cout << data << endl;
        stream.close();
    } else {
        spdlog::error("Connection to " + server_endpoint.address().to_string()
          + " could not be established! Error message " + stream.error().message());
    }
}


void RemoteFunctionCaller::printEndpoint() {
    spdlog::info("Client-Stub: Server-IP-Address: " + 
                 server_endpoint.address().to_string() + 
                 " Port: " + to_string(server_endpoint.port()));
}


void RemoteFunctionCaller::setEndpoint(string ip_address, 
  unsigned short port) {
    server_endpoint.address(ip::make_address(ip_address, endpoint_error));
    if (endpoint_error.value() != 0) {
        spdlog::error("IP-Address '" + ip_address + 
          "' is malformed! Error message: " + endpoint_error.message());
        return; 
    } 
    server_endpoint.port(port);
    printEndpoint();
}


RemoteFunctionCaller::RemoteFunctionCaller() :
  server_endpoint{ip::make_address("127.0.0.1"), 1113} {
    setEndpoint("127.0.0.1", 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address) {
    setEndpoint(dest_ip_address, 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address, 
  unsigned short port) {
    setEndpoint(dest_ip_address, port);
}