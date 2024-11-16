#include "Bag.h"
#include <iostream>

Bag::Bag(string bag, string stat, string lolo, string name, string contact, string flight, string prio){
    bagID = bag;
    status = stat;
    location = lolo;
    ownerName = name;
    ownerContact = contact;
    flightID = flight;
    priority = prio;
}

string Bag::getBagID(){return bagID;}
string Bag::getStatus(){return status;}
string Bag::getLocation(){return location;}
string Bag::getOwnerName(){return ownerName;}
string Bag::getOwnerContact(){return ownerContact;}
string Bag::getFlightID(){return flightID;}
string Bag::getPriority(){return priority;}

void Bag::setStatus(newstatus){status = newstatus}
void Bag::setLocation(newlocation){location = newlocation;}
void Bag::displayBagInfo(){
    cout << "Bag ID: " << bagID << endl;
    cout << "Status: " << status << endl;
    cout << "Location: " << location << endl;
    cout << "Owner Name: " << ownerName << endl;
    cout << "Owner Contact Info: " << ownerContact << endl;
    cout << "Flight ID: " << flightID << endl;
    cout << "Priority: " << priority << endl;
}