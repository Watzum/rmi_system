#include "skeleton.h"
#include <json.hpp>

std::string Skeleton::callFunction(const std::string functionName) {
    nlohmann::json j;
    //Funktionsdeklarationen müssen mit dem __FUNCTION__ Makro angegeben werden
    __VOID_FUNCTION__(go)
    __FUNCTION__(drink)
    __FUNCTION__(eat)
    __END__
}