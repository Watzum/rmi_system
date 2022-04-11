#pragma once

#include <string>

class AbstractClass {
  public:
    virtual ~AbstractClass() { }
    virtual void go(int i, std::string s) = 0;
    virtual bool drink(std::string s) = 0;
    virtual int eat() = 0;
};