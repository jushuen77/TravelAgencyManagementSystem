#include "Utils.h"
#include <iostream>
#include <limits>
#include <cstdlib>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

namespace {
    // Input stream closed (piped input exhausted, Ctrl+Z/Ctrl+D) - nothing
    // more can ever be read, so stop instead of re-prompting forever.
    void exitOnEOF() {
        if (cin.eof()) {
            cout << "\nInput stream closed. Exiting.\n";
            exit(0);
        }
    }
}

void ignoreLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readMenuChoice(int minVal, int maxVal) {
    int choice;
    while (true) {
        cout << "Enter choice (" << minVal << "-" << maxVal << "): ";
        if (cin >> choice && choice >= minVal && choice <= maxVal) {
            ignoreLine();
            return choice;
        }
        exitOnEOF();
        cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ".\n";
        cin.clear();
        ignoreLine();
    }
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            ignoreLine();
            if (value == 0) throw InputCancelled{};
            return value;
        }
        exitOnEOF();
        cout << "Invalid input. Please enter a whole number.\n";
        cin.clear();
        ignoreLine();
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            ignoreLine();
            if (value == 0) throw InputCancelled{};
            return value;
        }
        exitOnEOF();
        cout << "Invalid input. Please enter a number.\n";
        cin.clear();
        ignoreLine();
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string line;
    if (!getline(cin, line)) {
        exitOnEOF();
    }
    if (line == "0") throw InputCancelled{};
    return line;
}

int getTerminalWidth() {
    int width = 80; // fallback when the width can't be queried (e.g. piped/redirected output)
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    return clamp(width, 40, 100); // keep banners readable, not stretched across an ultra-wide window
}

string centerText(const string& text, int width) {
    if (static_cast<int>(text.length()) >= width) return text;
    int pad = (width - static_cast<int>(text.length())) / 2;
    return string(pad, ' ') + text;
}

void printBanner(const string& title) {
    string border(getTerminalWidth(), '=');
    cout << "\n" << border << "\n" << centerText(title) << "\n" << border << "\n";
}

void printSectionHeader(const string& title) {
    cout << "\n" << centerText(title) << "\n" << string(getTerminalWidth(), '-') << "\n";
}
