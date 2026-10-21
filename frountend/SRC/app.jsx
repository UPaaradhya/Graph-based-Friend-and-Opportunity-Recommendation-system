import React, { useEffect, useState } from "react";

async function api(path, options) {
  const res = await fetch("/api" + path, {
    headers: { "Content-Type": "application/json" },
    ...options,
  });
  const data = await res.json().catch(() => ({}));
  if (!res.ok) throw new Error(data.error || `Error ${res.status}`);
  return data;
}

const pretty = (s) => String(s).replace(/_/g, " ");

/* ---------------- Tags ---------------- */
function Tags({ items }) {
  return (
    <div className="tags">
      {items.map((t) => (
        <span className="tag" key={t}>{pretty(t)}</span>
      ))}
    </div>
  );
}

/* ---------------- Chip picker (skills / interests) ---------------- */
function ChipPicker({ title, items, selected, max, onChange }) {
  function toggle(item) {
    if (selected.includes(item)) {
      onChange(selected.filter((x) => x !== item));
    } else if (selected.length < max) {
      onChange([...selected, item]);
    }
  }
  return (
    <div className="picker">
      <p className="picker-title">
        {title} <span className="count">({selected.length}/{max})</span>
      </p>
      <div className="chips">
        {items.map((item) => (
          <button
            type="button"
            key={item}
            className={"chip" + (selected.includes(item) ? " on" : "")}
            onClick={() => toggle(item)}
          >
            {pretty(item)}
          </button>
        ))}
      </div>
    </div>
  );
}

/* ---------------- Login ---------------- */
function Login({ onLogin, goRegister, prefillId }) {
  const [id, setId] = useState(prefillId || "");
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");

  async function submit() {
    setError("");
    try {
      const r = await api("/login", {
        method: "POST",
        body: JSON.stringify({ id, password }),
      });
      onLogin(r.id);
    } catch (e) {
      setError(e.message);
    }
  }

  return (
    <div className="box">
      <h2>Welcome back 👋</h2>
      <input placeholder="User ID" value={id} onChange={(e) => setId(e.target.value)} />
      <input
        type="password"
        placeholder="Password"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
        onKeyDown={(e) => e.key === "Enter" && submit()}
      />
      <button onClick={submit}>Login</button>
      {error && <p className="error">{error}</p>}
      <p className="switch">
        New user? <a onClick={goRegister}>Create account</a>
      </p>
    </div>
  );
}

/* ---------------- Create Account ---------------- */
function Register({ goLogin, onCreated }) {
  const [catalog, setCatalog] = useState(null);
  const [name, setName] = useState("");
  const [course, setCourse] = useState("");
  const [skills, setSkills] = useState([]);
  const [interests, setInterests] = useState([]);
  const [password, setPassword] = useState("");
  const [error, setError] = useState("");
  const [newId, setNewId] = useState(null);

  useEffect(() => {
    api("/catalog")
      .then((c) => {
        setCatalog(c);
        setCourse(c.courses[0]);
      })
      .catch((e) => setError(e.message));
  }, []);

  async function submit() {
    setError("");
    try {
      const r = await api("/register", {
        method: "POST",
        body: JSON.stringify({ name, course, skills, interests, password }),
      });
      setNewId(r.id);
    } catch (e) {
      setError(e.message);
    }
  }

  if (newId) {
    return (
      <div className="box wide center">
        <h2>🎉 Account created!</h2>
        <p>Your User ID:</p>
        <div className="big-id">{newId}</div>
        <p className="note">Remember this ID. You need it to log in.</p>
        <button onClick={() => onCreated(String(newId))}>Go to login</button>
      </div>
    );
  }

  if (!catalog) {
    return (
      <div className="box wide">
        <p>Loading...</p>
        {error && <p className="error">{error}</p>}
      </div>
    );
  }

  return (
    <div className="box wide">
      <h2>Create Account ✨</h2>

      <label>Name</label>
      <input placeholder="Your full name" value={name} onChange={(e) => setName(e.target.value)} />

      <label>Course</label>
      <select value={course} onChange={(e) => setCourse(e.target.value)}>
        {catalog.courses.map((c) => (
          <option key={c} value={c}>{c}</option>
        ))}
      </select>

      <ChipPicker title="Choose your skills" items={catalog.skills} selected={skills} max={5} onChange={setSkills} />
      <ChipPicker title="Choose your interests" items={catalog.interests} selected={interests} max={4} onChange={setInterests} />

      <label>Password</label>
      <input
        type="password"
        placeholder="At least 4 characters"
        value={password}
        onChange={(e) => setPassword(e.target.value)}
      />

      <button onClick={submit}>Create account</button>
      {error && <p className="error">{error}</p>}
      <p className="switch">
        Already have an account? <a onClick={goLogin}>Login</a>
      </p>
    </div>
  );
}

/* ---------------- Dashboard ---------------- */
function Dashboard({ userId, onLogout }) {
  const [tab, setTab] = useState("profile");
  const [profile, setProfile] = useState(null);
  const [friendRecs, setFriendRecs] = useState([]);
  const [oppData, setOppData] = useState({ recommendations: [], taken: [] });
  const [msg, setMsg] = useState("");
  const [error, setError] = useState("");

  async function loadAll() {
    try {
      setError("");
      setProfile(await api(`/users/${userId}`));
      setFriendRecs(await api(`/users/${userId}/friend-recommendations`));
      setOppData(await api(`/users/${userId}/opportunities`));
    } catch (e) {
      setError(e.message);
    }
  }

  useEffect(() => { loadAll(); }, [userId]);

  async function addFriend(friendId) {
    try {
      await api(`/users/${userId}/friends`, {
        method: "POST",
        body: JSON.stringify({ friendId }),
      });
      setMsg("Friend added! 🎉");
      loadAll();
    } catch (e) { setError(e.message); }
  }

  async function takeOpportunity(opportunityId) {
    try {
      await api(`/users/${userId}/opportunities`, {
        method: "POST",
        body: JSON.stringify({ opportunityId }),
      });
      setMsg("Opportunity added! 🚀");
      loadAll();
    } catch (e) { setError(e.message); }
  }

  if (!profile) {
    return <p>Loading... {error && <span className="error">{error}</span>}</p>;
  }

  return (
    <div>
      {/* Profile header */}
      <div className="profile-head">
        <div className="avatar">{profile.name.charAt(0).toUpperCase()}</div>
        <div className="who">
          <h2>{profile.name}</h2>
          <p>ID: {profile.id} &nbsp;|&nbsp; {profile.course}</p>
        </div>
        <button className="gray" onClick={onLogout}>Logout</button>
      </div>

      {/* Stats */}
      <div className="stats">
        <div className="stat"><b>{profile.friends.length}</b><span>Friends</span></div>
        <div className="stat"><b>{friendRecs.length}</b><span>Suggestions</span></div>
        <div className="stat"><b>{oppData.recommendations.length}</b><span>Opportunities</span></div>
        <div className="stat"><b>{oppData.taken.length}</b><span>Received</span></div>
      </div>

      {/* Tabs */}
      <div className="tabs">
        <button className={tab === "profile" ? "active" : ""} onClick={() => setTab("profile")}>👤 Profile</button>
        <button className={tab === "friends" ? "active" : ""} onClick={() => setTab("friends")}>🤝 Friend Suggestions</button>
        <button className={tab === "opps" ? "active" : ""} onClick={() => setTab("opps")}>🚀 Opportunities</button>
      </div>

      {msg && <p className="ok">{msg}</p>}
      {error && <p className="error">{error}</p>}

      {tab === "profile" && (
        <div>
          <div className="card">
            <p className="label">Skills</p>
            <Tags items={profile.skills} />
            <p className="label">Interests</p>
            <Tags items={profile.interests} />
          </div>
          <h3>Your Friends ({profile.friends.length})</h3>
          <div className="grid">
            {profile.friends.length === 0 && <p className="empty">No friends yet. Check the Friend Suggestions tab!</p>}
            {profile.friends.map((f) => (
              <div className="card" key={f.id}>
                <b>{f.name}</b>
                <p>ID: {f.id}</p>
                <p>{f.course}</p>
              </div>
            ))}
          </div>
        </div>
      )}

      {tab === "friends" && (
        <div className="grid">
          {friendRecs.length === 0 && <p className="empty">No suggestions found.</p>}
          {friendRecs.map((f) => (
            <div className="card" key={f.id}>
              <div className="row">
                <b>{f.name}</b>
                <span className="match">{f.match}% match</span>
              </div>
              <p>ID: {f.id} | {f.course}</p>
              <p>Mutual friends: {f.mutualFriends}</p>
              <Tags items={[...f.commonSkills, ...f.commonInterests]} />
              <button onClick={() => addFriend(f.id)}>Add Friend</button>
            </div>
          ))}
        </div>
      )}

      {tab === "opps" && (
        <div>
          <div className="grid">
            {oppData.recommendations.length === 0 && <p className="empty">No opportunities found.</p>}
            {oppData.recommendations.map((o) => (
              <div className="card" key={o.id}>
                <div className="row">
                  <b>{pretty(o.name)}</b>
                  <span className="match">{o.match}% match</span>
                </div>
                <p>Type: {o.type}</p>
                <p>Skill: {pretty(o.skill)}</p>
                <p>Interest: {pretty(o.interest)}</p>
                <button onClick={() => takeOpportunity(o.id)}>Choose</button>
              </div>
            ))}
          </div>
          <h3>Already received ({oppData.taken.length})</h3>
          <div className="grid">
            {oppData.taken.map((o) => (
              <div className="card" key={o.id}>
                <b>{pretty(o.name)}</b>
                <p>{o.type}</p>
              </div>
            ))}
          </div>
        </div>
      )}
    </div>
  );
}

/* ---------------- Main App ---------------- */
export default function App() {
  const [userId, setUserId] = useState(null);
  const [view, setView] = useState("login");
  const [prefillId, setPrefillId] = useState("");

  return (
    <div className="container">
      <div className="hero">
        <h1>Graph Based Friend and Opportunity Recommendation System</h1>
        <p className="motto">Connect • Collaborate • Grow</p>
        <p className="sub">Every connection is a path to your next opportunity.</p>
      </div>

      {userId ? (
        <Dashboard userId={userId} onLogout={() => { setUserId(null); setView("login"); }} />
      ) : view === "login" ? (
        <Login
          onLogin={setUserId}
          goRegister={() => setView("register")}
          prefillId={prefillId}
        />
      ) : (
        <Register
          goLogin={() => setView("login")}
          onCreated={(id) => { setPrefillId(id); setView("login"); }}
        />
      )}
    </div>
  );
}