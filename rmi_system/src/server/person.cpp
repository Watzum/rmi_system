#include "person.h"

#include <iostream>
#include <string>

using namespace std;

void Person::go(int i) {
    cout << "I am going... somewhere " << i << endl;
    throw std::runtime_error("custom error");
}


bool Person::drink(std::string s) {
    cout << s << "! I am starting to drink!" << endl;
    return true;
}


 int Person::eat() {
    cout << "! I am starting to eat!" << endl;
    return 5;
}