#ifndef BILLING_H
#define BILLING_H

#include <vector>
#include "TravelTypes.h"

// Module 3: Billing / Payment & Reporting

double calculateTotal(double packagePrice, int numPeople, double discountRate);
bool processPayment(std::vector<Invoice>& invoices, int bookingID, double amountDue);
void generateInvoice(const Invoice& invoice);
void generateSalesReport(const std::vector<Invoice>& invoices);
bool payForBooking(std::vector<Invoice>& invoices, const std::vector<Booking>& bookings,
                    const std::vector<Package>& packages, const std::vector<Customer>& customers, int bookingID);
void billingMenu(std::vector<Invoice>& invoices, const std::vector<Booking>& bookings,
                  const std::vector<Package>& packages, const std::vector<Customer>& customers);

#endif
