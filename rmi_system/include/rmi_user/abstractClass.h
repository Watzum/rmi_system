#pragma once

class AbstractClass {
  public:
    virtual ~AbstractClass() { }
    virtual void go(int i) = 0;
    virtual bool drink() = 0;
    virtual int eat() = 0;
};