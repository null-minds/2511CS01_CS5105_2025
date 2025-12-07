# Seating Arrangement System

This system generates seating arrangements for exams based on timetable and room capacity.

## 🚀 Quick Start

### Option 1: Web Interface (Streamlit) - Recommended

**Using Docker (Easiest):**
```bash
docker-compose up --build
```
Then open `http://localhost:8501` in your browser.

**Or run locally:**
```bash
pip install -r requirements.txt
streamlit run streamlit_app.py
```

### Option 2: Command Line

Install the required packages:
```bash
pip install -r requirements.txt
```

Run the script from the command line:

```bash
python seating_arrangement.py --buffer <buffer_value> --mode <Sparse|Dense> [--input <input_file>] [--output <output_dir>]
```

### Parameters

- `--buffer` (required): Buffer value for capacity reduction. If buffer is 5 and room capacity is 50, only 45 students will be allocated.
- `--mode` (required): Allocation mode - either `Sparse` or `Dense`
  - **Sparse**: Allocates 50% of effective capacity per subject
  - **Dense**: Allocates 100% of effective capacity per subject
- `--input` (optional): Path to input Excel file (default: `input_data_tt.xlsx`)
- `--output` (optional): Output directory (default: `output`)

### Example

```bash
python seating_arrangement.py --buffer 5 --mode Dense
```

This will:
- Use a buffer of 5 seats per room
- Use Dense mode (fill rooms to effective capacity)
- Read from `input_data_tt.xlsx`
- Generate output in the `output` directory

## Input File Structure

The input Excel file should contain the following sheets:

1. **in_timetable**: Contains exam schedule with Date, Day, Morning, and Evening columns
2. **in_course_roll_mapping**: Maps courses to student roll numbers
3. **in_roll_name_mapping**: Maps roll numbers to student names
4. **in_room_capacity**: Contains room numbers and their capacities

## Output Files

The system generates:

1. **Individual room files**: `dd_mm_yyyy_subcode_classroom_session.xlsx` in `output/<date>/<session>/` folders
2. **op_overall_seating_arrangement.xlsx**: Overall allocation summary
3. **op_seats_left.xlsx**: Summary of allocated and vacant seats per room
4. **errors.txt**: Any errors or clashes detected during processing
5. **seating_arrangement.log**: Detailed execution log

## Features

- **Clash Detection**: Automatically detects and reports student clashes between courses in the same slot
- **Optimized Allocation**: 
  - Allocates largest courses first
  - Prefers same building (B1/B2) for a course
  - Prefers adjacent rooms for better movement
  - Minimizes number of rooms used per course
- **Error Handling**: Comprehensive error handling with logging
- **Unknown Names**: Students without name mapping are assigned "Unknown Name"

## 🌐 Web Interface Features (Streamlit)

The Streamlit web interface provides:
- **Easy file upload**: Drag and drop your Excel file
- **Interactive configuration**: Set buffer and mode with sliders
- **Real-time progress**: See processing status with progress indicators
- **Results viewer**: Browse and download generated files directly from the browser
- **Summary statistics**: View allocation metrics at a glance

## 🐳 Docker Support

The project includes Docker support for easy deployment:

- **Dockerfile**: Containerizes the application
- **docker-compose.yml**: Easy orchestration with volume mounts
- **Health checks**: Automatic container monitoring

See [README_DOCKER.md](README_DOCKER.md) for detailed Docker instructions.

## Notes

- The system checks for clashes and reports them to the terminal and errors.txt
- If there are more students than total capacity, an error message is displayed
- All input data is automatically stripped of extra spaces
- The web interface stores results in session state for easy access

