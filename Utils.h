#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>

extern const int SCREEN_WIDTH;

// Display functions
void printLine(char ch = '=', int length = SCREEN_WIDTH);
void printCentered(const std::string& text, bool newline = true);
void printCentered(const std::string& text, int offset);
void printField(const std::string& label, const std::string& value, int width = 40);
void printTableRow(const std::vector<std::string>& values, const std::vector<int>& widths);
void showSectionHeader(const std::string& title);
void showLogo(bool info = true);
void showError(const std::string& text);
void showInfo(const std::string& text);

// Input functions
std::string getString(const std::string& prompt, int length = 0, int offset = 0);
char getChar(const std::string& prompt, bool checkYesNo = false);
int getInt(const std::string& prompt);
int getInt(const std::string& prompt, int min, int max);
double getDouble(const std::string& prompt);
double getDouble(const std::string& prompt, double min, double max);

#endif