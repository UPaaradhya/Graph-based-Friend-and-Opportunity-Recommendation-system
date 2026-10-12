#include "Neo4jDatabase.h"
#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using boost::property_tree::ptree;

// libcurl calls this function when the server sends a response.
static size_t receiveResponse(char* data, size_t size, size_t count, void* userData)
{
    string* response = static_cast<string*>(userData);
    size_t length = size * count;
    response->append(data, length);
    return length;
}

// Escapes special characters in a Cypher query before adding it to JSON.
static string escapeJson(const string& text)
{
    string result = "";

    for (int i = 0; i < (int)text.length(); i++)
    {
        char character = text[i];

        if (character == '"')
        {
            result += "\\\"";
        }
        else if (character == '\\')
        {
            result += "\\\\";
        }
        else if (character == '\n')
        {
            result += "\\n";
        }
        else if (character == '\r')
        {
            result += "\\r";
        }
        else if (character == '\t')
        {
            result += "\\t";
        }
        else
        {
            result += character;
        }
    }

    return result;
}

Neo4jDatabase::Neo4jDatabase()
{
    uri = "http://localhost:7474/db/neo4j/tx/commit";
    username = "neo4j";
    password = "password";
    database = "neo4j";

    // Read the connection settings from config/neo4j.conf.
    ifstream file("config/neo4j.conf");
    string line;

    while (getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        size_t equalPosition = line.find('=');
        if (equalPosition == string::npos)
        {
            continue;
        }

        string key = line.substr(0, equalPosition);
        string value = line.substr(equalPosition + 1);

        if (key == "uri")
        {
            uri = value;
        }
        else if (key == "username")
        {
            username = value;
        }
        else if (key == "password")
        {
            password = value;
        }
        else if (key == "database")
        {
            database = value;
        }
    }

    // Add the database path if only the server address was supplied.
    if (uri.find("/db/") == string::npos)
    {
        uri = uri + "/db/" + database + "/tx/commit";
    }
}

bool Neo4jDatabase::connect()
{
    string response = request(
        "{\"statements\":[{\"statement\":\"RETURN 1 AS ok\"}]}");

    return !response.empty();
}

// Sends one HTTP request to Neo4j and returns its response.
string Neo4jDatabase::request(const string& body)
{
    CURL* curl = curl_easy_init();

    if (curl == NULL)
    {
        error = "Could not start the HTTP connection.";
        return "";
    }

    string response;
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
    curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receiveResponse);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);

    CURLcode result = curl_easy_perform(curl);
    long statusCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        error = curl_easy_strerror(result);
        return "";
    }

    if (statusCode < 200 || statusCode >= 300)
    {
        error = "HTTP status " + to_string(statusCode) + ": " + response;
        return "";
    }

    // Neo4j can return query errors inside an otherwise successful HTTP response.
    try
    {
        stringstream input(response);
        ptree root;
        boost::property_tree::read_json(input, root);

        boost::optional<ptree&> errors = root.get_child_optional("errors");
        if (errors && !errors->empty())
        {
            error = errors->front().second.get<string>("message", "Neo4j query error");
            return "";
        }
    }
    catch (const exception& problem)
    {
        error = string("Could not read Neo4j response: ") + problem.what();
        return "";
    }

    return response;
}

bool Neo4jDatabase::execute(const string& cypher, const string& parameters)
{
    string body = "{\"statements\":[{\"statement\":\"";
    body += escapeJson(cypher);
    body += "\",\"parameters\":" + parameters + "}]}";

    string response = request(body);
    return !response.empty();
}

vector<ptree> Neo4jDatabase::query(const string& cypher, const string& parameters)
{
    vector<ptree> rows;
    string body = "{\"statements\":[{\"statement\":\"";
    body += escapeJson(cypher);
    body += "\",\"parameters\":" + parameters + "}]}";

    string response = request(body);
    if (response.empty())
    {
        return rows;
    }

    try
    {
        stringstream input(response);
        ptree root;
        boost::property_tree::read_json(input, root);

        ptree& results = root.get_child("results");
        for (ptree::iterator result = results.begin(); result != results.end(); result++)
        {
            ptree& dataRows = result->second.get_child("data");

            for (ptree::iterator row = dataRows.begin(); row != dataRows.end(); row++)
            {
                boost::optional<ptree&> values = row->second.get_child_optional("row");
                if (values)
                {
                    rows.push_back(values.get());
                }
            }
        }
    }
    catch (const exception& problem)
    {
        error = problem.what();
    }

    return rows;
}

bool Neo4jDatabase::initialize()
{
    bool usersReady = execute(
        "CREATE CONSTRAINT user_id_unique IF NOT EXISTS "
        "FOR (u:User) REQUIRE u.id IS UNIQUE");

    if (!usersReady)
    {
        return false;
    }

    bool opportunitiesReady = execute(
        "CREATE CONSTRAINT opportunity_id_unique IF NOT EXISTS "
        "FOR (o:Opportunity) REQUIRE o.id IS UNIQUE");

    return opportunitiesReady;
}
