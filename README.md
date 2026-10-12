# Friend and Opportunity Recommendation System

A beginner-friendly C++17 console project using Neo4j for users, friendships, and opportunities.

## Features
- Create account with automatically assigned user ID
- Login using user ID and password
- Dashboard showing course, skills, interests, and current friends
- Friend recommendations based on shared skills, interests, and mutual friends
- Opportunity recommendations based on matching skills and interest areas
- Match percentages and user selection
- Separate source/header files for future frontend integration

## Requirements
- Windows 10/11
- MinGW-w64 g++ with C++17 support
- Neo4j 4.x/5.x with the HTTP transactional endpoint enabled

**No CMake, libcurl, Boost installation, or third-party C++ package is required.** The project uses the Windows built-in WinHTTP API and includes a small JSON reader in `src/boost/property_tree/` so you do not have to install Boost. This connection implementation is Windows-specific.

## Setup
1. Start your Neo4j database.
2. Copy `config/neo4j.conf.example` to `config/neo4j.conf`.
3. Put your local Neo4j URL, username, and password in `config/neo4j.conf`. Keep this file private; it is ignored by Git.
4. Open this folder in VS Code.
5. Run **Terminal → Run Build Task** (or press `Ctrl+Shift+B`).
6. Run `./friend_opportunity.exe` from the project root.

The default endpoint is `http://localhost:7474/db/neo4j/tx/commit`. Ensure your Neo4j version has that HTTP endpoint enabled.

## Manual build (PowerShell)
```powershell
g++ -std=c++17 -Isrc src/main.cpp src/Neo4jDatabase.cpp src/UserService.cpp src/GraphManager.cpp src/RecommendationService.cpp src/OpportunityService.cpp -o friend_opportunity.exe -lwinhttp
.\friend_opportunity.exe
```

## GitHub
The folder is structured to be committed directly. Before committing, confirm `config/neo4j.conf` is not staged. Only commit `config/neo4j.conf.example`, source files, `.vscode/tasks.json`, `.gitignore`, and this README.

## Security note
This is a student-project prototype. Passwords are currently stored as plain text in Neo4j. Use a project-only test password, not a personal password. Do not publish real credentials.
