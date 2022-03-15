#pragma once

class AbstractClass {
  public:
    virtual ~AbstractClass() { }
    virtual void go() = 0;
    virtual void drink() = 0;
    virtual void eat() = 0;
};