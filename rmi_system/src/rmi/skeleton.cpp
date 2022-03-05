#include "skeleton.h"
#include "error_handler.h"

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


void sendMessage(ip::tcp::socket& sock, const std::string& msg, 
  error_code& ec) {
    write(sock, buffer(msg, msg.size()), ec);
}


void Skeleton::serveClient(ip::tcp::socket&& sock) {
    error_code ec;
    streambuf buf;
    read_until(sock, buf, '\n', ec);
    if (eclog::error("Nachricht konnte nicht gelesen werden", sock, ec)) 
        return; //TODO: Send error message to client!
    std::string msg;
    std::istream is{&buf};
    getline(is, msg);
    std::string answer = callFunction(msg) ? "called" : "nofunc";
    sendMessage(sock, answer, ec);
    if (eclog::error("Antwort konnte nicht gesendet werden", sock, ec))
        return;
    spd::info("Nachricht gesendet: " + msg);
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