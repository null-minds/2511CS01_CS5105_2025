# Seating Arrangement System - Docker & Streamlit

This project now includes a web-based interface using Streamlit and can be run using Docker.

## Quick Start with Docker

### Option 1: Using Docker Compose (Recommended)

1. **Build and run the application:**
   ```bash
   docker-compose up --build
   ```

2. **Access the application:**
   - Open your browser and go to: `http://localhost:8501`

3. **Stop the application:**
   ```bash
   docker-compose down
   ```

### Option 2: Using Docker directly

1. **Build the Docker image:**
   ```bash
   docker build -t seating-arrangement .
   ```

2. **Run the container:**
   ```bash
   docker run -p 8501:8501 -v $(pwd)/output:/app/output seating-arrangement
   ```

3. **Access the application:**
   - Open your browser and go to: `http://localhost:8501`

## Running without Docker (Local Development)

### Prerequisites
- Python 3.8 or higher
- pip

### Installation

1. **Install dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

2. **Run Streamlit app:**
   ```bash
   streamlit run streamlit_app.py
   ```

3. **Access the application:**
   - Open your browser and go to: `http://localhost:8501`

## Features

### Web Interface (Streamlit)
- **Upload & Run Tab**: Upload Excel file, configure settings, and generate seating arrangements
- **View Results Tab**: Browse and download generated output files
- **About Tab**: Documentation and usage information

### Docker Features
- Containerized application for easy deployment
- Volume mounting for output files
- Health checks for container monitoring
- Automatic restart on failure

## File Structure

```
virtual/Intern Proj/
├── seating_arrangement.py    # Core logic
├── streamlit_app.py          # Streamlit web interface
├── Dockerfile                # Docker image definition
├── docker-compose.yml        # Docker Compose configuration
├── requirements.txt          # Python dependencies
├── .dockerignore            # Files to exclude from Docker build
├── input_data_tt.xlsx       # Input file (example)
└── output/                  # Generated output files
```

## Configuration

### Environment Variables
- `PYTHONUNBUFFERED=1`: Ensures Python output is not buffered

### Port Configuration
- Default port: `8501` (Streamlit default)
- Can be changed in `docker-compose.yml` or Docker run command

### Volume Mounts
- `./output:/app/output`: Maps local output directory to container
- `./input_data_tt.xlsx:/app/input_data_tt.xlsx:ro`: Mounts input file as read-only

## Usage

1. **Start the application** (using Docker Compose or directly)
2. **Open the web interface** at `http://localhost:8501`
3. **Upload your input Excel file** in the sidebar
4. **Configure settings:**
   - Buffer: Number of seats to leave empty per room
   - Mode: Dense (full capacity) or Sparse (50% capacity)
   - Output Directory: Where to save generated files
5. **Click "Generate Seating Arrangement"**
6. **View and download results** from the "View Results" tab

## Troubleshooting

### Port Already in Use
If port 8501 is already in use, change it in `docker-compose.yml`:
```yaml
ports:
  - "8502:8501"  # Use port 8502 instead
```

### Permission Issues
If you encounter permission issues with output directory:
```bash
chmod -R 777 output/
```

### Container Won't Start
Check logs:
```bash
docker-compose logs
```

## Production Deployment

For production deployment:

1. **Use environment variables** for sensitive configuration
2. **Set up reverse proxy** (nginx, Traefik) for HTTPS
3. **Use Docker secrets** for sensitive data
4. **Set resource limits** in docker-compose.yml:
   ```yaml
   deploy:
     resources:
       limits:
         cpus: '2'
         memory: 2G
   ```

## Support

For issues or questions, please check the main README.md file.

