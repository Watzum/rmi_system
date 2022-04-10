#pragma once

#include "abstractClass.h"
#include "remoteFunctionCaller.hpp"

#include <iostream>
#include <string>

#define __SEND_FUN__(type, ...) return RemoteFunctionCaller::returnFunctionCall<type>(__func__ __VA_OPT__(,) __VA_ARGS__);
#define __SEND_VOID_FUN__(...) RemoteFunctionCaller::sendFunctionCall(__func__, __VA_ARGS__);

class PersonStub : public AbstractClass, RemoteFunctionCaller {
  public:
    void go(int i) {
        __SEND_VOID_FUN__(i)
    }


    bool drink(std::string s) {
        __SEND_FUN__(bool, s)
    }


    int eat() {
        __SEND_FUN__(int)
    }
};