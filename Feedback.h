#ifndef FEEDBACK_H
#define FEEDBACK_H
#include "TravelTypes.h"
#include <vector>

extern int nextFeedbackID;

bool submitFeedback(std::vector<Feedback>& feedbacks, int customerID);

void viewMyFeedback(const std::vector<Feedback>& feedbacks, int customerID);

void viewAllFeedback(const std::vector<Feedback>& feedbacks);

void feedbackMenu(std::vector<Feedback>& feedbacks);

#endif