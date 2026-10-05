#include <iostream>
#include <string>
using namespace std;

class User {
public:
    int id;
    string name;
    string course;
    string skills;
    string interests;

    void createProfile() {
        cout << "Enter User ID: ";
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

    User user;

    cout << "====================================\n";
    cout << " Friend & Opportunity Recommendation\n";
    cout << "====================================\n\n";

    user.createProfile();

    user.displayProfile();

    return 0;
}
