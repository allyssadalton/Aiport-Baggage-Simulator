#include "Flight.h"
#include <iostream>

Flight::Flight(const string &flight, const string &departure, const string &destination, const chrono::system_clock::time_point &dtime, const chrono::system_clock::time_point &atime,
    const string &plane, int cap){
        flightID = flight;
        departurePlace = departure;
        arrivalPlace = destination;
        departureTime = dtime;
        arrivalTime = atime;
        planeType = plane;
        capacity = cap;
        status = "Scheduled & On-Time";

    }
    string Flight::getFlightID(){return flightID;}
    string Flight::getdeparturePlace(){return departurePlace;}
    string Flight::getarrivalPlace(){return arrivalPlace;}
    chrono::system_clock::time_point Flight::getdepartureTime(){return departureTime;}
    chrono::system_clock::time_point Flight::getarrivalTime(){return arrivalTime;}
    string Flight::getPlaneType(){return planeType;}
    int Flight::getCapacity(){return capacity;}
    string Flight::getStatus(){return status;}
    string Flight::getOverheadBin(){return overheadBinSpace[]}
    int Flight::overheadBinSpace(){
        for (int i = 0; i < 118, i++){
            if (i == 118 & overheadBin[i] != "0"){return 0;}
            else if (overheadBin[i] == "0"){return (118 - i);}
        }
    }

    void Flight::setStatus(string newstatus){status = newstatus;}

    void Flight::updateOverheadBin(string bagIDAdded){
        for (int i = 0; i < 118, i++){
            if (overheadBin[i] == "0"){overheadBin[i] == bagIDAdded;}
            //Maybe error case incase there's not room.
        }
    }
    void Flight::displayFlightInfo(){
        cout << "Flight ID: " << flightID << endl;
        cout << "Place of Departure: " << departurePlace << endl;
        cout << "Time of Departure: " << departureTime << endl;
        cout << "Place of Arrival: " << arrivalPlace  << endl;
        cout << "Time of Arrival: " << arrivalTime  << endl;
        cout << "Aircraft: " << planeType << endl;
        cout << "Plane Capacity: " << capacity  << endl;
    }