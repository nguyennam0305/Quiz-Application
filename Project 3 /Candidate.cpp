#include "Candidate.h"
#include <limits>

Candidate::Candidate() : candidateId(""), fullName(""), email("") {}

Candidate::Candidate(const std::string& id, const std::string& name,
                     const std::string& mail)
    : candidateId(id), fullName(name), email(mail) {}

void Candidate::input() {
    std::cout << "Candidate ID: ";
    std::getline(std::cin, candidateId);
    std::cout << "Full name: ";
    std::getline(std::cin, fullName);
    std::cout << "Email: ";
    std::getline(std::cin, email);
}

std::string Candidate::getCandidateId() const { return candidateId; }
std::string Candidate::getFullName() const { return fullName; }
std::string Candidate::getEmail() const { return email; }

std::istream& operator>>(std::istream& in, Candidate& candidate) {
    std::cout << "\n--- Candidate Information ---\n";
    std::cout << "Candidate ID: ";
    std::getline(in, candidate.candidateId);
    std::cout << "Full name: ";
    std::getline(in, candidate.fullName);
    std::cout << "Email: ";
    std::getline(in, candidate.email);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Candidate& candidate) {
    out << "Candidate ID: " << candidate.candidateId
        << "\nFull name: " << candidate.fullName
        << "\nEmail: " << candidate.email;
    return out;
}
