#include "statistics_manager_client.h"
#include "build/client.p/statistics_server.grpc.pb.h"
#include <spdlog/spdlog.h>

#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>


using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;


int StatisticsManagerClient::GetStatistics(const std::string& user) {
  spdlog::info("grpc: Die Funktion GetStatistics wird aufgerufen!");
  StatsRequest request;
  request.set_function_name(user);
  StatsReply reply;
  ClientContext context;
  Status status = stub_->GetStatistics(&context, request, &reply);
  if (status.ok()) {
    return reply.counter();
  } else {
    spdlog::error("grpc: Ein Fehler ist aufgetreten: " + status.error_message());
    return -1;
  }
}


std::string StatisticsManagerClient::GetFunctionNames() {
  spdlog::info("grpc: Die Funktion GetFunctionNames wird aufgerufen!");
  const ::google::protobuf::Empty request;
  FunctionNamesReply reply;
  ClientContext context;
  Status status = stub_->GetFunctionNames(&context, request, &reply);
  if (status.ok()) {
    return reply.names();
  } else {
    spdlog::error("grpc: Ein Fehler ist aufgetreten: " + status.error_message());
    return "";
  }
}