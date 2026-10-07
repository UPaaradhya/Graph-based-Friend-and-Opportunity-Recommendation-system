#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <cctype>

using namespace std;


// ================= USER CLASS =================

class User {
public:
    int id;
    string name;
    string course;
    vector<string> skills;
    vector<string> interests;

    void createProfile(int assignedId = -1) {

        if (assignedId == -1) {
            cout << "Enter User ID: ";
            cin >> id;
        } else {
            id = assignedId;
        }

        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter Course: ";
        cin >> course;

        int numberOfSkills = 0;

cout << "Enter number of skills: ";
cin >> numberOfSkills;

if (numberOfSkills < 0 || numberOfSkills > 20) {
    cout << "Invalid number of skills!" << endl;
    return;
}

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


// ================= RECOMMENDATION STRUCTURES =================

// Friend recommendation

struct Recommendation {

    int userIndex;
    int score;

    bool operator<(const Recommendation& other) const {

        return score < other.score;
    }
};


// Opportunity recommendation

struct OpportunityRecommendation {

    int opportunityIndex;
    int score;

    bool operator<(const OpportunityRecommendation& other) const {

        return score < other.score;
    }
};


// ================= TREE NODE =================

class OpportunityNode {
public:

    string category;

    vector<OpportunityNode*> children;

    OpportunityNode(string name) {

        category = name;
    }
};


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

void saveFriendshipsToFile(
    vector<vector<int>>& graph,
    vector<User>& users
) {
    ofstream file("friendships.txt");

    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].size(); j++) {

            // Save each friendship only once
            if (users[i].id < users[graph[i][j]].id) {
                file << users[i].id << " "
                     << users[graph[i][j]].id << endl;
            }
        }
    }

    file.close();

    cout << "Friendships saved successfully!" << endl;
}
void loadFriendshipsFromFile(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {
    ifstream file("friendships.txt");

    if (!file) {
        return;
    }

    int id1, id2;

    while (file >> id1 >> id2) {

        int user1 = findUserIndexByHash(userMap, id1);
        int user2 = findUserIndexByHash(userMap, id2);

        if (user1 != -1 && user2 != -1) {
            graph[user1].push_back(user2);
            graph[user2].push_back(user1);
        }
    }

    file.close();

    cout << "Friendships loaded successfully!" << endl;
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


// ================= FRIEND RECOMMENDATION =================

void friendRecommendation(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {

    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int start = findUserIndexByHash(userMap, id);

    if (start == -1) {

        cout << "User not found!\n";

        return;
    }
    if (graph[start].empty()) {

    cout << "This user has no friends yet.\n";

    return;
}

    vector<bool> visited(users.size(), false);

    queue<int> q;

    visited[start] = true;

    q.push(start);

    priority_queue<Recommendation> recommendations;

    while (!q.empty()) {

        int current = q.front();

        q.pop();

        for (int neighbour : graph[current]) {

            if (!visited[neighbour]) {

                visited[neighbour] = true;

                q.push(neighbour);

                bool alreadyFriend = false;

                for (int friendIndex : graph[start]) {

                    if (friendIndex == neighbour) {

                        alreadyFriend = true;

                        break;
                    }
                }

                if (neighbour != start && !alreadyFriend) {

                    int score = 0;


                    // Mutual friends

                    for (int friend1 : graph[start]) {

                        for (int friend2 : graph[neighbour]) {

                            if (friend1 == friend2) {

                                score = score + 2;
                            }
                        }
                    }


                    // Common skills

                    for (string skill1 : users[start].skills) {

                        for (string skill2 : users[neighbour].skills) {

                            if (skill1 == skill2) {

                                score = score + 1;
                            }
                        }
                    }


                    // Common interests

                    for (string interest1 : users[start].interests) {

                        for (string interest2 : users[neighbour].interests) {

                            if (interest1 == interest2) {

                                score = score + 1;
                            }
                        }
                    }


                    Recommendation r;

                    r.userIndex = neighbour;

                    r.score = score;

                    recommendations.push(r);
                }
            }
        }
    }

    cout << "\n===== Ranked Friend Recommendations =====\n";

    if (recommendations.empty()) {

        cout << "No new friend recommendations available.\n";

        return;
    }

    int rank = 1;

    while (!recommendations.empty()) {

        Recommendation top = recommendations.top();

        recommendations.pop();

        int maxScore = (2 * graph[start].size())
                     + users[start].skills.size()
                     + users[start].interests.size();

        int percentage = 0;
        if (maxScore > 0) {
            percentage = (top.score * 100) / maxScore;
        }

        cout << rank << ". "
             << users[top.userIndex].name
             << " | Match: "
             << percentage << "%"
             << endl;

        rank++;
    }
}


// ================= DFS =================

void dfs(
    vector<vector<int>>& graph,
    vector<User>& users,
    int current,
    vector<bool>& visited
) {

    visited[current] = true;

    cout << users[current].name << " ";

    for (int neighbour : graph[current]) {

        if (!visited[neighbour]) {

            dfs(graph, users, neighbour, visited);
        }
    }
}


void dfsTraversal(
    vector<vector<int>>& graph,
    vector<User>& users,
    unordered_map<int, int>& userMap
) {

    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int start = findUserIndexByHash(userMap, id);

    if (start == -1) {

        cout << "User not found!\n";

        return;
    }

    vector<bool> visited(users.size(), false);

   cout << "\nDFS Network Traversal:\n";

if (graph[start].empty()) {
    cout << users[start].name << " has no connections.\n";
    return;
}

dfs(graph, users, start, visited);

cout << endl;
    
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


// ================= OPPORTUNITY RECOMMENDATION =================

void recommendOpportunities(
    vector<User>& users,
    vector<Opportunity>& opportunities,
           unordered_map<int, int>& userMap
) {

    int id;

    cout << "Enter User ID: ";
    cin >> id;

    int userIndex = findUserIndexByHash(userMap, id);

    if (userIndex == -1) {

        cout << "User not found!\n";

        return;
    }
    if (users[userIndex].skills.empty() &&
    users[userIndex].interests.empty()) {

    cout << "This user has no skills or interests added yet.\n";

    return;
}

    priority_queue<OpportunityRecommendation> recommendations;


    for (int i = 0; i < opportunities.size(); i++) {

        int score = 0;


        // Check skills

        for (string skill : users[userIndex].skills) {

            if (skill == opportunities[i].skill) {

                score = score + 2;
            }
        }


        // Check interests

        for (string interest : users[userIndex].interests) {

            if (interest == opportunities[i].interest) {

                score = score + 2;
            }
        }


        // Add matching opportunities

        if (score > 0) {

            OpportunityRecommendation recommendation;

            recommendation.opportunityIndex = i;

            recommendation.score = score;

            recommendations.push(recommendation);
        }
    }


    cout << "\n===== Ranked Opportunity Recommendations =====\n";

    if (recommendations.empty()) {

        cout << "No matching opportunities found.\n";

        return;
    }

    int rank = 1;

    while (!recommendations.empty()) {

        OpportunityRecommendation top = recommendations.top();

        recommendations.pop();

        int percentage = (top.score * 100) / 4;

        cout << rank << ". "
             << opportunities[top.opportunityIndex].name
             << " | Type: "
             << opportunities[top.opportunityIndex].type
             << " | Match: "
             << percentage << "%"
             << endl;

        rank++;
    }
}


// ================= TREE =================

void addOpportunityToTree(
    OpportunityNode* root,
    Opportunity& opportunity
) {

    string type = opportunity.type;
    for (char &c : type) c = tolower(c);

    if (type == "project") {

        root->children[0]->children.push_back(
            new OpportunityNode(opportunity.name)
        );
    }

    else if (type == "workshop") {

        root->children[1]->children.push_back(
            new OpportunityNode(opportunity.name)
        );
    }

    else if (type == "course") {

        root->children[2]->children.push_back(
            new OpportunityNode(opportunity.name)
        );
    }

    else {

        cout << "Invalid opportunity type!\n";
    }
}

bool searchOpportunityInTree(
    OpportunityNode* root,
    string name
) {

    if (root->category == name) {

        cout << "\nOpportunity found: "
             << root->category
             << endl;

        return true;
    }

    for (int i = 0; i < root->children.size(); i++) {

        if (searchOpportunityInTree(
                root->children[i],
                name
            )) {

            return true;
        }
    }

    return false;
}
// ================= DISPLAY OPPORTUNITY TREE =================

void displayOpportunityTree(OpportunityNode* root, int level) {
    for (int i = 0; i < level; i++) {
        cout << "  ";
    }

    cout << "|-- " << root->category << endl;

    for (int i = 0; i < root->children.size(); i++) {
        displayOpportunityTree(root->children[i], level + 1);
    }
}


// ================= SAVE USERS =================

void saveUsersToFile(vector<User>& users) {
    ofstream file("users.txt");

    for (int i = 0; i < users.size(); i++) {
        file << users[i].id << endl;
        file << users[i].name << endl;
        file << users[i].course << endl;

        file << users[i].skills.size() << endl;
        for (int j = 0; j < users[i].skills.size(); j++) {
            file << users[i].skills[j] << endl;
        }

        file << users[i].interests.size() << endl;
        for (int j = 0; j < users[i].interests.size(); j++) {
            file << users[i].interests[j] << endl;
        }
    }

    file.close();

    cout << "Users saved successfully!" << endl;
}

void loadUsersFromFile(vector<User>& users) {
    ifstream file("users.txt");

    if (!file) {
        return;
    }

    User user;
    int numberOfSkills;
    int numberOfInterests;

    while (file >> user.id) {
        file.ignore();

        getline(file, user.name);
        getline(file, user.course);

        file >> numberOfSkills;
        file.ignore();

        user.skills.clear();

        for (int i = 0; i < numberOfSkills; i++) {
            string skill;
            getline(file, skill);
            user.skills.push_back(skill);
        }

        file >> numberOfInterests;
        file.ignore();

        user.interests.clear();

        for (int i = 0; i < numberOfInterests; i++) {
            string interest;
            getline(file, interest);
            user.interests.push_back(interest);
        }

        users.push_back(user);
    }

    file.close();

    cout << "Users loaded successfully!" << endl;
}


void saveOpportunitiesToFile(vector<Opportunity>& opportunities) {
    ofstream file("opportunities.txt");

    for (int i = 0; i < opportunities.size(); i++) {
        file << opportunities[i].id << " "
             << opportunities[i].name << " "
             << opportunities[i].type << " "
             << opportunities[i].skill << " "
             << opportunities[i].interest << endl;
    }

    file.close();

    cout << "Opportunities saved successfully!" << endl;
}
void loadOpportunitiesFromFile(vector<Opportunity>& opportunities) {
    ifstream file("opportunities.txt");

    if (!file) {
        return;
    }

    Opportunity opportunity;

    // Opportunity data is saved in one line per opportunity.
    while (file >> opportunity.id
                >> opportunity.name
                >> opportunity.type
                >> opportunity.skill
                >> opportunity.interest) {

        opportunities.push_back(opportunity);
    }

    file.close();

    cout << "Opportunities loaded successfully!" << endl;
}

// ================= PASSWORD FILE =================

void savePasswordsToFile(unordered_map<int, string>& passwords) {
    ofstream file("passwords.txt");

    for (auto entry : passwords) {
        file << entry.first << " " << entry.second << endl;
    }

    file.close();
}

void loadPasswordsFromFile(unordered_map<int, string>& passwords) {
    ifstream file("passwords.txt");

    if (!file) {
        return;
    }

    int id;
    string password;

    while (file >> id >> password) {
        passwords[id] = password;
    }

    file.close();
}

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


// ================= ADD RECOMMENDED FRIEND =================

void recommendAndAddFriend(
    int userIndex,
    vector<User>& users,
    vector<vector<int>>& graph
) {
    priority_queue<Recommendation> recommendations;

    for (int candidate = 0; candidate < users.size(); candidate++) {

        if (candidate == userIndex) {
            continue;
        }

        bool alreadyFriend = false;

        for (int friendIndex : graph[userIndex]) {
            if (friendIndex == candidate) {
                alreadyFriend = true;
                break;
            }
        }

        if (alreadyFriend) {
            continue;
        }

        int score = 0;

        // Mutual friends
        for (int friend1 : graph[userIndex]) {
            for (int friend2 : graph[candidate]) {
                if (friend1 == friend2) {
                    score = score + 2;
                }
            }
        }

        // Common skills
        for (string skill1 : users[userIndex].skills) {
            for (string skill2 : users[candidate].skills) {
                if (skill1 == skill2) {
                    score = score + 1;
                }
            }
        }

        // Common interests
        for (string interest1 : users[userIndex].interests) {
            for (string interest2 : users[candidate].interests) {
                if (interest1 == interest2) {
                    score = score + 1;
                }
            }
        }

        if (score > 0) {
            Recommendation recommendation;
            recommendation.userIndex = candidate;
            recommendation.score = score;
            recommendations.push(recommendation);
        }
    }

    cout << "\n===== FRIEND RECOMMENDATIONS =====\n";

    if (recommendations.empty()) {
        cout << "No suitable new friends found.\n";
        return;
    }

    vector<int> recommendedUsers;
    int rank = 1;

    while (!recommendations.empty()) {
        Recommendation top = recommendations.top();
        recommendations.pop();

        recommendedUsers.push_back(top.userIndex);

        int maxScore = (2 * graph[userIndex].size())
                     + users[userIndex].skills.size()
                     + users[userIndex].interests.size();

        int percentage = 0;
        if (maxScore > 0) {
            percentage = (top.score * 100) / maxScore;
        }

        cout << rank << ". "
             << users[top.userIndex].name
             << " (ID: " << users[top.userIndex].id << ")"
             << " | Match: " << percentage << "%" << endl;

        rank++;
    }

    int choice;

    cout << "\nEnter recommendation number to add as friend (0 to cancel): ";
    cin >> choice;

    if (choice == 0) {
        cout << "No friend added.\n";
        return;
    }

    if (choice < 1 || choice > recommendedUsers.size()) {
        cout << "Invalid recommendation number.\n";
        return;
    }

    int selectedUser = recommendedUsers[choice - 1];

    graph[userIndex].push_back(selectedUser);
    graph[selectedUser].push_back(userIndex);

    cout << "Friendship added successfully with "
         << users[selectedUser].name << "!\n";

    // Save the new friendship immediately.
    saveFriendshipsToFile(graph, users);
}


// ================= ADD RECOMMENDED OPPORTUNITY =================

void recommendAndChooseOpportunity(
    int userIndex,
    vector<User>& users,
    vector<Opportunity>& opportunities
) {
    priority_queue<OpportunityRecommendation> recommendations;

    for (int i = 0; i < opportunities.size(); i++) {
        int score = 0;

        for (string skill : users[userIndex].skills) {
            if (skill == opportunities[i].skill) {
                score = score + 2;
            }
        }

        for (string interest : users[userIndex].interests) {
            if (interest == opportunities[i].interest) {
                score = score + 2;
            }
        }

        if (score > 0) {
            OpportunityRecommendation recommendation;
            recommendation.opportunityIndex = i;
            recommendation.score = score;
            recommendations.push(recommendation);
        }
    }

    cout << "\n===== OPPORTUNITY RECOMMENDATIONS =====\n";

    if (recommendations.empty()) {
        cout << "No suitable opportunities found.\n";
        return;
    }

    vector<int> recommendedOpportunities;
    int rank = 1;

    while (!recommendations.empty()) {
        OpportunityRecommendation top = recommendations.top();
        recommendations.pop();

        recommendedOpportunities.push_back(top.opportunityIndex);

        int percentage = (top.score * 100) / 4;

        cout << rank << ". "
             << opportunities[top.opportunityIndex].name
             << " | Type: " << opportunities[top.opportunityIndex].type
             << " | Match: " << percentage << "%" << endl;

        rank++;
    }

    int choice;

    cout << "\nEnter recommendation number to choose (0 to cancel): ";
    cin >> choice;

    if (choice == 0) {
        cout << "No opportunity selected.\n";
        return;
    }

    if (choice < 1 || choice > recommendedOpportunities.size()) {
        cout << "Invalid recommendation number.\n";
        return;
    }

    int selectedOpportunity = recommendedOpportunities[choice - 1];

    cout << "\nYou selected: "
         << opportunities[selectedOpportunity].name << endl;
    opportunities[selectedOpportunity].displayOpportunity();
}


// ================= AUTO USER ID =================

int generateNextUserId(vector<User>& users) {
    int nextId = 1;

    for (int i = 0; i < users.size(); i++) {
        if (users[i].id >= nextId) {
            nextId = users[i].id + 1;
        }
    }

    return nextId;
}

// ================= MAIN =================

int main() {

    vector<User> users;
    loadUsersFromFile(users);

    unordered_map<int, string> passwords;
    loadPasswordsFromFile(passwords);

    // If no saved users exist, create users first.
    if (users.empty()) {
        int numberOfUsers;

        cout << "No users found. Create users first.\n";
        cout << "Enter number of users: ";
        cin >> numberOfUsers;

        for (int i = 0; i < numberOfUsers; i++) {
            User newUser;

            cout << "\nEnter details for User " << i + 1 << ":\n";
            int newId = generateNextUserId(users);
            newUser.createProfile(newId);

            cout << "Your User ID is: " << newId << endl;
            cout << "Please remember this User ID for login.\n";

            string newPassword;
            cout << "Create a password: ";
            cin >> newPassword;
            passwords[newId] = newPassword;

            users.push_back(newUser);
        }
        saveUsersToFile(users);
        savePasswordsToFile(passwords);
    }

    // Build hash map after all users are loaded/created.
    unordered_map<int, int> userMap;

    for (int i = 0; i < users.size(); i++) {
        userMap[users[i].id] = i;
    }

    // Build friendship graph and load saved friendships.
    vector<vector<int>> graph(users.size());
    loadFriendshipsFromFile(graph, users, userMap);

    // Load opportunities.
    vector<Opportunity> opportunities;
    loadOpportunitiesFromFile(opportunities);

    // Create opportunity tree.
    OpportunityNode* opportunityTree =
        new OpportunityNode("Opportunities");

    OpportunityNode* projectNode =
        new OpportunityNode("Project");

    OpportunityNode* workshopNode =
        new OpportunityNode("Workshop");

    OpportunityNode* courseNode =
        new OpportunityNode("Course");

    opportunityTree->children.push_back(projectNode);
    opportunityTree->children.push_back(workshopNode);
    opportunityTree->children.push_back(courseNode);

    // Add saved opportunities to the tree.
    for (int i = 0; i < opportunities.size(); i++) {
        addOpportunityToTree(opportunityTree, opportunities[i]);
    }

    // If no opportunities exist, create them first.
    if (opportunities.empty()) {
        int numberOfOpportunities;

        cout << "\nNo opportunities found. Create opportunities first.\n";
        cout << "Enter number of opportunities: ";
        cin >> numberOfOpportunities;

        for (int i = 0; i < numberOfOpportunities; i++) {
            Opportunity newOpportunity;

            cout << "\nEnter details for Opportunity " << i + 1 << ":\n";
            newOpportunity.createOpportunity();

            opportunities.push_back(newOpportunity);
            addOpportunityToTree(opportunityTree, newOpportunity);
        }
    }

    // ================= LOGIN SYSTEM =================

    int choice;

    do {
        cout << "\n========== FRIEND AND OPPORTUNITY SYSTEM ==========" << endl;
        cout << "1. Login" << endl;
        cout << "2. Create Account" << endl;
        cout << "3. Display All Users" << endl;
        cout << "4. Add New Opportunity" << endl;
        cout << "5. Save Users" << endl;
        cout << "6. Save Opportunities" << endl;
        cout << "7. Save Friendships" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
        {
            int userIndex = loginUser(users, userMap, passwords);

            if (userIndex == -1) {
                break;
            }

            int dashboardChoice;

            do {
                displayDashboard(userIndex, users, graph);

                cout << "\nWhat would you like to do?" << endl;
                cout << "1. Add Friends" << endl;
                cout << "2. Find Opportunities" << endl;
                cout << "3. Logout" << endl;
                cout << "Enter your choice: ";
                cin >> dashboardChoice;

                switch (dashboardChoice) {

                case 1:
                {
                    char answer;

                    cout << "\nDo you want to add friends? (y/n): ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y') {
                        recommendAndAddFriend(
                            userIndex,
                            users,
                            graph
                        );
                    }
                    else {
                        cout << "No friend recommendation requested.\n";
                    }

                    break;
                }

                case 2:
                {
                    char answer;

                    cout << "\nDo you want to find opportunities? (y/n): ";
                    cin >> answer;
                    answer = tolower(answer);

                    if (answer == 'y') {
                        recommendAndChooseOpportunity(
                            userIndex,
                            users,
                            opportunities
                        );
                    }
                    else {
                        cout << "No opportunity recommendation requested.\n";
                    }

                    break;
                }

                case 3:
                    cout << "Logging out...\n";
                    break;

                default:
                    cout << "Invalid choice!\n";
                }

            } while (dashboardChoice != 3);

            break;
        }

        case 2:
        {
            User newUser;

            cout << "\n========== CREATE ACCOUNT ==========\n";

            int newId = generateNextUserId(users);
            newUser.createProfile(newId);

            // The system automatically assigns a unique User ID.
            cout << "\nAccount created successfully!\n";
            cout << "Your User ID is: " << newId << endl;
            cout << "Please remember this User ID for login.\n";

            string newPassword;
            cout << "Create a password: ";
            cin >> newPassword;
            passwords[newId] = newPassword;

            users.push_back(newUser);
            userMap[newUser.id] = users.size() - 1;
            graph.push_back(vector<int>());

            // Save the new account immediately.
            saveUsersToFile(users);
            savePasswordsToFile(passwords);

            cout << "Account created and saved successfully!\n";
            break;
        }

        case 3:
            for (int i = 0; i < users.size(); i++) {
                users[i].displayProfile();
            }
            break;

        case 4:
        {
            Opportunity newOpportunity;

            cout << "\nEnter details for new opportunity:\n";
            newOpportunity.createOpportunity();

            opportunities.push_back(newOpportunity);
            addOpportunityToTree(opportunityTree, newOpportunity);

            cout << "New opportunity added successfully!\n";
            break;
        }

        case 5:
            saveUsersToFile(users);
            break;

        case 6:
            saveOpportunitiesToFile(opportunities);
            break;

        case 7:
            saveFriendshipsToFile(graph, users);
            break;

        case 9:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}
