#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <cctype>
#include <algorithm>   // ADDED (for find)
#include <curl/curl.h>
#include "json.hpp"

using namespace std;

// ================= ADDED: FIXED LISTS (same names as the Neo4j data) =================

const vector<string> SKILL_CATALOG = {
    "Python", "Machine_Learning", "Deep_Learning", "Data_Science",
    "Data_Analysis", "SQL", "Statistics", "Power_BI",
    "HTML_CSS", "JavaScript", "React", "NodeJS",
    "Web_Development", "MongoDB", "Django", "UI_UX_Design",
    "Cyber_Security", "Networking", "Linux", "Cryptography",
    "Penetration_Testing", "Digital_Forensics", "Android_Development", "Kotlin",
    "Flutter", "Java", "Swift", "Firebase",
    "Cloud_Computing", "AWS", "DevOps", "Docker",
    "Kubernetes", "Git", "C++", "C",
    "Data_Structures", "Algorithms", "Operating_Systems", "DBMS",
    "Communication", "Teamwork", "Leadership", "Problem_Solving",
    "Time_Management", "Critical_Thinking", "Graphic_Design", "Video_Editing",
    "Content_Writing", "Digital_Marketing", "Public_Speaking", "Social_Media"
};

const vector<string> INTEREST_CATALOG = {
    "Artificial_Intelligence", "Data_Analytics", "Research",
    "Robotics", "Startups", "Open_Source",
    "Design", "Freelancing", "Ethical_Hacking",
    "Capture_The_Flag", "Mobile_Apps", "Gaming",
    "Automation", "Cloud_Technologies", "Competitive_Programming",
    "Hackathons", "Music", "Photography",
    "Travel", "Reading", "Sports"
};

const vector<string> COURSE_CATALOG = {
    "B.Tech CSE AI ML", "B.Sc Data Science", "M.Sc Data Science",
    "BBA Business Analytics", "BCA", "B.Tech IT",
    "MCA", "B.Sc Computer Science", "B.Tech Cyber Security",
    "B.Tech CSE", "M.Tech CSE", "B.Tech Electronics and Communication",
    "BA Journalism and Mass Communication", "B.Des Graphic Design", "BBA Marketing",
    "B.Com", "BA English Literature"
};

// Choose ONE item from a numbered list.
string pickOne(const string& title, const vector<string>& list) {
    cout << "\n" << title << ":\n";
    for (size_t i = 0; i < list.size(); i++) {
        cout << "  " << i + 1 << ". " << list[i] << endl;
    }
    int c = 0;
    while (true) {
        cout << "Enter number: ";
        if (cin >> c && c >= 1 && c <= (int)list.size()) {
            return list[c - 1];
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid choice, try again.\n";
    }
}

// Choose MANY items from a numbered list (type numbers, then 0 to finish).
vector<string> pickMany(const string& title, const vector<string>& list, size_t maxCount) {
    vector<string> chosen;
    cout << "\n" << title << " (max " << maxCount << "):\n";
    for (size_t i = 0; i < list.size(); i++) {
        cout << "  " << i + 1 << ". " << list[i] << endl;
    }
    cout << "Enter numbers separated by spaces, then 0 to finish: ";
    int c;
    while (cin >> c) {
        if (c == 0) break;
        if (c < 1 || c > (int)list.size()) {
            cout << "Invalid number ignored.\n";
            continue;
        }
        if (chosen.size() >= maxCount) {
            cout << "Maximum reached, skipped.\n";
            continue;
        }
        const string& item = list[c - 1];
        if (find(chosen.begin(), chosen.end(), item) == chosen.end()) {
            chosen.push_back(item);
        }
    }
    if (cin.fail()) {
        cin.clear();
        cin.ignore(10000, '\n');
    }
    return chosen;
}



// ================= USER CLASS =================

class User {
public:
    int id;
    string name;
    string course;
    vector<string> skills;
    vector<string> interests;
    vector<int> takenOpportunities;   // ADDED: IDs of opportunities already received

    void createProfile(int assignedId = -1) {

        if (assignedId == -1) {
            cout << "Enter User ID: ";
            cin >> id;
        } else {
            id = assignedId;
        }

        // CHANGED: name can have spaces; course, skills and interests are chosen from lists
        cin.ignore(10000, '\n');
        cout << "Enter Name: ";
        getline(cin, name);

        course = pickOne("Choose your course", COURSE_CATALOG);
        skills = pickMany("Choose your skills", SKILL_CATALOG, 5);
        interests = pickMany("Choose your interests", INTEREST_CATALOG, 4);
    }

    void displayProfile() {

        cout << "\nUser ID: " << id;
        cout << "\nName: " << name;
        cout << "\nCourse: " << course;

        cout << "\nSkills: ";

        for (string skill : skills) {
            cout << skill << " ";
        }

        cout << "\nInterests: ";

        for (string interest : interests) {
            cout << interest << " ";
        }

        cout << endl;
    }
};


// ================= INHERITANCE =================

class Student : public User {
public:

    int semester;

    void createStudent() {

        createProfile();

        cout << "Enter Semester: ";
        cin >> semester;
    }

    void displayStudent() {

        displayProfile();

        cout << "Semester: " << semester << endl;
    }
};


class Mentor : public User {
public:

    string expertise;

    void createMentor() {

        createProfile();

        cout << "Enter Expertise: ";
        cin >> expertise;
    }

    void displayMentor() {

        displayProfile();

        cout << "Expertise: " << expertise << endl;
    }
};


// ================= OPPORTUNITY CLASS =================

class Opportunity {
public:

    int id;
    string name;
    string type;
    string skill;
    string interest;

    void createOpportunity() {

        cout << "Enter Opportunity ID: ";
        cin >> id;

        cout << "Enter Opportunity Name: ";
        cin >> name;

        cout << "Enter Type (Project/Workshop/Course): ";
        cin >> type;

        cout << "Enter Required Skill: ";
        cin >> skill;

        cout << "Enter Interest Area: ";
        cin >> interest;
    }

    void displayOpportunity() {

        cout << "\nOpportunity ID: " << id;
        cout << "\nName: " << name;
        cout << "\nType: " << type;
        cout << "\nRequired Skill: " << skill;
        cout << "\nInterest: " << interest << endl;
    }
};

#include "neo4j_storage.h"
