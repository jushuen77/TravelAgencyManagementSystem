#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

namespace {
    int nextBookingID = 1; // static/global-style ID counter
    int nextPackageID = 6; // continues after the 5 seeded sample packages
}

vector<Package> createSamplePackages() {
    return {
        {1, "Bali Beach Escape",       "Bali, Indonesia",       4,  899.00, 20},
        {2, "Tokyo City Explorer",     "Tokyo, Japan",          6, 2199.00, 15},
        {3, "Swiss Alps Adventure",    "Zermatt, Switzerland",  7, 3499.00, 10},
        {4, "Langkawi Island Getaway", "Langkawi, Malaysia",    3,  549.00, 25},
        {5, "Paris Romance Tour",      "Paris, France",         5, 2799.00, 12}
    };
}

void displayAvailablePackages(const vector<Package>& packages) {
    if (packages.empty()) {
        cout << "No packages available.\n";
        return;
    }
    cout << "\n" << centerText("Available Trip Packages", 75) << "\n";
    cout << left
         << setw(5) << "ID" << setw(24) << "Package" << setw(22) << "Destination"
         << setw(6) << "Days" << setw(10) << "Price" << setw(8) << "Seats" << "\n";
    cout << string(75, '-') << "\n";
    cout << fixed << setprecision(2);
    for (const auto& p : packages) {
        cout << left << setw(5) << p.id << setw(24) << p.name << setw(22) << p.destination
             << setw(6) << p.days << "$" << setw(9) << p.price << setw(8) << p.seatsAvailable << "\n";
    }
}

int findPackageByID(const vector<Package>& packages, int id) {
    for (size_t i = 0; i < packages.size(); ++i) {
        if (packages[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

void addPackage(vector<Package>& packages) {
    cout << "(Enter 0 at any prompt to cancel.)\n";
    Package p;

    string name;
    do {
        name = readLine("Enter package name: ");
        if (name.empty()) cout << "Package name cannot be empty.\n";
    } while (name.empty());
    p.name = name;

    string destination;
    do {
        destination = readLine("Enter destination: ");
        if (destination.empty()) cout << "Destination cannot be empty.\n";
    } while (destination.empty());
    p.destination = destination;

    int days;
    while (true) {
        days = readInt("Enter duration (days): ");
        if (days <= 0) { cout << "Duration must be positive.\n"; continue; }
        break;
    }
    p.days = days;

    double price;
    while (true) {
        price = readDouble("Enter price per person: $");
        if (price <= 0) { cout << "Price must be positive.\n"; continue; }
        break;
    }
    p.price = price;

    int seats;
    while (true) {
        seats = readInt("Enter number of seats available: ");
        if (seats <= 0) { cout << "Seats must be positive.\n"; continue; }
        break;
    }
    p.seatsAvailable = seats;

    p.id = nextPackageID++;
    packages.push_back(p);
    cout << "Package added successfully. Assigned ID: " << p.id << "\n";
}

bool editPackage(vector<Package>& packages, int id) {
    int idx = findPackageByID(packages, id);
    if (idx == -1) {
        cout << "Package ID " << id << " not found.\n";
        return false;
    }

    Package& p = packages[idx];
    cout << "(Enter 0 at any prompt to cancel the edit.)\n";

    string name = readLine("Enter new name (blank to keep \"" + p.name + "\"): ");
    if (!name.empty()) p.name = name;

    string destination = readLine("Enter new destination (blank to keep \"" + p.destination + "\"): ");
    if (!destination.empty()) p.destination = destination;

    string daysInput = readLine("Enter new duration in days (blank to keep " + to_string(p.days) + "): ");
    if (!daysInput.empty()) {
        try {
            int days = stoi(daysInput);
            if (days > 0) p.days = days;
            else cout << "Duration must be positive, kept previous value.\n";
        } catch (...) {
            cout << "Invalid number, kept previous value.\n";
        }
    }

    ostringstream oldPrice;
    oldPrice << fixed << setprecision(2) << p.price;
    string priceInput = readLine("Enter new price (blank to keep $" + oldPrice.str() + "): ");
    if (!priceInput.empty()) {
        try {
            double price = stod(priceInput);
            if (price > 0) p.price = price;
            else cout << "Price must be positive, kept previous value.\n";
        } catch (...) {
            cout << "Invalid number, kept previous value.\n";
        }
    }

    string seatsInput = readLine("Enter new seats available (blank to keep " + to_string(p.seatsAvailable) + "): ");
    if (!seatsInput.empty()) {
        try {
            int seats = stoi(seatsInput);
            if (seats >= 0) p.seatsAvailable = seats;
            else cout << "Seats cannot be negative, kept previous value.\n";
        } catch (...) {
            cout << "Invalid number, kept previous value.\n";
        }
    }

    cout << "Package updated successfully.\n";
    return true;
}

bool bookPackage(vector<Booking>& bookings, vector<Package>& packages, int customerID) {
    displayAvailablePackages(packages);
    cout << "(Enter 0 to cancel.)\n";
    int packageID = readInt("Enter package ID to book: ");
    int pIdx = findPackageByID(packages, packageID);
    if (pIdx == -1) {
        cout << "Package not found.\n";
        return false;
    }

    int numPeople = readInt("Enter number of people: ");
    if (numPeople <= 0) {
        cout << "Number of people must be positive.\n";
        return false;
    }
    if (packages[pIdx].seatsAvailable < numPeople) {
        cout << "Not enough seats available. Only " << packages[pIdx].seatsAvailable << " left.\n";
        return false;
    }

    Booking b;
    b.id = nextBookingID++;
    b.customerID = customerID;
    b.packageID = packages[pIdx].id;
    b.numPeople = numPeople;
    b.status = "Confirmed";
    bookings.push_back(b);
    packages[pIdx].seatsAvailable -= numPeople;

    cout << "Booking confirmed! Booking ID: " << b.id << "\n";
    return true;
}

bool cancelBooking(vector<Booking>& bookings, vector<Package>& packages, int bookingID) {
    for (auto& b : bookings) {
        if (b.id == bookingID) {
            if (b.status == "Cancelled") {
                cout << "Booking already cancelled.\n";
                return false;
            }
            int pIdx = findPackageByID(packages, b.packageID);
            if (pIdx != -1) packages[pIdx].seatsAvailable += b.numPeople;
            b.status = "Cancelled";
            cout << "Booking " << bookingID << " cancelled.\n";
            return true;
        }
    }
    cout << "Booking not found.\n";
    return false;
}

void viewItinerary(const vector<Booking>& bookings, const vector<Package>& packages, int customerID) {
    bool found = false;
    printSectionHeader("Itinerary for Customer ID " + to_string(customerID));
    for (const auto& b : bookings) {
        if (b.customerID == customerID) {
            found = true;
            int pIdx = findPackageByID(packages, b.packageID);
            cout << "Booking ID: " << b.id << " | Status: " << b.status << "\n";
            if (pIdx != -1) {
                cout << "  Package: " << packages[pIdx].name << " (" << packages[pIdx].destination << ")\n"
                     << "  Duration: " << packages[pIdx].days << " days | People: " << b.numPeople << "\n";
            }
        }
    }
    if (!found) cout << "No bookings found for this customer.\n";
}

void viewAllBookings(const vector<Booking>& bookings, const vector<Package>& packages,
                      const vector<Customer>& customers) {
    if (bookings.empty()) {
        cout << "No bookings recorded yet.\n";
        return;
    }
    cout << "\n" << centerText("All Bookings", 84) << "\n";
    cout << left
         << setw(10) << "BookID" << setw(10) << "CustID" << setw(20) << "Customer"
         << setw(24) << "Package" << setw(8) << "People" << setw(12) << "Status" << "\n";
    cout << string(84, '-') << "\n";
    for (const auto& b : bookings) {
        int pIdx = findPackageByID(packages, b.packageID);
        int cIdx = findCustomerByID(customers, b.customerID);
        string packageName = (pIdx != -1) ? packages[pIdx].name : "(unknown)";
        string customerName = (cIdx != -1) ? customers[cIdx].name : "(unknown)";
        cout << left << setw(10) << b.id << setw(10) << b.customerID << setw(20) << customerName
             << setw(24) << packageName << setw(8) << b.numPeople << setw(12) << b.status << "\n";
    }
}

void bookingMenu(vector<Booking>& bookings, vector<Package>& packages, const vector<Customer>& customers) {
    int choice;
    do {
        printSectionHeader("Trip Package Booking & Itinerary Management");
        cout << "  1. Browse Available Packages\n"
             << "  2. Add New Package\n"
             << "  3. Edit Package\n"
             << "  4. Book a Package\n"
             << "  5. Cancel a Booking\n"
             << "  6. View Itinerary by Customer ID\n"
             << "  7. View All Bookings\n"
             << "  0. Back to Main Menu\n";
        choice = readMenuChoice(0, 7);
        try {
            switch (choice) {
                case 1:
                    displayAvailablePackages(packages);
                    break;
                case 2:
                    addPackage(packages);
                    break;
                case 3: {
                    displayAvailablePackages(packages);
                    int pID = readInt("Enter package ID to edit: ");
                    editPackage(packages, pID);
                    break;
                }
                case 4: {
                    displayAllCustomers(customers);
                    int custID = readInt("Enter your customer ID: ");
                    if (findCustomerByID(customers, custID) == -1) {
                        cout << "Customer ID not found. Please register first.\n";
                        break;
                    }
                    bookPackage(bookings, packages, custID);
                    break;
                }
                case 5: {
                    viewAllBookings(bookings, packages, customers);
                    int bID = readInt("Enter booking ID to cancel: ");
                    cancelBooking(bookings, packages, bID);
                    break;
                }
                case 6: {
                    displayAllCustomers(customers);
                    int custID = readInt("Enter customer ID: ");
                    viewItinerary(bookings, packages, custID);
                    break;
                }
                case 7:
                    viewAllBookings(bookings, packages, customers);
                    break;
            }
        } catch (const InputCancelled&) {
            cout << "\nCancelled.\n";
        }
    } while (choice != 0);
}
