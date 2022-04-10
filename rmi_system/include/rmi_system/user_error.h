#include <exception>
#include <string>

class user_error : public std::exception {
  private: 
    std::string message;
  public: 
    user_error(const std::string s) : message{s} {}


    virtual const char* what() const throw() {
        return message.c_str();
    }
};