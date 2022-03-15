#pragma once

#include "abstractClass.h"

#include <asio.hpp>

class Skeleton {

  public: 
    Skeleton(AbstractClass*);
    explicit Skeleton(AbstractClass*, unsigned short port);
    void listenToFunctionCalls();

  private: 
    
    void serveClient(asio::ip::tcp::socket&& sock);
    bool callFunction(std::string functionName);
    void printEndpoint();
    AbstractClass* rmi_object;
    asio::ip::tcp::endpoint my_endpoint;
};