#include "TravelTypes.h"
#include "Customer.h"
#include "Booking.h"
#include "Billing.h"
#include "Auth.h"
#include "Utils.h"
#include "Voucher.h"
#include <vector>
#include <iostream>
using namespace std;

bool bookingBelongsTo(const vector<Booking>& bookings, int bookingID, int customerID) {
    for (const auto& b : bookings) {
        if (b.id == bookingID) return b.customerID == customerID;
    }
    return false;
}

void customerPortalMenu(int customerID, vector<Customer>& customers, vector<Booking>& bookings,
                         vector<Package>& packages, vector<Invoice>& invoices, vector<Voucher>& vouchers) {
    int choice;
    do {
        showLogo();
        int idx = findCustomerByID(customers, customerID);
        string name = (idx == -1) ? "Customer" : customers[idx].name;
        showSectionHeader("Welcome, " + name + "! ");
        printCentered("1. Browse Packages   ");
        printCentered("2. Book a Package    ");
        printCentered("3. View My Itinerary ");
        printCentered("4. Cancel My Booking ");
        printCentered("5. Make Payment      ");
        printCentered("6. Loyalty & Vouchers");
        printCentered("7. View My Profile   ");
        printCentered("8. Update My Profile ");
        printCentered("0. Logout            ");
        cout << endl;

        choice = getInt("Your choice: ", 0, 8);
        switch (choice) {
            case 1:
                displayAvailablePackages(packages);
                showInfo("");
                break;
            case 2:
                bookPackage(bookings, packages, customerID);
                break;
            case 3: {
                bool found = viewItinerary(bookings, packages, invoices, customerID);
                if (found) showInfo("");
                break;
            }
            case 4: {
                bool found = viewItinerary(bookings, packages, invoices, customerID);

                if (!found) break;

                cout << endl;
                int bookingID = getInt("Enter your booking ID to cancel: ");

                if (bookingID == 0) {
                    showInfo("Booking cancellation cancelled.");
                    break;
                }

                if (!bookingBelongsTo(bookings, bookingID, customerID)) showError("That booking does not belong to you");
                else cancelBooking(bookings, packages, bookingID);
                break;
            }
            case 5: {
                viewItinerary(bookings, packages, invoices, customerID);
                
                cout << endl;
                int bookingID = getInt("Enter your booking ID to pay for: ");
                if (bookingID == 0) {
                    showInfo("Cancellation cancelled.");
                    break;
                }
                if (!bookingBelongsTo(bookings, bookingID, customerID)) showError("That booking does not belong to you");
                else payForBooking(invoices, bookings, packages, customers, vouchers, bookingID);
                break;
            }
            case 6:
                int loyaltyChoice;
                
                do {
                    showSectionHeader("Loyalty & Vouchers");

                    int cIdx = findCustomerByID(customers, customerID);

                    if (cIdx != -1) {
                        printField("Earned Points: ", to_string(customers[cIdx].earnedPoints));

                        printField("Redeemable Points: ", to_string(customers[cIdx].redeemablePoints));
                    }
                    cout << endl;

                    printCentered("1. Redeem Points for Voucher");
                    printCentered("2. View My Vouchers         ");
                    printCentered("0. Back                     ");

                    cout << endl;

                    loyaltyChoice = getInt("Your choice: ", 0, 2);

                    switch (loyaltyChoice) {

                        case 1:
                            redeemVoucher(customers, vouchers, customerID);
                            break;

                        case 2:
                            viewVouchers(vouchers, customerID);
                            break;
                    }

                } while (loyaltyChoice != 0);
                
                break;
            case 7:
                if (idx != -1) printCustomerProfile(customers[idx]);
                break;
            case 8:
                updateCustomer(customers, customerID);
                break;
        }
    } while (choice != 0);

    printCentered("Logged out.");
}

void staffMenu(vector<Customer>& customers, vector<Booking>& bookings,
               vector<Package>& packages, vector<Invoice>& invoices, vector<Voucher>& vouchers) {
    int choice;
    do {
        showLogo();
        showSectionHeader("Staff Menu");
        printCentered("1. Customer Management             ");
        printCentered("2. Trip Package Booking & Itinerary");
        printCentered("3. Billing / Payment & Reporting   ");
        printCentered("0. Logout                          ");
        cout << endl;

        choice = getInt("Your choice: ", 0, 3);
        switch (choice) {
            case 1: customerMenu(customers); break;
            case 2: bookingMenu(bookings, packages, customers, invoices); break;
            case 3: billingMenu(invoices, bookings, packages, customers, vouchers); break;
        }
    } while (choice != 0);
    printCentered("Logged out.");
}

int main() {

    vector<Customer> customers;
    vector<Package> packages = createSamplePackages();
    vector<Booking> bookings;
    vector<Invoice> invoices;
    vector<Voucher> vouchers;
    vector<Staff> staffList = createSampleStaff();

    int choice;
    do {
        showLogo();
        showSectionHeader("Main Menu");

        printCentered("1. Customer Login  ");
        printCentered("2. Customer Sign Up");
        printCentered("3. Staff Login     ");
        printCentered("4. Exit            ");
        cout << endl;

        choice = getInt("Your choice: ", 1, 4);
        switch (choice) {
            case 1: {
                int custID = customerLogin(customers);
                if (custID != -1) customerPortalMenu(custID, customers, bookings, packages, invoices, vouchers);
                break;
            }
            case 2: {
                int custID = customerSignup(customers);
                if (custID == -1) break;
                showInfo("Account created! Logging you in...");
                customerPortalMenu(custID, customers, bookings, packages, invoices, vouchers);
                break;
            }
            case 3:
                if (staffLogin(staffList)) staffMenu(customers, bookings, packages, invoices, vouchers);
                break;
            case 4:
                cout << endl;
                printCentered("Thanks for using FreeRider Travel Agency Management System");
                printCentered("Goodbye, Please come again! =D");
                cout << endl;
                break;
        }
    } while (choice != 4);

    return 0;
}
