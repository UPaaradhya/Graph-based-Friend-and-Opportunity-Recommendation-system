// =====================================================================
// FRIENDSHIPS for the 1000 users  (run AFTER the users file)
//  - every user gets 1 or 2 friends (people with a similar profile)
//  - some users get more friends (they become "popular" users)
// Run with the loader:   .\load.exe YOUR_PASSWORD neo4j_add_friendships.cypher
// =====================================================================

// ---------- BLOCK 1: every user gets 1-2 friends (similar-profile chain) ----------
MATCH (a:User)
MATCH (b:User {id: a.id + 6})
MERGE (a)-[:FRIEND]-(b);

// ---------- BLOCK 2: 20 popular users get more friends ----------
MATCH (a:User)
WHERE a.id % 50 = 0
UNWIND [12, 18, 24, 1, 2, 3] AS d
MATCH (b:User {id: a.id + d})
MERGE (a)-[:FRIEND]-(b);

// ---------- BLOCK 3: 5 very popular users get even more friends ----------
MATCH (a:User)
WHERE a.id % 200 = 0
UNWIND [42, 48, 54, 60, 66, 72, 78, 84, 90, 96] AS d
MATCH (b:User {id: a.id + d})
MERGE (a)-[:FRIEND]-(b);

// ---------- BLOCK 4: check - how many users have how many friends ----------
MATCH (u:User)
OPTIONAL MATCH (u)-[:FRIEND]-(f:User)
WITH u, count(f) AS friends
RETURN friends, count(u) AS users
ORDER BY friends;
