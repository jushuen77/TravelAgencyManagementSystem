#include "Auth.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
using namespace std;

vector<Staff> createSeedStaff() {
    // ponytail: hardcoded demo accounts, no staff-signup flow requested.
    return {
        {1, "admin", "admin123"},
        {2, "agent1", "agent123"}
    };
}

int customerSignup(vector<Customer>& customers) {
    cout << "\n--- Customer Sign Up ---\n";
    registerCustomer(customers);
    return customers.back().id;
}

int customerLogin(const vector<Customer>& customers) {
    cout << "\n--- Customer Login ---\n";
    cout << "(Enter 0 to cancel.)\n";
    string email = readLine("Email: ");
    string password = readLine("Password: ");
    for (const auto& c : customers) {
        if (c.email == email && c.password == password) return c.id;
    }
    cout << "Invalid email or password.\n";
    return -1;
}

bool staffLogin(const vector<Staff>& staffList) {
    cout << "\n--- Staff Login ---\n";
    cout << "(Enter 0 to cancel.)\n";
    string username = readLine("Username: ");
    string password = readLine("Password: ");
    for (const auto& s : staffList) {
        if (s.username == username && s.password == password) return true;
    }
    cout << "Invalid username or password.\n";
    return false;
}
