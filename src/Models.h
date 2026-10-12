#pragma once

#include <string>
#include <vector>

// Stores the details of one user.
struct User
{
    int id = 0;
    std::string name;
    std::string course;
    std::string password;
    std::vector<std::string> skills;
    std::vector<std::string> interests;
};

// Stores the details of one opportunity.
struct Opportunity
{
    int id = 0;
    std::string name;
    std::string type;
    std::string skill;
    std::string interest;
};
