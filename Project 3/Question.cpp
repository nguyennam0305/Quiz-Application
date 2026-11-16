#include "Question.h"

Question::Question(int questionId, const std::string& text, double score)
    : id(questionId), content(text), points(score > 0 ? score : 1.0) {}

Question::~Question() = default;

int Question::getId() const { return this->id; }
std::string Question::getContent() const { return this->content; }
double Question::getPoints() const { return this->points; }
