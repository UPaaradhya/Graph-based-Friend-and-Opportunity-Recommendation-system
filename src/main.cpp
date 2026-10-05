#include <iostream>
#include <vector>
#include <queue>
using namespace std;

class User {
public:
    int id;
    string name;
    string course;
    vector<string> skills;
    vector<string> interests

void createProfile() {
    cout << "Enter User ID: ";
    cin >> id;

    cout << "Enter Name: ";
    cin >> name;

    cout << "Enter Course: ";
    cin >> course;

    int numberOfSkills;
    cout << "Enter number of skills: ";
    cin >> numberOfSkills;

    for (int i = 0; i < numberOfSkills; i++) {
        string skill;
        cout << "Enter skill " << i + 1 << ": ";
        cin >> skill;

        skills.push_back(skill);
    }

    int numberOfInterests;
    cout << "Enter number of interests: ";
    cin >> numberOfInterests;

    for (int i = 0; i < numberOfInterests; i++) {
        string interest;
        cout << "Enter interest " << i + 1 << ": ";
        cin >> interest;

        interests.push_back(interest);
    }
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

// Find the index of a user using their ID
int findUserIndex(vector<User>& users, int id) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i].id == id) {
            return i;
        }
    }

    return -1;
}

// Add friendship between two users
void addFriendship(vector<vector<int>>& graph, vector<User>& users) {
    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndex(users, id1);
    int user2 = findUserIndex(users, id2);

    if (user1 == -1 || user2 == -1) {
        cout << "User not found!\n";
        return;
    }

    graph[user1].push_back(user2);
    graph[user2].push_back(user1);

    cout << "Friendship added successfully!\n";
}

// Display direct friends
void displayFriends(vector<vector<int>>& graph, vector<User>& users) {
    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int user = findUserIndex(users, id);

    if (user == -1) {
        cout << "User not found!\n";
        return;
    }

    cout << "\nFriends of " << users[user].name << ":\n";

    for (int friendIndex : graph[user]) {
        cout << "- " << users[friendIndex].name << endl;
    }
}

//friend recommendations
void friendRecommendation(vector<vector<int>>& graph, vector<User>& users) {
    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int start = findUserIndex(users, id);

    if (start == -1) {
        cout << "User not found!\n";
        return;
    }

    vector<bool> visited(users.size(), false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "\nFriend Recommendations for "
         << users[start].name << ":\n";

    bool found = false;

    while (!q.empty()) {

        int current = q.front();
        q.pop();

        for (int neighbour : graph[current]) {

            if (!visited[neighbour]) {

                visited[neighbour] = true;
                q.push(neighbour);

                // Check if this person is a friend of the original user
                bool alreadyFriend = false;

                for (int friendIndex : graph[start]) {
                    if (friendIndex == neighbour) {
                        alreadyFriend = true;
                        break;
                    }
                }

                // Recommend only people who are not already friends
                if (neighbour != start && !alreadyFriend) {
                    cout << "- " << users[neighbour].name << endl;
                    found = true;
                }
            }
        }
    }

    if (!found) {
        cout << "No new friend recommendations available.\n";
    }
}

void dfs(vector<vector<int>>& graph, vector<User>& users, int current,
         vector<bool>& visited) {

    visited[current] = true;

    cout << users[current].name << " ";

    for (int neighbour : graph[current]) {

        if (!visited[neighbour]) {
            dfs(graph, users, neighbour, visited);
        }
    }
}

void dfsTraversal(vector<vector<int>>& graph, vector<User>& users) {

    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int start = findUserIndex(users, id);

    if (start == -1) {
        cout << "User not found!\n";
        return;
    }

    vector<bool> visited(users.size(), false);

    cout << "\nDFS Network Traversal:\n";

    dfs(graph, users, start, visited);

    cout << endl;
}
void findMutualFriends(vector<vector<int>>& graph, vector<User>& users) {

    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndex(users, id1);
    int user2 = findUserIndex(users, id2);

    if (user1 == -1 || user2 == -1) {
        cout << "User not found!\n";
        return;
    }
    void findCommonSkillsAndInterests(vector<User>& users) {

    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndex(users, id1);
    int user2 = findUserIndex(users, id2);

    if (user1 == -1 || user2 == -1) {
        cout << "User not found!\n";
        return;
    }

    bool commonSkill = false;
    bool commonInterest = false;

    cout << "\nCommon Skills:\n";

    for (string skill1 : users[user1].skills) {

        for (string skill2 : users[user2].skills) {

            if (skill1 == skill2) {
                cout << "- " << skill1 << endl;
                commonSkill = true;
            }
        }
    }

    if (!commonSkill) {
        cout << "No common skills.\n";
    }

    cout << "\nCommon Interests:\n";

    for (string interest1 : users[user1].interests) {

        for (string interest2 : users[user2].interests) {

            if (interest1 == interest2) {
                cout << "- " << interest1 << endl;
                commonInterest = true;
            }
        }
    }

    if (!commonInterest) {
        cout << "No common interests.\n";
    }
}

    bool found = false;

    cout << "\nMutual Friends:\n";

    // Check friends of first user
    for (int friend1 : graph[user1]) {

        // Check friends of second user
        for (int friend2 : graph[user2]) {

            if (friend1 == friend2) {
                cout << "- " << users[friend1].name << endl;
                found = true;
            }
        }
    }

    if (!found) {
        cout << "No mutual friends found.\n";
    }
}
int main() {

    vector<User> users;
    int numberOfUsers;

    cout << "Enter number of users: ";
    cin >> numberOfUsers;

    // Create users
    for (int i = 0; i < numberOfUsers; i++) {
        User newUser;

        cout << "\nEnter details for User " << i + 1 << ":\n";
        newUser.createProfile();

        users.push_back(newUser);
    }

    // Create graph
    vector<vector<int>> graph(users.size());

    int choice;

    do {
        cout << "\n========== MENU ==========\n";
        cout << "1. Display All Users\n";
        cout << "2. Add Friendship\n";
        cout << "3. Display Friends\n";
        cout << "4. Friend Recommendation\n";
        cout << "5.DFS Network Traversal\n";
        cout << "6.Find Mutual Friends\n";
        cout << "7.Common Skills and Interests\n"; 
        cout << "8. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            for (int i = 0; i < users.size(); i++) {
                users[i].displayProfile();
            }
            break;

        case 2:
            addFriendship(graph, users);
            break;

        case 3:
            displayFriends(graph, users);
            break;

        case 4:
            friendRecommendation(graph, users);
            break;

        case 5:
            dfsTraversal(graph,users);
            break;

        case 6:
            findMutualFriends(graph,users);
            break;

            case 7:
            CommonSkillsAndInterests(users);
            break;

        case 8:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}
