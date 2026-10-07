#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <fstream>

using namespace std;


// ================= USER CLASS =================

class User {
public:
    int id;
    string name;
    string course;
    vector<string> skills;
    vector<string> interests;

    void createProfile() {

        cout << "Enter User ID: ";
        cin >> id;

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

        cout << rank << ". "
             << users[top.userIndex].name
             << " | Score: "
             << top.score
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

        cout << rank << ". "
             << opportunities[top.opportunityIndex].name
             << " | Type: "
             << opportunities[top.opportunityIndex].type
             << " | Score: "
             << top.score
             << endl;

        rank++;
    }
}


// ================= TREE =================

void addOpportunityToTree(
    OpportunityNode* root,
    Opportunity& opportunity
) {

    if (opportunity.type == "Project") {

        root->children[0]->children.push_back(
            new OpportunityNode(opportunity.name)
        );
    }

    else if (opportunity.type == "Workshop") {

        root->children[1]->children.push_back(
            new OpportunityNode(opportunity.name)
        );
    }

    else if (opportunity.type == "Course") {

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

void displayOpportunityTree(
    OpportunityNode* root,
    int level
) {

    for (int i = 0; i < level; i++) {

        cout << "  ";
    }

    cout << "|-- " << root->category << endl;

    for (int i = 0; i < root->children.size(); i++) {

        displayOpportunityTree(
            root->children[i],
            level + 1
        );
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

    while (file >> opportunity.id) {
        file.ignore();

        getline(file, opportunity.name);
        getline(file, opportunity.type);
        getline(file, opportunity.skill);
        getline(file, opportunity.interest);

        opportunities.push_back(opportunity);
    }

    file.close();

    cout << "Opportunities loaded successfully!" << endl;
}

// ================= MAIN =================

int main() {

    vector<User> users;
    loadUsersFromFile(users);

    // ================= CREATE USERS =================

    int numberOfUsers;

    cout << "Enter number of new users: ";
    cin >> numberOfUsers;

    for (int i = 0; i < numberOfUsers; i++) {

        User newUser;

        cout << "\nEnter details for User "
             << i + 1
             << ":\n";

        newUser.createProfile();

        users.push_back(newUser);
    }

    // ================= HASH MAP =================

    unordered_map<int, int> userMap;

    for (int i = 0; i < users.size(); i++) {

        userMap[users[i].id] = i;
    }

    // ================= GRAPH =================

    vector<vector<int>> graph(users.size());

    // Load old friendships after users and userMap are ready
    loadFriendshipsFromFile(graph, users, userMap);

    // ================= LOAD OPPORTUNITIES =================

    vector<Opportunity> opportunities;
    loadOpportunitiesFromFile(opportunities);

    // ================= CREATE OPPORTUNITY TREE =================

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

    // Add already saved opportunities to the tree
    for (int i = 0; i < opportunities.size(); i++) {

        addOpportunityToTree(
            opportunityTree,
            opportunities[i]
        );
    }

    // ================= CREATE NEW OPPORTUNITIES =================

    int numberOfOpportunities;

    cout << "\nEnter number of new opportunities: ";
    cin >> numberOfOpportunities;

    for (int i = 0; i < numberOfOpportunities; i++) {

        Opportunity newOpportunity;

        cout << "\nEnter details for Opportunity "
             << i + 1
             << ":\n";

        newOpportunity.createOpportunity();

        opportunities.push_back(newOpportunity);

        addOpportunityToTree(
            opportunityTree,
            newOpportunity
        );
    }

    // ================= MENU =================

    int choice;

    do {

        cout << "\n========== MENU ==========\n";

        cout << "1. Display All Users\n";
        cout << "2. Add Friendship\n";
        cout << "3. Display Friends\n";
        cout << "4. Friend Recommendation\n";
        cout << "5. DFS Network Traversal\n";
        cout << "6. Find Mutual Friends\n";
        cout << "7. Common Skills and Interests\n";
        cout << "8. Recommend Opportunities\n";
        cout << "9. Display Opportunity Tree\n";
        cout << "10. Search Opportunity\n";
        cout << "11. Save Users\n";
        cout << "12. Save Opportunities\n";
        cout << "13. Save Friendships\n";
        cout << "14. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:

            for (int i = 0; i < users.size(); i++) {

                users[i].displayProfile();
            }

            break;


        case 2:

            addFriendship(
                graph,
                users,
                userMap
            );

            break;


        case 3:

            displayFriends(
                graph,
                users,
                userMap
            );

            break;


        case 4:

            friendRecommendation(
                graph,
                users,
                userMap
            );

            break;


        case 5:

            dfsTraversal(
                graph,
                users,
                userMap
            );

            break;


        case 6:

            findMutualFriends(
                graph,
                users,
                userMap
            );

            break;


        case 7:

            findCommonSkillsAndInterests(
                users,
                userMap
            );

            break;


        case 8:

            recommendOpportunities(
                users,
                opportunities,
                userMap
            );

            break;


        case 9:

            displayOpportunityTree(
                opportunityTree,
                0
            );

            break;


        case 10:
        {
            string name;

            cout << "Enter Opportunity Name: ";
            cin >> name;

            if (!searchOpportunityInTree(
                    opportunityTree,
                    name
                )) {

                cout << "Opportunity not found.\n";
            }

            break;
        }


        case 11:

            saveUsersToFile(users);

            break;


        case 12:

            saveOpportunitiesToFile(opportunities);

            break;


        case 13:

            saveFriendshipsToFile(
                graph,
                users
            );

            break;


        case 14:

            cout << "Exiting program...\n";

            break;


        default:

            cout << "Invalid choice!\n";
        }

    } while (choice != 14);


    return 0;
}
