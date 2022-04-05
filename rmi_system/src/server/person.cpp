#include "person.h"

#include <iostream>

using namespace std;

void Person::go(int i) {
    cout << "I am going... somewhere " << i << endl;
}


 int Person::eat() {
    cout << "I am starting to eat!" << endl;
    return 5;
}


bool Person::drink() {
    cout << "I am starting to drink!" << endl;
    return true;
}