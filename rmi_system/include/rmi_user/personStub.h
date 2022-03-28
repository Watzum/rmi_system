#pragma once

#include "abstractClass.h"
#include "remoteFunctionCaller.hpp"

#include <iostream>

#define __SEND_FUN__(type) return RemoteFunctionCaller::returnFunctionCall<type>(__func__);
#define __SEND_VOID_FUN__ RemoteFunctionCaller::sendFunctionCall(__func__);


// TODO?: AbstractClass = PersonStub, non-pure-virtual Funktion __SEND_FUN__
class PersonStub : public AbstractClass, RemoteFunctionCaller {
  public:
    void go() {
        __SEND_VOID_FUN__
    }

    int eat() {
        __SEND_FUN__(int)
    }

    bool drink() {
        __SEND_FUN__(bool)
    }
};