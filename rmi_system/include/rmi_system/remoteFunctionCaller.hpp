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
    ReturnValue* handleFunctionCall(std::string name, asio::error_code& ec);
    
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


ReturnValue* handleAnswer(asio::ip::tcp::socket& sock, asio::error_code& ec) {
    asio::streambuf buf;
    auto bytes_transferred = read(sock, buf.prepare(4), ec);
    buf.commit(bytes_transferred);
    if (ec.value() != 0) return nullptr;
    std::istream is{&buf};
    uint32_t protobufLength;
    is >> protobufLength;
    buf.consume(buf.size());
    bytes_transferred = asio::read(sock, buf.prepare(protobufLength), ec);
    if (ec.value() != 0) return nullptr;
    buf.commit(bytes_transferred);
    ReturnValue* returnValue = new ReturnValue;
    if (!returnValue->ParseFromIstream(&is)) {
        delete returnValue;
        spdlog::error("Rückgabewert konnte nicht deserialisiert werden!");
        return nullptr;
    }
    return returnValue;
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


ReturnValue* RemoteFunctionCaller::handleFunctionCall(std::string name, 
  asio::error_code& ec) {
    using namespace asio::ip;
    eclog::warn("Send function call cancelled because\
          endpoint is not set", endpoint_error);
    asio::io_context ctx;
    tcp::socket sock{ctx};
    sock.open(server_endpoint.protocol(), ec);
    if (eclog::error("Socket konnte nicht geöffnet werden", sock, ec))
        return nullptr;
    sock.connect(server_endpoint, ec);
    if (eclog::error("Verbindung zu " + server_endpoint.address().to_string()\
          + " konnte nicht aufgebaut werden", sock, ec)) return nullptr;
    sendProtoBuffer(sock, name, ec);
    if (eclog::error(name + " konnte nicht gesendet werden", sock, ec)) 
        return nullptr;
    spdlog::info("Funktionsaufruf: " + name + " wurde gesendet!");
    ReturnValue* returnValue = handleAnswer(sock, ec);
    if (eclog::error("Antwort konnte nicht gelesen werden", sock, ec)) {
        delete returnValue;
        return nullptr;
    }
    sock.close(); 
    return returnValue;
}


//Überprüft, ob der Aufruf durchgeführt werden konnte
//Wenn nicht wird eine entsprechende Exception geworfen
inline void checkSuccessOfFunctionCall(const ReturnValue* r, 
  const asio::error_code& ec) {
    if (ec.value() != 0) {
        throw rmi_error(ec.message());
    } 
    if (r == nullptr) {
        throw rmi_error("Funktionsaufruf konnte nicht deserialisiert werden!");
    }
    if (r->success() == false) {
        throw rmi_error("Entfernte Funktion konnte nicht aufgerufen werden");
    }
} 


template<typename T>
T RemoteFunctionCaller::returnFunctionCall(std::string name) {
    asio::error_code ec;
    ReturnValue* returnValue = handleFunctionCall(name, ec);
    checkSuccessOfFunctionCall(returnValue, ec);
    spdlog::info("Entfernte Funktion konnte aufgerufen werden");
    spdlog::info("Rückgabewert empfangen: " + returnValue->json_value());
    using json = nlohmann::json;
    json j = json::parse(returnValue->json_value());
    return j["returnValue"].get<T>();
}


void RemoteFunctionCaller::sendFunctionCall(const std::string name) {
    asio::error_code ec;
    ReturnValue* returnValue = handleFunctionCall(name, ec);
    checkSuccessOfFunctionCall(returnValue, ec);
    spdlog::info("Entfernte Funktion konnte aufgerufen werden");
}