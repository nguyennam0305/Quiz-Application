#pragma once
#include <iostream>
#include <string>
#include <vector>

// Abstract base class: every question must display itself and check an answer.
class Question {
protected:
    int id;
    std::string content;
    double points;

public:
    Question(int questionId, const std::string& text, double score);
    virtual ~Question();

    int getId() const;
    std::string getContent() const;
    double getPoints() const;

    virtual void display() const = 0;
    virtual bool isCorrect(const std::string& answer) const = 0;
    virtual std::string getCorrectAnswer() const = 0;
};
