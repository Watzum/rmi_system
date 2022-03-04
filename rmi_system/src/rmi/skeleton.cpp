#include "skeleton.h"
#include "error_handler.h"

#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <iostream>
#include <string>

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


std::string readFromSocket(ip::tcp:: socket& sock, error_code& ec) {
    streambuf buf;
    read_until(sock, buf, '\n', ec);
    if (ec.value() != 0) return "";
    std::string msg;
    std::istream is{&buf};
    getline(is, msg);
    spd::info("Nachricht gesendet: " + msg);
    return msg;
}


void sendMessage(ip::tcp::socket& sock, const std::string& msg, 
  error_code& ec) {
    write(sock, buffer(msg, msg.size()), ec);
    eclog::error("Antwort konnte nicht gesendet werden", sock, ec);
    spd::info("Nachricht gesendet: " + msg);
}


void Skeleton::listenToFunctionCalls() {
    using namespace asio::ip;
    error_code ec; 
    io_context ctx;
    tcp::acceptor acceptor{setUpAcceptor(ctx, my_endpoint, ec)};
    eclog::error("Acceptor konnte nicht erstellt werden", acceptor, ec);
    acceptor.listen();
    while (true) {
        try {
            tcp::socket sock{ctx};
            acceptor.accept(sock, ec);
            eclog::error("Verbindung konnte nicht akzeptiert werden", 
                acceptor, sock, ec);
            readFromSocket(sock, ec);
            eclog::error("Nachricht konnte nicht gelesen werden", sock, ec);
            sock.close();
        } catch (system_error& e) {}
    }
}


void Skeleton::printEndpoint() {
    spdlog::info("Server Skeleton: IP-Address: " + 
                 my_endpoint.address().to_string() + 
                 " Port: " + std::to_string(my_endpoint.port()));
}


Skeleton::Skeleton() : my_endpoint{ip::tcp::v4(), 1113} {
    printEndpoint();
}


Skeleton::Skeleton(unsigned short port) : my_endpoint{ip::tcp::v4(), port} {
    printEndpoint();
}