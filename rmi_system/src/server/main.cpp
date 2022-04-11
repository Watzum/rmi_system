#include "skeleton.h"
#include "person.h"

#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;


void setLogLevel(int i) {
    if (i == 0) {
        spdlog::set_level(spdlog::level::critical);
    } else if (i == 1) {
        spdlog::set_level(spdlog::level::err);
    } else if (i == 2) {
        spdlog::set_level(spdlog::level::warn);
    } else if (i == 3) {
        spdlog::set_level(spdlog::level::info);
    }
} 


int main(int argc, char* argv[]) { //int argc, char* argv[]
    CLI::App app{"RMI Server"};
    int log_level{3};
    app.add_option("--loglevel", log_level, "Log level: 0 = critical, 1 = error,\n2 = warning, 3 = info");
    CLI11_PARSE(app, argc, argv);

    Person p;
    return 0;
}