#pragma once
// neo4j_storage.h
// Replaces the .txt-file save/load functions in main.cpp.
// The function names and parameters are IDENTICAL to your old ones,
// so nothing else in main.cpp has to change.
//
// IMPORTANT: include this file in main.cpp AFTER the User and Opportunity
// classes are defined (right after the Opportunity class).

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "neo4j_client.h"

// ---- CHANGE THESE TO MATCH YOUR NEO4J INSTALLATION ----
inline Neo4jClient db("http://127.0.0.1:7474", "neo4j", "neo4j", "Deepak@123M");


// ================= CONNECT + CREATE CONSTRAINTS =================

inline bool connectNeo4j() {
    try {
        db.run("CREATE CONSTRAINT user_id IF NOT EXISTS "
               "FOR (u:User) REQUIRE u.id IS UNIQUE");
        db.run("CREATE CONSTRAINT opportunity_id IF NOT EXISTS "
               "FOR (o:Opportunity) REQUIRE o.id IS UNIQUE");

        std::cout << "Connected to Neo4j successfully!\n";
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j connection failed: " << e.what() << "\n";
        return false;
    }
}


// ================= USERS =================

inline void saveUsersToFile(std::vector<User>& users) {
    try {
        json list = json::array();

        for (User& u : users) {
            list.push_back(json{
                {"id", u.id},
                {"name", u.name},
                {"course", u.course},
                {"skills", u.skills},
                {"interests", u.interests}
            });
        }

        db.run(
            "UNWIND $users AS u "
            "MERGE (n:User {id: u.id}) "
            "SET n.name = u.name, n.course = u.course, "
            "    n.skills = u.skills, n.interests = u.interests",
            json{{"users", list}}
        );

        std::cout << "Users saved to Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (saveUsers): " << e.what() << std::endl;
    }
}

inline void loadUsersFromFile(std::vector<User>& users) {
    try {
        json rows = db.run(
            "MATCH (u:User) "
            "RETURN u.id, u.name, u.course, u.skills, u.interests "
            "ORDER BY u.id"
        );

        for (auto& r : rows) {
            User user;
            user.id     = r[0].get<int>();
            user.name   = r[1].is_string() ? r[1].get<std::string>() : "";
            user.course = r[2].is_string() ? r[2].get<std::string>() : "";

            if (r[3].is_array()) user.skills    = r[3].get<std::vector<std::string>>();
            if (r[4].is_array()) user.interests = r[4].get<std::vector<std::string>>();

            users.push_back(user);
        }

        std::cout << "Users loaded from Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (loadUsers): " << e.what() << std::endl;
    }
}


// ================= FRIENDSHIPS =================
// Stored as (:User)-[:FRIEND]-(:User). One relationship per pair.

inline void saveFriendshipsToFile(
    std::vector<std::vector<int>>& graph,
    std::vector<User>& users
) {
    try {
        json pairs = json::array();

        for (size_t i = 0; i < graph.size(); i++) {
            for (size_t j = 0; j < graph[i].size(); j++) {

                int a = users[i].id;
                int b = users[graph[i][j]].id;

                if (a < b) {   // save each friendship only once
                    pairs.push_back(json{{"a", a}, {"b", b}});
                }
            }
        }

        if (!pairs.empty()) {
            db.run(
                "UNWIND $pairs AS p "
                "MATCH (a:User {id: p.a}), (b:User {id: p.b}) "
                "MERGE (a)-[:FRIEND]-(b)",
                json{{"pairs", pairs}}
            );
        }

        std::cout << "Friendships saved to Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (saveFriendships): " << e.what() << std::endl;
    }
}

inline void loadFriendshipsFromFile(
    std::vector<std::vector<int>>& graph,
    std::vector<User>& users,
    std::unordered_map<int, int>& userMap
) {
    try {
        json rows = db.run(
            "MATCH (a:User)-[:FRIEND]-(b:User) "
            "WHERE a.id < b.id "
            "RETURN a.id, b.id"
        );

        for (auto& r : rows) {
            auto it1 = userMap.find(r[0].get<int>());
            auto it2 = userMap.find(r[1].get<int>());

            if (it1 != userMap.end() && it2 != userMap.end()) {
                graph[it1->second].push_back(it2->second);
                graph[it2->second].push_back(it1->second);
            }
        }

        std::cout << "Friendships loaded from Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (loadFriendships): " << e.what() << std::endl;
    }
}


// ================= OPPORTUNITIES =================

inline void saveOpportunitiesToFile(std::vector<Opportunity>& opportunities) {
    try {
        json list = json::array();

        for (Opportunity& o : opportunities) {
            list.push_back(json{
                {"id", o.id},
                {"name", o.name},
                {"type", o.type},
                {"skill", o.skill},
                {"interest", o.interest}
            });
        }

        if (!list.empty()) {
            db.run(
                "UNWIND $items AS o "
                "MERGE (n:Opportunity {id: o.id}) "
                "SET n.name = o.name, n.type = o.type, "
                "    n.skill = o.skill, n.interest = o.interest",
                json{{"items", list}}
            );
        }

        std::cout << "Opportunities saved to Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (saveOpportunities): " << e.what() << std::endl;
    }
}

inline void loadOpportunitiesFromFile(std::vector<Opportunity>& opportunities) {
    try {
        json rows = db.run(
            "MATCH (o:Opportunity) "
            "RETURN o.id, o.name, o.type, o.skill, o.interest "
            "ORDER BY o.id"
        );

        for (auto& r : rows) {
            Opportunity o;
            o.id       = r[0].get<int>();
            o.name     = r[1].get<std::string>();
            o.type     = r[2].get<std::string>();
            o.skill    = r[3].get<std::string>();
            o.interest = r[4].get<std::string>();
            opportunities.push_back(o);
        }

        std::cout << "Opportunities loaded from Neo4j!" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (loadOpportunities): " << e.what() << std::endl;
    }
}


// ================= PASSWORDS =================
// Stored as a property on the User node (User must be saved first).

inline void savePasswordsToFile(std::unordered_map<int, std::string>& passwords) {
    try {
        json items = json::array();

        for (auto& entry : passwords) {
            items.push_back(json{{"id", entry.first}, {"password", entry.second}});
        }

        if (!items.empty()) {
            db.run(
                "UNWIND $items AS i "
                "MATCH (u:User {id: i.id}) "
                "SET u.password = i.password",
                json{{"items", items}}
            );
        }
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (savePasswords): " << e.what() << std::endl;
    }
}

inline void loadPasswordsFromFile(std::unordered_map<int, std::string>& passwords) {
    try {
        json rows = db.run(
            "MATCH (u:User) WHERE u.password IS NOT NULL "
            "RETURN u.id, u.password"
        );

        for (auto& r : rows) {
            passwords[r[0].get<int>()] = r[1].get<std::string>();
        }
    }
    catch (const std::exception& e) {
        std::cout << "Neo4j error (loadPasswords): " << e.what() << std::endl;
    }
}
