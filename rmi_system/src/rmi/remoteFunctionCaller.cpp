/*
author: Watz Emil
class:  5AHIF
file:   remoteFunctionCaller.cpp
*/

#include "remoteFunctionCaller.h"
#include "error_handler.h"

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

using namespace std;
using namespace asio;

inline ip::tcp::socket setUpSocket(const ip::tcp::endpoint& ep, 
  error_code& ec) {
    using namespace asio::ip;
    io_context ctx;
    tcp::socket sock{ctx};
    sock.bind(ep, ec);
    return sock;
}


void RemoteFunctionCaller::sendFunctionCall(string name) {
    using namespace asio::ip;
    eclog::warn("Send function call cancelled because\
          endpoint is not set", endpoint_error);
    error_code ec;
    io_context ctx;
    tcp::socket sock{ctx};
    sock.open(server_endpoint.protocol(), ec);
    eclog::error("Socket konnte nicht geöffnet werden", sock, ec);
    sock.connect(server_endpoint, ec);
    eclog::error("Verbindung zu " + server_endpoint.address().to_string()\
          + " konnte nicht aufgebaut werden", sock, ec);
    std::string msg = name + '\n';
    asio::write(sock, asio::buffer(msg, msg.length()), ec);
    eclog::error(name + " konnte nicht gesendet werden", sock, ec);
    spdlog::info("Funktionsaufruf: " + name + " wurde gesendet!");
    sock.close();
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


RemoteFunctionCaller::RemoteFunctionCaller() {
    setEndpoint("127.0.0.1", 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address) {
    setEndpoint(dest_ip_address, 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(string dest_ip_address, 
  unsigned short port) {
    setEndpoint(dest_ip_address, port);
}