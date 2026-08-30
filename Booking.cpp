#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <format>
#include <vector>
#include <algorithm>
using namespace std;

int nextBookingID = 1;
int nextPackageID = 6;

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
        printCentered("No packages available.");
        return;
    }

    int packageWidth = 7;
    int destinationWidth = 11;

    for (const auto& p : packages) {
        packageWidth = max(packageWidth, static_cast<int>(p.name.length()));
        destinationWidth = max(destinationWidth, static_cast<int>(p.destination.length())
        );
    }

    vector<int> widths = {4, packageWidth + 2, destinationWidth + 2, 6, 13, 8};

    showSectionHeader("Available Trip Packages");

    printTableRow(
        {"ID", "Package", "Destination", "Days", "Price", "Seats"},
        widths
    );

    printLine('-', 75);

    for (const auto& p : packages) {
        printTableRow(
            {to_string(p.id), p.name, p.destination, to_string(p.days), format("RM{:.2f}", p.price),
             to_string(p.seatsAvailable)},
            widths
        );
    }
}

int findPackageByID(const vector<Package>& packages, int id) {
    for (size_t i = 0; i < packages.size(); ++i) {
        if (packages[i].id == id)
            return static_cast<int>(i);
    }

    return -1;
}

void addPackage(vector<Package>& packages) {
    Package p;

    printCentered("(Enter -1 at any prompt to cancel.)");

    // Package name
    while (true) {
        string name = getString("Enter package name: ");

        if (name == "-1") {
            printCentered("Package creation cancelled.");
            return;
        }

        p.name = name;
        break;
    }

    // Destination
    while (true) {
        string destination = getString("Enter destination: ");

        if (destination == "-1") {
            printCentered("Package creation cancelled.");
            return;
        }

        p.destination = destination;
        break;
    }

    // Duration
    int days = getInt("Enter duration (days): ", -1, 999);

    if (days == -1) {
        printCentered("Package creation cancelled.");
        return;
    }

    p.days = days;

    // Price
    double price = getDouble("Enter price per person: $", -1, 999999999);

    if (price == -1) {
        printCentered("Package creation cancelled.");
        return;
    }

    p.price = price;

    // Seats
    int seats = getInt("Enter number of seats available: ", -1, 999999);

    if (seats == -1) {
        printCentered("Package creation cancelled.");
        return;
    }

    p.seatsAvailable = seats;

    p.id = nextPackageID++;

    packages.push_back(p);

    printCentered(
        "Package added successfully. Assigned ID: "
        + to_string(p.id)
    );
}

bool editPackage(vector<Package>& packages, int id) {
    int idx = findPackageByID(packages, id);

    if (idx == -1) {
        showError("Package ID " + to_string(id) + " not found");
        return false;
    }

    Package& p = packages[idx];

    printCentered("(Enter -1 at any prompt to cancel the edit.)");

    // Package name
    string name = getString("Enter new name: ");

    if (name == "-1") {
        printCentered("Package edit cancelled.");
        return false;
    }

    // Destination
    string destination = getString("Enter new destination: ");

    if (destination == "-1") {
        printCentered("Package edit cancelled.");
        return false;
    }

    // Duration
    int days = getInt("Enter new duration in days: ", -1, 999);

    if (days == -1) {
        printCentered("Package edit cancelled.");
        return false;
    }

    // Price
    double price = getDouble("Enter new price: $", -1, 999999999);

    if (price == -1) {
        printCentered("Package edit cancelled.");
        return false;
    }

    // Seats
    int seats = getInt("Enter new seats available: ", -1, 999999);

    if (seats == -1) {
        printCentered("Package edit cancelled.");
        return false;
    }

    // Modify package details
    p.name = name;
    p.destination = destination;
    p.days = days;
    p.price = price;
    p.seatsAvailable = seats;

    printCentered("Package updated successfully.");
    return true;
}

bool bookPackage(vector<Booking>& bookings, vector<Package>& packages, int customerID) {
    displayAvailablePackages(packages);

    cout << "";
    printCentered("(Enter 0 at any prompt to cancel)");

    // Package ID
    int packageID = getInt("Enter package ID to book: ", 0, 999999);

    if (packageID == 0) {
        showInfo("Booking cancelled.");
        return false;
    }

    int pIdx = findPackageByID(packages, packageID);

    if (pIdx == -1) {
        showError("Package not found");
        return false;
    }

    if (packages[pIdx].seatsAvailable == 0) {
    showError("This package is sold out.");
    return false;
    }

    // Number of people
    int numPeople = getInt("Enter number of people: ", 0, packages[pIdx].seatsAvailable);

    if (numPeople == 0) {
        showInfo("Booking cancelled.");
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

    showInfo("Booking confirmed! Booking ID: " + to_string(b.id));

    return true;
}

bool cancelBooking(vector<Booking>& bookings, vector<Package>& packages, int bookingID) {
    for (auto& b : bookings) {
        if (b.id == bookingID) {
            if (b.status == "Cancelled") {
                printCentered("Booking already cancelled.");
                return false;
            }

            int pIdx = findPackageByID(packages, b.packageID);

            if (pIdx != -1)
                packages[pIdx].seatsAvailable += b.numPeople;

            b.status = "Cancelled";

            printCentered(
                "Booking "
                + to_string(bookingID)
                + " cancelled."
            );

            return true;
        }
    }

    showError("Booking not found");
    return false;
}

void viewItinerary(const vector<Booking>& bookings, const vector<Package>& packages, const vector<Invoice>& invoices,
                    int customerID) {
    bool found = false;

    showSectionHeader("Itinerary for Customer ID "+ to_string(customerID));

    for (const auto& b : bookings) {
        if (b.customerID == customerID) {
            found = true;

            string paymentStatus = "Unpaid";

            for (const auto& inv: invoices) {
                if (inv.bookingID == b.id) {
                    paymentStatus = inv.paymentStatus;
                    break;
                }
            }

            int pIdx = findPackageByID(packages, b.packageID);

            printCentered("Booking ID: " + to_string(b.id) + " | " + "Status: " + b.status + " | Payment: " + paymentStatus);

            if (pIdx != -1) {
                printCentered("Package: " + packages[pIdx].name + " (" + packages[pIdx].destination + ")");
                printCentered("Duration: " + to_string(packages[pIdx].days) + " days | People: " + to_string(b.numPeople));
            }
        }
    }

    if (!found)
        showInfo("No bookings found for this customer.");
}

void viewAllBookings(const vector<Booking>& bookings,const vector<Package>& packages,
                    const vector<Customer>& customers) {
    if (bookings.empty()) {
        showInfo("No bookings recorded yet.");
        return;
    }

    showSectionHeader("All Bookings");

    int customerWidth = 20;
    int packageWidth = 24;

    for (const auto& c : customers) {
        customerWidth = max(customerWidth,static_cast<int>(c.name.length()) + 2);
    }

    for (const auto& p : packages) {
        packageWidth = max(packageWidth,static_cast<int>(p.name.length()) + 2);
    }

    vector<int> widths = {10, 10, customerWidth + 2, packageWidth + 2, 8, 12};

    printTableRow(
        {"BookID", "CustID", "Customer", "Package", "People", "Status"},
        widths
    );

    printLine('-', 84);

    for (const auto& b : bookings) {
        int pIdx = findPackageByID(packages, b.packageID);
        int cIdx = findCustomerByID(customers, b.customerID);

        string packageName = (pIdx != -1) ? packages[pIdx].name : "(unknown)";

        string customerName = (cIdx != -1) ? customers[cIdx].name : "(unknown)";

        printTableRow(
            {to_string(b.id), to_string(b.customerID), customerName, packageName, 
                to_string(b.numPeople), b.status},
            widths
        );
    }
}

void bookingMenu(vector<Booking>& bookings,vector<Package>& packages, const vector<Customer>& customers, 
                    const vector<Invoice>& invoices) {
    int choice;

    do {
        showSectionHeader("Trip Package Booking & Itinerary Management");

        printCentered("1. Browse Available Packages    ", 5);
        printCentered("2. Add New Package              ", 5);
        printCentered("3. Edit Package                 ", 5);
        printCentered("4. Book a Package               ", 5);
        printCentered("5. Cancel a Booking             ", 5);
        printCentered("6. View Itinerary by Customer ID", 5);
        printCentered("7. View All Bookings            ", 5);
        printCentered("0. Back to Main Menu            ", 5);

        choice = getInt("Your choice: ", 0, 7);

        switch (choice) {
            case 1:
                displayAvailablePackages(packages);
                showInfo("");
                break;

            case 2:
                addPackage(packages);
                break;

            case 3: {
                displayAvailablePackages(packages);

                int pID = getInt("Enter package ID to edit: ", 0, 999999);

                if (pID == 0) {
                    printCentered("Package edit cancelled.");
                    break;
                }

                editPackage(packages, pID);
                break;
            }

            case 4: {
                displayAllCustomers(customers);

                int custID = getInt("Enter customer ID: ", 0, 999999);

                if (custID == 0) {
                    showInfo("Booking cancelled.");
                    break;
                }

                if (findCustomerByID(customers, custID) == -1) {
                    showError("Customer ID not found. Please register first");
                    break;
                }

                bookPackage(bookings, packages, custID);
                break;
            }

            case 5: {
                viewAllBookings(bookings, packages, customers);

                int bID = getInt("Enter booking ID to cancel: ", 0, 999999);

                if (bID == 0) {
                    printCentered("Cancellation cancelled.");
                    break;
                }

                cancelBooking(bookings,packages,bID);

                break;
            }

            case 6: {
                displayAllCustomers(customers);

                int custID = getInt("Enter customer ID: ",0,999999);

                if (custID == 0) {
                    printCentered("Viewing itinerary cancelled.");
                    break;
                }

                viewItinerary(bookings, packages, invoices, custID);

                break;
            }

            case 7:
                viewAllBookings(bookings, packages, customers);
                break;
        }

    } while (choice != 0);
}
