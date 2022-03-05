#pragma once

#include "abstractClass.h"
#include "remoteFunctionCaller.h"

#include <iostream>

#define __SEND_FUN__ RemoteFunctionCaller::sendFunctionCall(__func__);


// TODO?: AbstractClass = PersonStub, non-pure-virtual Funktion __SEND_FUN__
class PersonStub : public AbstractClass, RemoteFunctionCaller {
  public:
    void go() {
        __SEND_FUN__
    }

    void eat() {
        __SEND_FUN__
    }

    void drink() {
        __SEND_FUN__
    }
};