#include "skeleton.h"
#include <json.hpp>

using json = nlohmann::json;
#define __FUNCTION__(name)  if (functionName == #name)\
                                j["returnValue"] = rmi_object->name();
#define __VOID_FUNCTION__(name) if (functionName == #name)\
    if (std::is_void<decltype(rmi_object->name())>())\
        rmi_object->name();
    //sonst Fehlerbehandlung, weil keine void function
#define __END__ return j.dump();


std::string Skeleton::callFunction(const std::string functionName) {
    json j;
    //Funktionsdeklarationen müssen mit dem __FUNCTION__ Makro angegeben werden
    __VOID_FUNCTION__(go)
    __FUNCTION__(drink)
    __FUNCTION__(eat)
    __END__
}