#ifndef AUTH_H
#define AUTH_H

#include <vector>
#include "TravelTypes.h"

// Authentication: customer signup/login and staff login.

std::vector<Staff> createSampleStaff();
int customerSignup(std::vector<Customer>& customers);        // registers + returns the new customer's ID
int customerLogin(const std::vector<Customer>& customers);   // returns customer ID, or -1 on failure
bool staffLogin(const std::vector<Staff>& staffList);

#endif
