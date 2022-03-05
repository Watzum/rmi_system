#pragma once

#include "abstractClass.h"

#include <asio.hpp>

class Skeleton {

  public: 
    Skeleton(AbstractClass*);
    explicit Skeleton(AbstractClass*, unsigned short port);
    void listenToFunctionCalls();

  private: 
    
    void printEndpoint();
    AbstractClass* rmi_object;
    asio::ip::tcp::endpoint my_endpoint;
};