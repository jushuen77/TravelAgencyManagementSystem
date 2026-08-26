#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <cctype>
using namespace std;

namespace {
    int nextCustomerID = 1; // static/global-style ID counter

    bool isValidEmail(const string& email) {
        size_t at = email.find('@');
        if (at == string::npos || at == 0) return false;
        size_t dot = email.find('.', at);
        return dot != string::npos && dot < email.size() - 1;
    }

    bool isValidPhone(const string& phone) {
        if (phone.size() < 7 || phone.size() > 15) return false;
        for (char c : phone) {
            if (!isdigit(static_cast<unsigned char>(c))) return false;
        }
        return true;
    }

    bool emailExists(const vector<Customer>& customers, const string& email) {
        for (const auto& c : customers) {
            if (c.email == email) return true;
        }
        return false;
    }

    string chooseMembershipTier() {
        cout << "Select membership tier:\n"
             << "  1. Standard\n"
             << "  2. Silver\n"
             << "  3. Gold\n";
        int choice = readMenuChoice(1, 3);
        switch (choice) {
            case 2: return "Silver";
            case 3: return "Gold";
            default: return "Standard";
        }
    }
}

void registerCustomer(vector<Customer>& customers) {
    cout << "(Enter 0 at any prompt to cancel.)\n";
    Customer c;

    string name;
    do {
        name = readLine("Enter customer name: ");
        if (name.empty()) cout << "Name cannot be empty.\n";
    } while (name.empty());
    c.name = name;

    string email;
    while (true) {
        email = readLine("Enter email: ");
        if (!isValidEmail(email)) { cout << "Invalid email format.\n"; continue; }
        if (emailExists(customers, email)) { cout << "An account with this email already exists.\n"; continue; }
        break;
    }
    c.email = email;

    string phone;
    do {
        phone = readLine("Enter phone number (digits only): ");
        if (!isValidPhone(phone)) cout << "Invalid phone number (must be 7-15 digits).\n";
    } while (!isValidPhone(phone));
    c.phone = phone;

    string password;
    while (true) {
        password = readLine("Create a password (min 4 characters): ");
        if (password.size() < 4) { cout << "Password too short.\n"; continue; }
        string confirm = readLine("Confirm password: ");
        if (confirm != password) { cout << "Passwords do not match. Try again.\n"; continue; }
        break;
    }
    c.password = password;

    c.membershipTier = chooseMembershipTier();
    c.loyaltyPoints = STARTER_LOYALTY_POINTS;
    c.id = nextCustomerID++;

    customers.push_back(c);
    cout << "Customer registered successfully. Assigned ID: " << c.id << "\n";
}

int findCustomerByID(const vector<Customer>& customers, int id) {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

bool updateCustomer(vector<Customer>& customers, int id) {
    int idx = findCustomerByID(customers, id);
    if (idx == -1) {
        cout << "Customer ID " << id << " not found.\n";
        return false;
    }

    Customer& c = customers[idx];

    cout << "(Enter 0 at any prompt to cancel the update.)\n";
    string name = readLine("Enter new name (blank to keep \"" + c.name + "\"): ");
    if (!name.empty()) c.name = name;

    string email = readLine("Enter new email (blank to keep \"" + c.email + "\"): ");
    if (!email.empty()) {
        if (isValidEmail(email)) c.email = email;
        else cout << "Invalid email format, kept previous value.\n";
    }

    string phone = readLine("Enter new phone (blank to keep \"" + c.phone + "\"): ");
    if (!phone.empty()) {
        if (isValidPhone(phone)) c.phone = phone;
        else cout << "Invalid phone number, kept previous value.\n";
    }

    cout << "Customer updated successfully.\n";
    return true;
}

bool deleteCustomer(vector<Customer>& customers, int id) {
    int idx = findCustomerByID(customers, id);
    if (idx == -1) {
        cout << "Customer ID " << id << " not found.\n";
        return false;
    }
    customers.erase(customers.begin() + idx);
    cout << "Customer deleted successfully.\n";
    return true;
}

void printCustomerProfile(const Customer& c) {
    cout << "ID: " << c.id << "\nName: " << c.name << "\nEmail: " << c.email
         << "\nPhone: " << c.phone << "\nTier: " << c.membershipTier
         << "\nLoyalty Points: " << c.loyaltyPoints << "\n";
}

void displayAllCustomers(const vector<Customer>& customers) {
    if (customers.empty()) {
        cout << "No customers registered yet.\n";
        return;
    }
    cout << "\n" << centerText("Registered Customers", 88) << "\n";
    cout << left
         << setw(6) << "ID" << setw(20) << "Name" << setw(25) << "Email"
         << setw(15) << "Phone" << setw(12) << "Tier" << setw(10) << "Points" << "\n";
    cout << string(88, '-') << "\n";
    for (const auto& c : customers) {
        cout << left
             << setw(6) << c.id << setw(20) << c.name << setw(25) << c.email
             << setw(15) << c.phone << setw(12) << c.membershipTier << setw(10) << c.loyaltyPoints << "\n";
    }
}

void customerMenu(vector<Customer>& customers) {
    int choice;
    do {
        printSectionHeader("Customer / Member Management");
        cout << "  1. Register New Customer\n"
             << "  2. Update Customer\n"
             << "  3. Delete Customer\n"
             << "  4. Search Customer by ID\n"
             << "  5. View All Customers\n"
             << "  0. Back to Main Menu\n";
        choice = readMenuChoice(0, 5);
        try {
            switch (choice) {
                case 1:
                    registerCustomer(customers);
                    break;
                case 2: {
                    displayAllCustomers(customers);
                    int id = readInt("Enter customer ID: ");
                    updateCustomer(customers, id);
                    break;
                }
                case 3: {
                    displayAllCustomers(customers);
                    int id = readInt("Enter customer ID: ");
                    deleteCustomer(customers, id);
                    break;
                }
                case 4: {
                    int id = readInt("Enter customer ID: ");
                    int idx = findCustomerByID(customers, id);
                    if (idx == -1) {
                        cout << "Customer not found.\n";
                    } else {
                        printCustomerProfile(customers[idx]);
                    }
                    break;
                }
                case 5:
                    displayAllCustomers(customers);
                    break;
            }
        } catch (const InputCancelled&) {
            cout << "\nCancelled.\n";
        }
    } while (choice != 0);
}
