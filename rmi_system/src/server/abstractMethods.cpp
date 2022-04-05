#include "skeleton.h"
#include <json.hpp>

std::string Skeleton::callFunction(const std::string functionName, 
  nlohmann::json par) {
    nlohmann::json j;
    //Funktionsdeklarationen müssen mit dem __FUNCTION__ Makro angegeben werden
    __VOID_FUNCTION__(go, __ARGUMENT__(int, 1))
    __FUNCTION__(drink)
    __FUNCTION__(eat)
    __END__
}