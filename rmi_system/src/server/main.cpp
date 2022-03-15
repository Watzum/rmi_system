#include "skeleton.h"
#include "person.h"

#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;


int main(int argc, char* argv[]) { 
    CLI::App app{"RMI Server"};
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Welcome to the server!");

    /*GOOGLE_PROTOBUF_VERIFY_VERSION;
    FunctionCall* d = new FunctionCall;
    d->set_name("Hello World");
    string s;
    d->SerializeToString(&s);
    delete d;
    FunctionCall* d2 = new FunctionCall;
    d->ParseFromString(s);
    cout << s.length() + '\n' << endl;
    cout << d->name() << endl;
    delete d2;
    google::protobuf::ShutdownProtobufLibrary();*/
    Person p;
    Skeleton sk{&p};
    sk.listenToFunctionCalls();
    return 0;
}