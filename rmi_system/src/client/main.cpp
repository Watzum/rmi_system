#include "remoteFunctionCaller.hpp"
#include "personStub.h"

#include <json.hpp>
#include <asio.hpp>
#include "CLI11.hpp"
#include <spdlog/spdlog.h>

#include <iostream>

using namespace std;
using namespace asio;
using json = nlohmann::json;


int main(int argc, char* argv[]) {
    CLI::App app{"RMI Client"};
    CLI11_PARSE(app, argc, argv);

    spdlog::info("Client is now ready!");

    PersonStub st;
    cout << st.drink() << endl;
    st.go();
    cout << st.eat() << endl;
    return 0;
}