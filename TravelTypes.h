#ifndef TRAVELTYPES_H
#define TRAVELTYPES_H
#include <string>

// Shared named constants (Module 1 / Module 3)
const int STARTER_LOYALTY_POINTS = 100;
const double SST_RATE = 0.06;

struct Customer {
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string password;
    std::string membershipTier;
    int earnedPoints;
    int redeemablePoints;
};

struct Staff {
    int id;
    std::string username;
    std::string password;
};

struct Package {
    int id;
    std::string name;
    std::string destination;
    int days;
    double price;
    int seatsAvailable;
    std::string departureDate;
};

struct Booking {
    int id;
    int customerID;
    int packageID;
    int numPeople;
    std::string status; // "Confirmed" or "Cancelled"
};

struct Invoice {
    int id;
    int bookingID;
    double subtotal;
    double discount;
    double tax;
    double totalAmount;
    std::string paymentMethod;
    std::string paymentStatus;
};

struct Voucher {
    int id;
    int customerID;
    double value;
    std::string status;
};

struct Feedback {
    int id;
    int customerID;
    int rating;
    std::string comment;
};

#endif
