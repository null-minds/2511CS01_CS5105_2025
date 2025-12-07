#!/bin/bash
# Quick start script for Docker

echo "🚀 Starting Seating Arrangement System with Docker..."
echo ""

# Check if Docker is installed
if ! command -v docker &> /dev/null; then
    echo "❌ Docker is not installed. Please install Docker first."
    exit 1
fi

# Check if Docker Compose is installed
if ! command -v docker-compose &> /dev/null && ! docker compose version &> /dev/null; then
    echo "❌ Docker Compose is not installed. Please install Docker Compose first."
    exit 1
fi

# Build and run
echo "📦 Building and starting containers..."
if docker compose version &> /dev/null; then
    docker compose up --build
else
    docker-compose up --build
fi

