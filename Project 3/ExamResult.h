#pragma once
#include "Candidate.h"
#include <chrono>
#include <map>
#include <vector>

class ExamResult {
private:
    Candidate candidate;
    std::string startTime;
    long long durationSeconds;
    int correctCount;
    int totalQuestions;
    double score;
    double maximumScore;
    std::map<int, std::string> answers;

public:
    ExamResult(const Candidate& person, const std::string& started,
               long long duration, int correct, int total, double earned,
               double maximum, const std::map<int, std::string>& submitted);

    void display() const;
    double getScore() const;
};
