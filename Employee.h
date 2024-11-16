#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <string>
using namespace std; 


//Each instance of class Employee will be the username.
class Employee{
private:
    string employeeName;
    string username;
    string password;

public:
    Employee(string name, string user, string pass);
    bool login(string user, string pass);
    void greeting();
    void viewBagDetails(int bagID)
    void updateFlightBagsStatus(string flightID, string status)
    void addBagToUnclaimedList(int bagID)
    void contactPassenger(int passengerID)
}

#endif