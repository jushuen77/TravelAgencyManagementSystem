#ifndef UTILS_H
#define UTILS_H

#include <string>

// Shared input helpers used by all three module menus for consistent
// numeric/menu validation (invalid input reprompts instead of crashing).
void ignoreLine();
int readMenuChoice(int minVal, int maxVal);
int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
std::string readLine(const std::string& prompt);

#endif
