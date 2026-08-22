#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace {
    int nextBookingID = 1; // static/global-style ID counter
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
    cout << "\n--- Itinerary for Customer ID " << customerID << " ---\n";
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

void bookingMenu(vector<Booking>& bookings, vector<Package>& packages, const vector<Customer>& customers) {
    int choice;
    do {
        cout << "\n--- Trip Package Booking & Itinerary Management ---\n"
             << "1. Browse Available Packages\n"
             << "2. Book a Package\n"
             << "3. Cancel a Booking\n"
             << "4. View Itinerary\n"
             << "0. Back to Main Menu\n";
        choice = readMenuChoice(0, 4);
        try {
            switch (choice) {
                case 1:
                    displayAvailablePackages(packages);
                    break;
                case 2: {
                    int custID = readInt("Enter your customer ID: ");
                    if (findCustomerByID(customers, custID) == -1) {
                        cout << "Customer ID not found. Please register first.\n";
                        break;
                    }
                    bookPackage(bookings, packages, custID);
                    break;
                }
                case 3: {
                    int bID = readInt("Enter booking ID to cancel: ");
                    cancelBooking(bookings, packages, bID);
                    break;
                }
                case 4: {
                    int custID = readInt("Enter customer ID: ");
                    viewItinerary(bookings, packages, custID);
                    break;
                }
            }
        } catch (const InputCancelled&) {
            cout << "\nCancelled.\n";
        }
    } while (choice != 0);
}
