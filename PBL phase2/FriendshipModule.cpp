// User lookup, friendship management, friend display, mutual friends, and common profile data.
// ================= USER SEARCH =================

// Old linear search

int findUserIndex(vector<User>& users, int id) {

    for (int i = 0; i < users.size(); i++) {

        if (users[i].id == id) {

            return i;
        }
    }

    return -1;
}


// Hash-based search

int findUserIndexByHash(unordered_map<int, int>& userMap, int id) {

    if (userMap.find(id) != userMap.end()) {

        return userMap[id];
    }

    return -1;
}


// ================= FRIENDSHIP =================

void addFriendship(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {
    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndexByHash(userMap, id1);
    int user2 = findUserIndexByHash(userMap, id2);

    if (user1 == -1 || user2 == -1) {
        cout << "User not found!\n";
        return;
    }

    if (user1 == user2) {
        cout << "A user cannot be friends with themselves!\n";
        return;
    }

    for (int i = 0; i < graph[user1].size(); i++) {
        if (graph[user1][i] == user2) {
            cout << "Friendship already exists!\n";
            return;
        }
    }

    graph[user1].push_back(user2);
    graph[user2].push_back(user1);

    cout << "Friendship added successfully!\n";
}


// ================= DISPLAY FRIENDS =================

void displayFriends(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {

    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int user = findUserIndexByHash(userMap, id);

    if (user == -1) {

        cout << "User not found!\n";

        return;
    }

   cout << "\nFriends of " << users[user].name << ":\n";

if (graph[user].empty()) {
    cout << "This user has no friends yet.\n";
    return;
}

for (int friendIndex : graph[user]) {
    cout << "- " << users[friendIndex].name << endl;

    }
}



// ================= MUTUAL FRIENDS =================

void findMutualFriends(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {

    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndexByHash(userMap, id1);

    int user2 = findUserIndexByHash(userMap, id2);

 if (user1 == -1 || user2 == -1) {

    cout << "User not found!\n";

    return;
}

if (user1 == user2) {

    cout << "Please enter two different users.\n";

    return;
}

    bool found = false;

    cout << "\nMutual Friends:\n";

    for (int friend1 : graph[user1]) {

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


// ================= COMMON SKILLS AND INTERESTS =================

void findCommonSkillsAndInterests(
    vector<User>& users,
    unordered_map<int, int>& userMap
) {

    int id1, id2;

    cout << "Enter first User ID: ";
    cin >> id1;

    cout << "Enter second User ID: ";
    cin >> id2;

    int user1 = findUserIndexByHash(userMap, id1);

    int user2 = findUserIndexByHash(userMap, id2);

    if (user1 == -1 || user2 == -1) {

        cout << "User not found!\n";

        return;
    }
    if (user1 == user2) {
    cout << "Please enter two different users.\n";
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


