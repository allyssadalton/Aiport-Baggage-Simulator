#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
using namespace std; 

class Flight{
private:
    string flightID;
    string departurePlace;
    string arrivalPlace;
    chrono::system_clock::time_point departureTime; // Credits to ChatGPT
    chrono::system_clock::time_point arrivalTime;  //  Credits to ChatGPT
    string planeType;
    int capacity; 
    string status;
    string overheadBinSpace[118]; //Array in which each element is a bagID
    //NEEDS STACK FOR CHECKED BAGS ON THE FLIGHT 

public:
    Flight(const string &flight, const string &departure, const string &destination, const chrono::system_clock::time_point &dtime, const chrono::system_clock::time_point &atime,
    const string &plane, int cap);
    string getFlightID();
    string getdeparturePlace();
    string getarrivalPlace();
    chrono::system_clock::time_point getdepartureTime();
    chrono::system_clock::time_point getarrivalTime();
    string getPlaneType();
    int getCapacity();
    string getStatus();
    string getOverheadBin();
    int overheadBinSpace();
    void assignBagToFlight(Bag& bag); //NOT IN CPP
    void setStatus(string newstatus);
    void updateOverheadBin(string bagIDAdded);
    void displayFlightInfo();

}


#endif 