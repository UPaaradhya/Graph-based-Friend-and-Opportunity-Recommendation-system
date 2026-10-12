#pragma once

#include "Models.h"
#include "Neo4jDatabase.h"
#include <vector>

class GraphManager
{
public:
    explicit GraphManager(Neo4jDatabase& database) : db(database) {}

    bool addFriendship(int id1, int id2);
    std::vector<User> friendsOf(int id);
    std::vector<User> mutualFriends(int id1, int id2);
    bool areFriends(int id1, int id2);

private:
    Neo4jDatabase& db;
};
