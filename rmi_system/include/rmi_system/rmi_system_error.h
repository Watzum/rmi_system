#include <exception>
#include <string>

class rmi_system_error : public std::exception {
  private: 
    std::string message;
  public: 
    rmi_system_error(const std::string s) : message{s} {}


    virtual const char* what() const throw() {
        return message.c_str();
    }
};