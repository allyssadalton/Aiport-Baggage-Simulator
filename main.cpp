#include <iostream>
#include <string> 
#include <cstdlib>
#include <ctime>
#include <map>
#include <thread> 
#include <chrono>
#include <fstream>

using namespace std;

class Node{ 
    public:
    string val;
    Node* next;

    Node(string value){
        val = value;
        next = nullptr;
    }
};

class LinkedList{ 
    private:
    Node* head;
    Node* tail;

    public:
    LinkedList(){ 
        head = nullptr;
        tail = nullptr;
    }

    ~LinkedList(){ 
        Node* curr = head;
        while (curr != nullptr){
            Node* temp = curr -> next;
            delete curr;
            curr = nullptr;
            curr = temp;
        }
    }

    string returnHead(){
        if (head == nullptr){return "";}
        return head->val;
    }

    bool isEmpty(){ 
        if (head == nullptr){return true;}
        else{return false;}
    }

    bool checkInList(string value){
        Node* curr = head;
        while (curr != nullptr){
            if (curr -> val == value) {return true;}
            curr = curr -> next;
        }
        return false;
    }

    void addToListEnd(string value){ 
        Node* val = new Node(value);
        if (head == nullptr){
            head = val;
            tail = val;
        } 
        else{
            tail -> next = val;
            tail = val;
        }
    }

    void removeBagFromList(string bagID){
        if (head == nullptr){
            cout << "The list is empty. No bag to remove." << endl;
            return;
        }
        if (head -> val == bagID){
            Node* temp = head;
            head = head -> next;
            if (head == nullptr){tail = nullptr;}
            delete temp;
            return;
        }
        Node* current = head;
        while (current -> next != nullptr && current -> next -> val != bagID){current = current -> next;}
        if (current -> next == nullptr){
            cout << "Bag ID: " << bagID << " not found in the list." << endl;
            return;
        }
        Node* temp = current -> next;
        current->next = temp -> next;
        if (current->next == nullptr){tail = current;}
        delete temp;
    }

    void removeUserFromList(string username){
        Node* user = new Node(username);
        if (head == nullptr){
            cout << "User not in database. " << endl;
            return;
        }
        else{
            Node* temp = head;
            if (checkInList(username)){
                while (temp != user && temp -> next != nullptr){
                    temp = temp -> next;
                    if (temp == user){
                        delete temp -> next;
                        temp -> next = nullptr;
                        delete temp;
                        temp = nullptr;
                    }
                }
            }
            else{
                cout << "User not in database. " << endl;
                return;
            }
        }
    }

    void moveHeadToEnd(){
        if (head == nullptr || head -> next == nullptr){
            cout << "No need to move the head." << endl;
            return;
        }
        Node* oldHead = head;  
        head = head -> next;        
        oldHead -> next = nullptr;
        tail -> next = oldHead;   
        tail = oldHead;
    }

    int size(){
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current -> next;
        }
        return count;
    }


    void viewList(){
        if (head == nullptr){cout << "List Empty." << endl;}
        else{
            Node* temp = head;
            while (temp != nullptr){ 
                cout << temp -> val << endl; 
                temp = temp -> next; 
            }
        }
    }
};

class Stacks{
    private:
    Node* top;
    
    public: 
    Stacks(){top = nullptr;}

    ~Stacks() {
        while (!isEmpty()){pop();}
    }   

    bool isEmpty(){return top == nullptr;}

    void push(string value){
        Node* newNode = new Node(value);
        newNode -> next = top;
        top = newNode;
    }

    string pop(){
        if (isEmpty()) {return "Empty";}
        string poppedValue = top -> val;
        Node* temp = top;
        top = top -> next;
        delete temp;
        return poppedValue;
    }

    string peek(){
        if (isEmpty()){return "Empty";}
        return top -> val;
    }  
};

class Queues{
    private:
    Node* head;
    Node* tail;
    int size;
    
    public: 
    Queues(){
        head = nullptr; 
        tail = nullptr;  
        size = 0;     
    }

    ~Queues(){
        while(!isEmpty()){dequeue();}
    }

    void enqueue(string bagID){
        Node* newNode = new Node(bagID);
        if (isEmpty()){
            head = newNode;
            tail = newNode;
        } 
        else{
            tail -> next = newNode;   
            tail = newNode;        
        }
        size++;
    }

    string dequeue(){
        if (isEmpty()){return "Empty";}
        string bagID = head -> val; 
        Node* temp = head;       
        head = head -> next;    
        delete temp; 
        temp = nullptr;            
        size--;
        if (isEmpty()){tail = nullptr;} 
        if (head == nullptr){tail = nullptr;}
        return bagID; 
    }

    bool isEmpty(){return size == 0;}

    string peek(){
        if (isEmpty()) {return "";}
        return head -> val; 
    }

    int getSize() {return size;}
};


class Bags{ 
    private: 
    string overheadBinArray[118] = {}; //array to keep track of bags in overhead bins
    LinkedList baggageCarousel; //holds all luggage on baggage carousel
    LinkedList unclaimedBaggage; //holds all unclaimed baggage
    LinkedList priorityBagList; //holds all the bags that've been checked in as a priority bag
    LinkedList carryOns; //holds all the bags that've been registered as carryons
    LinkedList checkedInBags; //holds all the bags that've been checked in the entire time
    Stacks onPlaneStack; //holds all the bags that've been placed on under plane
    Queues priorityQueue; //queue for checkedin bags that have priority
    Queues regularQueue; //queue for checkedin bags that don't have priority
    map<string, string> bagStatusMap; //map to link bagID and where the bag is
    
    public:

    void moveBagToEndOfCarousel(string bagID){
        removeBagFromCarousel(bagID);
        baggageCarousel.addToListEnd(bagID);
    }
    
    bool isCarouselEmpty(){
        if (baggageCarousel.isEmpty()){return true;}
        else{return false;}
    }

    void createArray(){
        for (int i = 0; i < 118; i++){overheadBinArray[i] = " ";}
    }

    void logEvent(string event){ //ChatGPT wrote this for me
        ofstream logFile("simulation_log.txt", ios::app); // Open in append mode
        if (logFile.is_open()){
            logFile << event << endl;
            logFile.close();
        } 
        else{cerr << "Error: Unable to open log file." << endl;}
    }

    void addToCheckedInBags(string bagID){checkedInBags.addToListEnd(bagID);}

    void randomBagEvents(string bagID){
        int event = rand() % 100;
        if (event < 5){cout << "Oh no! Bag ID: " << bagID << " has been damaged!" << endl;} 
        else if (event < 10){
            cout << "Alert! Bag ID: " << bagID << " has been misplaced and added to unclaimed baggage." << endl;
            onPlaneStack.pop();
            addBagToUnclaimedList(bagID);
        } 
        else{
            cout << "All bags are accounted for." << endl;
        }
    }

    bool emptyCarryOnBags(){
        if(carryOns.isEmpty()){return true;}
        else{return false;}
    }

    bool emptyCheckedInBags(){
        if(checkedInBags.isEmpty()){return true;}
        else{return false;}
    }

    void moveFromPriorityToPlane(){
        while (priorityQueue.getSize() != 0){
            string bag = priorityQueue.peek();
            priorityQueue.dequeue();
            cout << "Bag ID: " << bag <<  " Moved from Priority Queue to Security Check." << endl;
            CheckedBagSecurityCheck(bag);
            onPlaneStack.push(bag);
            cout << "Bag ID: " << bag <<  " Moved from Security Check to Plane." << endl;
            updateStatus(bag, "On Plane");
        }
    }

    void moveFromRegularToPlane(){
        while (regularQueue.getSize() != 0){
            string bag = regularQueue.peek();
            regularQueue.dequeue();
            cout << "Bag ID: " << bag <<  " Moved from Checked Bag Queue to Security Check." << endl;
            CheckedBagSecurityCheck(bag);
            onPlaneStack.push(bag);
            cout << "Bag ID: " << bag <<  " Moved from Security Check to Plane." << endl;
            updateStatus(bag, "On Plane");
        }
    }

    void moveFromPlaneToCarousel(){
        while (!onPlaneStack.isEmpty()){
            randomBagEvents(onPlaneStack.peek());
            updateStatus(onPlaneStack.peek(), "On Baggage Carousel");
            baggageCarousel.addToListEnd(onPlaneStack.pop());
        }
    }

    void addToCarryOns(string bagID){carryOns.addToListEnd(bagID);}

    void checkIn(){
        string bag;
        string response;
        cout << "Enter Bag ID: " << endl;
        cin >> bag;
        cout << "Is this bag a carry-on? Please enter Y or N: " << endl;
        while (true){
            cin >> response;
            if (response == "Y" || response == "y"){
                addToCarryOns(bag);
                updateStatus(bag, "Carry On");
                CarryOnBagSecurityCheck(bag);
                updateStatus(bag, "Carry On with Passenger");
                return;
            }
            if (response == "N" || response == "n"){
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
            cin >> response;
            if (response == "Y" || response == "y"){
                priorityBagList.addToListEnd(bag);
                checkedInBags.addToListEnd(bag);
                updateStatus(bag, "Priority Checked In");
                addToPriorityQueue(bag);
                cout << "Bag Checked Into Priority Queue" << endl;
                return;
            }
            else if (response == "N" || response == "N"){
                updateStatus(bag, "Checked In");
                checkedInBags.addToListEnd(bag);
                addToRegularQueue(bag);
                cout << "Bag Checked Into Queue" << endl;
                return;
            }
            else{
                cout << "Invalid response. Please enter Y or N: " << endl;
                continue;
            }
        }

    }

    string peekCarousel(){return baggageCarousel.returnHead();}

    void addToPriorityQueue(string bagID){
        priorityQueue.enqueue(bagID);
        addToCheckedInBags(bagID);
        updateStatus(bagID, "Priority Queue");
    }

    void addToRegularQueue(string bagID){ 
        regularQueue.enqueue(bagID);
        addToCheckedInBags(bagID);
        updateStatus(bagID, "Regular Checked Bag Queue");
    }

    void addToCheckedBagsLoadedOnPlaneStack(string bagID){ 
        onPlaneStack.push(bagID);
        updateStatus(bagID, "On Plane");
    }

    bool checkInCheckedInBags(string bagID) {return checkedInBags.checkInList(bagID);}

    bool checkInCarryOns(string bagID){
        if (carryOns.checkInList(bagID)){return true;}
        else{return false;}
    }
    
    //Claude helped me with this function so it could be a bit more extensive
    void CarryOnBagSecurityCheck(string bagID){ //technically done, but could be cooler
        bool prohibitedItem = false;
        updateStatus(bagID, "Security Check");
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
        updateStatus(bagID, "Security Check");
        bool explosive = false;
        bool prohibitedItem = false;
        bool overWeight = false;

        cout << "Preforming Security Screening on Bag (Bag ID: " << bagID << ")..." << endl;
        int randomCheck = rand() % 10;
        if (randomCheck <= 1) {explosive = true;}
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
    
    bool overheadBinSpace(){
        if (overheadBinArray[117] != " "){return false;}
        else{return true;}
    }

    void addBagToOverheadBin(string bagID){ 
        if (overheadBinSpace()){
            for (int i = 0; i < 118; i++){
                if (overheadBinArray[i] == ""){
                    overheadBinArray[i] = bagID;
                    updateStatus(bagID, "In Overhead Bin");
                    break;
                }
            }
        }
        else if (!overheadBinSpace()){
            cout << "There's no space. A flight attendent is coming to get your bag so it can be checked." << endl;
            carryOns.removeBagFromList(bagID);
            checkedInBags.addToListEnd(bagID);
            addToRegularQueue(bagID);
        }
    }

    void addBagToCarousel(string bagID){ 
        baggageCarousel.addToListEnd(bagID);
        updateStatus(bagID, "Baggage Carousel");
    }

    void removeBagFromCarousel(string bagID){baggageCarousel.removeBagFromList(bagID);}

    void addBagToUnclaimedList(string bagID){
        unclaimedBaggage.addToListEnd(bagID);
        updateStatus(bagID, "Unclaimed Baggage");
    }

    void removeBagFromUnclaimedList(string bagID){
        unclaimedBaggage.removeBagFromList(bagID);
        updateStatus(bagID, "Picked Up by Passenger.");
    }

    void checkForBagsLeftOnCarousel(){ 
        if (baggageCarousel.isEmpty()){
            return;
        }
        else{
            moveFromCarouselToUnclaimed(baggageCarousel.returnHead());
        }
    }

    void moveHeadToEndCarousel(){baggageCarousel.moveHeadToEnd();}

    void moveFromCarouselToUnclaimed(string bagID){ 
        removeBagFromCarousel(bagID);
        addBagToUnclaimedList(bagID);
    }

    void updateStatus(string bagID, string newLocation){
        bagStatusMap[bagID] = newLocation;
        if (newLocation == "On Plane"){notifyPassenger(bagID, "Plane");}
        else{notifyPassenger(bagID, newLocation);}
        ofstream logFile("simulation_log.txt", ios::app);
        if (logFile.is_open()){
            logFile << "Bag ID " << bagID << " status updated to: " << newLocation << endl;
            logFile.close();
        } 
        else{cerr << "Error: Unable to open log file." << endl;}
    }

    int getCarouselSize(){return baggageCarousel.size();}

    void viewUnclaimedList(){unclaimedBaggage.viewList();}

    bool checkInUnclaimedList(string bagID){
        if (unclaimedBaggage.checkInList(bagID)){return true;}
        else{return false;}
    }

    void viewCarousel(){baggageCarousel.viewList();} //done

    string getStatus(string bagID){
        if (bagStatusMap.find(bagID) != bagStatusMap.end()){return bagStatusMap[bagID];} 
        else {return "null";}
    }

    void viewStatus(string bagID){
        if (bagStatusMap.find(bagID) != bagStatusMap.end()){ //Claude AI wrote this for me
            cout << "Bag ID: " << bagID << " - Current Status: " << bagStatusMap[bagID] << endl;
        } 
        else {cout << "No status found for Bag ID: " << bagID << endl;}
    }

    void moveBagToEndOfCarousel(){baggageCarousel.moveHeadToEnd();}

    void notifyPassenger(string bagID, string message){
        cout << "Passenger Notification: Bag " << bagID << " is now in the " << message << "." << endl;
    } 
};

Bags globalBags;

class AirportEmployeeUI{
    private:
    map<string, string> employeeLogins;

    public: 
    void run(){
        string username;
        string password;
        cout << "Enter username: ";
        cin >> username;
        cout << endl << "Enter password: ";
        cin >> password;
        if (login(username, password)){
            cout << endl << "Valid Login" << endl;
            if (username == "ADMIN"){
                while (true){
                    showAdminMenu();
                    if (menuReponse()){break;}
                }
            }
            else{
                while (true){
                    showMenu();
                    if (menuReponse()){break;}
                }
            }
        }
        else{
            cout << endl << "Invalid Login" << endl;
            cout << endl;
            return;
        }
        
    }

    bool login(string user, string pass) { //ChatGPT wrote this function for me
        auto it = employeeLogins.find(user);
        return (it != employeeLogins.end() && it->second == pass);
    }

    void removeUser(string user) {//ChatGPT wrote this function for me
        if (user == "ADMIN") {
            cout << "You can't remove the admin user." << std::endl;
            return;
        }

        auto it = employeeLogins.find(user); // Locate the user in the map
        if (it != employeeLogins.end()){ // If user exists
            string response;
            cout << "Are you sure you want to remove user: " << user << "? Enter Y or N: ";
            while (true) {
                cin >> response;
                if (response == "Y" || response == "y"){
                    employeeLogins.erase(it); // Remove the user from the map
                    cout << "User: " << user << " has been removed." << endl;
                    break;
                } 
                else if (response == "N" || response == "n"){
                    cout << "Okay! User was not removed." << endl;
                    return;
                } 
                else{cout << "Your response is invalid. Please enter Y or N: ";}
            }
        } 
        else {cout << "User " << user << " is not in the database." << endl;}
    }

    void addUser(string user, string pass){employeeLogins[user] = pass;} 

    void showMenu(){
        cout << "-----Employee Interface-----" << endl;
        cout << "1. Check in Bag(s)" << endl;
        cout << "2. Check Bag(s) Status & Location" << endl;
        cout << "3. Update Bag(s) Status & Location" << endl;
        cout << "4. Update Bag(s) Type" << endl;
        cout << "5. Add Bag(s) to Unclaimed Baggage List" << endl;
        cout << "6. View Unclaimed Baggage List" << endl;
        cout << "7. View Baggage Carousel" << endl;
        cout << "8. Move Priority Checked In Bags to Plane" << endl;
        cout << "9. Move Regular Checked In Bags to Plane" << endl;
        cout << "10. Move All Checked In Bags to Plane" << endl;
        cout << "11. Move All Bags from Plane to Carousel" << endl;
        cout << "12. Exit" << endl;
    }

    bool menuReponse(){
        string answer = "";
        cout << "Enter the menu number for the item in which you are wanting to access" << endl;
        cin >> answer;
        if (answer == "12"){return true;}
        else if (answer == "1"){
            int numberOfBags;
            cout << "How many bags: " << endl;
            cin >> numberOfBags;
            menuOption1(numberOfBags);
        }

        else if (answer == "2"){
            int numberOfBags;
            cout << "How many bags: " << endl;
            cin >> numberOfBags;
            menuOption2(numberOfBags);
        }

        else if (answer == "3"){
            int numberOfBags;
            cout << "How many bags: " << endl;
            cin >> numberOfBags;
            menuOption3(numberOfBags);
        }

        else if (answer == "4"){
            int numberOfBags;
            cout << "How many bags: " << endl;
            cin >> numberOfBags;
            menuOption4(numberOfBags);
        }

        else if (answer == "5"){
            int numberOfBags;
            cout << "How many bags: " << endl;
            cin >> numberOfBags;
            menuOption5(numberOfBags);
        }

        else if (answer == "6"){menuOption6();}

        else if (answer == "7"){menuOption7();}
        else if (answer == "8"){menuOption8();}
        else if (answer == "9"){menuOption9();}
        else if (answer == "10"){
            menuOption9();
            menuOption8();
        }
        else if (answer == "11"){menuOption11();}
        else if (answer == "13"){menuOption13();}
        else{
            cout << "Invalid Reponse" << endl;
            menuReponse();
        }

        return false;
    }

    void showAdminMenu(){
        cout << "-----Admin Interface-----" << endl;
        cout << "1. Check in Bag(s)" << endl;
        cout << "2. Check Bag(s) Status & Location" << endl;
        cout << "3. Update Bag(s) Status & Location" << endl;
        cout << "4. Update Bag(s) Type" << endl;
        cout << "5. Add Bag(s) to Unclaimed Baggage List" << endl;
        cout << "6. View Unclaimed Baggage List" << endl;
        cout << "7. View Baggage Carousel" << endl;
        cout << "8. Move Priority Checked In Bags to Plane" << endl;
        cout << "9. Move Regular Checked In Bags to Plane" << endl;
        cout << "10. Move All Checked In Bags to Plane" << endl;
        cout << "11. Move All Bags from Plane to Carousel" << endl;
        cout << "12. Exit" << endl;
        cout << "13. Add New User" << endl;
    }

    void menuOption1(int numberOfBags){
        for (int i = 0; i < numberOfBags; i++){globalBags.checkIn();}
    }

    void menuOption2(int numberOfBags){
        for (int i = 0; i < numberOfBags; i++){
            string bag;
            cout << "Enter Bag ID: ";
            cin >> bag;
            globalBags.viewStatus(bag);
        }
    }

    void menuOption3(int numberOfBags){ 
        for (int i = 0; i < numberOfBags; i++){
            string bag;
            string response;
            cout << "Enter Bag ID: ";
            cin >> bag;
            if (globalBags.getStatus(bag) == "Regular Checked Bag Queue" || globalBags.getStatus(bag) == "Priority Queue" || globalBags.getStatus(bag) == "On Plane"){
                cout << "You cannot update the status of Bag with ID: " << bag << " right now." << endl;
                return;
            }
            cout << "What do you want the new status to be? " << endl;
            cout << "1. In Unclaimed Baggage" << endl;
            cout << "2. Bag Picked Up" << endl;
            cin >> response;
            if (globalBags.getStatus(bag) == "On Baggage Carousel" & response == "1"){globalBags.moveFromCarouselToUnclaimed(bag);}
            else if (globalBags.getStatus(bag) == "On Baggage Carousel" & response == "2"){globalBags.removeBagFromCarousel(bag);}
            else if (globalBags.getStatus(bag) == "null"){cout << "Bag Not Found" << endl;}
        }
    }

    void menuOption4(int numberOfBags){
        for (int i = 0; i < numberOfBags; i++){
            cout << "When prompted, please enter what you'd like the bag to be changed to." << endl;
            globalBags.checkIn();
        }
    }

    void menuOption5(int numberOfBags){
        string bagID;
        for (int i = 0; i < numberOfBags; i++){
            cout << "Please enter one Bag ID: " << endl;
            cin >> bagID;
            globalBags.removeBagFromCarousel(bagID);
            globalBags.addBagToUnclaimedList(bagID);
        }
    }

    void menuOption6(){globalBags.viewUnclaimedList();} 

    void menuOption7(){globalBags.viewCarousel();} 

    void menuOption8(){globalBags.moveFromPriorityToPlane();} 

    void menuOption9(){globalBags.moveFromRegularToPlane();} 

    void menuOption11(){globalBags.moveFromPlaneToCarousel();}

    void menuOption13(){
        string user;
        string pass;
        cout << "Enter username: ";
        cin >> user;
        cout << endl << "Enter password: ";
        cin >> pass;
        addUser(user, pass);
        cout << endl << "User Added" << endl;
    }
};

class PassengerUI{
    public:
    bool login(string bagID){
        if (!globalBags.emptyCarryOnBags()){ 
            if (globalBags.checkInCarryOns(bagID)){
                cout << "Login Successful!" << endl;
                return true;
            }
        }
        if (globalBags.emptyCheckedInBags()){
            cout << "Login Failed. Invalid Bag ID." << endl;
            return false;
        } 
        else{
            if(globalBags.checkInCheckedInBags(bagID)){
                cout << "Login Successful!" << endl;
                return true;
            } 
            else{
                cout << "Login Failed. Invalid Bag ID." << endl;
                return false;
            }
        }
    }

    void showMenu(){
        cout << "-----Passenger Interface-----" << endl;
        cout << "1. Check Bag Status" << endl;
        cout << "2. Check Unclaimed Baggage" << endl;
        cout << "3. Report Bag Concerns" << endl;
        cout << "4. Exit" << endl;
    }

    void menuResponse(string bagID){
        while (true){
            showMenu();
            string choice;
            cout << "Enter your choice (1-5): ";
            cin >> choice;
            if (choice == "1"){checkBagStatus(bagID);}
            else if (choice == "2"){checkUnclaimedBaggage(bagID);}
            else if (choice == "3"){reportBagConcerns(bagID);}
            else if (choice == "4"){
                cout << "Thank you for using our service. Goodbye!" << endl;
                break;
            }
            else{cout << "Invalid choice. Please try again." << endl;}
        }
    }

    void checkBagStatus(string bagID){globalBags.viewStatus(bagID);}

    void checkUnclaimedBaggage(string bagID){
        cout << "Unclaimed Baggage List:" << endl;
        globalBags.checkInUnclaimedList(bagID);
    }

    void reportBagConcerns(string bagID){
        int choice;
        cout << "Bag Concerns Menu:" << endl;
        cout << "1. Bag is Missing" << endl;
        cout << "2. Bag is Damaged" << endl;
        cout << "3. Incorrect Bag Received" << endl;
        cout << "4. Other Concerns" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        switch(choice) { //Claude AI wrote the switch cases fr me 
            case 1:
                globalBags.addBagToUnclaimedList(bagID);
                globalBags.removeBagFromCarousel(bagID);
                cout << "Your bag has been reported as missing. Our staff will investigate." << endl;
                break;
            case 2:
                cout << "We're sorry to hear your bag is damaged. Please speak with our customer service." << endl;
                break;
            case 3:
                cout << "Please contact our staff to resolve the bag mix-up." << endl;
                break;
            case 4:
                cout << "Please describe your concern to our customer service representative." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    }

    void run(){
        while (true){
            cout << "-----Passenger Login-----" << endl;
            string bagID;
            cout << "Enter Bag ID Number: ";
            cin >> bagID;
            if (login(bagID)){
                menuResponse(bagID);
                break;
            } 
            else{break;}
        }
    }
};

class Simulation{
    private:
    static const int MAX_BAGS = 150; 
    string carryOnBags[MAX_BAGS];
    int carryOnCount = 0;
    int priorityCount = 0;
    int regularCount = 0;
    AirportEmployeeUI ui;

    public:
    void inputCarryOnBags(){
        int numBags;
        cout << "How many Carry-On Bags will be processed?" << endl;
        while (true){
            cout << "Must be less than 150" << endl;
            cin >> numBags;
            if(numBags > 150){continue;}
            else{break;}
        }
        carryOnCount = numBags;
        for (int i = 0; i < numBags; i++){
            string bag;
            cout << "Enter Carry-On Bag ID " << i + 1 << ": ";
            cin >> bag;
            carryOnBags[i] = bag;
            globalBags.addToCarryOns(bag);
            globalBags.updateStatus(bag, "Carry On");
        }
    }

    void inputPriorityBags(){
        int numBags;
        cout << "How many Priority Bags will be processed?" << endl;
        while (true){
            cout << "Must be less than 150" << endl;
            cin >> numBags;
            if(numBags > 150){continue;}
            else{break;}
        }
        priorityCount = numBags;
        for (int i = 0; i < numBags; i++){
            string bag;
            cout << "Enter Priority Bag ID " << i + 1 << ": ";
            cin >> bag;
            globalBags.addToPriorityQueue(bag);
            globalBags.addToCheckedInBags(bag);
        }
    }

    void inputRegularBags() {
        int numBags;
        cout << "How many Regular Bags will be processed?" << endl;
        while (true){
            cout << "Must be less than 150" << endl;
            cin >> numBags;
            if(numBags > 150){continue;}
            else{break;}
        }
        regularCount = numBags;
        for (int i = 0; i < numBags; i++){
            string bag;
            cout << "Enter Regular Bag ID " << i + 1 << ": ";
            cin >> bag;
            globalBags.addToRegularQueue(bag);
            globalBags.addToCheckedInBags(bag);
            globalBags.updateStatus(bag, "Checked In");
        }
    }

    void processCarryOnBags(){
        cout << "--- Processing Carry-On Bags ---" << endl;
        for (int i = 0; i < carryOnCount; i++){
            globalBags.CarryOnBagSecurityCheck(carryOnBags[i]);
            globalBags.addBagToOverheadBin(carryOnBags[i]);
            globalBags.updateStatus(carryOnBags[i], "Carry On with Passenger");
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }

    void processPriorityBags(){
        cout << "--- Processing Priority Bags ---" << endl;
            ui.menuOption8();
            this_thread::sleep_for(chrono::milliseconds(500));
    }

    void processRegularBags(){
        cout << "--- Processing Regular Bags ---" << endl;
        ui.menuOption9();
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    void moveToCarousel(){
        cout << "--- Moving Bags to Carousel ---" << endl;
        globalBags.moveFromPlaneToCarousel();
    }

    void randomlyPickUpBags(){
        cout << "--- Picking Up Bags from Carousel ---" << endl;
        if (globalBags.isCarouselEmpty()){
            cout << "No bags on the carousel to pick up." << endl;
            return;
        }
        int initialSize = globalBags.getCarouselSize(); 
        for (int i = 0; i < initialSize; i++) {
            string bagID = globalBags.peekCarousel(); 
            int randomChance = rand() % 100;        
            if (randomChance < 95){ 
                cout << "Bag ID: " << bagID << " picked up by passenger." << endl;
                globalBags.removeBagFromCarousel(bagID); 
            } 
            else{
                cout << "Bag ID: " << bagID << " remains on the carousel." << endl;
                cout << "Bag ID: " << bagID << " moved to unclaimed baggage." << endl;
                globalBags.moveFromCarouselToUnclaimed(bagID);
            }
        }
    }

    void checkAndMoveToUnclaimedBags(){
        cout << "--- Checking for Unclaimed Bags on Carousel ---" << endl;
        string bagID;
        while (!globalBags.emptyCheckedInBags()){
            bagID = globalBags.peekCarousel();
            cout << "Bag ID: " << bagID << " still on the carousel. Moving to unclaimed baggage." << endl;
            globalBags.moveFromCarouselToUnclaimed(bagID);
        }
    }

    void runSimulation(){
        carryOnCount = 0;
        priorityCount = 0;
        regularCount = 0;

        inputCarryOnBags();
        inputPriorityBags();
        inputRegularBags();

        processCarryOnBags();
        processRegularBags();
        processPriorityBags();

        moveToCarousel();
        randomlyPickUpBags();

        cout << "--- Simulation Complete ---" << endl;
        globalBags.logEvent("Full Baggage Handling Simulation Completed");
    }
};

int main(){
    srand(time(nullptr));
    AirportEmployeeUI employeeUI;
    PassengerUI passengerUI;
    Simulation sim;
    
    globalBags.createArray();

    employeeUI.addUser("ADMIN", "Pass123"); //creates admin account
    while (true){
        cout << "1. Employee Login" << endl;
        cout << "2. Passenger Login" << endl;
        cout << "3. Run Baggage Simulation" << endl;
        cout << "4. Exit" << endl;
        string response;
        cin >> response;
        if (response == "1"){employeeUI.run();}
        else if(response == "2"){passengerUI.run();}
        else if(response == "3"){sim.runSimulation();}
        else if(response == "4"){break;}
        else{cout << "Invalid Reponse." << endl;}
    }
    
    return 0;
}