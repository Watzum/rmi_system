#pragma once

#include <asio.hpp>
#include <string>

class RemoteFunctionCaller {

  public: 

    RemoteFunctionCaller();
    explicit RemoteFunctionCaller(std::string dest_ip_address);
    RemoteFunctionCaller(std::string dest_ip_address, unsigned short port);
    ~RemoteFunctionCaller();
    void sendFunctionCall(std::string name);
    
  private: 

    void setEndpoint(std::string ip_address, unsigned short port);
    void printEndpoint();

    asio::ip::tcp::endpoint server_endpoint;
    asio::error_code endpoint_error;
};