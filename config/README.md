# Connectly frontend

This is the complete React/Vite frontend for the Friend Connection and Opportunity Recommendation System.

## Files

| File | What it does |
| --- | --- |
| `package.json` | Lists React/Vite dependencies and `dev`, `build`, and `preview` scripts. |
| `vite.config.js` | Starts Vite on port 5173 and proxies `/api` to the C++ API on port 8080. |
| `.env.example` | Shows how to select `mock` or `api` data mode. |
| `index.html` | Minimal Vite HTML document. |
| `src/main.jsx` | Mounts React in the browser. |
| `src/App.jsx` | All pages, forms, navigation, UI states, and reusable page components. |
| `src/styles.css` | Responsive, mobile-friendly visual design. |
| `src/api/client.js` | Real API requests to `src/main_api.cpp`. |
| `src/api/mockApi.js` | Separate sample data for a safe visual preview. |
| `src/api/index.js` | Picks mock or API mode based on `VITE_DATA_MODE`. |

## Start in preview mode

```powershell
pnpm install
Copy-Item .env.example .env
pnpm dev
```

Leave `VITE_DATA_MODE=mock` in `.env`, then log in with:

```text
Email: demo@connectly.local
Password: demo1234
```

## Start with the C++ and Neo4j backend

1. First start `friend_opportunity_api.exe` from the repository root.
2. In this folder, create `.env` from `.env.example`.
3. Change its value to `VITE_DATA_MODE=api`.
4. Run `pnpm dev` and open the Vite URL.

The frontend does not talk directly to Neo4j or execute C++ itself. It calls the local C++ API, which calls the existing C++ services and Neo4j database.
