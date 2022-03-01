#include <iostream>
#include <asio.hpp>
#include "CLI11.hpp"


using namespace std;
using namespace asio;

int main(int argc, char* argv[]) {
    CLI::App app{"RMI Server"};
    CLI11_PARSE(app, argc, argv);
    std::cout << "Hello, I am the server!" << std::endl;
    auto a1{ip::make_address("127.0.0.1")};
    cout << a1.to_string() << ", " << a1 << endl;   
}