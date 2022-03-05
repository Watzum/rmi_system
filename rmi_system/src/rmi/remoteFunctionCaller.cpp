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

using namespace asio;

void handleAnswer(ip::tcp::socket& sock, error_code& ec) {
    char reply[6];
    size_t reply_length = asio::read(sock, asio::buffer(reply, 6), ec);
    if (ec.value() != 0) return;
    std::cout << "reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << '\n';
}


void RemoteFunctionCaller::sendFunctionCall(std::string name) {
    using namespace asio::ip;
    eclog::warn("Send function call cancelled because\
          endpoint is not set", endpoint_error);
    error_code ec;
    io_context ctx;
    tcp::socket sock{ctx};
    sock.open(server_endpoint.protocol(), ec);
    if (eclog::error("Socket konnte nicht geöffnet werden", sock, ec)) return;
    sock.connect(server_endpoint, ec);
    if (eclog::error("Verbindung zu " + server_endpoint.address().to_string()\
          + " konnte nicht aufgebaut werden", sock, ec)) return;
    std::string msg = name + '\n';
    asio::write(sock, asio::buffer(msg, msg.length()), ec);
    if (eclog::error(name + " konnte nicht gesendet werden", sock, ec)) return;
    spdlog::info("Funktionsaufruf: " + name + " wurde gesendet!");
    handleAnswer(sock, ec);
    if (eclog::error("Antwort konnte nicht gelesen werden", sock, ec)) return;
    sock.close(); 
}


void RemoteFunctionCaller::printEndpoint() {
    spdlog::info("Client-Stub: Server-IP-Address: " + 
                 server_endpoint.address().to_string() + 
                 " Port: " + std::to_string(server_endpoint.port()));
}


void RemoteFunctionCaller::setEndpoint(std::string ip_address, 
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


RemoteFunctionCaller::RemoteFunctionCaller(std::string dest_ip_address) {
    setEndpoint(dest_ip_address, 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(std::string dest_ip_address, 
  unsigned short port) {
    setEndpoint(dest_ip_address, port);
}