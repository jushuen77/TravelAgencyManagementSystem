#include "Feedback.h"
#include "Utils.h"
#include "Customer.h"
#include <iostream>
#include <format>
using namespace std;

int nextFeedbackID = 1;

bool submitFeedback(vector<Feedback>& feedbacks, int customerID) {
    showSectionHeader("Customer Feedback");

    printCentered("(Enter 0 at any prompt to cancel)");
    cout << endl;

    int rating = getInt("Enter rating (1-5 stars): ", 0, 5);
    if (rating == 0) {
        showInfo("Feedback submission cancelled.");
        return false;
    }

    string comment = getString("Enter your feedback: ", 0, -15);
    if (comment == "0") {
        showInfo("Feedback submission cancelled.");
        return false;
    }

    Feedback f;

    f.id = nextFeedbackID++;
    f.customerID = customerID;
    f.rating = rating;
    f.comment = comment;

    feedbacks.push_back(f);

    showInfo("Thank you! Your feedback has been submitted.");

    return true;
}

void viewMyFeedback(const vector<Feedback>& feedbacks, int customerID) {

    showSectionHeader("My Feedback");

    bool found = false;

    for (const auto& feedback : feedbacks) {

        if (feedback.customerID != customerID) continue;

        found = true;

        printField("Feedback ID: ", to_string(feedback.id));

        printField("Rating: ", to_string(feedback.rating) + "/5");

        printField("Comment: ", feedback.comment);

        printLine('-', 60);
    }

    if (!found) {
        printCentered("You have not submitted any feedback yet.");
    }
}

void viewAllFeedback(const vector<Feedback>& feedbacks) {

    showSectionHeader("Customer Feedback");

    if (feedbacks.empty()) {
        printCentered("No customer feedback has been submitted.");
        return;
    }

    vector<int> widths = {10, 10, 15, 50};

    printTableRow(
        {"ID", "CustID", "Rating", "Comment"},
        widths
    );

    printLine('-', 85);

    for (const auto& feedback : feedbacks) {

        string rating = to_string(feedback.rating) + "/5";

        printTableRow(
            {to_string(feedback.id), to_string(feedback.customerID), rating, feedback.comment},
            widths
        );
    }

    cout << endl;
}

void feedbackMenu(vector<Feedback>& feedbacks) {

    int choice;

    do {
        showSectionHeader("Customer Feedback Management");

        printCentered("1. View All Feedback");
        printCentered("0. Back to Staff Menu");

        cout << endl;

        choice = getInt("Your choice: ", 0, 1);

        switch (choice) {

            case 1:
                viewAllFeedback(feedbacks);
                showInfo("");
                break;
        }

    } while (choice != 0);
}