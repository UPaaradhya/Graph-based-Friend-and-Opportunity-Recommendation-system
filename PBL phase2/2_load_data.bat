@echo off
cd /d "%~dp0"
echo Make sure Neo4j Desktop shows RUNNING.
echo This DELETES old data and loads 100 users, 75 opportunities, friendships.
pause
load.exe "Deepak@123M" neo4j_100_users.cypher
echo.
echo You should see [100,75,212,20] above. Then run 3_run_app.bat
pause
