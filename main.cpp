#include <iostream>
#include <string> 
#include <cstdlib>
#include <ctime>

using namespace std;

class Node{
    public:
    string val;
    Node* next;

    Node(string value){
        val = value;
        next = nullptr;
    }
}

class LinkedList{
    private:
    Node* head;
    Node* tail;

    public:
    LinkedList(){ //done
        head = nullptr;
        tail = nullptr;
    }

    ~LinkedList(){ //done
        Node* curr = head;
        while (curr != nullptr){
            Node* temp = curr -> next;
            delete curr;
            curr = nullptr;
            curr = temp;
            
        }

    }

    bool isEmpty(){ //done
        if (head == nullptr){return true;}
        else{return false;}

    }

    bool checkInList(string value){//done
        Node* curr = head;
        Node* val = Node(value);
        while (curr -> next != nullptr){
            if (curr -> next == val){return true;}
            else{continue;}
        }
        return false;
    }

    void addToListEnd(string value){ //done
        Node* newNode = new Node(value);
        if (head == nullptr){
            head = newNode;
        }   
        else{
            Node* temp = head;
            while (temp -> next != nullptr){
                temp = temp -> next;
            }
            temp = newNode();
        }

    }

    void removeBagFromList(string bagID){ //done
        Node* bag = Node(bagID);
        if (head == nullptr){
            cout << "Bag not in database. " << endl;
            return;
        }
        else{
            Node* temp = head;
            if (checkInList(bagID)){
                while (temp != bag && temp -> next != nullptr){
                    temp = temp -> next;
                    if (temp == bag){
                        delete temp;
                        temp = null;
                    }
                }
            }
            else{
                cout << "Bag not in database. " << endl;
                return;
            }
        }
    }

    void removeUserFromList(string user){ //done
        Node* username = Node(user);
        if (head == nullptr){
            cout << "User not in database. " << endl;
            return;
        }
        else{
            Node* temp = head;
            if (checkInList(user)){
                while (temp != username && temp -> next != nullptr){
                    temp = temp -> next;
                    if (temp = username){
                        delete temp -> next;
                        temp -> next = null;
                        delete temp;
                        temp = null;
                    }
                }
            }
            else{
                cout << "User not in database. " << endl;
                return;
            }
        }
    }
}

class Bags{
    private: 
    string overheadBinArray[118]; //array to keep track of bags in overhead bins
    //might might array ^ a linked list array
    LinkedList baggageCarousel;
    LinkedList unclaimedBaggage;
    LinkedList priorityBagList;
    LinkedList carryOns; //holds all the bags that've been registered as carryons
    LinkedList checkedInBags; //holds all the bags that've been checked in 

    //priorityqueue
    //regularqueue
    //onplanestack


    
    public:
    //status update
    //check for connecting flight


    void checkIn(){//may or may not be done
        string bag;
        string reponse;
        cout << "Enter Bag ID: " << endl;
        cin >> bag;
        cout << "Is this bag a carry-on? Please enter Y or N: " << endl;
        while (true){
            if (reponse == "Y" || response == "y"){
                carryOns.addToListEnd(bag);
                return;
            }
            if (response == "N" || reponse == "n"){
                checkedInBags.addToListEnd(bag);
                break;
            }
            else{
                cout << "Invalid response. Please enter Y or N: " << endl;
                continue;
            }
        }

        cout << "Is the bag a priority bag? Please enter Y or N: " << endl;
        while (true){
            cin >> reponse;
            if (reponse == "Y" || response == "y"){priorityBagList.addToListEnd(bag);}
            else if (reponse == "N" || response == "N"){break;}
            else{
                cout << "Invalid response. Please enter Y or N: " << endl;
                continue;
            }
        }

    }

    void priorityStatus(string bagID){ //done
        if (priorityBagList.checkInList(bagID)){
            addToPriorityQueue(bagID)
        }
        if (!priorityBagList.checkInList(bagID)){
            addToRegularQueue(bagID)
        }
    }

    void addToPriorityQueue(string bagID){ //not done
        updateStatus("Priority Queue")
    }

    void addToRegularQueue(string bagID){ //not done
        updateStatus("Regular Checked Bag Queue")
    }

    void addToCheckedBagsLoadedOnPlaneStack(string bagID){ //not done
        updateStatus("On Plane")
    }
    
    //Claude helped me with this function so it could be a bit more extensive?
    void CarryOnBagSecurityCheck(string bagID){ //technically done, but could be cooler
        bool prohibitedItem = false;

        //x-ray
        cout << "Scanning bag (Bag ID: " << bagID << ") through X-Ray Machine." << endl;

        //simulates prohibited item based on random number value
        int randomCheck = rand() % 10;
        if (randomCheck < 2) {prohibitedItem = true;}

        if (prohibitedItem){
            cout << "ALERT: Prohibited Item Detected In Bag (Bag ID: " << bagID << ")!!" << endl;
            cout << "A TSA Agent Must Manually Inspect The Bag." << endl;
        }

        else{cout << "Bag (Bag ID: " << bagID << ") passed the security screening." << endl;}
    }

    //Claude helped me with this function so it could be a bit more extensive?
    void CheckedBagSecurityCheck(string bagID){//technically done, but could be cooler
        bool explosive = false;
        bool prohibitedItem = false;
        bool overWeight = false;

        cout << "Preforming Security Screening on Bag (Bag ID: " << bagID << ")..." << endl;
        int randomCheck = rand() % 10;
        if (randomCheck =< 1) {explosive = true;}
        if (randomCheck >= 8) {prohibitedItem = true;}
        if (randomCheck % 10 < 3) {overWeight = true;}

        if (explosive) {
            cout << "CRITICAL SECURITY ALERT: Potential Explosives detected in Checked Bag (ID: " << bagID << ")!!" << endl;
            cout << "Immediate intervention required. Bag isolated for further investigation." << endl;
        }

        if (prohibitedItem){
            cout << "SECURITY WARNING: Potentially Dangerous Items detected in Checked Bag (ID: " << bagID << ")." << endl;
            cout << "Bag requires detailed manual inspection by security personnel." << endl;
        }
    
        if (overWeight) {
            cout << "NOTE: Checked Bag (ID: " << bagID << ") exceeds weight limits." << endl;
            cout << "Additional fees or handling may be required." << endl;
        }

        else{
            cout << "Checked Bag (ID: " << bagID << ") successfully passed comprehensive security screening." << endl;
        }
    }
    
    bool overheadBinSpace(){ //done
        if (overheadBinArray[117] != null){return false;}
        else{return true;}
    }

    void addBagToOverheadBin(string bagID){ //done, but double check
        if (overheadBinSpace()){
            for (int i = 0; i < 118; i++){
                if (overheadBinArray[i] == null){overheadBinArray[i] = bagID;}
            }
        }
        else if (!overheadBinSpace()){
            cout << "There's no space. A flight attendent is coming to get your bag so it can be checked." endl;
            carryOns.removeBagFromList(bagID);
            checkedInBags.addToListEnd(bagID);
            addToRegularQueue(bagID);
        }
    }

    void addBagToCarousel(string bagID){baggageCarousel.addToListEnd(bagID);}//done

    void removeBagFromCarousel(string bagID){baggageCarousel.removeBagFromList(bagID);}//done

    void addBagToUnclaimedList(string bagID){unclaimedBaggage.addToListEnd(bagID);}//done

    void removeBagFromUnclaimedList(string bagID){unclaimedBaggage.removeBagFromList(bagID);}//done

    void checkForBagsLeftOnCarousel(){ //not done
        if (baggageCarousel.isEmpty()){
            return;
        }
        else{

        }
    }

    void moveFromCarouselToUnclaimed(string bagID){ //done
        removeBagFromCarousel(string bagID)
        addBagToUnclaimedList(string bagID)
    }

    void updateStatus(string bag ID, string newLocation){ //not done
    }

    void notifyPassenger(string message){} //not done
}

Bags globalbags;

class AirportEmployeeUI{
    private:
    LinkedList employeeLogins;
    public: 
    bool login(string user, string pass){//done
        if (checkInList(user)) && checkInList(pass)){return true;}
        else(return false;)
    } 
    void removeUser(string user){//done
        if (user == "ADMIN"){
            cout << "You can't remove the admin user." << endl;
            return;
        }
        else{
            if (checkInList(user)){
                string reponse;
                cout << "Are you sure you want to remove user: " << user << "? Enter Y or N: " << endl;
                while (true){
                    cin >> reponse;
                    if (reponse == "Y" or "y"){
                        employeeLogins.removeUserFromList(user);
                        cout << "User: " << user << " has been removed."
                    }
                    if (reponse == "N" or "n"){
                        cout << "Okay!" << endl;
                        return;
                    }
                    else(
                        cout << "Your reponse is invalid. Please enter Y or N: " << endl;
                    )
                }
            }
            else{
                cout << "User " << user << " is not in the database. " << endl;
            }
        }
    }
    void addUser(string user, string pass){
        employeeLogins.addToListEnd(user);
        employeeLogins.addToListEnd(pass);
    } //done
    void showMenu(){//not done
        cout << "-----Employee Interface-----" << endl;
        cout << "1. " << endl;
        cout << "2. " << endl;
        cout << "3. " << endl;
        cout << "4. " << endl;
        cout << "5. " << endl;
        cout << "6. " << endl;
        cout << "7. " << endl;
        cout << "8. " << endl;
        cout << "9. " << endl;

    }
    void menuReponse(){//not done
        string answer;
        cout << "Enter the menu number for the item in which you are wanting to access"
    }
    void showAdminMenu(){//not done
        cout << "-----Admin Interface-----" << endl;
        cout << "1. Add employee user" << endl;
        cout << "2. " << endl;
        cout << "3. " << endl;
        cout << "4. " << endl;
        cout << "5. " << endl;
        cout << "6. " << endl;
        cout << "7. " << endl;
        cout << "8. " << endl;
        cout << "9. " << endl;
    }
    void adminMenuReponse(){ //not done
        string answer;
        cout << "Enter the menu number for the item in which you are wanting to access"
    }
}

class PassengerUI{
    //Boarding ID 2d array
    //log in using boarding ID
    //show menu
    //select thing 
}

int main(){
    srand(time(nullptr));
    AirportEmployeeUI employeeUI;
    employeeUI.addUser("ADMIN", "Pass123") //creates admin account

    return 0;
}