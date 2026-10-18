# Connectly Frontend

The React 19 + Vite 7 frontend application for the Friend Connection and Opportunity Recommendation System.

## File Structure

| File | Purpose |
| :--- | :--- |
| `package.json` | Lists React 19, React-DOM, and Vite development dependencies. |
| `vite.config.js` | Configures the local development server (port 5173) and `/api` proxy to the backend (port 8080). |
| `index.html` | HTML template mounting the React root. |
| `src/main.jsx` | React entry point attaching `App.jsx` to DOM root `#root`. |
| `src/App.jsx` | Complete single-page dashboard containing Auth, Profile, Friend Recommendations, Opportunity Recommendations, Network Analytics, and Directory Tree. |
| `src/styles.css` | Dark navy modern theme matching `#0b142b`. |
| `src/api/client.js` | REST API client communicating with backend endpoints. |
| `src/api/mockApi.js` | Standalone mock data store for visual previews without starting the backend. |
| `src/api/index.js` | Selects between live API and mock preview based on `VITE_DATA_MODE`. |

## Development Setup

```powershell
# 1. Install dependencies
npm install

# 2. Run development server
npm run dev
```

Open your browser at `http://localhost:5173`.
To change between Live Backend API and Offline Mock Preview, set `VITE_DATA_MODE=api` or `VITE_DATA_MODE=mock` in `.env`.
