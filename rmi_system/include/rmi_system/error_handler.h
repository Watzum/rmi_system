#include <asio.hpp>
#include <spdlog/spdlog.h>

#include <string>

namespace eclog {
    inline bool error(const std::string& s, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            return true;
        }
        return false;
    }


    inline bool error(const std::string& s, asio::ip::tcp::socket& sock, 
    asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            sock.close();
            return true;
        }
        return false;
    }


    inline bool error(const std::string& s, asio::ip::tcp::acceptor& acc, 
    asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            acc.close();
            return true;
        }
        return false;
    }


    inline bool error(const std::string& s, asio::ip::tcp::acceptor& acc, 
    asio::ip::tcp::socket& sock, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::error(s + ": " +  ec.message());
            acc.close();
            sock.close();
            return true;
        }
        return false;
    }


    inline bool warn(const std::string& s, asio::error_code& ec)  {
        if (ec.value() != 0) {
            spdlog::warn(s + ": " +  ec.message());
            return true;
        }
        return false;
    }
}