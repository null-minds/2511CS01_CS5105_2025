@echo off
REM Quick start script for Docker on Windows

echo 🚀 Starting Seating Arrangement System with Docker...
echo.

REM Check if Docker is installed
docker --version >nul 2>&1
if errorlevel 1 (
    echo ❌ Docker is not installed. Please install Docker first.
    exit /b 1
)

REM Build and run
echo 📦 Building and starting containers...
docker-compose up --build

pause

