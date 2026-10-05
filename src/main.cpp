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


// Find the position of a user using their ID
int findUserIndex(vector<User>& users, int id) {

    for (int i = 0; i < users.size(); i++) {

        if (users[i].id == id) {
            return i;
        }
    }

    return -1;
}


// Add friendship between two users
void addFriendship(vector<vector<int>>& graph,
                   vector<User>& users,
                   int id1,
                   int id2) {

    int user1 = findUserIndex(users, id1);
    int user2 = findUserIndex(users, id2);

    if (user1 == -1 || user2 == -1) {
        cout << "\nUser not found!\n";
        return;
    }

    graph[user1].push_back(user2);
    graph[user2].push_back(user1);

    cout << "\nFriendship added successfully!\n";
}


// Display friends of a particular user
void displayFriends(vector<vector<int>>& graph,
                    vector<User>& users,
                    int id) {

    int userIndex = findUserIndex(users, id);

    if (userIndex == -1) {
        cout << "\nUser not found!\n";
        return;
    }

    cout << "\nFriends of " << users[userIndex].name << ":\n";

    if (graph[userIndex].empty()) {
        cout << "No friends yet.\n";
        return;
    }

    for (int friendIndex : graph[userIndex]) {
        cout << "- " << users[friendIndex].name << endl;
    }
}


int main() {

    vector<User> users;

    int numberOfUsers;

    cout << "====================================\n";
    cout << " Friend & Opportunity Recommendation\n";
    cout << "====================================\n";

    cout << "\nHow many users do you want to create? ";
    cin >> numberOfUsers;

    // Create users
    for (int i = 0; i < numberOfUsers; i++) {

        User user;

        cout << "\n--- Creating User " << i + 1 << " ---\n";

        user.createProfile();

        users.push_back(user);
    }


    // Create graph
    vector<vector<int>> graph(users.size());


    int choice;

    do {

        cout << "\n========== MENU ==========\n";
        cout << "1. Display All Users\n";
        cout << "2. Add Friendship\n";
        cout << "3. Display Friends\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;


        if (choice == 1) {

            for (int i = 0; i < users.size(); i++) {
                users[i].displayProfile();
            }

        }


        else if (choice == 2) {

            int id1, id2;

            cout << "\nEnter first User ID: ";
            cin >> id1;

            cout << "Enter second User ID: ";
            cin >> id2;

            addFriendship(graph, users, id1, id2);

        }


        else if (choice == 3) {

            int id;

            cout << "\nEnter User ID: ";
            cin >> id;

            displayFriends(graph, users, id);

        }


        else if (choice == 4) {

            cout << "\nThank you for using the system!\n";

        }


        else {

            cout << "\nInvalid choice!\n";

        }

    } while (choice != 4);


    return 0;
}
