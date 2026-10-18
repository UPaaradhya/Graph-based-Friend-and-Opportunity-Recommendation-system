#include "UserModule.cpp"
#include "RecommendationTypes.cpp"
#include "FriendshipModule.cpp"
#include "FriendRecommendationModule.cpp"
#include "OpportunityRecommendationModule.cpp"
#include "AuthenticationModule.cpp"

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
    if (!connectNeo4j()) return 1;

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
    loadTakenOpportunities(users);   // ADDED: load opportunities users already received

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

        case 8:
            cout << "Exiting program...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}
