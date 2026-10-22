@echo off
g++ -std=c++17 -Wall -Wextra -Iinclude main.cpp glad.c -o nave.exe -LC:/msys64/ucrt64/lib -lglfw3 -lopengl32 -lgdi32
if errorlevel 1 exit /b %errorlevel%
echo Compilacion correcta: nave.exe
