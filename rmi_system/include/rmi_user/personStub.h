#pragma once

#include "abstractClass.h"
#include "remoteFunctionCaller.hpp"

#include <iostream>

//#define __SEND_FUN__(type) ((type) == void) ? RemoteFunctionCaller::sendFunctionCall(__func__); : return RemoteFunctionCaller::returnFunctionCall<type>(__func__);

#define __SEND_FUN__(type) return RemoteFunctionCaller::returnFunctionCall<type>(__func__);
#define __SEND_VOID_FUN__(...) RemoteFunctionCaller::sendFunctionCall(__func__, __VA_ARGS__);

class PersonStub : public AbstractClass, RemoteFunctionCaller {
  public:
    void go(int i) {
        __SEND_VOID_FUN__(i)
    }

    int eat() {
        __SEND_FUN__(int)
    }

    bool drink() {
        __SEND_FUN__(bool)
    }
};