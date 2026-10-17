// load_dataset.cpp
// Reads a .cypher file and sends each statement (split by ';') to Neo4j, one by one.
//
// Compile:  g++ -std=c++17 load_dataset.cpp -o load.exe -lcurl
// Run:      .\load.exe YOUR_NEO4J_PASSWORD neo4j_users_only_1000_RUN_ALL.cypher

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "neo4j_client.h"

using namespace std;

static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Usage: load.exe NEO4J_PASSWORD file.cypher\n";
        return 1;
    }

    Neo4jClient client("http://127.0.0.1:7474", "neo4j", "neo4j", argv[1]);

    ifstream file(argv[2]);
    if (!file) {
        cout << "Cannot open file: " << argv[2] << "\n";
        return 1;
    }

    // Read file, dropping comment lines that start with //
    string line, text;
    while (getline(file, line)) {
        if (trim(line).rfind("//", 0) == 0) continue;
        text += line + "\n";
    }

    // Split into statements at ';'
    vector<string> statements;
    string current;
    for (char c : text) {
        if (c == ';') {
            string st = trim(current);
            if (!st.empty()) statements.push_back(st);
            current.clear();
        } else {
            current += c;
        }
    }
    string last = trim(current);
    if (!last.empty()) statements.push_back(last);

    cout << "Found " << statements.size() << " statements.\n";

    for (size_t i = 0; i < statements.size(); i++) {
        cout << "Running statement " << i + 1 << " of " << statements.size() << "... " << flush;
        try {
            json rows = client.run(statements[i]);
            cout << "OK";
            // Show small results (the check query and the sample users)
            if (!rows.empty() && rows.size() <= 10) {
                cout << "\n";
                for (auto& r : rows) cout << "   " << r.dump() << "\n";
            } else {
                cout << "\n";
            }
        } catch (const exception& e) {
            cout << "FAILED\n   " << e.what() << "\n";
            return 1;
        }
    }

    cout << "\nDone! All statements finished.\n";
    return 0;
}
