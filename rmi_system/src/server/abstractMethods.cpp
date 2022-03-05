#define __FUNCTION__(name) if (functionName == #name) {\
    rmi_object->name();\
    return true;\
    }
#define __END__ return false;
//#define __END__ else std::cout << "No such function" << std::endl;
#include "skeleton.h"

bool Skeleton::callFunction(std::string functionName) {
    //Funktionsdeklarationen müssen mit dem __FUNCTION__ Makro angegeben werden
    __FUNCTION__(go)
    __FUNCTION__(drink)
    __FUNCTION__(eat)
    __END__
}
