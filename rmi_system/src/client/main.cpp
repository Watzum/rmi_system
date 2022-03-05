#include "remoteFunctionCaller.h"
#include "personStub.h"

#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;

int main(int argc, char* argv[]) {
    CLI::App app{"RMI Client"};
    string functionName = "default";
    app.add_option("-f,--function", functionName, "the function to call");
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Client is now ready!");

    PersonStub st;
    st.go();
    st.drink();
    st.eat();
}