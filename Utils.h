#ifndef UTILS_H
#define UTILS_H

#include <string>

// Thrown by readLine/readInt/readDouble when the user enters "0" to bail
// out of a multi-field prompt. Menu loops catch this around their switch
// and just redraw, so cancelling always returns to the nearest menu.
struct InputCancelled {};

// Shared input helpers used by all three module menus for consistent
// numeric/menu validation (invalid input reprompts instead of crashing).
void ignoreLine();
int readMenuChoice(int minVal, int maxVal);
int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
std::string readLine(const std::string& prompt);

#endif
