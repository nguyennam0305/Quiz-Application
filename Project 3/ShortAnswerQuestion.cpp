#include "ShortAnswerQuestion.h"
#include <algorithm>
#include <cctype>

namespace {
std::string normalize(std::string value) {
    auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}
}

ShortAnswerQuestion::ShortAnswerQuestion(int id, const std::string& text,
                                         const std::string& correct, double score)
    : Question(id, text, score), correctAnswer(correct) {}

void ShortAnswerQuestion::display() const {
    std::cout << "\nQuestion " << id << " [Short answer, " << points
              << " point(s)]\n" << content << '\n';
}

bool ShortAnswerQuestion::isCorrect(const std::string& answer) const {
    return normalize(answer) == normalize(correctAnswer);
}

std::string ShortAnswerQuestion::getCorrectAnswer() const {
    return correctAnswer;
}
