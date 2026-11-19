#include "Exam.h"
#include "MultipleChoiceQuestion.h"
#include "ShortAnswerQuestion.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

int Exam::totalExams = 0;

namespace {
std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::vector<std::string> split(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    std::stringstream stream(line);
    std::string field;
    while (std::getline(stream, field, delimiter)) fields.push_back(trim(field));
    if (!line.empty() && line.back() == delimiter) fields.push_back("");
    return fields;
}

std::string currentDateTime() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::ostringstream out;
    out << std::put_time(&localTime, "%d/%m/%Y %H:%M:%S");
    return out.str();
}

std::string upper(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return value;
}
}

Exam::Exam(int limitMinutes)
    : timeLimitMinutes(limitMinutes > 0 ? limitMinutes : 30), started(false) {
    ++totalExams;
}

Exam::~Exam() = default;

bool Exam::loadQuestions(const std::string& fileName) {
    std::ifstream input(fileName);
    if (!input.is_open()) return false;

    questions.clear();
    answers.clear();

    std::string line;
    int questionId = 0;
    while (std::getline(input, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        std::vector<std::string> f = split(line, '|');
        ++questionId;
        try {
            if (f.size() == 8 && f[0] == "MCQ") {
                std::vector<std::string> options = {f[2], f[3], f[4], f[5]};
                std::string correct = upper(f[6]);
                if (correct.size() != 1 || correct[0] < 'A' || correct[0] > 'D') continue;
                questions.push_back(std::make_unique<MultipleChoiceQuestion>(
                    questionId, f[1], options, correct, std::stod(f[7])));
            } else if (f.size() == 4 && f[0] == "SHORT") {
                questions.push_back(std::make_unique<ShortAnswerQuestion>(
                    questionId, f[1], f[2], std::stod(f[3])));
            } else {
                std::cerr << "Skipped invalid question line " << questionId << ".\n";
            }
        } catch (...) {
            std::cerr << "Skipped malformed question line " << questionId << ".\n";
        }
    }
    return !questions.empty();
}

int Exam::getQuestionCount() const { return static_cast<int>(questions.size()); }
int Exam::getTimeLimitMinutes() const { return timeLimitMinutes; }

void Exam::start() {
    startClock = std::chrono::steady_clock::now();
    startTimeText = currentDateTime();
    started = true;
}

bool Exam::timeIsOver() const {
    if (!started) return false;
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now() - startClock).count();
    return elapsed >= static_cast<long long>(timeLimitMinutes) * 60;
}

void Exam::showProgress() const {
    int answered = static_cast<int>(answers.size());
    std::cout << "\nAnswered: " << answered << "/" << questions.size()
              << " | Unanswered: " << questions.size() - answered << '\n';
    if (started) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startClock).count();
        long long remaining = std::max(0LL,
            static_cast<long long>(timeLimitMinutes) * 60 - elapsed);
        std::cout << "Time remaining: " << remaining / 60 << "m "
                  << remaining % 60 << "s\n";
    }
}

void Exam::showQuestion(int number) const {
    if (number < 1 || number > getQuestionCount()) return;
    questions[number - 1]->display();
    auto it = answers.find(number);
    std::cout << "Your current answer: "
              << (it == answers.end() ? "[Not answered]" : it->second) << '\n';
}

void Exam::takeExam() {
    if (!started) start();

    std::cout << "\nThe exam starts from Question 1 and continues in order.\n"
              << "Type SKIP to leave a question for later, or BACK to revisit the previous question.\n";

    // First pass: present questions in their original order (1, 2, 3, ...).
    int current = 1;
    while (current <= getQuestionCount() && !timeIsOver()) {
        std::cout << "\n--------------- QUESTION " << current << "/"
                  << getQuestionCount() << " ---------------\n";
        showProgress();
        showQuestion(current);

        const bool isMCQ = dynamic_cast<const MultipleChoiceQuestion*>(
            questions[current - 1].get()) != nullptr;
        std::cout << (isMCQ
            ? "Enter answer (A, B, C or D), SKIP, or BACK: "
            : "Enter your answer, SKIP, or BACK: ");

        std::string input;
        std::getline(std::cin >> std::ws, input);
        input = trim(input);
        const std::string command = upper(input);

        if (command == "SKIP") {
            std::cout << "Question " << current << " skipped. You can answer it later.\n";
            ++current;
            continue;
        }
        if (command == "BACK") {
            if (current > 1) --current;
            else std::cout << "This is already the first question.\n";
            continue;
        }

        if (isMCQ) {
            // Only one character A/B/C/D is accepted for multiple-choice questions.
            if (command.size() != 1 || command[0] < 'A' || command[0] > 'D') {
                std::cout << "Invalid answer! Please enter a valid option: A, B, C or D.\n";
                continue; // Stay on the same question until a valid answer or command is entered.
            }
            answers[current] = command;
        } else {
            if (input.empty()) {
                std::cout << "Answer cannot be empty. Enter an answer or type SKIP.\n";
                continue;
            }
            answers[current] = input;
        }
        ++current;
    }

    if (timeIsOver()) {
        std::cout << "\nTime is up. Your answers will be submitted automatically.\n";
        return;
    }

    // Review pass: candidates can return to skipped questions and change any answer.
    while (!timeIsOver()) {
        std::cout << "\n--------------- REVIEW ANSWERS ---------------\n";
        showProgress();
        std::cout << "Question status: ";
        for (int i = 1; i <= getQuestionCount(); ++i) {
            std::cout << i << (answers.count(i) ? "[Done] " : "[Skipped] ");
        }
        std::cout << "\nEnter a question number to answer/edit it, or 0 to submit: ";

        std::string choice;
        std::getline(std::cin >> std::ws, choice);
        choice = trim(choice);
        if (choice == "0") break;

        int number = 0;
        try {
            std::size_t used = 0;
            number = std::stoi(choice, &used);
            if (used != choice.size()) number = 0;
        } catch (...) {
            number = 0;
        }
        if (number < 1 || number > getQuestionCount()) {
            std::cout << "Invalid question number. Please choose from 1 to "
                      << getQuestionCount() << ", or enter 0 to submit.\n";
            continue;
        }

        showQuestion(number);
        const bool isMCQ = dynamic_cast<const MultipleChoiceQuestion*>(
            questions[number - 1].get()) != nullptr;
        std::cout << (isMCQ
            ? "Enter new answer (A, B, C or D), SKIP to keep it unanswered: "
            : "Enter new answer, SKIP to keep it unanswered: ");
        std::string input;
        std::getline(std::cin >> std::ws, input);
        input = trim(input);
        const std::string command = upper(input);

        if (command == "SKIP") continue;
        if (isMCQ) {
            if (command.size() != 1 || command[0] < 'A' || command[0] > 'D') {
                std::cout << "Invalid answer! Please enter a valid option: A, B, C or D.\n";
                continue;
            }
            answers[number] = command;
        } else if (!input.empty()) {
            answers[number] = input;
        } else {
            std::cout << "Answer cannot be empty.\n";
        }
    }

    if (timeIsOver()) {
        std::cout << "\nTime is up. Your answers will be submitted automatically.\n";
    }
}

ExamResult Exam::createResult(const Candidate& candidate) const {
    int correct = 0;
    double earned = 0.0;
    double maximum = 0.0;

    for (const auto& question : questions) {
        maximum += question->getPoints();
        auto it = answers.find(question->getId());
        if (it != answers.end() && question->isCorrect(it->second)) {
            ++correct;
            earned += question->getPoints();
        }
    }

    long long duration = 0;
    if (started) {
        duration = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - startClock).count();
    }

    return ExamResult(candidate, startTimeText, duration, correct,
                      getQuestionCount(), earned, maximum, answers);
}
