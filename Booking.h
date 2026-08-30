#ifndef BOOKING_H
#define BOOKING_H
#include <vector>
#include "TravelTypes.h"

// Module 2: Trip Package Booking & Itinerary Management

std::vector<Package> createSamplePackages();
void displayAvailablePackages(const std::vector<Package>& packages);
int findPackageByID(const std::vector<Package>& packages, int id);
void addPackage(std::vector<Package>& packages);
bool editPackage(std::vector<Package>& packages, int id);
bool bookPackage(std::vector<Booking>& bookings, std::vector<Package>& packages, int customerID);
bool cancelBooking(std::vector<Booking>& bookings, std::vector<Package>& packages, int bookingID);
void viewItinerary(const std::vector<Booking>& bookings, const std::vector<Package>& packages, 
    const std::vector<Invoice>& invoices, int customerID);
void viewAllBookings(const std::vector<Booking>& bookings, const std::vector<Package>& packages,
                      const std::vector<Customer>& customers);
void bookingMenu(std::vector<Booking>& bookings, std::vector<Package>& packages,
                  const std::vector<Customer>& customers, const std::vector<Invoice>& invoices);

#endif
