#include "UserService.h"
#include <iostream>
#include <sstream>
#include <limits>

using namespace std;

// Escapes text before it is placed inside a JSON request.
static string escapeText(const string& text)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        if (text[i] == '\\' || text[i] == '"')
        {
            result += '\\';
        }

        if (text[i] != '\n' && text[i] != '\r')
        {
            result += text[i];
        }
    }

    return result;
}

// Joins skills or interests into one value for storage in Neo4j.
static string joinValues(const vector<string>& values)
{
    string result = "";

    for (int i = 0; i < (int)values.size(); i++)
    {
        if (i > 0)
        {
            result += '|';
        }
        result += values[i];
    }

    return result;
}

// Splits a stored value back into a list.
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

// Converts one database row into a User object.
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
        user.password = item->second.get_value<string>();
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

vector<User> UserService::allUsers()
{
    vector<User> users;
    vector<boost::property_tree::ptree> rows;

    rows = db.query(
        "MATCH (u:User) "
        "RETURN u.id,u.name,u.course,u.password,u.skills,u.interests ORDER BY u.id");

    for (int i = 0; i < (int)rows.size(); i++)
    {
        users.push_back(readUser(rows[i]));
    }

    return users;
}

bool UserService::getUser(int id, User& user)
{
    vector<boost::property_tree::ptree> rows;
    rows = db.query(
        "MATCH (u:User {id:$id}) "
        "RETURN u.id,u.name,u.course,u.password,u.skills,u.interests",
        "{\"id\":" + to_string(id) + "}");

    if (rows.empty())
    {
        return false;
    }

    user = readUser(rows[0]);
    return true;
}

bool UserService::createAccount()
{
    string name;
    string course;
    string password;
    int number;

    cout << "Name (one word): ";
    cin >> name;

    cout << "Course (one word): ";
    cin >> course;

    cout << "Create password: ";
    cin >> password;

    cout << "Number of skills: ";
    if (!(cin >> number) || number < 0 || number > 30)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number.\n";
        return false;
    }

    vector<string> skills;
    vector<string> interests;

    for (int i = 0; i < number; i++)
    {
        string skill;
        cout << "Skill " << i + 1 << ": ";
        cin >> skill;
        skills.push_back(skill);
    }

    cout << "Number of interests: ";
    if (!(cin >> number) || number < 0 || number > 30)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number.\n";
        return false;
    }

    for (int i = 0; i < number; i++)
    {
        string interest;
        cout << "Interest " << i + 1 << ": ";
        cin >> interest;
        interests.push_back(interest);
    }

    vector<boost::property_tree::ptree> idRows;
    idRows = db.query("MATCH (u:User) RETURN coalesce(max(u.id),99)+1");

    int nextId = 100;
    if (!idRows.empty())
    {
        try
        {
            nextId = idRows[0].begin()->second.get_value<int>();
        }
        catch (...)
        {
            nextId = 100;
        }
    }

    string parameters = "{\"id\":" + to_string(nextId)
        + ",\"name\":\"" + escapeText(name)
        + "\",\"course\":\"" + escapeText(course)
        + "\",\"password\":\"" + escapeText(password)
        + "\",\"skills\":\"" + escapeText(joinValues(skills))
        + "\",\"interests\":\"" + escapeText(joinValues(interests)) + "\"}";

    bool saved = db.execute(
        "CREATE (u:User {id:$id,name:$name,course:$course,password:$password,skills:$skills,interests:$interests})",
        parameters);

    if (!saved)
    {
        cout << "Account creation failed: " << db.lastError() << "\n";
        return false;
    }

    cout << "Account created. Your user ID is " << nextId << "\n";
    return true;
}

bool UserService::login(User& loggedIn)
{
    int id;
    string password;

    cout << "User ID: ";
    cin >> id;

    cout << "Password: ";
    cin >> password;

    if (!getUser(id, loggedIn) || loggedIn.password != password)
    {
        cout << "Invalid ID or password.\n";
        return false;
    }

    cout << "Welcome, " << loggedIn.name << "!\n";
    return true;
}
