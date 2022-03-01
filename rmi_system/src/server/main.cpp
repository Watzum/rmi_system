#include <iostream>
#include <asio.hpp>

using namespace std;
using namespace asio;

int main() {
    std::cout << "Hello, I am the server!" << std::endl;
    auto a1{ip::make_address("127.0.0.1")};
    cout << a1.to_string() << ", " << a1 << endl;   
}