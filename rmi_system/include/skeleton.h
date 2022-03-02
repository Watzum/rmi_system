#pragma once

#include <asio.hpp>

class Skeleton {

  public: 
    
    Skeleton();
    explicit Skeleton(unsigned short port);

  private: 
    
    void printEndpoint();

    asio::ip::tcp::endpoint my_endpoint;
};