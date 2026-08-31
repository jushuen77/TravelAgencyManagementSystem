#include "Voucher.h"
#include "Utils.h"
#include "Customer.h"
#include <iostream>
#include <format>
using namespace std;

int nextVoucherID = 1;

struct VoucherOption {
    int pointsRequired;
    double value;
};

const vector<VoucherOption> voucherOptions = {
    {500, 5.00},
    {900, 10.00},
    {2000, 25.00},
    {3500, 50.00}
};

bool redeemVoucher(vector<Customer>& customers, vector<Voucher>& vouchers, int customerID) {

    int cIdx = findCustomerByID(customers, customerID);

    if (cIdx == -1) {
        showError("Customer not found");
        return false;
    }

    Customer& customer = customers[cIdx];

    showSectionHeader("Redeem Loyalty Points");

    cout << endl;
    printCentered("Available Vouchers");
    cout << endl;

    vector<int> widths = {8, 18, 18};

    printTableRow(
        {"Option", "Points", "Voucher"},
        widths
    );

    printLine('-', 44);

    for (size_t i = 0; i < voucherOptions.size(); ++i) {
        printTableRow(
            {
                to_string(i + 1),
                to_string(voucherOptions[i].pointsRequired),
                format("RM{:.2f}", voucherOptions[i].value)
            },
            widths
        );
    }

    printLine('-', 44);

    printCentered("Redeemable Points: " + to_string(customer.redeemablePoints));
    
    cout << endl;

    int choice = getInt("Select voucher (0 to cancel): ", 0, static_cast<int>(voucherOptions.size()));

    if (choice == 0) {
        showInfo("Voucher redemption cancelled.");
        return false;
    }

    const VoucherOption& selected = voucherOptions[choice - 1];

    if (customer.redeemablePoints < selected.pointsRequired) {
        showError(format("You need {} redeemable points for this voucher",selected.pointsRequired));
        return false;
    }

    customer.redeemablePoints -= selected.pointsRequired;

    Voucher voucher;

    voucher.id = nextVoucherID++;
    voucher.customerID = customerID;
    voucher.value = selected.value;
    voucher.status = "Available";

    vouchers.push_back(voucher);

    showInfo(
        format(
            "RM{:.2f} voucher redeemed successfully! Voucher ID: {}",
            voucher.value,
            voucher.id
        )
    );

    return true;
}

void viewVouchers(const vector<Voucher>& vouchers, int customerID) {

    showSectionHeader("My Vouchers");

    bool found = false;

    for (const auto& voucher: vouchers) {
        if (voucher.customerID == customerID) {
            found = true;
            break;
        }
    }

    if (!found) {
        showInfo("You do not have any vouchers.");
        return;
    }

    vector<int> widths = {15, 15, 15};

    printTableRow(
        {"Voucher ID", "Value", "Status"},
        widths
    );

    printLine('-', 40);

    for (const auto& voucher : vouchers) {
        if (voucher.customerID != customerID) continue;

        printTableRow(
            { to_string(voucher.id), format("RM{:.2f}", voucher.value), voucher.status },
            widths
        );
    }

    showInfo("");
}

double selectVoucher(vector<Voucher>& vouchers, int customerID, int& selectedVoucherID) {
    selectedVoucherID = -1;

    vector<int> availableIndexes;

    for (size_t i = 0; i < vouchers.size(); ++i) {
        if (vouchers[i].customerID == customerID && vouchers[i].status == "Available") {
            availableIndexes.push_back(static_cast<int>(i));
        }
    }

    // No avialable vouchers
    if (availableIndexes.empty()) { 
        showInfo("You do not have any available vouchers.");
        return 0.0; 
    }

    cout << endl;
    printCentered("Available Vouchers");

    vector<int> widths = {8, 15, 15};

    printTableRow(
        {"Voucher ID", "Value", "Status"},
        widths
    );

    printLine('-', 40);

    for (size_t i = 0; i < availableIndexes.size(); ++i) {

        const Voucher& voucher = vouchers[availableIndexes[i]];

        printTableRow(
            {to_string(voucher.id), format("RM{:.2f}", voucher.value), voucher.status},
            widths
        );
    }

    cout << endl;

    int choice = getInt("Select voucher (0 to cancel): ", 0, static_cast<int>(availableIndexes.size()));

    if (choice == 0) {
        showInfo("Voucher selection cancelled.");
        return 0.0;
    }

    // Find selected voucher
    for (int index: availableIndexes) {
        if (vouchers[index].id == choice) {
            selectedVoucherID = vouchers[index].id;
            return vouchers[index].value;
        }
    }
    
    showError("Invalid or unavailable voucher");
    return 0.0;
}