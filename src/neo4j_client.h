#pragma once

// This file connects our C++ project to Neo4j.
// We use libcurl to send requests and json to read the response.

#include <string>
#include <stdexcept>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

class Neo4jClient {
private:
    std::string endpoint;
    std::string userpwd;

    // This function collects the reply sent by Neo4j.
    static size_t writeCallback(char* data, size_t size, size_t count, void* userData) {
        std::string* response = static_cast<std::string*>(userData);
        response->append(data, size * count);
        return size * count;
    }

public:
    // Prepare the database URL and login details.
    Neo4jClient(const std::string& host,
                const std::string& database,
                const std::string& user,
                const std::string& password) {
        endpoint = host + "/db/" + database + "/tx/commit";
        userpwd = user + ":" + password;
    }

    // Send a Cypher query to Neo4j and return the result rows.
    json run(const std::string& cypher, const json& params = json::object()) {
        json statement;
        statement["statement"] = cypher;
        statement["parameters"] = params;

        // Neo4j expects the query inside a "statements" list.
        json request;
        request["statements"] = json::array();
        request["statements"].push_back(statement);

        std::string requestBody = request.dump();
        std::string response;

        CURL* curl = curl_easy_init();
        if (curl == nullptr) {
            throw std::runtime_error("Could not initialise libcurl");
        }

        struct curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Accept: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, endpoint.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode result = curl_easy_perform(curl);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (result != CURLE_OK) {
            throw std::runtime_error(
                std::string("Cannot reach Neo4j (check if it is running): ")
                + curl_easy_strerror(result));
        }

        // Convert the response text into a JSON object.
        json reply = json::parse(response, nullptr, false);
        if (reply.is_discarded()) {
            throw std::runtime_error("Neo4j returned an unreadable response. Check the URL.");
        }

        // If Neo4j reports a query error, show its message.
        if (reply.contains("errors") && !reply["errors"].empty()) {
            throw std::runtime_error(
                reply["errors"][0].value("message", "Unknown Neo4j error"));
        }

        // Extract only the rows that our project needs.
        json rows = json::array();
        for (auto& item : reply["results"][0]["data"]) {
            rows.push_back(item["row"]);
        }

        return rows;
    }
};
