#pragma once
#include "Question.h"

class ShortAnswerQuestion : public Question {
private:
    std::string correctAnswer;

public:
    ShortAnswerQuestion(int id, const std::string& text,
                        const std::string& correct, double points);

    void display() const override;
    bool isCorrect(const std::string& answer) const override;
    std::string getCorrectAnswer() const override;
};
