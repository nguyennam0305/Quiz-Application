#include "ExamResult.h"
#include <iomanip>
#include <iostream>

ExamResult::ExamResult(const Candidate& person, const std::string& started,
                       long long duration, int correct, int total, double earned,
                       double maximum, const std::map<int, std::string>& submitted)
    : candidate(person), startTime(started), durationSeconds(duration),
      correctCount(correct), totalQuestions(total), score(earned),
      maximumScore(maximum), answers(submitted) {}

void ExamResult::display() const {
    long long minutes = durationSeconds / 60;
    long long seconds = durationSeconds % 60;

    std::cout << "\n\n============================================\n";
    std::cout << "                 EXAM RESULT                \n";
    std::cout << "============================================\n";
    std::cout << candidate << '\n';
    std::cout << "Exam start time: " << startTime << '\n';
    std::cout << "Total duration: " << minutes << " minute(s) "
              << seconds << " second(s)\n";
    std::cout << "Correct answers: " << correctCount << " / "
              << totalQuestions << '\n';
    std::cout << "Total score: " << std::fixed << std::setprecision(2)
              << score << " / " << maximumScore << '\n';

    std::cout << "\nSubmitted answers:\n";
    if (answers.empty()) {
        std::cout << "  No answers submitted.\n";
    } else {
        for (const auto& item : answers) {
            std::cout << "  Question " << item.first << ": " << item.second << '\n';
        }
    }
    std::cout << "============================================\n";
}

double ExamResult::getScore() const { return score; }
