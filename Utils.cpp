#include "Utils.h"
#include <iostream>
#include <limits>
#include <cstdlib>
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
