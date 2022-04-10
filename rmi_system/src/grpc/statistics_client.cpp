#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "build/client.p/statistics_server.grpc.pb.h"

#include "statistics_manager_client.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;



  // Assembles the client's payload, sends it and presents the response back
  // from the server.
  int StatisticsManagerClient::GetStatistics(const std::string& user) {
    // Data we are sending to the server.
    StatsRequest request;
    request.set_function_name(user);

    // Container for the data we expect from the server.
    StatsReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->GetStatistics(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.counter();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return -1;
    }
  }

  std::string StatisticsManagerClient::GetFunctionNames() {
    // Data we are sending to the server.
    const ::google::protobuf::Empty request;

    // Container for the data we expect from the server.
    FunctionNamesReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.

    Status status = stub_->GetFunctionNames(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.names();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return "";
    }
  }

/*
int main(int argc, char** argv) {
  // Instantiate the client. It requires a channel, out of which the actual RPCs
  // are created. This channel models a connection to an endpoint specified by
  // the argument "--target=" which is the only expected argument.
  // We indicate that the channel isn't authenticated (use of
  // InsecureChannelCredentials()).
  std::string target_str;
  std::string arg_str("--target");
  if (argc > 1) {
    std::string arg_val = argv[1];
    size_t start_pos = arg_val.find(arg_str);
    if (start_pos != std::string::npos) {
      start_pos += arg_str.size();
      if (arg_val[start_pos] == '=') {
        target_str = arg_val.substr(start_pos + 1);
      } else {
        std::cout << "The only correct argument syntax is --target="
                  << std::endl;
        return 0;
      }
    } else {
      std::cout << "The only acceptable argument is --target=" << std::endl;
      return 0;
    }
  } else {
    target_str = "localhost:50051";
  }
  StatisticsManagerClient greeter(
      grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials()));
  std::string user("eat");
  int reply = greeter.GetStatistics(user);
  std::cout << "Counter received " << user << ": " << std::to_string(reply) << std::endl;
  std::string functionNames = greeter.GetFunctionNames();
  std::cout << functionNames << std::endl;
  return 0;
}*/