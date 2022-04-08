#pragma once

#include "abstractClass.h"
#include "skeleton.h"

class Person : public AbstractClass, public Skeleton {
  public:
    Person() : Skeleton(this) {
      listenToFunctionCalls();
    }
    ~Person() { };
    void go(int i);
    int eat();
    bool drink();
};