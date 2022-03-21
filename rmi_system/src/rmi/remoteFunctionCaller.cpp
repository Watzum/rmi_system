/*
author: Watz Emil
class:  5AHIF
file:   remoteFunctionCaller.cpp
*/

#include "remoteFunctionCaller.h"
#include "error_handler.h"
#include "functioncall.pb.h"

#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

using namespace asio;

void handleAnswer(ip::tcp::socket& sock, error_code& ec) {
    char reply[1];
    asio::read(sock, asio::buffer(reply, 1), ec);
    if (ec.value() != 0) return;
    if (strcmp(reply, "1") == 0) {
        spdlog::info("Funktion wurde ausgeführt!");
    } else if (strcmp(reply, "0") == 0) {
        spdlog::warn("Funktion konnte nicht gefunden werden!");
    } else {
        spdlog::error("Funktionsaufruf fehlgeschlagen!");
    }
}


void sendProtoBuffer(ip::tcp::socket& sock, std::string name, 
  error_code& ec) {
    FunctionCall* d = new FunctionCall;
    d->set_name(name);
    asio::streambuf buf;
    buf.prepare(4);
    std::ostream os(&buf);
    uint32_t protobufLength = d->ByteSizeLong();
    os << protobufLength;
    buf.commit(4 - buf.size());
    size_t serializeSuccessful = d->SerializeToOstream(&os);
    if (!serializeSuccessful)
        spdlog::error("Funktionsaufruf konnte nicht serialisiert werden!");
    asio::write(sock, buf.data(), ec);
    delete d;
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
    sendProtoBuffer(sock, name, ec);
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
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    setEndpoint("127.0.0.1", 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(std::string dest_ip_address) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    setEndpoint(dest_ip_address, 1113);
}


RemoteFunctionCaller::RemoteFunctionCaller(std::string dest_ip_address, 
  unsigned short port) {
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    setEndpoint(dest_ip_address, port);
}


RemoteFunctionCaller::~RemoteFunctionCaller() {
    google::protobuf::ShutdownProtobufLibrary();
}