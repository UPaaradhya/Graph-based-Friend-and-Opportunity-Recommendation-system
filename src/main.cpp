#include <iostream>
#include <string>
#include <vector>
using namespace std;

class User {
public:
    int id;
    string name;
    string course;
    string skills;
    string interests;

    void createProfile() {
        cout << "\nEnter User ID: ";
        cin >> id;

        cin.ignore();

        cout << "Enter Name: ";
        getline(cin, name);

        cout << "Enter Course: ";
        getline(cin, course);

        cout << "Enter Skills: ";
        getline(cin, skills);

        cout << "Enter Interests: ";
        getline(cin, interests);
    }

    void displayProfile() {
        cout << "\n========== USER PROFILE ==========\n";
        cout << "User ID    : " << id << endl;
        cout << "Name       : " << name << endl;
        cout << "Course     : " << course << endl;
        cout << "Skills     : " << skills << endl;
        cout << "Interests  : " << interests << endl;
        cout << "=================================\n";
    }
};

int main() {

    vector<User> users;

    int numberOfUsers;

    cout << "====================================\n";
    cout << " Friend & Opportunity Recommendation\n";
    cout << "====================================\n";

    cout << "\nHow many users do you want to create? ";
    cin >> numberOfUsers;

    for (int i = 0; i < numberOfUsers; i++) {

        User user;

        cout << "\n--- Creating User " << i + 1 << " ---\n";

        user.createProfile();

        users.push_back(user);
    }

    cout << "\n\n========== ALL USERS ==========\n";

    for (int i = 0; i < users.size(); i++) {
        users[i].displayProfile();
    }

    return 0;
}
