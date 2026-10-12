#pragma once

#include "Models.h"
#include "Neo4jDatabase.h"
#include <vector>

class UserService
{
public:
    explicit UserService(Neo4jDatabase& database) : db(database) {}

    bool createAccount();
    bool login(User& loggedIn);
    std::vector<User> allUsers();
    bool getUser(int id, User& user);

private:
    Neo4jDatabase& db;
};
