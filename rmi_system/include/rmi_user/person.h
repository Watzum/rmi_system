#pragma once

#include "abstractClass.h"
#include "skeleton.h"

#include <string>

class Person : public AbstractClass, public Skeleton {
  public:
    Person() : Skeleton(this) {
      listenToFunctionCalls();
    }
    ~Person() { };
    void go(int i, std::string s);
    bool drink(std::string s);
    int eat();
};