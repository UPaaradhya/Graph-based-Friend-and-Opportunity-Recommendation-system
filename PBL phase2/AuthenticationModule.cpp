// Login and user dashboard functions.
// ================= LOGIN =================

int loginUser(
    vector<User>& users,
    unordered_map<int, int>& userMap,
    unordered_map<int, string>& passwords
) {
    int id;
    string password;

    cout << "\nEnter User ID to login: ";
    cin >> id;

    int userIndex = findUserIndexByHash(userMap, id);

    if (userIndex == -1) {
        cout << "User not found!\n";
        return -1;
    }

    // Existing accounts created before password support can set a password
    // the first time they log in.
    if (passwords.find(id) == passwords.end()) {
        cout << "No password is set for this account.\n";
        cout << "Create a password: ";
        cin >> password;
        passwords[id] = password;
        savePasswordsToFile(passwords);
        cout << "Password created successfully!\n";
        cout << "Login successful!\n";
        return userIndex;
    }

    cout << "Enter Password: ";
    cin >> password;

    if (password != passwords[id]) {
        cout << "Incorrect password!\n";
        return -1;
    }

    cout << "\nLogin successful!\n";
    return userIndex;
}


// ================= USER DASHBOARD =================

void displayDashboard(
    int userIndex,
    vector<User>& users,
    vector<vector<int>>& graph
) {
    cout << "\n========================================\n";
    cout << "           USER DASHBOARD\n";
    cout << "========================================\n";

    cout << "\nWelcome, " << users[userIndex].name << "!\n";
    cout << "User ID: " << users[userIndex].id << endl;
    cout << "Course: " << users[userIndex].course << endl;

    cout << "\nYour Skills: ";
    if (users[userIndex].skills.empty()) {
        cout << "None";
    }
    else {
        for (string skill : users[userIndex].skills) {
            cout << skill << " ";
        }
    }

    cout << "\nYour Interests: ";
    if (users[userIndex].interests.empty()) {
        cout << "None";
    }
    else {
        for (string interest : users[userIndex].interests) {
            cout << interest << " ";
        }
    }

    cout << "\n\nYour Friends: ";

    if (graph[userIndex].empty()) {
        cout << "None";
    }
    else {
        cout << "\n";
        for (int friendIndex : graph[userIndex]) {
            cout << "- " << users[friendIndex].name
                 << " (ID: " << users[friendIndex].id << ")\n";
        }
    }

    cout << "\n========================================\n";
}


