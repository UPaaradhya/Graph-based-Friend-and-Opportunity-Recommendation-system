// Opportunity recommendations and opportunity category tree operations.
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




// ================= ADD RECOMMENDED OPPORTUNITY =================

void recommendAndChooseOpportunity(
    int userIndex,
    vector<User>& users,
    vector<Opportunity>& opportunities
) {
    priority_queue<OpportunityRecommendation> recommendations;

    for (int i = 0; i < opportunities.size(); i++) {
        // ADDED: do not suggest an opportunity the user has already received
        bool alreadyTaken = false;
        for (int takenId : users[userIndex].takenOpportunities) {
            if (takenId == opportunities[i].id) {
                alreadyTaken = true;
            }
        }
        if (alreadyTaken) {
            continue;
        }

        int score = 0;
        bool skillMatched = false;

        // CHANGED: an opportunity is suggested only if the user has the required skill
        for (string skill : users[userIndex].skills) {
            if (skill == opportunities[i].skill) {
                score = score + 2;
                skillMatched = true;
            }
        }

        // A matching interest only raises the match percentage (bonus)
        if (skillMatched) {
            for (string interest : users[userIndex].interests) {
                if (interest == opportunities[i].interest) {
                    score = score + 2;
                }
            }
        }

        if (skillMatched && score > 0) {
            OpportunityRecommendation recommendation;
            recommendation.opportunityIndex = i;
            recommendation.score = score;
            recommendations.push(recommendation);
        }
    }

    // ADDED: show opportunities this user has already received
    if (!users[userIndex].takenOpportunities.empty()) {
        cout << "\nOpportunities you have already received:\n";
        for (int takenId : users[userIndex].takenOpportunities) {
            for (int j = 0; j < opportunities.size(); j++) {
                if (opportunities[j].id == takenId) {
                    cout << "- " << opportunities[j].name
                         << " (" << opportunities[j].type << ")\n";
                }
            }
        }
    }

    cout << "\n===== OPPORTUNITY RECOMMENDATIONS =====\n";

    if (recommendations.empty()) {
        cout << "No suitable opportunities found.\n";
        return;
    }

    vector<int> recommendedOpportunities;
    int rank = 1;

    while (!recommendations.empty() && rank <= 10) {   // CHANGED: top 10 only
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

    // ADDED: remember that this user has now received this opportunity
    users[userIndex].takenOpportunities.push_back(opportunities[selectedOpportunity].id);
    saveTakenOpportunity(users[userIndex].id, opportunities[selectedOpportunity].id);
    cout << "\nAdded to your opportunities. It will not be suggested again.\n";
}


