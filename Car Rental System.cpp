#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_FLEET_SIZE = 100;
const double DAMAGE_CHARGE = 5000.0;

class Vehicle {
public:
    virtual void display() const = 0;
    virtual double getRate() const = 0;
    virtual string getType() const = 0;
    virtual string getMake() const = 0;
    virtual string getModel() const = 0;
    virtual string getYear() const = 0;
    virtual void rentVehicle() = 0;
    virtual void returnVehicle() = 0;
    virtual bool isRentedOut() const = 0;
    virtual ~Vehicle() {} // Ensure a virtual destructor for proper cleanup
};

class Car : public Vehicle {
private:
    string type;
    string make;
    string model;
    string year;
    double dailyRate;
    bool isRented;
public:
    Car() : type(""), make(""), model(""), year(""), dailyRate(0.0), isRented(false) {}

    Car(string tp, string mk, string mdl, string yr, double rate) 
        : type(tp), make(mk), model(mdl), year(yr), dailyRate(rate), isRented(false) {}

    void display() const override {
        cout << "Type: " << type << ", Make: " << make << ", Model: " << model 
             << ", Year: " << year << ", Rate per day: Rs" << dailyRate << endl;
    }

    double getRate() const override {
        return dailyRate;
    }

    string getType() const override {
        return type;
    }

    string getMake() const override {
        return make;
    }

    string getModel() const override {
        return model;
    }

    string getYear() const override {
        return year;
    }

    void rentVehicle() override {
        isRented = true;
    }

    void returnVehicle() override {
        isRented = false;
    }

    bool isRentedOut() const override {
        return isRented;
    }
};

class Customer {
private:
    string name;
    string phone;
    string cnic;
public:
    Customer() : name(""), phone(""), cnic("") {}

    Customer(string nm, string ph, string cn) : name(nm), phone(ph), cnic(cn) {}

    void display() const {
        cout << "Customer Name: " << name << ", Phone: " << phone << ", CNIC: " << cnic << endl;
    }

    string getName() const {
        return name;
    }

    string getPhone() const {
        return phone;
    }

    string getCNIC() const {
        return cnic;
    }
};

class RentalSystem {
private:
    Vehicle* fleet[MAX_FLEET_SIZE];
    int fleetSize;
    Customer* rentedCustomers[MAX_FLEET_SIZE];
    int rentalDays[MAX_FLEET_SIZE];
    bool damage[MAX_FLEET_SIZE];

public:
    RentalSystem() : fleetSize(0) {
        for (int i = 0; i < MAX_FLEET_SIZE; ++i) {
            fleet[i] = 0;
            rentedCustomers[i] = 0;
            rentalDays[i] = 0;
            damage[i] = false;
        }
        loadFleet();
    }

    ~RentalSystem() {
        for (int i = 0; i < fleetSize; ++i) {
            delete fleet[i];
            delete rentedCustomers[i];
        }
    }

    void addVehicleToFleet() {
        if (fleetSize >= MAX_FLEET_SIZE) {
            cout << "Fleet is full. Cannot add more vehicles." << endl;
            return;
        }
        string type, make, model, year;
        double rate;

        cin.ignore();
        cout << "Enter vehicle type (Sedan/SUV): ";
        getline(cin, type);
        cout << "Enter vehicle make: ";
        getline(cin, make);
        cout << "Enter vehicle model: ";
        getline(cin, model);
        cout << "Enter vehicle year: ";
        getline(cin, year);
        cout << "Enter vehicle daily rate: ";
        cin >> rate;

        fleet[fleetSize++] = new Car(type, make, model, year, rate);
        saveFleet();
    }

    void displayAvailableVehicles() const {
        cout << "Available Vehicles:" << endl;
        for (int i = 0; i < fleetSize; ++i) {
            if (!fleet[i]->isRentedOut()) {
                cout << i + 1 << ". ";
                fleet[i]->display();
            }
        }
    }

    void rentVehicle(int vehicleIndex, const Customer& customer, int days) {
        if (vehicleIndex < 1 || vehicleIndex > fleetSize) {
            cout << "Invalid vehicle selection." << endl;
            return;
        }
        Vehicle* vehicle = fleet[vehicleIndex - 1];
        if (vehicle->isRentedOut()) {
            cout << "Vehicle is already rented out." << endl;
            return;
        }
        vehicle->rentVehicle();
        rentedCustomers[vehicleIndex - 1] = new Customer(customer);
        rentalDays[vehicleIndex - 1] = days;
        damage[vehicleIndex - 1] = false; // Assuming no damage initially
        cout << "Vehicle rented successfully." << endl;
    }

    void returnVehicle(int vehicleIndex, bool vehicleDamage) {
        if (vehicleIndex < 1 || vehicleIndex > fleetSize) {
            cout << "Invalid vehicle selection." << endl;
            return;
        }
        Vehicle* vehicle = fleet[vehicleIndex - 1];
        if (!vehicle->isRentedOut()) {
            cout << "Vehicle is not rented out." << endl;
            return;
        }
        vehicle->returnVehicle();
        damage[vehicleIndex - 1] = vehicleDamage;

        double totalCharge = vehicle->getRate() * rentalDays[vehicleIndex - 1];
        if (vehicleDamage) {
            totalCharge += DAMAGE_CHARGE;
        }
        printReceipt(vehicleIndex - 1, totalCharge);
        delete rentedCustomers[vehicleIndex - 1];
        rentedCustomers[vehicleIndex - 1] = 0;
        rentalDays[vehicleIndex - 1] = 0;
        damage[vehicleIndex - 1] = false;
    }

    void displayRentedVehicles() const {
        cout << "Vehicles currently on rent:" << endl;
        bool hasRentedVehicles = false;
        for (int i = 0; i < fleetSize; ++i) {
            if (fleet[i]->isRentedOut()) {
                cout << "Vehicle " << i + 1 << ":" << endl;
                fleet[i]->display();
                rentedCustomers[i]->display();
                cout << "Rental Days: " << rentalDays[i] << endl;
                hasRentedVehicles = true;
            }
        }
        if (!hasRentedVehicles) {
            cout << "No vehicles currently on rent." << endl;
        }
    }

    void displayReceipts() const {
        ifstream inFile("receipts.txt");
        if (!inFile.is_open()) {
            cout << "Unable to open receipts file or file does not exist." << endl;
            return;
        }
        string line;
        cout << "\n--- Receipts ---" << endl;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        cout << "----------------" << endl;
        inFile.close();
    }

    void saveFleet() const {
        ofstream outFile("fleet.txt");
        for (int i = 0; i < fleetSize; ++i) {
            outFile << fleet[i]->getType() << "," << fleet[i]->getMake() << "," << fleet[i]->getModel() << "," 
                    << fleet[i]->getYear() << "," << fleet[i]->getRate() << endl;
        }
        outFile.close();
    }

    void loadFleet() {
        ifstream inFile("fleet.txt");
        if (!inFile.is_open()) {
            cout << "Unable to open fleet file. Starting with empty fleet." << endl;
            return;
        }
        string type, make, model, year, rateStr;
        while (getline(inFile, type, ',')) {
            getline(inFile, make, ',');
            getline(inFile, model, ',');
            getline(inFile, year, ',');
            getline(inFile, rateStr);
            double rate;// = stod(rateStr);
            fleet[fleetSize++] = new Car(type, make, model, year, rate);
        }
        inFile.close();
    }

    void printReceipt(int vehicleIndex, double totalCharge) {
        ofstream outFile("receipts.txt", ios::app);
        if (!outFile.is_open()) {
            cout << "Error opening receipts file." << endl;
            return;
        }
        outFile << "--- Receipt ---" << endl;
        outFile << "Customer Name: " << rentedCustomers[vehicleIndex]->getName() << ", Phone: " << rentedCustomers[vehicleIndex]->getPhone() 
                << ", CNIC: " << rentedCustomers[vehicleIndex]->getCNIC() << endl;
        outFile << "Vehicle: " << fleet[vehicleIndex]->getType() << ", Make: " << fleet[vehicleIndex]->getMake() 
                << ", Model: " << fleet[vehicleIndex]->getModel() << ", Year: " << fleet[vehicleIndex]->getYear() 
                << ", Rate per day: Rs" << fleet[vehicleIndex]->getRate() << endl;
        outFile << "Days Rented: " << rentalDays[vehicleIndex] << endl;
        outFile << "Damage Charge: " << (damage[vehicleIndex] ? DAMAGE_CHARGE : 0) << endl;
        outFile << "Total Charge: Rs" << totalCharge << endl;
        outFile << "----------------" << endl;
        outFile.close();
        rentedCustomers[vehicleIndex]->display();
        fleet[vehicleIndex]->display();
        cout << "Days Rented: " << rentalDays[vehicleIndex] << endl;
        cout << "Damage Charge: " << (damage[vehicleIndex] ? DAMAGE_CHARGE : 0) << endl;
        cout << "Total Charge: Rs" << totalCharge << endl;
        cout << "----------------" << endl;
    }
};

int main() {
    RentalSystem rentalSystem;
    char choice;
    string adminUsername = "admin";
    string adminPassword = "password";
    while (true) {
        cout << "Welcome to Car Rental System! " << endl;
        cout << "1. Enter 'admin' to login as administrator" << endl;
        cout << "2. Enter 'customer' to continue as customer" << endl;
        cout << "3. Enter 'exit' to exit" << endl;
        string loginChoice;
        cin >> loginChoice;

        if (loginChoice == "admin") {
            string username, password;
            cout << "Enter administrator username: ";
            cin >> username;
            cout << "Enter administrator password: ";
            cin >> password;

            if (username == adminUsername && password == adminPassword) {
                while (true) {
                    cout << "\n--- Administrator Menu ---" << endl;
                    cout << "1. Add Vehicle to Fleet" << endl;
                    cout << "2. Display Available Vehicles" << endl;
                    cout << "3. Display All Receipts" << endl;
                    cout << "4. Logout" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice) {
                        case '1':
                            rentalSystem.addVehicleToFleet();
                            break;
                        case '2':
                            rentalSystem.displayAvailableVehicles();
                            break;
                        case '3':
                            rentalSystem.displayReceipts();
                            break;
                        case '4':
                            goto adminLogout;
                        default:
                            cout << "Invalid choice. Please try again." << endl;
                    }
                }
                adminLogout:;
            } else {
                cout << "Invalid administrator credentials." << endl;
            }
        } else if (loginChoice == "customer") {
            string name, phone, cnic;
            cin.ignore();
            cout << "Enter your name: ";
            getline(cin, name);
            cout << "Enter your phone number: ";
            getline(cin, phone);
            cout << "Enter your CNIC: ";
            getline(cin, cnic);

            Customer customer(name, phone, cnic);
            while (true) {
                cout << "\n--- Customer Menu ---" << endl;
                cout << "1. Display Available Vehicles" << endl;
                cout << "2. Rent a Vehicle" << endl;
                cout << "3. Return a Vehicle" << endl;
                cout << "4. Display Vehicles Currently on Rent" << endl;
                cout << "5. Logout" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case '1':
                        rentalSystem.displayAvailableVehicles();
                        break;
                    case '2': {
                        int vehicleIndex, days;
                        cout << "Enter the vehicle number you want to rent: ";
                        cin >> vehicleIndex;
                        cout << "Enter the number of days you want to rent: ";
                        cin >> days;
                        rentalSystem.rentVehicle(vehicleIndex, customer, days);
                        break;
                    }
                    case '3': {
                        bool vehicleDamage;
                        cout << "Is there any damage to the vehicle? (1 for Yes, 0 for No): ";
                        cin >> vehicleDamage;
                        cout << "Enter the vehicle number you want to return: ";
                        int vehicleIndex;
                        cin >> vehicleIndex;
                        rentalSystem.returnVehicle(vehicleIndex, vehicleDamage);
                        break;
                    }
                    case '4':
                        rentalSystem.displayRentedVehicles();
                        break;
                    case '5':
                        goto customerLogout;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                }
            }
            customerLogout:;
        } else if (loginChoice == "exit") {
            break;
        } else {
            cout << "Invalid choice. Please try again." << endl;
        }
    }
    return 0;
}

