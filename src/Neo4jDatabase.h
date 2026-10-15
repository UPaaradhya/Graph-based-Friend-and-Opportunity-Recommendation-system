#pragma once

#include <string>
#include <vector>
#include <simple_json/ptree.hpp>

class Neo4jDatabase
{
public:
    Neo4jDatabase();

    bool connect();
    bool initialize();
    bool execute(const std::string& cypher, const std::string& parameters = "{}");
    std::vector<simple_json::ptree> query(
        const std::string& cypher,
        const std::string& parameters = "{}");

    std::string lastError() const
    {
        return error;
    }

private:
    std::string uri;
    std::string username;
    std::string password;
    std::string database;
    std::string error;

    std::string request(const std::string& body);
};
