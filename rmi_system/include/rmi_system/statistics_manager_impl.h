#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <map>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "build/server.p/statistics_server.grpc.pb.h"

class StatisticsManagerImpl final : public StatisticsManager::Service {
  private:
    std::map<std::string, int> callCounter;
    std::mutex mtx;
  
  public: 
    grpc::Status GetStatistics(grpc::ServerContext* context, const StatsRequest* request,
                  StatsReply* reply) override;
    grpc::Status GetFunctionNames(grpc::ServerContext* context, const ::google::protobuf::Empty* request,
                  FunctionNamesReply* reply) override;

    
    void incrementCounter(std::string functionName) {
      std::scoped_lock lck{mtx};
      if (callCounter.contains(functionName)) {
        callCounter[functionName]++;
      } else {
        callCounter[functionName] = 0;
      }
    }


    int getCounter(std::string functionName) {
      std::scoped_lock lck{mtx};
      int v{callCounter[functionName]};
      return v;
    }
};