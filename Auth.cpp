#include "Auth.h"
#include "Customer.h"
#include "Utils.h"
#include <iostream>
using namespace std;

vector<Staff> createSampleStaff() {
    return {
        {1, "jushuen", "admin123"},
        {2, "brian", "admin123"},
        {3, "ethan", "admin123"}
    };
}

int customerSignup(vector<Customer>& customers) {
    showLogo();
    showSectionHeader("Customer Sign Up");
    if (!registerCustomer(customers)) return -1;
    return customers.back().id;
}

int customerLogin(const vector<Customer>& customers) {
    showLogo();
    showSectionHeader("Customer Login");
    printCentered("(Enter 0 at any prompt to cancel)");
    cout << endl;

    string email = getString("Email: ", 0, -8);

    if (email == "0") { 
        showInfo("Login cancelled.");
        return -1;
    }

    string password = getString("Password: ", 0, -10);

    if (password == "0") { 
        showInfo("Login cancelled.");
        return -1;
    }
    for (const auto& c : customers) {
        if (c.email == email && c.password == password) return c.id;
    }
    showError("Invalid email or password");
    return -1;
}

bool staffLogin(const vector<Staff>& staffList) {
    showLogo();
    showSectionHeader("Staff Login");
    printCentered("(Enter 0 at any prompt to cancel)");
    cout << endl;

    string username = getString("Username: ", 0, -10);

    if (username == "0") { 
        showInfo("Login cancelled.");
        return false;
    }

    string password = getString("Password: ", 0, -10);

    if (password == "0") { 
        showInfo("Login cancelled.");
        return false;
    }

    for (const auto& s : staffList) {
        if (s.username == username && s.password == password) return true;
    }
    showError("Invalid username or password");
    return false;
}
