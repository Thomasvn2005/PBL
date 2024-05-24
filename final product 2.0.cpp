#include <iostream>
#include <string>
#include <vector>
#include <ctime> // For handling dates
#include <fstream> // For file handling
#include <limits> // For numeric limits
#include <algorithm>
using namespace std;
// Define structure for maintenance tasks
struct MaintenanceTask {
    string description;
    tm dueDate;
    bool completed;

    MaintenanceTask(string desc, tm due) : description(desc), dueDate(due), completed(false) {}
};

// Define structure for vehicle
struct Vehicle {
    string licensePlate;
    string brand;
    string color;
    bool available;
    string condition;
    vector<MaintenanceTask> maintenanceSchedule; // Move maintenance schedule here

    Vehicle(string lp, string b, string c, bool av, string cond)
        : licensePlate(lp), brand(b), color(c), available(av), condition(cond) {}
};

// Base class for car
class Car {
protected:
    string carType;
    vector<MaintenanceTask> maintenanceSchedule; // Maintenance schedule for each vehicle
public:
    Car(string carType) : carType(carType) {}
    virtual double dailyRentalRate() const = 0; // Pure virtual function for daily rental rate
    virtual ~Car() {} // Virtual destructor for proper cleanup
    string getcarType() const { return carType; }

    // Method to add maintenance task to the schedule
    void addMaintenanceTask(const string& description, const tm& dueDate) {
        maintenanceSchedule.push_back(MaintenanceTask(description, dueDate));
    }

    // Method to remove a maintenance task from the schedule based on description
    void removeMaintenanceTask(const string& description) {
        auto it = std::find_if(maintenanceSchedule.begin(), maintenanceSchedule.end(), [&](const MaintenanceTask& task) {
            return task.description == description;
        });
        if (it != maintenanceSchedule.end()) {
            maintenanceSchedule.erase(it);
            cout << "Maintenance task with description '" << description << "' removed successfully." << endl;
        } else {
            cout << "Maintenance task with description '" << description << "' not found." << endl;
        }
    }


    
};

// Derived classes for 4-seater and 7-seater cars
class Car4Seater : public Car {
public:
    Car4Seater() : Car("4-seater") {}
    double dailyRentalRate() const override {
        return 1000;
    }
};
class Car7Seater : public Car {
public:
    Car7Seater() : Car("7-seater") {}
    double dailyRentalRate() const override {
        return 2000;
    }
};

// Function to read a date from the input
tm EnterDate(const string& prompt) {
    tm date = {};
    cout << prompt;
    cout << " (dd mm yyyy): ";
    cin >> date.tm_mday >> date.tm_mon >> date.tm_year;
    date.tm_mon--; // tm_mon is 0-based in struct tm
    date.tm_year -= 1900; // tm_year is year since 1900 in struct tm
    return date;
}

// Base class to represent a customer
class Customer {
protected:
    string Name;
    string Address;
    string PhoneNumber;
    string Brand;
    string Reason;
    Car* carType; // Pointer to Car
    tm RentalDate;
    tm ReturnDate;
    Vehicle* car;
public:
        
    // Constructor to initialize customer details
    Customer(string Name, string Address, string PhoneNumber, string Brand,string Reason, Car* carType, tm RentalDate, tm ReturnDate, Vehicle* car) {
        this->Name = Name;
        this->Address = Address;
        this->PhoneNumber = PhoneNumber;
        this->Brand = Brand;
        this->Reason = Reason;
        this->carType = carType;
        this->RentalDate = RentalDate;
        this->ReturnDate = ReturnDate;
        this->car = car;
        this->car->available = false;
    }

    // Public member functions to access member variables
    string getName() const { return Name; }
    string getAddress() const { return Address; }
    string getPhoneNumber() const { return PhoneNumber; }
    string getBrand() const { return Brand; }
    string getReason() const { return Reason; }
    Car* getVehicle() const { return carType; }
    tm getRentalDate() const { return RentalDate; }
    tm getReturnDate() const { return ReturnDate; }

     // Setter methods
    void setName(const string& name) { Name = name; }
    void setAddress(const string& address) { Address = address; }
    void setPhoneNumber(const string& phoneNumber) { PhoneNumber = phoneNumber; }
    void setReason(const string& reason) { Reason = reason; }
    void setReturnDate(const tm& returnDate) { ReturnDate = returnDate; }

    // Method to display customer information
    virtual void GetCustomerInfo() const {
        cout << "Name: " << Name << endl;
        cout << "Address: " << Address << endl;
        cout << "Phone number: " << PhoneNumber << endl;
        cout << "Brand: " << Brand << endl;
        cout << "Reason for renting: " << Reason << endl;
        cout << "Car type: " << carType->getcarType() << endl;
        cout << "License plate: " << car->licensePlate << endl;
        cout << "Rental Date: " << RentalDate.tm_mday << "/" << RentalDate.tm_mon + 1 << "/" << RentalDate.tm_year + 1900 << endl;
        cout << "Return Date: " << ReturnDate.tm_mday << "/" << ReturnDate.tm_mon + 1 << "/" << ReturnDate.tm_year + 1900 << endl;
        cout << "Number of rental days: " << RentalDays() << endl;
        cout << "Total rental cost: $" << calculateRentalCost() << endl;
    }

    // Method to calculate the number of rental days
    int RentalDays() const {
        time_t tRental = mktime(const_cast<tm*>(&RentalDate));
        time_t tReturn = mktime(const_cast<tm*>(&ReturnDate));
        double seconds = difftime(tReturn, tRental);
        return seconds / (60 * 60 * 24); // Convert seconds to days
    }

    void extendRentalPeriod(const tm& newReturnDate) {
        setReturnDate(newReturnDate);
        cout << "Rental period extended successfully." << endl;
    }

    // Method to calculate the rental cost
    virtual double calculateRentalCost() const {
        return RentalDays() * carType->dailyRentalRate();
    }

    virtual ~Customer() {
        car->available = true;
        delete carType;
    } // Virtual destructor for proper cleanup

    void changeCustomerInfo(const string& newName, const string& newAddress, const string& newPhoneNumber) {
        setName(newName);
        setAddress(newAddress);
        setPhoneNumber(newPhoneNumber);
        cout << "Customer information updated successfully." << endl;
    }
};

// Derived class to represent a VIP customer
class CustomerVIP : public Customer {
private:
    double discountRate; // Discount rate for VIP customers
public:
    CustomerVIP(string Name, string Address, string PhoneNumber, string Brand,string Reason, Car* carType, tm RentalDate, tm ReturnDate, Vehicle* car, double discountRate)
        : Customer(Name, Address, PhoneNumber, Brand,Reason, carType, RentalDate, ReturnDate, car), discountRate(discountRate) {}

    // Overridden method to display VIP customer information
    void GetCustomerInfo() const override {
        Customer::GetCustomerInfo();
        cout << "Customer type: VIP" << endl;
        cout << "Discount rate: " << discountRate * 100 << "%" << endl;
        cout << "Total rental cost after discount: $" << calculateRentalCost() << endl;
    }

    // Overridden method to calculate the rental cost with discount
    double calculateRentalCost() const override {
        return Customer::calculateRentalCost() * (1 - discountRate);
    }

    // Method to calculate the original rental cost before discount
    double calculateBaseRentalCost() const {
        return Customer::calculateRentalCost();
    }
};

// Function to print a bill with beautiful borders
void printBill(const Customer* cus) {
    cout << "-----------------------------------------" << endl;
    cout << "|                 BILL                  |" << endl;
    cout << "-----------------------------------------" << endl;
    cus->GetCustomerInfo();

    double originalCost = cus->calculateRentalCost();
    double discountedCost = originalCost;

    const CustomerVIP* vipCustomer = dynamic_cast<const CustomerVIP*>(cus);
    if (vipCustomer) {
        originalCost = vipCustomer->calculateBaseRentalCost();
        discountedCost = vipCustomer->calculateRentalCost();
        cout << "Total rental cost (before discount): $" << originalCost << endl;
        cout << "Total rental cost afther discount: $" << discountedCost << endl;
    } else {
        cout << "Total rental cost: $" << originalCost << endl;
    }

    cout << "-----------------------------------------" << endl;
    char damage;
    cout << "Did the customer cause any damage to the car? (y/n)? ";
    cin >> damage;
    double insuranceCost = 0;
    if (damage == 'y' || damage == 'Y') {
        char tier;
        cout << "Enter the extent of the damage (A/B/C): ";
        cin >> tier;
        switch (tier) {
            case 'A': case 'a':
                insuranceCost = 500;
                break;
            case 'B': case 'b':
                insuranceCost = 200;
                break;
            case 'C': case 'c':
                insuranceCost = 100;
                break;
            default:
                cout << "Invalid extent, insurance fee not applicable" << endl;
                insuranceCost = 0;
                break;
        }
    }
    double totalCost = discountedCost + insuranceCost;
    cout << "Insurance fee: $" << insuranceCost << endl;
    cout << "Total amount: $" << totalCost << endl;
    cout << "-----------------------------------------" << endl;
}
bool checkPassword() {
    string correctPassword;
    ifstream file("D:\\pb\\pass.txt"); // Mở tập tin chứa mật khẩu
    if (file.is_open()) {
        getline(file, correctPassword); // Đọc mật khẩu từ tập tin
        file.close(); // Đóng tập tin sau khi đọc xong
    } else {
        cerr << "Cannot open the file pass.txt\n";
        return false; // Trả về false nếu không thể mở tập tin
    }

    string enteredPassword;
    cout << "#========================================#" << endl;
    cout << "#                                        #" << endl;
    cout << "#      Enter the password to access      #" << endl;
    cout << "#                                        #" << endl;
    cout << "#========================================#" << endl;
    cout << "| Password : ";
    cin >> enteredPassword;
    cout << "=========================================" << endl;
    return enteredPassword == correctPassword;
}

// Function to initialize the vehicle list
void initializeCar(vector<Vehicle>& VehicleList) {
    VehicleList.push_back(Vehicle("4S1234", "Toyota", "Red", true, "Good"));
    VehicleList.push_back(Vehicle("4S5678", "Honda", "Blue", true, "Good"));
    VehicleList.push_back(Vehicle("7S2345", "Ford", "Black", true, "Good"));
    VehicleList.push_back(Vehicle("7S6789", "Chevrolet", "White", true, "Good"));
}

// Function to find a vehicle by license plate
Vehicle* findCar(const vector<Vehicle>& VehicleList, const string& licensePlate) {
    for (auto& xe : VehicleList) {
        if (xe.licensePlate == licensePlate && xe.available) {
            return const_cast<Vehicle*>(&xe);
        }
    }
    return nullptr;
}

void DisplayCarList(const vector<Vehicle>& VehicleList) {
    cout << "-----------------------------------------" << endl;
    cout << "|          AVAILABLE CAR LIST           |" << endl;
    cout << "-----------------------------------------" << endl;
    for (const auto& xe : VehicleList) {
        cout << "license plate number: " << xe.licensePlate << endl;
        cout << "Brand: " << xe.brand << endl;
        cout << "Color: " << xe.color << endl;
        cout << "Availability: " << (xe.available ? "Available" : "Rented") << endl;
        cout << "Condition: " << xe.condition << endl;
        cout << "-----------------------------------------" << endl;
    }
}

void addCarMaintenance(vector<Vehicle>& VehicleList) {
    string licenseplate;
    cout << "Enter the license plate number of the car needing maintenance: ";
    cin >> licenseplate;
    Vehicle* car = nullptr;

    // Find the vehicle with the specified license plate
    for (auto& v : VehicleList) {
        if (v.licensePlate == licenseplate) {
            car = &v;
            break;
        }
    }

    if (car == nullptr) {
        cout << "Car does not exist or is currently rented" << endl;
        return;
    }

    cout << "Enter maintenance description: ";
    string description;
    cin.ignore();
    getline(cin, description);
    tm dueDate = EnterDate("Enter maintenance date");

    // Add maintenance task to the vehicle's maintenance schedule
    car->maintenanceSchedule.push_back(MaintenanceTask(description, dueDate));
    cout << "Maintenance has been added for the car " << licenseplate << endl;
}

void deleteCarMaintenance(vector<Vehicle>& VehicleList) {
    string licenseplate;
    cout << "Enter the license plate number of the car needing maintenance deletion: ";
    cin >> licenseplate;
    Vehicle* car = nullptr;

    // Find the vehicle with the specified license plate
    for (auto& v : VehicleList) {
        if (v.licensePlate == licenseplate) {
            car = &v;
            break;
        }
    }

    if (car == nullptr) {
        cout << "The car does not exist or is currently being rented." << endl;
        return;
    }

    cout << "Enter the maintenance description to be deleted: ";
    string description;
    cin.ignore(); // Clear the input buffer
    getline(cin, description);

    // Find and remove the maintenance task from the vehicle's maintenance schedule
    auto it = std::find_if(car->maintenanceSchedule.begin(), car->maintenanceSchedule.end(), [&](const MaintenanceTask& task) {
        return task.description == description;
    });

    if (it != car->maintenanceSchedule.end()) {
        car->maintenanceSchedule.erase(it);
        cout << "Maintenance task with description '" << description << "' removed successfully." << endl;
    } else {
        cout << "Maintenance task with description '" << description << "' not found." << endl;
    }
}

void displayCarMaintenance(const vector<Vehicle>& VehicleList) {
    string licenseplate;
    cout << "Enter the license plate number to display the maintenance schedule: ";
    cin >> licenseplate;
    Vehicle* car = nullptr;

    // Find the vehicle with the specified license plate
    for (const auto& v : VehicleList) {
        if (v.licensePlate == licenseplate) {
            car = const_cast<Vehicle*>(&v);
            break;
        }
    }

    if (car == nullptr) {
        cout << "The car does not exist." << endl;
        return;
    }

    cout << "-----------------------------------------" << endl;
    cout << "|       MAINTENANCE SCHEDULE OF THE CAR          |" << endl;
    cout << "-----------------------------------------" << endl;
    if (car->maintenanceSchedule.empty()) {
        cout << "There is no maintenance scheduled for this car." << endl;
    } else {
        for (const auto& task : car->maintenanceSchedule) {
            cout << "Description: " << task.description << endl;
            cout << "Due Date: " << task.dueDate.tm_mday << "/" << task.dueDate.tm_mon + 1 << "/" << task.dueDate.tm_year + 1900 << endl;
            cout << "Status: " << (task.completed ? "Completed" : "Pending") << endl;
            cout << "-----------------------------------------" << endl;
        }
    }
}

void saveDeletedCustomerInfo(const Customer* customer, const string& filePath) {
    ofstream file(filePath, ios::app);
    if (file.is_open()) {
        file << "Name: " << customer->getName() << endl;
        file << "Address: " << customer->getAddress() << endl;
        file << "Phone Number: " << customer->getPhoneNumber() << endl;
        file << "Desired Brand: " << customer->getBrand() << endl;
        file << "Vehicle Type: " << customer->getVehicle()->getcarType() << endl;
        file << "Rental Date: " << customer->getRentalDate().tm_mday << "/" << customer->getRentalDate().tm_mon + 1 << "/" << customer->getRentalDate().tm_year + 1900 << endl;
        file << "Return Date: " << customer->getReturnDate().tm_mday << "/" << customer->getReturnDate().tm_mon + 1 << "/" << customer->getReturnDate().tm_year + 1900 << endl;
        file << "-----------------------------------------" << endl;
        file.close();
    } else {
        cout << "Unable to open file to save deleted customer information." << endl;
    }
}



int main() {
   
    int attempts = 0;
    bool isAuthenticated = false;
    while (attempts < 3 && !isAuthenticated) {
        if (checkPassword()) {
            isAuthenticated = true;
            cout << "@------------------------------------------------------@" << endl;
            cout << "|                                                      |" << endl;
            cout << "|  Password correct. You have successfully logged in.  | \n";
            cout << "|                                                      |" << endl;
            cout << "@------------------------------------------------------@" << endl;
            cout << "========================================================" << endl;

        } else {
            attempts++;
            cout << "@------------------------------------------------------@" << endl;
            cout << "|                                                      |" << endl;
            cout << "|                  Incorrect password                  |\n";      
            cout << "|                                                      |" << endl;
            cout << "@------------------------------------------------------@" << endl;
            cout << "========================================================" << endl;
        }
    }
    if (!isAuthenticated) {
        cout << "@---------------------------------------------------------------------------@" << endl;
        cout << "|                                                                           |" << endl;
        cout << "|   You have entered the wrong password more than 3 times. Access denied.   |\n";
        cout << "|                                                                           |" << endl;
        cout << "@---------------------------------------------------------------------------@" << endl;
        cout << "=============================================================================" << endl;

        return 0;
    }

    vector<Customer*> CustomerList; // Use vector to manage the list of customers
    vector<Vehicle> VehicleList; // List of vehicles
    initializeCar(VehicleList); // Initialize vehicle list

    // Menu options
    cout << "-----------------------------------------" << endl;
    cout << "| CAR RENTAL CONTRACT MANAGEMENT PROGRAM |" << endl;
    cout << "-----------------------------------------" << endl;
    cout << "|  1. Add customer                       |" << endl;
    cout << "|  2. Add VIP customer                   |" << endl;
    cout << "|  3. Delete customer                    |" << endl;
    cout << "|  4. Display customer list              |" << endl;
    cout << "|  5. Display car List                   |" << endl;
    cout << "|  6. Add car maintainance               |" << endl;
    cout << "|  7. Delete car maintainance            |" << endl;
    cout << "|  8. Display car maintanance list       |" << endl;
    cout << "|  9. Extend rental period               |" << endl; 
    cout << "| 10. Change customer information        |" << endl;  
    cout << "|  0. Exit                               |" << endl;
    cout << "-----------------------------------------" << endl;

    int Choice;
    do {
        cout << "Enter your choice: ";
        cin >> Choice;
        cin.ignore(); // Clear the newline character from the input buffer
        switch (Choice) {

            case 1: {
                string Name, Address, PhoneNumber, Brand,Reason, carType4o7, licensePLate;
                Car* carType;
                Vehicle* car;
                cout << "Enter customer name: ";
                getline(cin, Name);
                cout << "Enter address: ";
                getline(cin, Address);
                cout << "Enter phone number: ";
                getline(cin, PhoneNumber);
                cout << "Enter the car brand the customer wants to rent: ";
                getline(cin, Brand);
                cout << "Enter reason for renting: ";
                getline(cin, Reason);
                cout << "Enter car type (4-seater/7-seater): ";
                getline(cin, carType4o7);
                cout << "Enter lisence plate number: ";
                getline(cin, licensePLate);
                car = findCar(VehicleList, licensePLate);

                if (!car) {
                    cout << "The car is not available or invalid license plate number!" << endl;
                    break;
                }
                if (carType4o7 == "4-seater") {
                    carType = new Car4Seater();
                } else if (carType4o7 == "7-seater") {
                    carType = new Car7Seater();
                } else {
                    cout << "Invalid car type!" << endl;
                    break;
                }

                tm RentalDate = EnterDate("Enter rental date");
                tm ReturnDate = EnterDate("Enter return date");

                CustomerList.push_back(new Customer(Name, Address, PhoneNumber, Brand,Reason, carType, RentalDate, ReturnDate, car));
                break;
            }

            case 2: {
                string Name, Address, PhoneNumber, Brand,Reason,  carType4o7, licensePLate;
                Car* carType;
                Vehicle* car;
                double discountRate;
                cout << "Enter customer name: ";
                getline(cin, Name);
                cout << "Enter address: ";
                getline(cin, Address);
                cout << "Enter phone number: ";
                getline(cin, PhoneNumber);
                cout << "Enter the car brand the customer wants to rent: ";
                getline(cin, Brand);
                cout << "Enter reason for renting: ";
                getline(cin, Reason);
                cout << "Enter car type (4-seater/7-seater): ";
                getline(cin, carType4o7);
                cout << "Enter license plate: ";
                getline(cin, licensePLate);
                car = findCar(VehicleList, licensePLate);
                if (!car) {
                    cout << "The car is not available or invalid license plate number!" << endl;
                    break;
                }
                if (carType4o7 == "4-seater") {
                    carType = new Car4Seater();
                } else if (carType4o7 == "7-seater") {
                    carType = new Car7Seater();
                } else {
                    cout << "Invalid car type!" << endl;
                    break;
                }

                tm RentalDate = EnterDate("Enter rental date");
                tm ReturnDate = EnterDate("Enter return date");
                cout << "Enter the discount rate for VIP customers (e.g., 0.1 for 10%): ";
                cin >> discountRate;
                cin.ignore();

                CustomerList.push_back(new CustomerVIP(Name, Address, PhoneNumber, Brand, Reason, carType, RentalDate, ReturnDate, car, discountRate));
                break;
            }

            case 3: {
                int Position;
                cout << "Enter the position of the customer you want to delete: ";
                cin >> Position;
                if (Position >= 1 && Position <= CustomerList.size()) {
                    printBill(CustomerList[Position - 1]);
                    saveDeletedCustomerInfo(CustomerList[Position - 1], "D:\\pb\\savedcustomer.txt");
                    delete CustomerList[Position - 1];
                    CustomerList.erase(CustomerList.begin() + (Position - 1));
                    cout << "Delete successfully" << endl;
                } else {
                    cout << "Invalid position" << endl;
                }
                break;
            }

            case 4: {
                if (CustomerList.empty()) {
                    cout << "Customer list is empty!" << endl;
                } else {
                    cout << "Customer list:" << endl;
                        cout << "-----------------------------------------" << endl;
                    for (int i = 0; i < CustomerList.size(); ++i) {
                        cout << "Customer number " << i + 1 << ":" << endl;
                        CustomerList[i]->GetCustomerInfo();
                        cout << "-----------------------------------------" << endl;

                    }
                }
                break;
            }

            case 5: {
                DisplayCarList(VehicleList);
                break;
            }

            case 6: {
                addCarMaintenance(VehicleList);
                break;
            }

            case 7: {
                deleteCarMaintenance(VehicleList);
                break;
            }

            case 8: { // New case for displaying maintenance list
                displayCarMaintenance(VehicleList);
            break;
            }

            case 9: { // Extend rental period
            int position;
            cout << "Enter the position of the customer you want to extend the rental period for: ";
            cin >> position;
            if (position >= 1 && position <= CustomerList.size()) {
                tm newReturnDate = EnterDate("Enter new return date");
                CustomerList[position - 1]->extendRentalPeriod(newReturnDate);
            } else {
                cout << "Invalid position!" << endl;
            }
            break;
            }

            case 10: { // Change customer information
            int position;
            cout << "Enter the position of the customer you want to change the information for: ";
            cin >> position;
            cin.ignore(); // Clear the input buffer
                if (position >= 1 && position <= CustomerList.size()) {
                    cout << "What information do you want to change?" << endl;
                    cout << "1. Name" << endl;
                    cout << "2. Address" << endl;
                    cout << "3. Phone number" << endl;
                    cout << "Enter your choice: ";
                    int changeChoice;
                    cin >> changeChoice;
                    cin.ignore(); // Clear the input buffer
                    switch (changeChoice) {
                        case 1: { // Change name
                            string newName;
                            cout << "Enter new name: ";
                            getline(cin, newName);
                            CustomerList[position - 1]->setName(newName);
                            cout << "Name changed successfully." << endl;
                            break;
                        }
                        case 2: { // Change address
                            string newAddress;
                            cout << "Enter new address: ";
                            getline(cin, newAddress);
                            CustomerList[position - 1]->setAddress(newAddress);
                            cout << "Address changed successfully." << endl;
                            break;
                        }
                        case 3: { // Change phone number
                            string newPhoneNumber;
                            cout << "Enter new phone number: ";
                            getline(cin, newPhoneNumber);
                            CustomerList[position - 1]->setPhoneNumber(newPhoneNumber);
                            cout << "Phone number changed successfully." << endl;
                            break;
                        }
                        default:
                            cout << "Invalid choice!" << endl;
                            break;
                    }
                } else {
                    cout << "Invalid position!" << endl;
                }
                break;
            }
            case 0:
                cout << "Exit the program, wish you a good day" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
                break;
        }
    } while (Choice != 0);

    // Clean up dynamically allocated memory
    for (Customer* cus : CustomerList) {
        delete cus;
    }

    return 0;
}
