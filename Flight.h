#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <iostream>
using namespace std; 

class Flight{
private:
    string flightID;
    string departurePlace;
    string arrivalPlace;
    //departure time
    //arrival time
    string planeType;
    int capacity;
    string status;

public:
    string getFlightStatus();
    void updateFlightStatus(newStatus);
    //baglist, idk datatype
    //getPassengerManifest, idk datatype



}


#endif 