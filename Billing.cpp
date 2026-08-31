#include "Billing.h"
#include "Booking.h"
#include "Customer.h"
#include "Utils.h"
#include "Voucher.h"
#include <iostream>
#include <vector>
#include <format>
using namespace std;

int nextInvoiceID = 1;

double getMembershipDiscount(const string& tier) {
    if (tier == "Gold") return 0.10;
    if (tier == "Silver") return 0.05;
    return 0.0;
}

bool processPayment(vector<Invoice>& invoices, int bookingID, 
                    double subtotal, double membershipDiscount, double voucherDiscount, double amountDue) {
    showSectionHeader("Process Payment");

    printField("Subtotal: ", format("RM{:.2f}", subtotal));
    printField("Membership Discount: ", format("-RM{:.2f}", membershipDiscount));
    printField("Voucher Discount: ",format("-RM{:.2f}", voucherDiscount));

    double tax = amountDue - (subtotal - membershipDiscount - voucherDiscount);
    printField("SST Tax: ", format("RM{:.2f}", tax));
    printField("Amount Due: ", format("RM{:.2f}", amountDue));

    cout << endl;
    printCentered("Select Payment Method");
    printCentered("1. Cash    ");
    printCentered("2. Card    ");
    printCentered("3. E-Wallet");
    printCentered("0. Cancel  ");

    cout << endl;
    int methodChoice = getInt("Your choice: ", 0, 3);

    if (methodChoice == 0) {
        showInfo("Payment cancelled.");
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
        showError(format("Insufficient payment. Amount due is RM{:.2f}", amountDue));
        return false;
    }

    Invoice inv;

    inv.id = nextInvoiceID++;
    inv.bookingID = bookingID;
    inv.subtotal = subtotal;
    inv.discount = membershipDiscount + voucherDiscount;
    inv.tax = tax;
    inv.totalAmount = amountDue;
    inv.paymentMethod = method;
    inv.paymentStatus = "Paid";

    invoices.push_back(inv);

    if (amountPaid > amountDue) {
        printCentered(format("Change due: RM{:.2f}",amountPaid - amountDue));
    }

    return true;
}

void generateInvoice(const Invoice& invoice, const Booking& booking, const Package& package, const Customer& customer) {
    showLogo();
    showSectionHeader("INVOICE");

    vector<int> widths = {20, 30};

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
        {"Departure", package.departureDate},
        {20, 30}
    );

    printTableRow(
        {"Duration", to_string(package.days) + " Days"},
        widths
    );
    
    printLine('-', 50);
    
    printTableRow(
        {"Subtotal", format("RM{:.2f}", invoice.subtotal)},
        widths
    );

    printTableRow(
        {"Discount", format("-RM{:.2f}", invoice.discount)},
        widths
    );
    printTableRow(
        {"SST Tax (6%)", format("RM{:.2f}", invoice.tax)},
        widths
    );

    printTableRow(
        {"Total Paid", format("RM{:.2f}", invoice.totalAmount)},
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

    printLine('-', 50);

    showInfo("");
}

void generateSalesReport(const vector<Invoice>& invoices) {
    showLogo();

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
            {to_string(inv.id), to_string(inv.bookingID), inv.paymentMethod, format("RM{:.2f}", inv.totalAmount)},
            widths
        );
    }

    printLine('-', 44);

    printField("Total Transactions: ", to_string(invoices.size()));
    printField("Total Revenue: ", format("RM{:.2f}", totalRevenue));
    printField("Average Booking Value: ", format("RM{:.2f}", totalRevenue / invoices.size()));

    showInfo("");
}

bool bookingAlreadyPaid(const vector<Invoice>& invoices,int bookingID) {
    for (const auto& invoice : invoices) {
        if (invoice.bookingID == bookingID && invoice.paymentStatus == "Paid") return true;
    }

    return false;
}

bool payForBooking(vector<Invoice>& invoices, const vector<Booking>& bookings, const vector<Package>& packages,
                    vector<Customer>& customers, vector<Voucher>& vouchers, int bookingID) {

    // find specfic booking
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

    if (cIdx == -1) {
        showError("Customer data missing for this booking");
        return false;
    }

    // Membership discount

    double discountRate = getMembershipDiscount(customers[cIdx].membershipTier);

    double subtotal = packages[pIdx].price * target->numPeople;

    double membershipDiscount = subtotal * discountRate;

    double discountedSubtotal = subtotal - membershipDiscount;

    // Calculate voucher discount
    int selectedVoucherID = -1;
    double voucherDiscount = 0.0;

    char useVoucher = getChar("Would you like to use a voucher (Y/N): ", true);

    if (useVoucher == 'Y') {
        voucherDiscount = selectVoucher(vouchers, target->customerID, selectedVoucherID);

        if (voucherDiscount > discountedSubtotal) { voucherDiscount = discountedSubtotal; }
    }

    // Calculate final subtotal
    double finalSubtotal = discountedSubtotal - voucherDiscount;
    double amountDue = finalSubtotal * (1.0 + SST_RATE);

    if (processPayment(invoices, bookingID, subtotal, membershipDiscount, voucherDiscount, amountDue)) {

        // consume voucher
        if (selectedVoucherID != -1) {
            for (auto& voucher: vouchers) {
                if (voucher.id == selectedVoucherID && voucher.status == "Available") {
                    voucher.status = "Used";
                    break;
                }
            }
        }

        // Update earned & redeemable points
        int pointsEarned = calculatePointsEarned(amountDue);
        customers[cIdx].earnedPoints += pointsEarned;
        customers[cIdx].redeemablePoints += pointsEarned;

        // Update membership tier
        updateMembershipTier(customers[cIdx]);

        showInfo("Loyalty Points Earned: " + to_string(pointsEarned));

        generateInvoice(invoices.back(), *target, packages[pIdx], customers[cIdx]);

        return true;
    }

    return false;
}

void billingMenu( vector<Invoice>& invoices, const vector<Booking>& bookings, const vector<Package>& packages,
                 vector<Customer>& customers, vector<Voucher>& vouchers) {
    int choice;

    do {
        showSectionHeader("Billing / Payment & Reporting");

        printCentered("1. Process Payment for a Booking");
        printCentered("2. View Sales Report            ");
        printCentered("0. Back to Staff Menu           ");

        cout << endl;
        choice = getInt("Your choice: ", 0, 2);

        switch (choice) {
            case 1: {
                viewAllBookings(bookings, packages, customers);

                cout << endl;
                int bookingID = getInt("Enter booking ID to pay for: ", 0, 999999);

                if (bookingID == 0) {
                    printCentered("Payment cancelled.");
                    break;
                }

                payForBooking(invoices, bookings, packages, customers, vouchers, bookingID);

                break;
            }

            case 2:
                generateSalesReport(invoices);
                break;
        }

    } while (choice != 0);
}