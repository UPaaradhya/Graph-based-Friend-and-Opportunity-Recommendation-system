const http = require("http");

// ====== SETTINGS ======
const NEO4J_URL = "http://127.0.0.1:7474/db/neo4j/tx/commit";
const NEO4J_USER = "neo4j";
// Neo4j password: wahi jo aapke backend ki neo4j_storage.h mein hai
const NEO4J_PASSWORD = process.env.NEO4J_PASSWORD || "Deepak@123M";
const PORT = 8080;
// ======================

// ====== Courses / Skills / Interests (C++ project wali same list) ======
const CATALOG = {
  courses: [
    "B.Tech CSE AI ML", "B.Sc Data Science", "M.Sc Data Science",
    "BBA Business Analytics", "BCA", "B.Tech IT",
    "MCA", "B.Sc Computer Science", "B.Tech Cyber Security",
    "B.Tech CSE", "M.Tech CSE", "B.Tech Electronics and Communication",
    "BA Journalism and Mass Communication", "B.Des Graphic Design", "BBA Marketing",
    "B.Com", "BA English Literature",
  ],
  skills: [
    "Python", "Machine_Learning", "Deep_Learning", "Data_Science",
    "Data_Analysis", "SQL", "Statistics", "Power_BI",
    "HTML_CSS", "JavaScript", "React", "NodeJS",
    "Web_Development", "MongoDB", "Django", "UI_UX_Design",
    "Cyber_Security", "Networking", "Linux", "Cryptography",
    "Penetration_Testing", "Digital_Forensics", "Android_Development", "Kotlin",
    "Flutter", "Java", "Swift", "Firebase",
    "Cloud_Computing", "AWS", "DevOps", "Docker",
    "Kubernetes", "Git", "C++", "C",
    "Data_Structures", "Algorithms", "Operating_Systems", "DBMS",
    "Communication", "Teamwork", "Leadership", "Problem_Solving",
    "Time_Management", "Critical_Thinking", "Graphic_Design", "Video_Editing",
    "Content_Writing", "Digital_Marketing", "Public_Speaking", "Social_Media",
  ],
  interests: [
    "Artificial_Intelligence", "Data_Analytics", "Research",
    "Robotics", "Startups", "Open_Source",
    "Design", "Freelancing", "Ethical_Hacking",
    "Capture_The_Flag", "Mobile_Apps", "Gaming",
    "Automation", "Cloud_Technologies", "Competitive_Programming",
    "Hackathons", "Music", "Photography",
    "Travel", "Reading", "Sports",
  ],
};

// ---------- Function to talk to Neo4j ----------
async function run(cypher, params = {}) {
  const res = await fetch(NEO4J_URL, {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
      Accept: "application/json",
      Authorization:
        "Basic " + Buffer.from(`${NEO4J_USER}:${NEO4J_PASSWORD}`).toString("base64"),
    },
    body: JSON.stringify({ statements: [{ statement: cypher, parameters: params }] }),
  });
  const reply = await res.json();
  if (reply.errors && reply.errors.length) throw new Error(reply.errors[0].message);
  return reply.results[0].data.map((d) => d.row);
}

// ---------- Data load karna ----------
async function loadAll() {
  const userRows = await run(
    "MATCH (u:User) RETURN u.id, u.name, u.course, u.skills, u.interests ORDER BY u.id"
  );
  const users = userRows.map((r) => ({
    id: r[0],
    name: r[1] || "",
    course: r[2] || "",
    skills: Array.isArray(r[3]) ? r[3] : [],
    interests: Array.isArray(r[4]) ? r[4] : [],
  }));
  const byId = new Map(users.map((u) => [u.id, u]));
  const friends = new Map(users.map((u) => [u.id, new Set()]));

  const friendRows = await run(
    "MATCH (a:User)-[:FRIEND]-(b:User) WHERE a.id < b.id RETURN a.id, b.id"
  );
  for (const [a, b] of friendRows) {
    if (friends.has(a) && friends.has(b)) {
      friends.get(a).add(b);
      friends.get(b).add(a);
    }
  }
  return { users, byId, friends };
}

async function loadOpportunities() {
  const rows = await run(
    "MATCH (o:Opportunity) RETURN o.id, o.name, o.type, o.skill, o.interest ORDER BY o.id"
  );
  return rows.map((r) => ({ id: r[0], name: r[1], type: r[2], skill: r[3], interest: r[4] }));
}

async function loadTaken(userId) {
  const rows = await run(
    "MATCH (u:User {id: $id})-[:TOOK]->(o:Opportunity) RETURN o.id",
    { id: userId }
  );
  return rows.map((r) => r[0]);
}

// ---------- Recommendation logic (C++ jaisi hi) ----------
function recommendFriends(userId, data) {
  const me = data.byId.get(userId);
  const myFriends = data.friends.get(userId);
  const maxScore = 2 * myFriends.size + me.skills.length + me.interests.length;
  const list = [];

  for (const c of data.users) {
    if (c.id === userId || myFriends.has(c.id)) continue;

    const theirFriends = data.friends.get(c.id);
    let mutual = 0;
    for (const f of myFriends) if (theirFriends.has(f)) mutual++;

    const commonSkills = c.skills.filter((s) => me.skills.includes(s));
    const commonInterests = c.interests.filter((i) => me.interests.includes(i));

    const score = mutual * 2 + commonSkills.length + commonInterests.length;
    if (score > 0) {
      list.push({
        id: c.id,
        name: c.name,
        course: c.course,
        mutualFriends: mutual,
        commonSkills,
        commonInterests,
        score,
        match: maxScore > 0 ? Math.min(100, Math.round((score * 100) / maxScore)) : 0,
      });
    }
  }
  list.sort((a, b) => b.score - a.score);
  return list.slice(0, 10);
}

function recommendOpportunities(user, opportunities, takenIds) {
  const list = [];
  for (const o of opportunities) {
    if (takenIds.includes(o.id)) continue;
    if (!user.skills.includes(o.skill)) continue; // skill match is required
    let score = 2;
    if (user.interests.includes(o.interest)) score += 2;
    list.push({ ...o, score, match: Math.min(100, Math.round((score * 100) / 4)) });
  }
  list.sort((a, b) => b.score - a.score);
  return list.slice(0, 10);
}

// ---------- Helper functions ----------
function send(res, status, body) {
  res.writeHead(status, {
    "Content-Type": "application/json",
    "Access-Control-Allow-Origin": "*",
    "Access-Control-Allow-Headers": "Content-Type",
    "Access-Control-Allow-Methods": "GET,POST,OPTIONS",
  });
  res.end(JSON.stringify(body));
}

function readBody(req) {
  return new Promise((resolve) => {
    let raw = "";
    req.on("data", (c) => (raw += c));
    req.on("end", () => {
      try { resolve(raw ? JSON.parse(raw) : {}); } catch { resolve({}); }
    });
  });
}

function friendList(userId, data) {
  return [...data.friends.get(userId)].map((id) => {
    const u = data.byId.get(id);
    return { id: u.id, name: u.name, course: u.course };
  });
}

// ---------- Server ----------
const server = http.createServer(async (req, res) => {
  if (req.method === "OPTIONS") return send(res, 200, {});

  const path = req.url.split("?")[0];

  try {
    // GET /api/catalog
    if (req.method === "GET" && path === "/api/catalog") {
      return send(res, 200, CATALOG);
    }

    // POST /api/register  (naya account)
    if (req.method === "POST" && path === "/api/register") {
      const { name, course, skills, interests, password } = await readBody(req);

      if (!name || !String(name).trim())
        return send(res, 400, { error: "Name is required" });
      if (!password || String(password).length < 4)
        return send(res, 400, { error: "Password must be at least 4 characters" });
      if (!CATALOG.courses.includes(course))
        return send(res, 400, { error: "Please choose a course" });

      const sk = (skills || []).filter((s) => CATALOG.skills.includes(s)).slice(0, 5);
      const it = (interests || []).filter((i) => CATALOG.interests.includes(i)).slice(0, 4);

      const rows = await run("MATCH (u:User) RETURN coalesce(max(u.id), 0)");
      const newId = rows[0][0] + 1;

      await run(
        "CREATE (u:User {id: $id, name: $name, course: $course, skills: $skills, interests: $interests, password: $password})",
        { id: newId, name: String(name).trim(), course, skills: sk, interests: it, password: String(password) }
      );
      return send(res, 200, { id: newId });
    }

    // POST /api/login
    if (req.method === "POST" && path === "/api/login") {
      const { id, password } = await readBody(req);
      const userId = Number(id);
      const rows = await run("MATCH (u:User {id: $id}) RETURN u.password", { id: userId });
      if (rows.length === 0) return send(res, 404, { error: "User not found" });

      const saved = rows[0][0];
      if (saved === null || saved === undefined) {
        // first login: password gets set (same as the C++ app)
        await run("MATCH (u:User {id: $id}) SET u.password = $p", { id: userId, p: String(password) });
      } else if (saved !== String(password)) {
        return send(res, 401, { error: "Incorrect password" });
      }
      return send(res, 200, { id: userId });
    }

    // /api/users/:id/...
    const m = path.match(/^\/api\/users\/(\d+)(?:\/([a-z-]+))?$/);
    if (m) {
      const userId = Number(m[1]);
      const part = m[2];
      const data = await loadAll();
      const user = data.byId.get(userId);
      if (!user) return send(res, 404, { error: "User not found" });

      // GET /api/users/1  -> profile + friends
      if (req.method === "GET" && !part) {
        return send(res, 200, { ...user, friends: friendList(userId, data) });
      }

      // GET /api/users/1/friends
      if (req.method === "GET" && part === "friends") {
        return send(res, 200, friendList(userId, data));
      }

      // GET /api/users/1/friend-recommendations
      if (req.method === "GET" && part === "friend-recommendations") {
        return send(res, 200, recommendFriends(userId, data));
      }

      // POST /api/users/1/friends  { friendId }
      if (req.method === "POST" && part === "friends") {
        const { friendId } = await readBody(req);
        const fid = Number(friendId);
        if (!data.byId.has(fid)) return send(res, 404, { error: "Friend not found" });
        if (fid === userId) return send(res, 400, { error: "Cannot add yourself" });
        if (data.friends.get(userId).has(fid))
          return send(res, 400, { error: "Already friends" });
        await run(
          "MATCH (a:User {id: $a}), (b:User {id: $b}) MERGE (a)-[:FRIEND]-(b)",
          { a: userId, b: fid }
        );
        return send(res, 200, { message: "Friend added" });
      }

      // GET /api/users/1/opportunities
      if (req.method === "GET" && part === "opportunities") {
        const opportunities = await loadOpportunities();
        const takenIds = await loadTaken(userId);
        return send(res, 200, {
          recommendations: recommendOpportunities(user, opportunities, takenIds),
          taken: opportunities.filter((o) => takenIds.includes(o.id)),
        });
      }

      // POST /api/users/1/opportunities  { opportunityId }
      if (req.method === "POST" && part === "opportunities") {
        const { opportunityId } = await readBody(req);
        await run(
          "MATCH (u:User {id: $u}), (o:Opportunity {id: $o}) MERGE (u)-[:TOOK]->(o)",
          { u: userId, o: Number(opportunityId) }
        );
        return send(res, 200, { message: "Opportunity added" });
      }
    }

    send(res, 404, { error: "Route not found" });
  } catch (e) {
    console.error(e.message);
    send(res, 500, { error: e.message });
  }
});

server.listen(PORT, () => {
  console.log(`API server running at http://127.0.0.1:${PORT}`);
});