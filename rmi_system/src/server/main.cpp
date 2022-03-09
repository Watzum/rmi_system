#include "skeleton.h"
#include "person.h"
#include "dummy.pb.h"

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

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    Dummy *d = new Dummy;
    d->set_text("Hello World");
    cout << d->text() << endl;
    delete d;
    google::protobuf::ShutdownProtobufLibrary();
    /*Person p;
    Skeleton sk{&p};
    sk.listenToFunctionCalls();*/
}