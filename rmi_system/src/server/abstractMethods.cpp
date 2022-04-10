#include "skeleton.h"
#include <json.hpp>

#include <string>

std::string Skeleton::callFunction(const std::string functionName, 
  nlohmann::json par) {
    nlohmann::json j;
    //Funktionsdeklarationen müssen mit dem __FUNCTION__ Makro angegeben werden
    __VOID_FUNCTION__(go, __ARGUMENT__(int, 1))
    __FUNCTION__(drink, __ARGUMENT__(std::string, 1))
    __FUNCTION__(eat)
    __END__
}