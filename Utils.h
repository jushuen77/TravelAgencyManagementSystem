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

// Shared console formatting so every menu/header/table looks consistent.
// Width is read from the actual terminal (clamped to a sane range) so
// headers center correctly regardless of window size.
int getTerminalWidth();
std::string centerText(const std::string& text, int width = getTerminalWidth());
void printBanner(const std::string& title);        // heavy '=' box, top-level screens
void printSectionHeader(const std::string& title);  // centered title + '-' divider, submenus

// Turns on ANSI escape support in classic Windows Command Prompt (Windows
// Terminal / VS Code already have it on). Call once at program start; a
// no-op everywhere else. Without this, color codes print as raw "\033[..."
// text instead of rendering as color.
void enableAnsiColors();

// Color-coded feedback so success/error/warning messages are visually
// distinct at a glance instead of blending into plain white text.
void printSuccess(const std::string& msg);
void printError(const std::string& msg);
void printWarning(const std::string& msg);
void printInfo(const std::string& msg);

#endif
