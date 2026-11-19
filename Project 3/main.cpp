#include <iostream>
#include <limits>
#include "Exam.h"

using namespace std;

int main() {
    cout << "============================================\n";
    cout << "              QUIZ APPLICATION              \n";
    cout << "============================================\n";

    Candidate candidate;
    cin >> candidate;

    Exam exam;
    const string questionFile = "questions.txt";

    if (!exam.loadQuestions(questionFile)) {
        cerr << "Cannot load question file: " << questionFile << '\n';
        cout << "Check that questions.txt is in the same folder as the executable.\n";
        return 1;
    }

    cout << "\nHello, " << candidate.getFullName() << "!\n";
    cout << "Total questions: " << exam.getQuestionCount() << '\n';
    cout << "Exam time limit: " << exam.getTimeLimitMinutes() << " minutes\n";
    cout << "Press Enter when you are ready to start...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    exam.start();
    exam.takeExam();

    ExamResult result = exam.createResult(candidate);
    result.display();

    cout << "\nThank you for taking the exam!\n";
    return 0;
}
