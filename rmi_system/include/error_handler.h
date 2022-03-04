#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <string>

namespace eclog {
    inline void error(const std::string& s, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            throw asio::system_error(ec);
        }
    }


    inline void error(const std::string& s, asio::ip::tcp::socket& sock, 
    asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            sock.close();
            throw asio::system_error(ec);
        }
    }


    inline void error(const std::string& s, asio::ip::tcp::acceptor& acc, 
    asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            acc.close();
            throw asio::system_error(ec);
        }
    }


    inline void error(const std::string& s, asio::ip::tcp::acceptor& acc, 
    asio::ip::tcp::socket& sock, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            acc.close();
            sock.close();
            throw asio::system_error(ec);
        }
    }


    inline void warn(const std::string& s, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::warn(s + ": " +  ec.message());
            throw asio::system_error(ec);
        }
    }
}