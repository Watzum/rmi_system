#pragma once
#include "error_handler.h"
#include "rmi_exception.h"
#include "functioncall.pb.h"
#include "returnvalue.pb.h"

#include <spdlog/spdlog.h>
#include <json.hpp>
#include <asio.hpp>

#include <iostream>
#include <string>

class RemoteFunctionCaller {

  public: 

    RemoteFunctionCaller();
    explicit RemoteFunctionCaller(std::string dest_ip_address);
    RemoteFunctionCaller(std::string dest_ip_address, unsigned short port);
    ~RemoteFunctionCaller();
    template<typename T>
    T returnFunctionCall(std::string name);
    void sendFunctionCall(std::string name);
    std::string handleFunctionCall(std::string name, asio::error_code& ec);
    
  private: 

    void setEndpoint(std::string ip_address, unsigned short port);
    void printEndpoint();

    asio::ip::tcp::endpoint server_endpoint;
    asio::error_code endpoint_error;
};

void RemoteFunctionCaller::printEndpoint() {
    spdlog::info("Client-Stub: Server-IP-Address: " + 
                 server_endpoint.address().to_string() + 
                 " Port: " + std::to_string(server_endpoint.port()));
}


void RemoteFunctionCaller::setEndpoint(std::string ip_address, 
  unsigned short port) {
    server_endpoint.address(asio::ip::make_address(ip_address, endpoint_error));
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


RemoteFunctionCaller::~RemoteFunctionCaller() {
    google::protobuf::ShutdownProtobufLibrary();
}


std::string handleAnswer(asio::ip::tcp::socket& sock, asio::error_code& ec) {
    asio::streambuf buf;
    auto bytes_transferred = read(sock, buf.prepare(4), ec);
    buf.commit(bytes_transferred);
    if (ec.value() != 0) return "";
    std::istream is{&buf};
    uint32_t protobufLength;
    is >> protobufLength;
    buf.consume(buf.size());
    bytes_transferred = asio::read(sock, buf.prepare(protobufLength), ec);
    if (ec.value() != 0) return "";
    buf.commit(bytes_transferred);
    ReturnValue* returnValue = new ReturnValue;
    if (!returnValue->ParseFromIstream(&is)) {
        delete returnValue;
        spdlog::error("Rückgabewert konnte nicht deserialisiert werden!");
        return "";
    }
    std::string j = returnValue->json_value();
    spdlog::info("Rückgabewert empfangen: " + returnValue->json_value());
    delete returnValue;
    return j;
}


void sendProtoBuffer(asio::ip::tcp::socket& sock, std::string name, 
  asio::error_code& ec) {
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


template<typename T>
T RemoteFunctionCaller::returnFunctionCall(std::string name) {
    asio::error_code ec;
    std::string s{handleFunctionCall(name, ec)};
    if (ec.value() != 0) {
        throw rmi_error(ec.message());
    }
    using json = nlohmann::json;
    json j = json::parse(s);
    return j["returnValue"].get<T>();
}


void RemoteFunctionCaller::sendFunctionCall(const std::string name) {
    asio::error_code ec;
    handleFunctionCall(name, ec);
    if (ec.value() != 0) {
        throw rmi_error(ec.message());
    }
}


std::string RemoteFunctionCaller::handleFunctionCall(std::string name, 
  asio::error_code& ec) {
    using namespace asio::ip;
    eclog::warn("Send function call cancelled because\
          endpoint is not set", endpoint_error);
    asio::io_context ctx;
    tcp::socket sock{ctx};
    sock.open(server_endpoint.protocol(), ec);
    if (eclog::error("Socket konnte nicht geöffnet werden", sock, ec))
        return "";
    sock.connect(server_endpoint, ec);
    if (eclog::error("Verbindung zu " + server_endpoint.address().to_string()\
          + " konnte nicht aufgebaut werden", sock, ec)) return "";
    sendProtoBuffer(sock, name, ec);
    if (eclog::error(name + " konnte nicht gesendet werden", sock, ec)) 
        return "";
    spdlog::info("Funktionsaufruf: " + name + " wurde gesendet!");
    std::string jsonString = handleAnswer(sock, ec);
    if (eclog::error("Antwort konnte nicht gelesen werden", sock, ec))
        return "";
    sock.close(); 
    return jsonString;
}