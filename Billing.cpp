#include "Billing.h"
#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace {
    int nextInvoiceID = 1; // static/global-style ID counter

    double getMembershipDiscount(const string& tier) {
        if (tier == "Gold") return 0.10;
        if (tier == "Silver") return 0.05;
        return 0.0;
    }
}

double calculateTotal(double packagePrice, int numPeople, double discountRate) {
    double subtotal = packagePrice * numPeople;
    double discounted = subtotal * (1.0 - discountRate);
    return discounted * (1.0 + SST_RATE);
}

bool processPayment(vector<Invoice>& invoices, int bookingID, double amountDue) {
    cout << fixed << setprecision(2);
    cout << "Amount due: $" << amountDue << "\n";
    cout << "Select payment method:\n  1. Cash\n  2. Card\n  3. E-Wallet\n  0. Cancel\n";
    int methodChoice = readMenuChoice(0, 3);
    if (methodChoice == 0) {
        cout << "Payment cancelled.\n";
        return false;
    }
    string method = (methodChoice == 1) ? "Cash" : (methodChoice == 2) ? "Card" : "E-Wallet";

    double amountPaid = readDouble("Enter payment amount: $");
    if (amountPaid <= 0) {
        cout << "Payment amount must be positive.\n";
        return false;
    }
    if (amountPaid < amountDue) {
        cout << "Insufficient payment. Amount due is $" << amountDue << ".\n";
        return false;
    }

    Invoice inv;
    inv.id = nextInvoiceID++;
    inv.bookingID = bookingID;
    inv.subtotal = amountDue / (1.0 + SST_RATE);
    inv.tax = amountDue - inv.subtotal;
    inv.totalAmount = amountDue;
    inv.paymentMethod = method;
    inv.paymentStatus = "Paid";
    invoices.push_back(inv);

    if (amountPaid > amountDue) {
        cout << "Change due: $" << (amountPaid - amountDue) << "\n";
    }

    generateInvoice(inv);
    return true;
}

void generateInvoice(const Invoice& invoice) {
    printBanner("INVOICE");
    cout << left << setw(20) << "  Invoice ID:" << invoice.id << "\n";
    cout << left << setw(20) << "  Booking ID:" << invoice.bookingID << "\n";
    cout << fixed << setprecision(2);
    cout << left << setw(20) << "  Subtotal:" << "$" << invoice.subtotal << "\n";
    cout << left << setw(20) << "  SST Tax (6%):" << "$" << invoice.tax << "\n";
    cout << left << setw(20) << "  Total Paid:" << "$" << invoice.totalAmount << "\n";
    cout << left << setw(20) << "  Payment Method:" << invoice.paymentMethod << "\n";
    cout << left << setw(20) << "  Status:" << invoice.paymentStatus << "\n";
    cout << string(getTerminalWidth(), '=') << "\n";
}

void generateSalesReport(const vector<Invoice>& invoices) {
    if (invoices.empty()) {
        cout << "No transactions recorded yet.\n";
        return;
    }
    double totalRevenue = 0;
    for (const auto& inv : invoices) totalRevenue += inv.totalAmount;

    printSectionHeader("Sales Report");
    cout << left << setw(10) << "InvID" << setw(10) << "BookID" << setw(12) << "Method" << setw(10) << "Total" << "\n";
    cout << string(42, '-') << "\n";
    cout << fixed << setprecision(2);
    for (const auto& inv : invoices) {
        cout << left << setw(10) << inv.id << setw(10) << inv.bookingID
             << setw(12) << inv.paymentMethod << "$" << inv.totalAmount << "\n";
    }
    cout << string(42, '-') << "\n";
    cout << "Total Transactions: " << invoices.size() << "\n";
    cout << "Total Revenue: $" << totalRevenue << "\n";
    cout << "Average Booking Value: $" << (totalRevenue / invoices.size()) << "\n";
}

bool payForBooking(vector<Invoice>& invoices, const vector<Booking>& bookings,
                    const vector<Package>& packages, const vector<Customer>& customers, int bookingID) {
    const Booking* target = nullptr;
    for (const auto& b : bookings) {
        if (b.id == bookingID) { target = &b; break; }
    }
    if (!target) {
        cout << "Booking not found.\n";
        return false;
    }
    if (target->status == "Cancelled") {
        cout << "This booking was cancelled and cannot be paid.\n";
        return false;
    }

    int pIdx = findPackageByID(packages, target->packageID);
    if (pIdx == -1) {
        cout << "Package data missing for this booking.\n";
        return false;
    }

    int cIdx = findCustomerByID(customers, target->customerID);
    double discount = (cIdx == -1) ? 0.0 : getMembershipDiscount(customers[cIdx].membershipTier);

    double amountDue = calculateTotal(packages[pIdx].price, target->numPeople, discount);
    return processPayment(invoices, bookingID, amountDue);
}

void billingMenu(vector<Invoice>& invoices, const vector<Booking>& bookings,
                  const vector<Package>& packages, const vector<Customer>& customers) {
    int choice;
    do {
        printSectionHeader("Billing / Payment & Reporting");
        cout << "  1. Process Payment for a Booking\n"
             << "  2. View Sales Report\n"
             << "  0. Back to Main Menu\n";
        choice = readMenuChoice(0, 2);
        try {
            switch (choice) {
                case 1: {
                    int bookingID = readInt("Enter booking ID to pay for: ");
                    payForBooking(invoices, bookings, packages, customers, bookingID);
                    break;
                }
                case 2:
                    generateSalesReport(invoices);
                    break;
            }
        } catch (const InputCancelled&) {
            cout << "\nCancelled.\n";
        }
    } while (choice != 0);
}
