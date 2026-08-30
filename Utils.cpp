#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <format>
#include <vector>
#include <stdexcept>
using namespace std;

const int SCREEN_WIDTH = 100;

void printLine(char ch, int length) {
    printCentered(string(length, ch), false);
    cout << endl;
}

void printCentered(const string& text, bool newline) {
    int length = static_cast<int>(text.length());

    if (length >= SCREEN_WIDTH) {
        cout << text;
    } else {
        int leftPadding = (SCREEN_WIDTH - length) / 2;
        cout << string(leftPadding, ' ') << text;
    }

    if (newline) {
        cout << endl;
    }
}

void printCentered(const string& text, int offset) {
    int length = static_cast<int>(text.length());

    if (length >= SCREEN_WIDTH) {
        cout << text;
    } else {
        int leftPadding = (SCREEN_WIDTH - length) / 2 + offset;
        cout << string(leftPadding, ' ') << text << endl;
    }
}

void printField(const string& label, const string& value, int width) {
    int leftPadding = (SCREEN_WIDTH - width) / 2;

    cout << string(leftPadding, ' ')
         << right << setw(18) << label
         << value << endl;
}

void printTableRow(const vector<string>& values, const vector<int>& widths) {
    if (values.size() != widths.size()) return;

    int totalWidth = 0;

    for (int width : widths) totalWidth += width;

    int leftPadding = (SCREEN_WIDTH - totalWidth) / 2;

    cout << string(leftPadding, ' ');

    for (size_t i = 0; i < values.size(); ++i)
        cout << left << setw(widths[i]) << values[i];

    cout << endl;
}

void showSectionHeader(const string& title) {
    cout << endl;
    printCentered(title);
    printCentered("=" + string(static_cast<int>(title.length())+4, '-') + "=");
    cout << endl;
}

void showLogo(bool info) {
    cout << endl;

    printLine('=');
    printCentered("  ___ ___ ___ ___ ___ ___ ___ ___ ___ ___   _____ ___  _____   _____ _   ___ ");
    printCentered("  | __| _ \\ __| __| _ \\_ _|   \\  | __| _ \\ |_   _| _ \\/ _ \\ \\ / / __| | / __|");
    printCentered("  | _||   / _|| _||   /| || |) | | _||   /   | | |   / /_\\ \\ V /| _|| |_\\__ \\");
    printCentered("  |_| |_|_\\___|___|_|_\\___|___/  |___|_|_\\   |_| |_|_\\_/ \\_\\_/  |___|____|___/");
    
    if (info) {
        printCentered("ADVENTURE | EXPLORE | DISCOVER (Inquiries: 012-345 6789)");
    }

    printLine('=');
}

void showError(const string& text) {
    cout << endl;
    printCentered(format("Error: {}!", text));
    printCentered("Press enter to continue...", false);
    cin.get();
    cout << endl;
}

void showInfo(const string& text) {
    cout << endl;
    printCentered(text);
    printCentered("Press enter to continue...", false);
    cin.get();
    cout << endl;
}

string getString(const string& prompt, int length, int offset) {
    string input;

    while (true) {
        printCentered(prompt, offset);

        if (!getline(cin, input)) {
            showError("Input stream closed. Exiting program");
            exit(0);
        }

        if (input.empty()) {
            showError("Input cannot be empty");
            continue;
        }

        if (length > 0 && input.length() > length) {
            showError(format("Input cannot exceed {} characters", length));
            continue;
        }

        return input;
    }
}

char getChar(const string& prompt, bool checkYesNo) {
    while (true) {
        string input = getString(prompt);

        if (input.length() != 1) {
            showError("Please enter a single character");
            continue;
        }

        char ch = input[0];

        if (checkYesNo) {
            ch = static_cast<char>(toupper(ch));

            if (ch != 'Y' && ch != 'N') {
                showError("Input can only be Y or N");
                continue;
            }
        }

        return ch;
    }
}

int getInt(const string& prompt) {
    while (true) {
        string input = getString(prompt);

        try {
            size_t pos;
            int value = stoi(input, &pos);

            if (pos != input.length()) {
                showError("Please enter a valid integer");
                continue;
            }

            return value;
        }
        catch (const invalid_argument) {
            showError("Please enter a valid integer");
        }
        catch (const out_of_range&) {
            showError("The number is too large or too small");
        }
    }
}

int getInt(const string& prompt, int min, int max) {
    while (true) {
        string input = getString(prompt);

        try {
            size_t pos;
            int value = stoi(input, &pos);

            if (pos != input.length()) {
                showError("Please enter a valid integer");
                continue;
            }
            
            if (value < min || value > max) {
                showError(format("Input must be between {} and {}", min, max));
                continue;
            }

            return value;
        }
        catch (const invalid_argument) {
            showError("Please enter a valid integer");
        }
        catch (const out_of_range&) {
            showError("The number is too large or too small");
        }
    }
}

double getDouble(const string& prompt) {
    while (true) {
        string input = getString(prompt);

        try {
            size_t pos;
            double value = stod(input, &pos);

            if (pos != input.length()) {
                showError("Please enter a valid number");
                continue;
            }

            return value;
        }
        catch (const invalid_argument) {
            showError("Please enter a valid integer");
        }
        catch (const out_of_range&) {
            showError("The number is too large or too small");
        }
    }
}

double getDouble(const string& prompt, double min, double max) {
    while (true) {
        string input = getString(prompt);

        try {
            size_t pos;
            double value = stod(input, &pos);

            if (pos != input.length()) {
                showError("Please enter a valid number");
                continue;
            }

            if (value < min || value > max) {
                showError(format("Input must be between {} and {}", min, max));
                continue;
            }

            return value;
        }
        catch (const invalid_argument) {
            showError("Please enter a valid integer");
        }
        catch (const out_of_range&) {
            showError("The number is too large or too small");
        }
    }
}
