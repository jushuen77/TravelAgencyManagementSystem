#include "Customer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <regex>
using namespace std;

int nextCustomerID = 1;

//use regex to verify if is valid email pattern
bool isValidEmail(const string& email) {
    const regex pattern(R"(^[\w.%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    return regex_match(email, pattern);
}

//use regex to verify same thing as email
bool isValidPhone(const string& phone) {
    if (phone.size() < 7 || phone.size() > 15)
        return false;

    for (char c : phone) {
        if (!isdigit(static_cast<unsigned char>(c)))
            return false;
    }

    return true;
}

//check if email exists
bool emailExists(const vector<Customer>& customers,const string& email) {
    for (const auto& c : customers) {
        if (c.email == email)
            return true;
    }

    return false;
}


string chooseMembershipTier() {
    cout << endl;
    printCentered("Select membership tier");
    printCentered("1. Standard");
    printCentered("2. Silver  ");
    printCentered("3. Gold    ");
    cout << endl;

    int choice = getInt("Your choice: ", 0, 3);

    if (choice == 0)
        return "";

    switch (choice) {
        case 2:
            return "Silver";

        case 3:
            return "Gold";

        default:
            return "Standard";
    }
}

bool registerCustomer(vector<Customer>& customers) {
    Customer c;

    printCentered("(Enter 0 at any prompt to cancel)");
    cout << endl;
    // Name
    while (true) {
        string name = getString("Enter name: ", 0, -10);

        if (name == "0") {
            showInfo("Registration cancelled.");
            return false;
        }

        c.name = name;
        break;
    }

    // Email (will keep trying to get user email, and only exit once one of the criteria below is hit)
    while (true) {
        string email = getString("Enter email: ", 0, -10);

        if (email == "0") {
            showInfo("Registration cancelled.");
            return false;
        }

        if (!isValidEmail(email)) {
            showError("Invalid email format");
            continue;
        }

        if (emailExists(customers, email)) {
            showError("An account with this email already exists");
            continue;
        }

        c.email = email;
        break;
    }

    // Phone (same logic as he email above)
    while (true) {
        string phone = getString("Enter phone number (digits only): ", 0, -21);

        if (phone == "0") {
            showInfo("Registration cancelled.");
            return false;
        }

        if (!isValidPhone(phone)) {
            showError("Invalid phone number (must be 7-15 digits)");
            continue;
        }

        c.phone = phone;
        break;
    }

    // Password (same logic as above)
    while (true) {
        string password = getString("Create a password (min 4 characters): ", 0, -23);

        if (password == "0") {
            showInfo("Registration cancelled.");
            return false;
        }

        if (password.size() < 4) {
            showError("Password must contain at least 4 characters");
            continue;
        }

        string confirm = getString("Confirm password: ", 0, -13);

        if (confirm == "0") {
            showInfo("Registration cancelled.");
            return false;
        }

        if (confirm != password) {
            showError("Passwords do not match. Try again");
            continue;
        }

        c.password = password;
        break;
    }

    // Membership tier
    c.membershipTier = chooseMembershipTier();

    if (c.membershipTier.empty()) {
        showInfo("Registration cancelled.");
        return false;
    }

    c.earnedPoints = STARTER_LOYALTY_POINTS;
    c.redeemablePoints = STARTER_LOYALTY_POINTS;

    c.id = nextCustomerID++;

    customers.push_back(c);

    cout << endl;
    printCentered("Customer registered successfully. Assigned ID: " + to_string(c.id));

    return true;
}

//find and get customer by their id
int findCustomerByID(const vector<Customer>& customers,int id) {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i].id == id)
            return static_cast<int>(i);
    }

    return -1;
}

//update customer details function
bool updateCustomer(vector<Customer>& customers, int id) {
    int idx = findCustomerByID(customers, id);

    if (idx == -1) {
        showError("Customer ID " + to_string(id) + " not found");
        return false;
    }

    Customer& c = customers[idx];

    showSectionHeader("Update Profile");
    printCentered("(Enter -1 at any prompt to cancel, 0 to keep current value)");
    cout << endl;

    // Name
    string name = getString("Enter new name: ");

    if (name == "-1") {
        showInfo("Update cancelled.");
        return false;
    }

    if (name == "0") {
        name = c.name;
    }

    // Email
    string email;

    while (true) {
        email = getString("Enter new email: ");

        if (email == "-1") {
            showInfo("Update cancelled.");
            return false;
        }

        if (email == "0") {
            email = c.email;
            break;
        }

        if (!isValidEmail(email)) {
            showError("Invalid email format");
            continue;
        }

        if (email != c.email && emailExists(customers, email)) {
            showError("An account with this email already exists");
            continue;
        }

        break;
    }

    // Phone
    string phone;

    while (true) {
        phone = getString("Enter new phone number (digits only): ");

        if (phone == "-1") {
            showInfo("Update cancelled.");
            return false;
        }

        if (phone == "0") {
            phone = c.phone;
            break;
        }

        if (!isValidPhone(phone)) {
            showError("Invalid phone number (must be 7-15 digits)");
            continue;
        }

        break;
    }

    // Apply changes only after all fields are valid
    c.name = name;
    c.email = email;
    c.phone = phone;

    showInfo("Customer updated successfully.");
    return true;
}


bool deleteCustomer(vector<Customer>& customers, int id) {
    int idx = findCustomerByID(customers, id);

    if (idx == -1) {
        showError("Customer ID " + to_string(id) + " not found");
        return false;
    }

    char confirm = getChar("Are you sure you want to delete this customer (Y/N): ", true);
    if (confirm == 'N') {
        showInfo("Deletion cancelled.");
        return false;
    }

    customers.erase(customers.begin() + idx);

    showInfo("Customer deleted successfully.");
    return true;
}

void printCustomerProfile(const Customer& c) {
    showSectionHeader("User Profile");
    printField("ID: ", to_string(c.id));
    printField("Name: ", c.name);
    printField("Email: ", c.email);
    printField("Phone: ", c.phone);
    printField("Tier: ", c.membershipTier);
    printField("Earned Points: ", to_string(c.earnedPoints));
    printField("Redeemable Points: ", to_string(c.redeemablePoints));
    showInfo("");
    cout << endl;
}

bool displayAllCustomers(const vector<Customer>& customers) {
    showSectionHeader("Registered Customers");

    if (customers.empty()) {
        printCentered("No customers registered yet.");
        return false;
    }

    vector<int> widths = {6, 16, 23, 15, 12, 13, 15};

    printTableRow(
        {"ID", "Name", "Email", "Phone", "Tier", "Earned", "Redeemable"},
        widths);

    printLine('-', 100);

    for (const auto &c : customers) {
        printTableRow(
            {to_string(c.id), c.name, c.email, c.phone, c.membershipTier, 
                to_string(c.earnedPoints), to_string(c.redeemablePoints)},
            widths
        );
    }

    return true;
}

void customerMenu(vector<Customer>& customers) {
    int choice;

    do {
        showSectionHeader("Customer Management");
        printCentered("1. Register New Customer");
        printCentered("2. Update Customer      ");
        printCentered("3. Delete Customer      ");
        printCentered("4. Search Customer by ID");
        printCentered("5. View All Customers   ");
        printCentered("0. Back to Main Menu    ");
        cout << endl;

        choice = getInt("Your choice: ", 0, 5);

        switch (choice) {
            case 1:
                registerCustomer(customers);
                break;

            case 2: {
                if (!displayAllCustomers(customers)) {
                    showInfo("");
                    break;
                }

                cout << endl;
                int id = getInt("Enter customer ID (0 to cancel): ", 0, 999999);

                if (id == 0) {
                    showInfo("Update cancelled.");
                    break;
                }

                updateCustomer(customers, id);
                break;
            }

            case 3: {
                if (!displayAllCustomers(customers)) {
                    showInfo("");
                    break;
                }

                cout << endl;
                int id = getInt("Enter customer ID (0 to cancel): ", 0, 999999);

                if (id == 0) {
                    showInfo("Delete cancelled.");
                    break;
                }

                deleteCustomer(customers, id);
                break;
            }

            case 4: {
                int id = getInt("Enter customer ID (0 to cancel): ", 0, 999999);

                if (id == 0) {
                    showInfo("Search cancelled.");
                    break;
                }

                int idx = findCustomerByID(customers, id);

                if (idx == -1) {
                    showError("Customer not found");
                } else {
                    printCustomerProfile(customers[idx]);
                }

                break;
            }

            case 5:
                displayAllCustomers(customers);
                showInfo("");
                break;
        }

    } while (choice != 0);
}

int calculatePointsEarned(double amount) {
    return static_cast<int>(amount);
}

//the code pretty self explanatory idk what to explain\
//basically get arguments pass value then check if exsits or no
//then maybe get user input will push pop or something into an list or array to store the data.
//then a bunch of test cases and what not(input validation)