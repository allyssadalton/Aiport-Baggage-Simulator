#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <random>
#include <chrono>
#include <algorithm>
#include <functional>
#include <limits>
#include <iomanip>
#include <cctype>
#include <cstdlib>

using namespace std;

// Forward declarations
class Bag;
class Flight;
class Passenger;
class Timer;
class BaggageSystem;

// Timer class to manage simulated time
class Timer {
private:
    int currentTime; // Time in minutes

public:
    Timer() : currentTime(0) {}

    void advanceTime(int minutes) {currentTime += minutes;}

    int getCurrentTime() const {return currentTime;}

    string getFormattedTime() const {
        int hours = currentTime / 60;
        int minutes = currentTime % 60;
        return to_string(hours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes);
    }
};

// Enhanced Bag class combining both implementations
class Bag {
private:
    string bagID;
    string status;
    string location;
    string passengerID;
    string flightID;
    bool priority;
    bool lost;

public:
    Bag(const string& id, const string& pID, const string& fID, bool isPriority)
        : bagID(id)
        , status("Checked")
        , location("Counter")
        , passengerID(pID)
        , flightID(fID)
        , priority(isPriority)
        , lost(false) {}

    void updateStatus(const string& newStatus) {status = newStatus;}

    void updateLocation(const string& newLocation) {location = newLocation;}

    void markAsLost() {
        lost = true;
        status = "Lost";
    }

    // Getters
    string getBagID() const {return bagID;}
    string getStatus() const {return status;}
    string getLocation() const {return location;}
    string getPassengerID() const {return passengerID;}
    string getFlightID() const {return flightID;}
    bool isPriority() const {return priority;}
    bool isLost() const {return lost;}
};

// Flight class
class Flight {
private:
    string flightID;
    string departurePlace;
    string arrivalPlace;
    string departureTime;
    string arrivalTime;
    string status;
    vector<Bag*> checkedBags;

public:
    Flight(const string& id, const string& dep, const string& arr,
           const string& depTime, const string& arrTime)
        : flightID(id)
        , departurePlace(dep)
        , arrivalPlace(arr)
        , departureTime(depTime)
        , arrivalTime(arrTime)
        , status("Scheduled") {}

    void addBag(Bag* bag) {checkedBags.push_back(bag);}

    void removeBag(const string& bagID) {
        checkedBags.erase(
            remove_if(checkedBags.begin(), checkedBags.end(),[&bagID](const Bag* bag) {return bag->getBagID() == bagID;}),
            checkedBags.end()
        );
    }

    void updateStatus(const std::string& newStatus) {status = newStatus;}

    // Getters
    string getFlightID() const {return flightID;}
    string getStatus() const {return status;}
    const vector<Bag*>& getCheckedBags() const {return checkedBags;}
};

// Passenger class
class Passenger {
private:
    string passengerID;
    std::string name;
    std::vector<Bag*> bags;

public:
    Passenger(const std::string& id, const std::string& n)
        : passengerID(id), name(n) {}

    void addBag(Bag* bag) {
        bags.push_back(bag);
    }

    // Getters
    std::string getPassengerID() const { return passengerID; }
    std::string getName() const { return name; }
    const std::vector<Bag*>& getBags() const { return bags; }
};

// Enhanced BaggageSystem class
class BaggageSystem {
private:
    std::map<std::string, Flight*> flights;
    std::map<std::string, Passenger*> passengers;
    std::priority_queue<
        Bag*,
        std::vector<Bag*>,
        std::function<bool(Bag*, Bag*)>
    > processingQueue;
    std::vector<Bag*> loadedBags;
    std::vector<Bag*> lostBags;
    std::vector<Bag*> unclaimedBags;
    Timer timer;
    
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<> errorDist;

    static constexpr double ERROR_RATE = 0.05;

public:
    void displayPassengerBags(const std::string& passengerID) {
        auto it = passengers.find(passengerID);
        if (it != passengers.end()) {
            const auto& bags = it->second->getBags();
            std::cout << "\nBags for passenger " << it->second->getName() << ":\n";
            if (bags.empty()) {
                std::cout << "No bags checked in.\n";
            } else {
                for (const Bag* bag : bags) {
                    std::cout << "\nBag ID: " << bag->getBagID() << std::endl
                          << "Status: " << bag->getStatus() << std::endl
                          << "Location: " << bag->getLocation() << std::endl
                          << "Flight: " << bag->getFlightID() << std::endl
                          << "Priority: " << (bag->isPriority() ? "Yes" : "No") << std::endl;
                }
            }
        } else {
            std::cout << "Passenger not found.\n";
        }
    }
    BaggageSystem() 
        : processingQueue(
            [](Bag* a, Bag* b) { return !a->isPriority() && b->isPriority(); }
        )
        , gen(rd())
        , errorDist(0.0, 1.0) {}

    void addFlight(Flight* flight) {
        flights[flight->getFlightID()] = flight;
    }

    void addPassenger(Passenger* passenger) {
        passengers[passenger->getPassengerID()] = passenger;
    }

    Bag* checkInBag(const std::string& passengerID, 
                    const std::string& flightID,
                    bool priority) {
        static int bagCounter = 1;
        std::string bagID = "BAG" + std::to_string(bagCounter++);
        
        Bag* newBag = new Bag(bagID, passengerID, flightID, priority);
        passengers[passengerID]->addBag(newBag);
        flights[flightID]->addBag(newBag);
        processingQueue.push(newBag);
        
        return newBag;
    }

    void processBags() {
        while (!processingQueue.empty()) {
            Bag* bag = processingQueue.top();
            processingQueue.pop();

            timer.advanceTime(5); // 5 minutes per bag processing

            if (errorDist(gen) < ERROR_RATE) {
                bag->markAsLost();
                lostBags.push_back(bag);
                std::cout << "Bag " << bag->getBagID() << " has been lost" << std::endl;
            } else {
                loadedBags.push_back(bag);
                bag->updateLocation("Loaded");
                bag->updateStatus("Loaded");
            }
        }
    }

    void simulateFlightArrival(const std::string& flightID) {
        Flight* flight = flights[flightID];
        flight->updateStatus("Arrived");
        timer.advanceTime(15); // 15 minutes for plane to taxi and park
        
        std::cout << "Flight " << flightID << " arrived at " 
                  << timer.getFormattedTime() << std::endl;
        
        processBaggageCarousel(flightID);
    }

    void processBaggageCarousel(const std::string& flightID) {
        Flight* flight = flights[flightID];
        std::cout << "Processing bags for flight " << flightID << std::endl;

        for (Bag* bag : flight->getCheckedBags()) {
            if (!bag->isLost()) {
                if (errorDist(gen) < ERROR_RATE) {
                    bag->updateStatus("Delayed");
                    timer.advanceTime(30);
                    std::cout << "Bag " << bag->getBagID() << " delayed" << std::endl;
                }
                bag->updateLocation("Carousel");
                timer.advanceTime(5);
            }
        }
    }

    void displayBagStatus(const std::string& bagID) {
        for (const auto& flight : flights) {
            for (const Bag* bag : flight.second->getCheckedBags()) {
                if (bag->getBagID() == bagID) {
                    std::cout << "Bag ID: " << bag->getBagID() << std::endl
                              << "Status: " << bag->getStatus() << std::endl
                              << "Location: " << bag->getLocation() << std::endl
                              << "Flight: " << bag->getFlightID() << std::endl
                              << "Priority: " << (bag->isPriority() ? "Yes" : "No") << std::endl;
                    return;
                }
            }
        }
        std::cout << "Bag not found." << std::endl;
    }

    void displaySystemStatus() const {
        std::cout << "=== Baggage System Status === Time: " 
                  << timer.getFormattedTime() << std::endl;
        std::cout << "Bags in processing queue: " << processingQueue.size() << std::endl;
        std::cout << "Bags loaded: " << loadedBags.size() << std::endl;
        std::cout << "Lost bags: " << lostBags.size() << std::endl;
        std::cout << "Unclaimed bags: " << unclaimedBags.size() << std::endl;
        
        if (!lostBags.empty()) {
            std::cout << "\nLost Bags Details:\n";
            for (const Bag* bag : lostBags) {
                std::cout << "Bag ID: " << bag->getBagID()
                         << ", Flight: " << bag->getFlightID()
                         << ", Priority: " << (bag->isPriority() ? "Yes" : "No")
                         << std::endl;
            }
        }
    }

    ~BaggageSystem() {
        // Cleanup allocated memory
        for (auto& flight : flights) {
            delete flight.second;
        }
        for (auto& passenger : passengers) {
            delete passenger.second;
        }
        // Cleanup bags in various containers
        for (Bag* bag : loadedBags) delete bag;
        for (Bag* bag : lostBags) delete bag;
        for (Bag* bag : unclaimedBags) delete bag;
        while (!processingQueue.empty()) {
            delete processingQueue.top();
            processingQueue.pop();
        }
    }
};

// Add these after the existing includes


// Add these user interface classes before the main() function

class UserInterface {
protected:
    BaggageSystem& system;

    void clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            //system("clear");
        #endif
    }

    void waitForEnter() {
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string getInput(const std::string& prompt) {
        std::string input;
        std::cout << prompt;
        std::getline(std::cin, input);
        return input;
    }

public:
    UserInterface(BaggageSystem& sys) : system(sys) {}
    virtual void showMenu() = 0;
    virtual ~UserInterface() = default;
};

class PassengerInterface : public UserInterface {
private:
    std::string passengerID;
    
    void checkInBaggage() {
        std::string flightID = getInput("Enter your flight number: ");
        
        std::string priorityInput;
        bool priority = false;
        do {
            priorityInput = getInput("Do you want priority baggage handling? (y/n): ");
        } while (priorityInput != "y" && priorityInput != "n");
        
        priority = (priorityInput == "y");
        
        Bag* bag = system.checkInBag(passengerID, flightID, priority);
        std::cout << "\nBag checked in successfully!\n";
        std::cout << "Your bag ID is: " << bag->getBagID() << "\n";
        
        waitForEnter();
    }
    
    void trackBaggage() {
        std::string bagID = getInput("Enter your bag ID: ");
        system.displayBagStatus(bagID);
        waitForEnter();
    }

public:
    PassengerInterface(BaggageSystem& sys) : UserInterface(sys) {
        clearScreen();
        std::cout << "=== Passenger Login ===\n";
        passengerID = getInput("Enter your Passenger ID: ");
    }

    void showMenu() override {
        while (true) {
            clearScreen();
            std::cout << "=== Passenger Menu ===\n"
                      << "1. Check in baggage\n"
                      << "2. Track baggage\n"
                      << "3. View my bags\n"
                      << "4. Log out\n"
                      << "\nEnter your choice (1-4): ";

            std::string choice = getInput("");
            
            if (choice == "1") {
                checkInBaggage();
            }
            else if (choice == "2") {
                trackBaggage();
            }
            else if (choice == "3") {
                system.displayPassengerBags(passengerID);
                waitForEnter();
            }
            else if (choice == "4") {
                break;
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
                waitForEnter();
            }
        }
    }
};

class EmployeeInterface : public UserInterface {
private:
    std::string employeeID;
    
    void processFlightArrival() {
        std::string flightID = getInput("Enter flight number to process: ");
        system.simulateFlightArrival(flightID);
        waitForEnter();
    }
    
    void viewSystemStatus() {
        system.displaySystemStatus();
        waitForEnter();
    }
    
    void searchBag() {
        std::string bagID = getInput("Enter bag ID to search: ");
        system.displayBagStatus(bagID);
        waitForEnter();
    }
    
    void processBags() {
        system.processBags();
        std::cout << "Bags processed successfully!\n";
        waitForEnter();
    }

public:
    EmployeeInterface(BaggageSystem& sys) : UserInterface(sys) {
        clearScreen();
        std::cout << "=== Employee Login ===\n";
        employeeID = getInput("Enter your Employee ID: ");
    }

    void showMenu() override {
        while (true) {
            clearScreen();
            std::cout << "=== Employee Menu ===\n"
                      << "1. Process flight arrival\n"
                      << "2. View system status\n"
                      << "3. Search for bag\n"
                      << "4. Process bags in queue\n"
                      << "5. Add new flight\n"
                      << "6. Log out\n"
                      << "\nEnter your choice (1-6): ";

            std::string choice = getInput("");
            
            if (choice == "1") {
                processFlightArrival();
            }
            else if (choice == "2") {
                viewSystemStatus();
            }
            else if (choice == "3") {
                searchBag();
            }
            else if (choice == "4") {
                processBags();
            }
            else if (choice == "5") {
                addNewFlight();
            }
            else if (choice == "6") {
                break;
            }
            else {
                std::cout << "Invalid choice. Please try again.\n";
                waitForEnter();
            }
        }
    }

    void addNewFlight() {
        std::string flightID = getInput("Enter flight number: ");
        std::string departure = getInput("Enter departure city: ");
        std::string arrival = getInput("Enter arrival city: ");
        std::string depTime = getInput("Enter departure time (HH:MM): ");
        std::string arrTime = getInput("Enter arrival time (HH:MM): ");

        Flight* newFlight = new Flight(flightID, departure, arrival, depTime, arrTime);
        system.addFlight(newFlight);
        
        std::cout << "\nFlight added successfully!\n";
        waitForEnter();
    }
};

// Add these methods to the BaggageSystem class
/*
class BaggageSystem {
    // ... (previous code remains the same)

public:
    // Add these new methods
    
};
*/
// Replace the main() function with this new version

int main() {
    BaggageSystem system;

    // Pre-populate some test data
    Flight* flight1 = new Flight("FL001", "New York", "Los Angeles", "10:00", "13:00");
    Flight* flight2 = new Flight("FL002", "Los Angeles", "Chicago", "14:00", "19:00");
    system.addFlight(flight1);
    system.addFlight(flight2);

    Passenger* passenger1 = new Passenger("P001", "John Doe");
    Passenger* passenger2 = new Passenger("P002", "Jane Smith");
    system.addPassenger(passenger1);
    system.addPassenger(passenger2);

    while (true) {
        //system("clear");  // or system("cls") on Windows
        std::cout << "=== Airport Baggage System ===\n"
                  << "1. Passenger Login\n"
                  << "2. Employee Login\n"
                  << "3. Exit\n"
                  << "\nEnter your choice (1-3): ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            PassengerInterface passengerUI(system);
            passengerUI.showMenu();
        }
        else if (choice == "2") {
            EmployeeInterface employeeUI(system);
            employeeUI.showMenu();
        }
        else if (choice == "3") {
            std::cout << "Thank you for using the Airport Baggage System!\n";
            break;
        }
        else {
            std::cout << "Invalid choice. Please try again.\n";
            std::cout << "Press Enter to continue...";
            std::cin.get();
        }
    }

    return 0;
}
/*
int main() {
    BaggageSystem system;

    // Create flights
    Flight* flight1 = new Flight("FL001", "New York", "Los Angeles", "10:00", "13:00");
    Flight* flight2 = new Flight("FL002", "Los Angeles", "Chicago", "14:00", "19:00");
    system.addFlight(flight1);
    system.addFlight(flight2);

    // Create passengers
    Passenger* passenger1 = new Passenger("P001", "John Doe");
    Passenger* passenger2 = new Passenger("P002", "Jane Smith");
    Passenger* passenger3 = new Passenger("P003", "Bob Johnson");
    system.addPassenger(passenger1);
    system.addPassenger(passenger2);
    system.addPassenger(passenger3);

    // Check in bags with different priorities
    Bag* bag1 = system.checkInBag("P001", "FL001", true);  // Priority bag
    Bag* bag2 = system.checkInBag("P002", "FL001", false); // Regular bag
    Bag* bag3 = system.checkInBag("P003", "FL002", true);  // Priority bag

    // Display initial status
    system.displaySystemStatus();

    // Process bags
    system.processBags();
    system.displaySystemStatus();

    // Simulate flight arrivals
    system.simulateFlightArrival("FL001");
    system.simulateFlightArrival("FL002");

    // Display final status and individual bag statuses
    system.displaySystemStatus();
    system.displayBagStatus(bag1->getBagID());
    system.displayBagStatus(bag2->getBagID());
    system.displayBagStatus(bag3->getBagID());

    return 0;
}
*/