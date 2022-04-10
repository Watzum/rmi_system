#pragma once

#include "abstractClass.h"
#include "functioncall.pb.h"
#include "statistics_manager_impl.h"

#include <json.hpp>
#include <asio.hpp>

class Skeleton {

  public: 
    Skeleton(AbstractClass*);
    Skeleton(AbstractClass*, unsigned short port);
    void listenToFunctionCalls();
    void setPort();

  private: 
    void startStatisticsManager();
    void serveClient(asio::ip::tcp::socket&& sock);
    void answerClient(asio::ip::tcp::socket& sock, FunctionCall* f, 
      asio::error_code& ec);
    std::string callFunction(std::string functionName, nlohmann::json par);
    void printEndpoint();
    AbstractClass* rmi_object;
    asio::ip::tcp::endpoint my_endpoint;
    StatisticsManagerImpl service;
};

//Für den Aufruf in abstractMethods.cpp
#define __ARGUMENT__(type, place) par[#place].get<type>()
#define __FUNCTION__(name, ...)  if (functionName == #name)\
                                j["returnValue"] = rmi_object->name(__VA_ARGS__);
#define __VOID_FUNCTION__(name, ...) if (functionName == #name)\
    rmi_object->name(__VA_ARGS__);
    //if (std::is_void<decltype(rmi_object->name(5, __VA_ARGS__))>())
    //TODO: sonst Fehlerbehandlung, weil keine void function
#define __END__ return j.dump();