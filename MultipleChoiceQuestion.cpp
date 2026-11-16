#include "MultipleChoiceQuestion.h"
#include <cctype>

MultipleChoiceQuestion::MultipleChoiceQuestion(
    int id, const std::string& text, const std::vector<std::string>& choices,
    const std::string& correct, double score)
    : Question(id, text, score), options(choices), correctAnswer(correct) {}

void MultipleChoiceQuestion::display() const {
    std::cout << "\nQuestion " << id << " [" << points << " point(s)]\n";
    std::cout << content << '\n';
    for (std::size_t i = 0; i < options.size(); ++i) {
        std::cout << "  " << static_cast<char>('A' + i) << ". " << options[i] << '\n';
    }
}

bool MultipleChoiceQuestion::isCorrect(const std::string& answer) const {
    if (answer.empty()) return false;
    char value = static_cast<char>(std::toupper(
        static_cast<unsigned char>(answer[0])));
    return std::string(1, value) == correctAnswer;
}

std::string MultipleChoiceQuestion::getCorrectAnswer() const {
    return correctAnswer;
}
