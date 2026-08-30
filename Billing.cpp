#include "Billing.h"
#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <format>
using namespace std;

int nextInvoiceID = 1;

double getMembershipDiscount(const string& tier) {
    if (tier == "Gold") return 0.10;

    if (tier == "Silver") return 0.05;

    return 0.0;
}

double calculateTotal(double packagePrice, int numPeople, double discountRate) {
    double subtotal = packagePrice * numPeople;
    double discounted = subtotal * (1.0 - discountRate);

    return discounted * (1.0 + SST_RATE);
}

bool processPayment(vector<Invoice>& invoices, int bookingID, double amountDue) {
    showSectionHeader("Process Payment");
    printCentered(format("Amount due: RM{:.2f}", amountDue));

    printCentered("Select Payment Method");
    printCentered("1. Cash    ");
    printCentered("2. Card    ");
    printCentered("3. E-Wallet");
    printCentered("0. Cancel  ");

    int methodChoice = getInt("Your choice: ", 0, 3);

    if (methodChoice == 0) {
        printCentered("Payment cancelled.");
        return false;
    }

    string method;

    switch (methodChoice) {
        case 1: method = "Cash"; break;

        case 2: method = "Card"; break;

        default: method = "E-Wallet"; break;
    }

    double amountPaid = getDouble("Enter payment amount: RM", 0, 999999999);

    if (amountPaid == 0) {
        printCentered("Payment cancelled.");
        return false;
    }

    if (amountPaid < amountDue) {
        showError(format("Insufficient payment. Amount due is ${:.2f}", amountDue));
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
        printCentered(format("Change due: ${:.2f}",amountPaid - amountDue));
    }

    return true;
}

void generateInvoice(const Invoice& invoice, const Booking& booking, const Package& package, const Customer& customer) {
    showSectionHeader("INVOICE");

    vector<int> widths = {20, 30};

    printTableRow(
        {"Field", "Row"},
        widths
    );

    printLine('-', 50);


    printTableRow(
        {"Invoice ID", to_string(invoice.id)},
        widths
    );

    printTableRow(
        {"Booking ID", to_string(invoice.bookingID)},
        widths
    );

    printTableRow(
        {"Customer ID", to_string(customer.id)},
        widths
    );

    printTableRow(
        {"Customer Name", customer.name},
        widths
    );

    printTableRow(
        {"People", to_string(booking.numPeople)},
        widths
    );

    printTableRow(
        {"Package", package.name},
        widths
    );

    printTableRow(
        {"Destination", package.destination},
        widths
    );

    printTableRow(
        {"Duration", to_string(package.days) + " Days"},
        widths
    );
    
    printTableRow(
        {"Subtotal", "RM" + format("RM{:.2f}", invoice.subtotal)},
        widths
    );

    printTableRow(
        {"SST Tax (6%)", "RM" + format("RM{:.2f}", invoice.tax)},
        widths
    );

    printTableRow(
        {"Total Paid", "RM" + format("RM{:.2f}", invoice.totalAmount)},
        widths
    );

    printTableRow(
        {"Payment Method", invoice.paymentMethod},
        widths
    );

    printTableRow(
        {"Status", invoice.paymentStatus},
        widths
    );

    cout << fixed << setprecision(2);

    cout << left << setw(20) << "  Invoice ID:" << invoice.id << "\n";

    cout << left << setw(20) << "  Booking ID:" << invoice.bookingID << "\n";

    cout << left << setw(20) << "  Subtotal:" << "$" << invoice.subtotal << "\n";

    cout << left << setw(20) << "  SST Tax (6%):" << "$" << invoice.tax << "\n";

    cout << left << setw(20) << "  Total Paid:" << "$" << invoice.totalAmount << "\n";

    cout << left << setw(20) << "  Payment Method:" << invoice.paymentMethod << "\n";

    cout << left << setw(20) << "  Status:" << invoice.paymentStatus << "\n";

    cout << string(50, '=') << "\n";
}

void generateSalesReport(const vector<Invoice>& invoices) {
    if (invoices.empty()) {
        showInfo("No transactions recorded yet.");
        return;
    }

    double totalRevenue = 0;

    for (const auto& inv : invoices) totalRevenue += inv.totalAmount;

    showSectionHeader("Sales Report");

    vector<int> widths = {10, 10, 12, 12};

    printTableRow(
        {"InvID", "BookID", "Method", "Total"},
        widths
    );

    printLine('-', 44);

    for (const auto& inv : invoices) {
        printTableRow(
            {to_string(inv.id), to_string(inv.bookingID), inv.paymentMethod, format("${:.2f}", inv.totalAmount)},
            widths
        );
    }

    printLine('-', 44);

    printField("Total Transactions: ", to_string(invoices.size()));
    printField("Total Revenue: ", format("RM{:.2f}, totalRevenue"));
    printField("Average Booking Value: ", format("RM{:.2f}", totalRevenue / invoices.size()));
}

bool bookingAlreadyPaid(const vector<Invoice>& invoices,int bookingID) {
    for (const auto& invoice : invoices) {
        if (invoice.bookingID == bookingID && invoice.paymentStatus == "Paid") return true;
    }

    return false;
}

bool payForBooking(vector<Invoice>& invoices, const vector<Booking>& bookings, const vector<Package>& packages,
                    const vector<Customer>& customers, int bookingID) {

    const Booking* target = nullptr;

    for (const auto& b : bookings) {
        if (b.id == bookingID) {
            target = &b;
            break;
        }
    }

    if (!target) {
        showError("Booking not found");
        return false;
    }

    if (target->status == "Cancelled") {
        showError("This booking was cancelled and cannot be paid");
        return false;
    }

    if (bookingAlreadyPaid(invoices, bookingID)) {
        showError("This booking has already been paid");
        return false;
    }

    int pIdx = findPackageByID(packages, target->packageID);

    if (pIdx == -1) {
        showError("Package data missing for this booking");
        return false;
    }

    int cIdx = findCustomerByID(customers,target->customerID);

    double discount = (cIdx == -1) ? 0.0 : getMembershipDiscount(customers[cIdx].membershipTier);

    double amountDue = calculateTotal(packages[pIdx].price, target->numPeople, discount);

    if (processPayment(invoices,bookingID,amountDue)) {
        generateInvoice(invoices.back(), *target, packages[pIdx], customers[cIdx]);

        return true;
    }

    return false;
}

void billingMenu( vector<Invoice>& invoices, const vector<Booking>& bookings, const vector<Package>& packages,
                 const vector<Customer>& customers) {
    int choice;

    do {
        showSectionHeader("Billing / Payment & Reporting");

        printCentered("1. Process Payment for a Booking");
        printCentered("2. View Sales Report            ");
        printCentered("0. Back to Main Menu            ");

        choice = getInt("Your choice: ", 0, 2);

        switch (choice) {
            case 1: {
                viewAllBookings(bookings, packages, customers);

                int bookingID = getInt("Enter booking ID to pay for: ", 0, 999999);

                if (bookingID == 0) {
                    printCentered("Payment cancelled.");
                    break;
                }

                payForBooking(invoices, bookings, packages, customers, bookingID);

                break;
            }

            case 2:
                generateSalesReport(invoices);
                break;
        }

    } while (choice != 0);
}