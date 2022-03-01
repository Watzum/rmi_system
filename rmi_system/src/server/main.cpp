#include <iostream>
#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

using namespace std;
using namespace asio;
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    //CLI11
    CLI::App app{"RMI Server"};
    CLI11_PARSE(app, argc, argv);

    //asio
    std::cout << "Hello, I am the server!" << std::endl;
    auto a1{ip::make_address("127.0.0.1")};
    cout << a1.to_string() << ", " << a1 << endl; 

    //spdlog
    spdlog::info("Welcome to spdlog!");
    spdlog::error("An error occured...!!!");  

    //json 
    json j;
    j["pi"] = 3.1415;
    j["happy"] = true;
    cout << j.dump() << endl;
}