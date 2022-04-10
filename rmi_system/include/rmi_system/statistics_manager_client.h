#include "build/client.p/statistics_server.grpc.pb.h"
#include <grpcpp/grpcpp.h>

#include <iostream>
#include <memory>
#include <string>

class StatisticsManagerClient {
  public: 
    StatisticsManagerClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(StatisticsManager::NewStub(channel)) {}
    int GetStatistics(const std::string&);
    std::string GetFunctionNames();
  private:
    std::unique_ptr<StatisticsManager::Stub> stub_;
};