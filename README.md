# Friend and Opportunity Recommendation System

A simple C++ project that stores users, friendships, and opportunities in a Neo4j database.

## Project files

- `src/main.cpp` — starts the program, displays the menu, logs users in, and shows the dashboard.
- `src/Models.h` — contains the `User` and `Opportunity` structures.
- `src/UserService.h` and `src/UserService.cpp` — creates accounts, logs users in, and reads user details.
- `src/GraphManager.h` and `src/GraphManager.cpp` — adds friendships and finds current or mutual friends.
- `src/RecommendationService.h` and `src/RecommendationService.cpp` — calculates recommendation scores and lets the user select a result.
- `src/OpportunityService.h` and `src/OpportunityService.cpp` — adds and reads opportunities.
- `src/Neo4jDatabase.h` and `src/Neo4jDatabase.cpp` — sends database requests to Neo4j.

The C++ code uses beginner-friendly loops and `if` statements for the recommendation logic. The recommendation lists are sorted with a basic bubble sort rather than a sorting function or lambda.

## What the program does

1. Connects to Neo4j.
2. Lets a user create an account. User IDs start at 100 and increase automatically.
3. Lets a user log in with their ID and password.
4. Displays their course, skills, interests, and current friends.
5. Asks whether they want friend recommendations and opportunity recommendations.
6. Shows recommendation match percentages and lets the user choose a friend or opportunity.
7. Lets a user add an opportunity.

Friend recommendations use shared skills, shared interests, and mutual friends. Opportunity recommendations use matching skills and interest areas.

## Requirements

- C++17 compiler
- CMake 3.16 or later
- libcurl development files
- Boost headers (Property Tree)
- Neo4j 4.x or 5.x with the HTTP transactional endpoint enabled

The HTTP and JSON handling in `Neo4jDatabase.cpp` is required for the C++ program to communicate with Neo4j. It is more technical than the recommendation logic, but it is kept in one separate file.

## Set up Neo4j

1. Start your local Neo4j database.
2. Copy `config/neo4j.conf.example` to `config/neo4j.conf`.
3. Open `config/neo4j.conf` and enter your database password.
4. Keep `config/neo4j.conf` private because it contains your password.

The example uses this local endpoint:

`http://localhost:7474/db/neo4j/tx/commit`

The database name is `neo4j` by default.

## Build

Run these commands from the project folder:

```bash
cmake -S . -B build
cmake --build build
```

Then run the generated program from the project folder so it can find `config/neo4j.conf`:

```bash
./build/friend_opportunity
```

On Windows, run the executable created inside the `build` folder. Your CMake setup must be able to find libcurl and Boost headers.

## Important notes

- A successful compile checks the C++ code, but you must run Neo4j and enter the correct credentials to test database features.
- This is a student-project prototype. Passwords are stored as plain text in Neo4j. Do not use a real password or a password you use elsewhere.
- Names, course names, skills, interests, and opportunity names are currently entered as one word. Use underscores if a value needs spaces.
