@echo off
cd /d "%~dp0"
echo Building the data loader...
g++ -std=c++17 load_dataset.cpp -o load.exe -lcurl
if errorlevel 1 goto fail
echo Building the application...
g++ -std=c++17 MainModule.cpp -o app.exe -lcurl
if errorlevel 1 goto fail
echo.
echo BUILD OK. Now run 2_load_data.bat (first time only), then 3_run_app.bat
pause
exit /b 0
:fail
echo.
echo BUILD FAILED. Copy the first 10 lines of the text above and send them.
echo (If it says Permission denied, close app.exe first and run again.)
pause
exit /b 1
