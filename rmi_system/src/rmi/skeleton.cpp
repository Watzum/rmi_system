#include "skeleton.h"
#include "error_handler.h"
#include "functioncall.pb.h"
#include "returnvalue.pb.h"

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>
#include <thread>

using namespace asio;
namespace spd = spdlog;

inline ip::tcp::acceptor setUpAcceptor(io_context& ctx,
  const ip::tcp::endpoint& ep, error_code& ec) {
    using namespace asio::ip;
    tcp::acceptor acc{ctx};
    acc.open(ep.protocol(), ec);
    acc.bind(ep, ec);
    return acc;
}

void sendProtoBuffer(ip::tcp::socket& sock, 
  ReturnValue* r, error_code& ec) {
    asio::streambuf buf;
    buf.prepare(4);
    std::ostream os(&buf);
    uint32_t protobufLength = r->ByteSizeLong();
    os << protobufLength;
    buf.commit(4 - buf.size());
    size_t serializeSuccessful = r->SerializeToOstream(&os);
    if (!serializeSuccessful)
        spdlog::error("Funktionsaufruf konnte nicht serialisiert werden!");
    asio::write(sock, buf.data(), ec);
}


void Skeleton::answerClient(ip::tcp::socket& sock, FunctionCall* d, 
  asio::error_code& ec) {
    ReturnValue* returnValue = new ReturnValue;
    nlohmann::json j = nlohmann::json::parse(d->json_arguments());
    //TODO: JSON Fehlerbehandlung!! -> z.B. type_error
    returnValue->set_json_value(callFunction(d->name(), j));
    returnValue->set_success(true); //TODO: Fehlerbehandlung
    spdlog::info("Sende Rückgabewert: " + returnValue->json_value());
    sendProtoBuffer(sock, returnValue, ec);
    delete returnValue;
}


void Skeleton::serveClient(ip::tcp::socket&& sock) {
    error_code ec;
    streambuf buf;
    auto bytes_transferred = read(sock, buf.prepare(4), ec);
    buf.commit(bytes_transferred);
    if (eclog::error("Nachricht konnte nicht gelesen werden", sock, ec)) 
        return;
    std::istream is{&buf};
    uint32_t protobufLength;
    is >> protobufLength;
    buf.consume(buf.size());
    bytes_transferred = asio::read(sock, buf.prepare(protobufLength), ec);
    if (eclog::error("Nachricht konnte nicht gelesen werden", sock, ec)) 
        return;
    buf.commit(bytes_transferred);
    FunctionCall* functionCall = new FunctionCall;
    if (!functionCall->ParseFromIstream(&is)) {
        delete functionCall;
        spdlog::error("Funktionsaufruf konnte nicht deserialisiert werden!");
        return;
    }
    answerClient(sock, functionCall, ec);
    if (eclog::error("Antwort konnte nicht gesendet werden", sock, ec)) {
        delete functionCall;
        return;
    }
    spd::info("Antwort gesendet!");
    sock.close();
}


void Skeleton::listenToFunctionCalls() {
    using namespace asio::ip;
    error_code ec; 
    io_context ctx;
    tcp::acceptor acceptor{setUpAcceptor(ctx, my_endpoint, ec)};
    if (eclog::error("Acceptor konnte nicht erstellt werden", acceptor, ec))
        return;
    acceptor.listen();
    while (true) {
        tcp::socket sock{ctx};
        acceptor.accept(sock, ec);
        if (eclog::error("Verbindung konnte nicht akzeptiert werden", 
            acceptor, sock, ec)) return;
        std::thread t{&Skeleton::serveClient, this, std::move(sock)};
        t.detach();
    }
}


void Skeleton::printEndpoint() {
    spdlog::info("Server Skeleton: IP-Address: " + 
                 my_endpoint.address().to_string() + 
                 " Port: " + std::to_string(my_endpoint.port()));
}


Skeleton::Skeleton(AbstractClass* a) 
  : rmi_object{a}, my_endpoint{ip::tcp::v4(), 1113} {
    printEndpoint();
}


Skeleton::Skeleton(AbstractClass* a, unsigned short port) 
  : rmi_object{a}, my_endpoint{ip::tcp::v4(), port} {
    printEndpoint();
}