#ifndef BILLING_H
#define BILLING_H

#include <vector>
#include "TravelTypes.h"

// Module 3: Billing / Payment & Reporting

bool processPayment(std::vector<Invoice>& invoices, int bookingID, double subtotal, double membershipDiscount, double loyaltyDiscount, double amountDue);
void generateInvoice(const Invoice& invoice, const Booking& booking, const Package& package, const Customer& customer);
void generateSalesReport(const std::vector<Invoice>& invoices);
bool payForBooking(std::vector<Invoice>& invoices, const std::vector<Booking>& bookings,
                    const std::vector<Package>& packages, std::vector<Customer>& customers,
                    std::vector<Voucher>& vouchers, int bookingID);
void billingMenu(std::vector<Invoice>& invoices, const std::vector<Booking>& bookings,
                  const std::vector<Package>& packages, std::vector<Customer>& customers, std::vector<Voucher>& vouchers);

#endif
