#ifndef BAG_H
#define BAG_H

#include <string>
using namespace std; 

class Bag{
private:
    string bagID;
    string status;
    string location;
    string ownerName;
    string ownerContact;
    string flightID:
    string priority;

public: 
    Bag(string bag, string stat, string lolo, string name, string contact, string flight, string prio);
    string getBagID();
    string getStatus();
    string getLocation();
    string getOwnerName();
    string getOwnerContact();
    string getFlightID();
    string getPriority();

    void setStatus(newstatus);
    void setLocation(newlocation);
    void displayBagInfo();
}

#endif