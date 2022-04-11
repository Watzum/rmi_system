#include "remoteFunctionCaller.hpp"
#include "personStub.h"

#include "CLI11.hpp"

#include <iostream>

using namespace std;

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


int main(int argc, char* argv[]) {
    CLI::App app{"RMI Client"};
    unsigned int log_level{3};
    app.add_option("--loglevel", log_level, "Log level: 0 = fatal, 1 = error,\n2 = warning, 3 = info");
    CLI11_PARSE(app, argc, argv)
    setLogLevel(log_level);

    PersonStub st;
    cout << st.drink("Hello!...") << endl;
    try {
        st.go(5, "Emil");
    } catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    cout << st.eat() << endl;
    cout << st.GetFunctionNames() << endl;
    cout << st.GetStatistics("eat") << endl;
    return 0;
}