#ifndef VOUCHER_H
#define VOUCHER_H
#include "TravelTypes.h"
#include <vector>

bool redeemVoucher(std::vector<Customer>& customers, std::vector<Voucher>& vouchers, int customerID);
void viewVouchers(const std::vector<Voucher>& vouchers, int customerID);
double selectVoucher(std::vector<Voucher>& vouchers, int customerID, int& selectedVoucherID);

#endif