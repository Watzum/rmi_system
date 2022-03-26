#include "person.h"

#include <iostream>

using namespace std;

void Person::go() {
    cout << "I am going... somewhere" << endl;
}


 int Person::eat() {
    cout << "I am starting to eat!" << endl;
    return 5;
}


bool Person::drink() {
    cout << "I am starting to drink!" << endl;
    return true;
}