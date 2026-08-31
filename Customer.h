#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <vector>
#include <algorithm>
#include "TravelTypes.h"

// Module 1: Customer / Member Management

bool registerCustomer(std::vector<Customer>& customers);
int findCustomerByID(const std::vector<Customer>& customers, int id);
bool updateCustomer(std::vector<Customer>& customers, int id);
bool deleteCustomer(std::vector<Customer>& customers, int id);
void displayAllCustomers(const std::vector<Customer>& customers);
void printCustomerProfile(const Customer& c);
void customerMenu(std::vector<Customer>& customers);

int calculatePointsEarned(double amount);

double calculateLoyaltyDiscount(
    int availablePoints,
    double subtotal,
    int& pointsUsed
);

#endif

