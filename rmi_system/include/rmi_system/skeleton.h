#pragma once

#include "abstractClass.h"
#include "functioncall.pb.h"

#include <asio.hpp>

class Skeleton {

  public: 
    Skeleton(AbstractClass*);
    explicit Skeleton(AbstractClass*, unsigned short port);
    void listenToFunctionCalls();

  private: 
    
    void serveClient(asio::ip::tcp::socket&& sock);
    void answerClient(asio::ip::tcp::socket& sock, FunctionCall* f, 
      asio::error_code& ec);
    std::string callFunction(std::string functionName);
    void printEndpoint();
    AbstractClass* rmi_object;
    asio::ip::tcp::endpoint my_endpoint;
};

//Für den Aufruf in abstractMethods.cpp
#define __FUNCTION__(name)  if (functionName == #name)\
                                j["returnValue"] = rmi_object->name();
#define __VOID_FUNCTION__(name) if (functionName == #name)\
    if (std::is_void<decltype(rmi_object->name(5))>())\
        rmi_object->name(5);
    //TODO: sonst Fehlerbehandlung, weil keine void function
#define __END__ return j.dump();