#include "Employee.h"
#include <iostream>

Employee::Employee(string name, string user, string pass){
    employeeName = name;
    username = user;
    password = pass;
}

bool Employee::login(string pass){
    if (password == pass){return true;} //true if password is correct
    return false;
}

void Employee::greeting(){
    cout << "Hello, " << employeeName << ". I hope you are having a good day."
}