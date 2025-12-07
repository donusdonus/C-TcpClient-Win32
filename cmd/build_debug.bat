@echo off
del /f /s /q "../build/app.exe"
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build ../build --parallel 4