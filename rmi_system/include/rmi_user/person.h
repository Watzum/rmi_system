#pragma once

#include "abstractClass.h"

class Person : public AbstractClass {
  public:
    ~Person() { };
    void go();
    int eat();
    bool drink();
};