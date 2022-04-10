#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "build/server.p/statistics_server.grpc.pb.h"

#include "statistics_manager_impl.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;


Status StatisticsManagerImpl::GetStatistics(ServerContext* context, const StatsRequest* request,
                StatsReply* reply) {
  context = context; //wegen "unused variable" Warnung :)
  int counter{getCounter(request->function_name())};
  reply->set_counter(counter);
  return Status::OK;
}


grpc::Status StatisticsManagerImpl::GetFunctionNames(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                  FunctionNamesReply* reply) {
    request = request; //wegen "unused variable" Warnung :)
    context = context;
    std::string s{""};
    for(auto const& imap : callCounter) {
        s += imap.first + ", ";
    }
    reply->set_names(s);
    return Status::OK;
}