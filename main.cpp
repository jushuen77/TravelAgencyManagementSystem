#include "TravelTypes.h"
#include "Customer.h"
#include "Booking.h"
#include "Billing.h"
#include "Auth.h"
#include "Utils.h"
#include <vector>
#include <iostream>
using namespace std;

namespace {

bool bookingBelongsTo(const vector<Booking>& bookings, int bookingID, int customerID) {
    for (const auto& b : bookings) {
        if (b.id == bookingID) return b.customerID == customerID;
    }
    return false;
}

void customerPortalMenu(int customerID, vector<Customer>& customers, vector<Booking>& bookings,
                         vector<Package>& packages, vector<Invoice>& invoices) {
    int choice;
    do {
        int idx = findCustomerByID(customers, customerID);
        string name = (idx == -1) ? "Customer" : customers[idx].name;
        cout << "\n--- Welcome, " << name << " ---\n"
             << "1. Browse Packages\n"
             << "2. Book a Package\n"
             << "3. View My Itinerary\n"
             << "4. Cancel My Booking\n"
             << "5. Make Payment\n"
             << "6. View My Profile\n"
             << "7. Update My Profile\n"
             << "0. Logout\n";
        choice = readMenuChoice(0, 7);
        switch (choice) {
            case 1:
                displayAvailablePackages(packages);
                break;
            case 2:
                bookPackage(bookings, packages, customerID);
                break;
            case 3:
                viewItinerary(bookings, packages, customerID);
                break;
            case 4: {
                int bookingID = readInt("Enter your booking ID to cancel: ");
                if (!bookingBelongsTo(bookings, bookingID, customerID)) cout << "That booking does not belong to you.\n";
                else cancelBooking(bookings, packages, bookingID);
                break;
            }
            case 5: {
                int bookingID = readInt("Enter your booking ID to pay for: ");
                if (!bookingBelongsTo(bookings, bookingID, customerID)) cout << "That booking does not belong to you.\n";
                else payForBooking(invoices, bookings, packages, customers, bookingID);
                break;
            }
            case 6:
                if (idx != -1) printCustomerProfile(customers[idx]);
                break;
            case 7:
                updateCustomer(customers, customerID);
                break;
        }
    } while (choice != 0);
    cout << "Logged out.\n";
}

void staffMenu(vector<Customer>& customers, vector<Booking>& bookings,
               vector<Package>& packages, vector<Invoice>& invoices) {
    int choice;
    do {
        cout << "\n--- Staff Menu ---\n"
             << "1. Customer Management\n"
             << "2. Trip Package Booking & Itinerary\n"
             << "3. Billing / Payment & Reporting\n"
             << "0. Logout\n";
        choice = readMenuChoice(0, 3);
        switch (choice) {
            case 1: customerMenu(customers); break;
            case 2: bookingMenu(bookings, packages, customers); break;
            case 3: billingMenu(invoices, bookings, packages, customers); break;
        }
    } while (choice != 0);
    cout << "Logged out.\n";
}

} // namespace

int main() {
    vector<Customer> customers;
    vector<Package> packages = createSamplePackages();
    vector<Booking> bookings;
    vector<Invoice> invoices;
    vector<Staff> staffList = createSeedStaff();

    int choice;
    do {
        cout << "\n=========================================\n"
             << "   TRAVEL AGENCY MANAGEMENT SYSTEM\n"
             << "=========================================\n"
             << "1. Customer Login\n"
             << "2. Customer Sign Up\n"
             << "3. Staff Login\n"
             << "0. Exit\n";
        choice = readMenuChoice(0, 3);
        switch (choice) {
            case 1: {
                int custID = customerLogin(customers);
                if (custID != -1) customerPortalMenu(custID, customers, bookings, packages, invoices);
                break;
            }
            case 2: {
                int custID = customerSignup(customers);
                cout << "Account created! Logging you in...\n";
                customerPortalMenu(custID, customers, bookings, packages, invoices);
                break;
            }
            case 3:
                if (staffLogin(staffList)) staffMenu(customers, bookings, packages, invoices);
                break;
            case 0:
                cout << "Thank you for using the Travel Agency Management System. Goodbye!\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
