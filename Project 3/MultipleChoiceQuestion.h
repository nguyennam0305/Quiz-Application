#pragma once
#include "Question.h"

class MultipleChoiceQuestion : public Question {
private:
    std::vector<std::string> options;
    std::string correctAnswer;

public:
    MultipleChoiceQuestion(int id, const std::string& text,
                           const std::vector<std::string>& choices,
                           const std::string& correct, double points);

    void display() const override;
    bool isCorrect(const std::string& answer) const override;
    std::string getCorrectAnswer() const override;
};
