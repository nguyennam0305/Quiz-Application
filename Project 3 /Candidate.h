#pragma once
#include <iostream>
#include <string>

class Candidate {
private:
    std::string candidateId;
    std::string fullName;
    std::string email;

public:
    Candidate();
    Candidate(const std::string& id, const std::string& name, const std::string& mail);

    void input();
    std::string getCandidateId() const;
    std::string getFullName() const;
    std::string getEmail() const;

    friend std::istream& operator>>(std::istream& in, Candidate& candidate);
    friend std::ostream& operator<<(std::ostream& out, const Candidate& candidate);
};
