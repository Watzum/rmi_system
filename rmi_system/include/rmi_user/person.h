#pragma once

#include "abstractClass.h"

class Person : public AbstractClass {
  public:
    ~Person() { };
    void go(int i);
    int eat();
    bool drink();
};