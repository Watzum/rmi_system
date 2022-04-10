#include "skeleton.h"
#include "person.h"

#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;


int main() { //int argc, char* argv[]
    /*CLI::App app{"RMI Server"};
    std::string s{"127.0.0.1"};
    unsigned int port{1113};
    unsigned int log_level{0};
    app.add_option("--server_port", port, "The TCP-port of the server");
    app.add_option("--loglevel", log_level, "Log level: 0 = fatal, 1 = error,\n2 = warning, 3 = info");
    CLI11_PARSE(app, argc, argv);*/

    Person p;
    return 0;
}