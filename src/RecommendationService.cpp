#include "RecommendationService.h"
#include <iostream>

using namespace std;

// Converts a value such as "C++|Python" into a list of values.
static vector<string> splitValues(const string& text)
{
    vector<string> values;
    string current = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '|')
        {
            if (current != "")
            {
                values.push_back(current);
            }
            current = "";
        }
        else
        {
            current += text[i];
        }
    }

    if (current != "")
    {
        values.push_back(current);
    }

    return values;
}

// Checks whether a value is present in a list.
static bool containsValue(const vector<string>& values, const string& wanted)
{
    for (int i = 0; i < (int)values.size(); i++)
    {
        if (values[i] == wanted)
        {
            return true;
        }
    }

    return false;
}

// Reads a user returned by a Neo4j query.
static User readUser(const boost::property_tree::ptree& row)
{
    User user;
    boost::property_tree::ptree::const_iterator item = row.begin();

    if (item == row.end())
    {
        return user;
    }

    user.id = item->second.get_value<int>();
    item++;

    if (item != row.end())
    {
        user.name = item->second.get_value<string>();
        item++;
    }

    if (item != row.end())
    {
        user.course = item->second.get_value<string>();
        item++;
    }

    if (item != row.end())
    {
        user.skills = splitValues(item->second.get_value<string>());
        item++;
    }

    if (item != row.end())
    {
        user.interests = splitValues(item->second.get_value<string>());
    }

    return user;
}

void RecommendationService::recommendFriends(int userId)
{
    vector<boost::property_tree::ptree> myRows;
    myRows = db.query(
        "MATCH (u:User {id:$id}) RETURN u.id,u.name,u.course,u.skills,u.interests",
        "{\"id\":" + to_string(userId) + "}");

    if (myRows.empty())
    {
        cout << "User not found.\n";
        return;
    }

    User currentUser = readUser(myRows[0]);

    vector<boost::property_tree::ptree> rows;
    rows = db.query(
        "MATCH (u:User) WHERE u.id <> $id "
        "AND NOT EXISTS { MATCH (:User {id:$id})-[:FRIENDS_WITH]-(u) } "
        "RETURN u.id,u.name,u.course,u.skills,u.interests",
        "{\"id\":" + to_string(userId) + "}");

    vector<User> suggestedUsers;
    vector<int> scores;

    for (int i = 0; i < (int)rows.size(); i++)
    {
        User otherUser = readUser(rows[i]);
        int score = 0;

        for (int j = 0; j < (int)currentUser.skills.size(); j++)
        {
            if (containsValue(otherUser.skills, currentUser.skills[j]))
            {
                score++;
            }
        }

        for (int j = 0; j < (int)currentUser.interests.size(); j++)
        {
            if (containsValue(otherUser.interests, currentUser.interests[j]))
            {
                score++;
            }
        }

        vector<User> mutual = graph.mutualFriends(userId, otherUser.id);
        score = score + (int)mutual.size() * 2;

        if (score > 0)
        {
            suggestedUsers.push_back(otherUser);
            scores.push_back(score);
        }
    }

    // Simple bubble sort: highest score comes first.
    for (int i = 0; i < (int)scores.size(); i++)
    {
        for (int j = 0; j < (int)scores.size() - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                User tempUser = suggestedUsers[j];
                suggestedUsers[j] = suggestedUsers[j + 1];
                suggestedUsers[j + 1] = tempUser;
            }
        }
    }

    if (suggestedUsers.empty())
    {
        cout << "No friend recommendations found.\n";
        return;
    }

    int highestScore = scores[0];

    cout << "\nFriend recommendations:\n";
    for (int i = 0; i < (int)suggestedUsers.size(); i++)
    {
        int percentage = scores[i] * 100 / highestScore;
        cout << i + 1 << ". ID " << suggestedUsers[i].id
             << " - " << suggestedUsers[i].name
             << " | Match: " << percentage << "%\n";
    }

    int chosenId;
    cout << "Enter the ID of the friend to add, or 0 to cancel: ";
    cin >> chosenId;

    if (chosenId == 0)
    {
        return;
    }

    bool found = false;
    for (int i = 0; i < (int)suggestedUsers.size(); i++)
    {
        if (suggestedUsers[i].id == chosenId)
        {
            found = true;
        }
    }

    if (!found)
    {
        cout << "Please enter an ID from the recommendation list.\n";
        return;
    }

    if (graph.addFriendship(userId, chosenId))
    {
        cout << "Friend added successfully.\n";
    }
    else
    {
        cout << "Could not add this friend.\n";
    }
}

void RecommendationService::recommendOpportunities(int userId)
{
    vector<boost::property_tree::ptree> userRows;
    userRows = db.query(
        "MATCH (u:User {id:$id}) RETURN u.skills,u.interests",
        "{\"id\":" + to_string(userId) + "}");

    if (userRows.empty())
    {
        cout << "User not found.\n";
        return;
    }

    vector<string> userSkills;
    vector<string> userInterests;
    boost::property_tree::ptree::const_iterator item = userRows[0].begin();

    if (item != userRows[0].end())
    {
        userSkills = splitValues(item->second.get_value<string>());
        item++;
    }

    if (item != userRows[0].end())
    {
        userInterests = splitValues(item->second.get_value<string>());
    }

    vector<Opportunity> allOpportunities = opportunities.all();
    vector<Opportunity> suggestedOpportunities;
    vector<int> scores;

    for (int i = 0; i < (int)allOpportunities.size(); i++)
    {
        int score = 0;

        if (containsValue(userSkills, allOpportunities[i].skill))
        {
            score = score + 2;
        }

        if (containsValue(userInterests, allOpportunities[i].interest))
        {
            score = score + 2;
        }

        if (score > 0)
        {
            suggestedOpportunities.push_back(allOpportunities[i]);
            scores.push_back(score);
        }
    }

    // Simple bubble sort: highest score comes first.
    for (int i = 0; i < (int)scores.size(); i++)
    {
        for (int j = 0; j < (int)scores.size() - 1; j++)
        {
            if (scores[j] < scores[j + 1])
            {
                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                Opportunity tempOpportunity = suggestedOpportunities[j];
                suggestedOpportunities[j] = suggestedOpportunities[j + 1];
                suggestedOpportunities[j + 1] = tempOpportunity;
            }
        }
    }

    if (suggestedOpportunities.empty())
    {
        cout << "No matching opportunities found.\n";
        return;
    }

    int highestScore = scores[0];

    cout << "\nOpportunity recommendations:\n";
    for (int i = 0; i < (int)suggestedOpportunities.size(); i++)
    {
        int percentage = scores[i] * 100 / highestScore;
        cout << i + 1 << ". ID " << suggestedOpportunities[i].id
             << " - " << suggestedOpportunities[i].name
             << " (" << suggestedOpportunities[i].type << ")"
             << " | Match: " << percentage << "%\n";
    }

    int selectedId;
    cout << "Enter an opportunity ID to select, or 0 to cancel: ";
    cin >> selectedId;

    if (selectedId == 0)
    {
        return;
    }

    for (int i = 0; i < (int)suggestedOpportunities.size(); i++)
    {
        if (suggestedOpportunities[i].id == selectedId)
        {
            cout << "Selected opportunity: "
                 << suggestedOpportunities[i].name << "\n";
            return;
        }
    }

    cout << "Please enter an ID from the recommendation list.\n";
}
