// Friend recommendations, DFS network traversal, and adding a recommended friend.
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

    while (!recommendations.empty() && rank <= 10) {   // CHANGED: top 10 only
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


