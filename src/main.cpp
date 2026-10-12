#include "Neo4jDatabase.h"
#include "UserService.h"
#include "GraphManager.h"
#include "RecommendationService.h"
#include "OpportunityService.h"
#include <iostream>
#include <limits>

using namespace std;

bool askYesOrNo(string question)
{
    string answer;

    while (true)
    {
        cout << question << " (y/n): ";
        cin >> answer;

        if (answer == "y" || answer == "Y")
        {
            return true;
        }

        if (answer == "n" || answer == "N")
        {
            return false;
        }

        cout << "Please enter y or n.\n";
    }
}

void showDashboard(User user, GraphManager& graph)
{
    cout << "\n========== YOUR DASHBOARD ==========\n";
    cout << "User ID: " << user.id << "\n";
    cout << "Name: " << user.name << "\n";
    cout << "Course: " << user.course << "\n";

    cout << "Skills: ";
    if (user.skills.size() == 0)
    {
        cout << "None";
    }
    else
    {
        for (int i = 0; i < (int)user.skills.size(); i++)
        {
            cout << user.skills[i] << " ";
        }
    }

    cout << "\nInterests: ";
    if (user.interests.size() == 0)
    {
        cout << "None";
    }
    else
    {
        for (int i = 0; i < (int)user.interests.size(); i++)
        {
            cout << user.interests[i] << " ";
        }
    }

    cout << "\n\nCurrent friends:\n";
    vector<User> friends = graph.friendsOf(user.id);

    if (friends.size() == 0)
    {
        cout << "No friends added yet.\n";
    }
    else
    {
        for (int i = 0; i < (int)friends.size(); i++)
        {
            cout << "- " << friends[i].name
                 << " (ID " << friends[i].id << ")\n";
        }
    }

    cout << "====================================\n";
}

int main()
{
    Neo4jDatabase database;

    cout << "Connecting to Neo4j...\n";
    if (!database.connect())
    {
        cout << "Could not connect to Neo4j.\n";
        cout << database.lastError() << "\n";
        cout << "Check Neo4j and config/neo4j.conf.\n";
        return 1;
    }

    if (!database.initialize())
    {
        cout << "Could not prepare the database.\n";
        cout << database.lastError() << "\n";
        return 1;
    }

    cout << "Connected to Neo4j successfully.\n";

    UserService userService(database);
    GraphManager graph(database);
    OpportunityService opportunityService(database);
    RecommendationService recommendationService(
        database, graph, opportunityService);

    int choice;

    while (true)
    {
        cout << "\n===== FRIEND & OPPORTUNITY SYSTEM =====\n";
        cout << "1. Login\n";
        cout << "2. Create account\n";
        cout << "3. Add opportunity\n";
        cout << "0. Exit\n";
        cout << "Choose: ";

        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        if (choice == 0)
        {
            break;
        }
        else if (choice == 2)
        {
            userService.createAccount();
        }
        else if (choice == 3)
        {
            opportunityService.addOpportunity();
        }
        else if (choice == 1)
        {
            User currentUser;

            if (!userService.login(currentUser))
            {
                continue;
            }

            // Show the dashboard after a successful login.
            userService.getUser(currentUser.id, currentUser);
            showDashboard(currentUser, graph);

            if (askYesOrNo("Would you like friend recommendations?"))
            {
                recommendationService.recommendFriends(currentUser.id);
            }

            if (askYesOrNo("Would you like opportunity recommendations?"))
            {
                recommendationService.recommendOpportunities(currentUser.id);
            }

            cout << "\nYou have been logged out.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
