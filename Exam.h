#pragma once
#include "Candidate.h"
#include "Question.h"
#include "ExamResult.h"
#include <chrono>
#include <memory>
#include <map>
#include <string>
#include <vector>

class Exam {
private:
    std::vector<std::unique_ptr<Question>> questions; // STL vector
    std::map<int, std::string> answers;               // question number -> answer
    int timeLimitMinutes;
    std::chrono::steady_clock::time_point startClock;
    std::string startTimeText;
    bool started;

    void showProgress() const;
    bool timeIsOver() const;
    void showQuestion(int number) const;

public:
    static int totalExams; // static class member

    explicit Exam(int limitMinutes = 30);
    ~Exam();

    bool loadQuestions(const std::string& fileName);
    int getQuestionCount() const;
    int getTimeLimitMinutes() const;
    void start();
    void takeExam();
    ExamResult createResult(const Candidate& candidate) const;
};
