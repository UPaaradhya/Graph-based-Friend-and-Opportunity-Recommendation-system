#include "GraphManager.h"
#include <string>

using namespace std;

// Splits a stored list such as "C++|Python" into separate values.
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

// Converts one row from Neo4j into a User object.
static User readUser(const simple_json::ptree& row)
{
    User user;
    simple_json::ptree::const_iterator item = row.begin();

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

bool GraphManager::areFriends(int id1, int id2)
{
    vector<simple_json::ptree> rows;
    rows = db.query(
        "MATCH (:User {id:$a})-[:FRIENDS_WITH]-(:User {id:$b}) RETURN 1",
        "{\"a\":" + to_string(id1) + ",\"b\":" + to_string(id2) + "}");

    return !rows.empty();
}

bool GraphManager::addFriendship(int id1, int id2)
{
    if (id1 == id2)
    {
        return false;
    }

    vector<simple_json::ptree> users;
    users = db.query(
        "MATCH (a:User {id:$a}),(b:User {id:$b}) RETURN a.id,b.id",
        "{\"a\":" + to_string(id1) + ",\"b\":" + to_string(id2) + "}");

    if (users.empty())
    {
        return false;
    }

    if (areFriends(id1, id2))
    {
        return false;
    }

    return db.execute(
        "MATCH (a:User {id:$a}),(b:User {id:$b}) "
        "MERGE (a)-[:FRIENDS_WITH]-(b)",
        "{\"a\":" + to_string(id1) + ",\"b\":" + to_string(id2) + "}");
}

vector<User> GraphManager::friendsOf(int id)
{
    vector<User> friends;
    vector<simple_json::ptree> rows;

    rows = db.query(
        "MATCH (u:User {id:$id})-[:FRIENDS_WITH]-(f:User) "
        "RETURN f.id,f.name,f.course,f.skills,f.interests ORDER BY f.name",
        "{\"id\":" + to_string(id) + "}");

    for (int i = 0; i < (int)rows.size(); i++)
    {
        friends.push_back(readUser(rows[i]));
    }

    return friends;
}

vector<User> GraphManager::mutualFriends(int id1, int id2)
{
    vector<User> friends;
    vector<simple_json::ptree> rows;

    rows = db.query(
        "MATCH (:User {id:$a})-[:FRIENDS_WITH]-(f:User)"
        "-[:FRIENDS_WITH]-(:User {id:$b}) "
        "RETURN DISTINCT f.id,f.name,f.course,f.skills,f.interests ORDER BY f.name",
        "{\"a\":" + to_string(id1) + ",\"b\":" + to_string(id2) + "}");

    for (int i = 0; i < (int)rows.size(); i++)
    {
        friends.push_back(readUser(rows[i]));
    }

    return friends;
}
