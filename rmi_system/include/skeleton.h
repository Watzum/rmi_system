#pragma once

#include <asio.hpp>

class Skeleton {

  public: 
    
    Skeleton();
    explicit Skeleton(unsigned short port);
    void receiveFunctionCall();

  private: 
    
    void printEndpoint();

    asio::ip::tcp::endpoint my_endpoint;
};