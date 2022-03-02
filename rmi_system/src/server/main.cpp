#include "skeleton.h"

#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;


int main(int argc, char* argv[]) { 
    //CLI11
    CLI::App app{"RMI Server"};
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Welcome to the server!");
    Skeleton sk;
    sk.receiveFunctionCall();
}