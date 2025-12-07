# 📋 Step-by-Step Setup and Run Guide

## Prerequisites

1. **Python 3.8 or higher** - Download from [python.org](https://www.python.org/downloads/)
2. **pip** (usually comes with Python)
3. **Git** (optional, if cloning from repository)

---

## Step 1: Get the Project Files

### Option A: If you already have the files
- Navigate to the project folder: `virtual/Intern Proj`

### Option B: If you need to download/clone
1. Download or clone the repository
2. Navigate to the project directory:
   ```bash
   cd virtual/Intern Proj
   ```

---

## Step 2: Verify Project Structure

Make sure you have these files in the project folder:
- ✅ `streamlit_app.py` - Main Streamlit application
- ✅ `seating_arrangement.py` - Core logic
- ✅ `requirements.txt` - Python dependencies
- ✅ `input_data_tt.xlsx` - Input Excel file (or prepare your own)
- ✅ `photos/` folder (create if it doesn't exist) - For student photos

---

## Step 3: Create Photos Folder (Optional but Recommended)

1. Create a folder named `photos` in the project directory:
   ```bash
   mkdir photos
   ```

2. Add student photos to this folder:
   - Format: `ROLL.jpg` (e.g., `1401ZZ17.jpg`, `1401ZZ18.jpg`)
   - Supported formats: `.jpg`, `.jpeg`, `.png`
   - If a photo is missing, the system will show "No Image Available" placeholder

---

## Step 4: Install Python Dependencies

### On Windows (PowerShell/Command Prompt):
```bash
cd "virtual\Intern Proj"
python -m pip install -r requirements.txt
```

### On Mac/Linux:
```bash
cd virtual/Intern Proj
pip3 install -r requirements.txt
```

**What gets installed:**
- pandas - For data processing
- openpyxl - For Excel file handling
- numpy - For numerical operations
- streamlit - For web interface
- reportlab - For PDF generation
- Pillow - For image processing

**Expected output:** All packages should install successfully without errors.

---

## Step 5: Prepare Input Excel File

Your input Excel file (`input_data_tt.xlsx`) must have these sheets:

### Sheet 1: `in_timetable`
| Date | Day | Morning | Evening |
|------|-----|---------|---------|
| 2025-10-31 | Monday | CE1101;CS1101 | EE5206 |
| 2025-11-01 | Tuesday | MM2101 | CS1104 |

### Sheet 2: `in_course_roll_mapping`
| course_code | rollno |
|-------------|--------|
| CE1101 | 1401ZZ17 |
| CE1101 | 1401ZZ18 |

### Sheet 3: `in_roll_name_mapping`
| Roll | Name |
|------|------|
| 1401ZZ17 | Kanishka Singh Sola |
| 1401ZZ18 | Student Name |

### Sheet 4: `in_room_capacity`
| Room No. | Exam Capacity |
|----------|---------------|
| 104 | 50 |
| 105 | 60 |
| R106 | 45 |

---

## Step 6: Run the Project

### Method 1: Using Streamlit (Web Interface) - **RECOMMENDED**

1. **Open Terminal/Command Prompt** in the project directory

2. **Run the Streamlit app:**
   ```bash
   streamlit run streamlit_app.py
   ```

3. **The app will start and show:**
   ```
   You can now view your Streamlit app in your browser.
   
   Local URL: http://localhost:8501
   Network URL: http://192.168.x.x:8501
   ```

4. **Open your web browser** and go to: `http://localhost:8501`

5. **Use the interface:**
   - Upload your Excel file or use the default `input_data_tt.xlsx`
   - Set Buffer value (default: 5)
   - Select Mode: Dense or Sparse
   - Set Output Directory (default: "output")
   - Click "🚀 Generate Seating Arrangement"
   - Wait for processing to complete
   - View and download results from the "View Results" tab

### Method 2: Using Command Line

```bash
python seating_arrangement.py --buffer 5 --mode Dense --input input_data_tt.xlsx --output output
```

**Parameters:**
- `--buffer`: Number of seats to reduce from room capacity (default: 5)
- `--mode`: `Dense` (fill to capacity) or `Sparse` (50% capacity)
- `--input`: Path to input Excel file (optional)
- `--output`: Output directory (optional)

---

## Step 7: Access Generated Files

After running, check the `output` folder:

```
output/
├── 31_10_2025/
│   ├── Morning/
│   │   ├── 2025_10_31_Morning_R104_CE1101.pdf  ← Attendance PDF
│   │   ├── 2025_10_31_Morning_R104_CS1101.pdf
│   │   ├── 31_10_2025_CE1101_104_morning.xlsx   ← Excel file
│   │   └── ...
│   └── Evening/
│       └── ...
├── op_overall_seating_arrangement.xlsx
└── op_seats_left.xlsx
```

---

## Step 8: View PDF Attendance Sheets

1. **Navigate to output folder:**
   ```
   output/DD_MM_YYYY/SESSION/
   ```

2. **Find PDF files** with format:
   - `YYYY_MM_DD_SESSION_ROOM_SUBCODE.pdf`
   - Example: `2025_10_31_Morning_R104_CE1101.pdf`

3. **Each PDF contains:**
   - IITP Attendance System header
   - Date, Shift, Room No, Student count, Subject
   - Grid layout (5 rows × 3 columns) with student cards
   - Student photos (if available in `photos/` folder)
   - Student names, roll numbers, signature lines
   - Invigilator section at bottom

---

## Troubleshooting

### Issue: "Module not found" error
**Solution:** Reinstall dependencies:
```bash
pip install -r requirements.txt
```

### Issue: Port 8501 already in use
**Solution:** Use a different port:
```bash
streamlit run streamlit_app.py --server.port 8502
```

### Issue: Photos not showing in PDF
**Solution:** 
- Check that photos are in `photos/` folder
- Verify filename matches roll number exactly (e.g., `1401ZZ17.jpg`)
- Check file format is `.jpg`, `.jpeg`, or `.png`

### Issue: Excel file not reading
**Solution:**
- Verify all required sheets exist
- Check column names match exactly
- Ensure date format is correct

### Issue: PDF generation fails
**Solution:**
- Check `seating_arrangement.log` for errors
- Verify `photos/` folder exists (even if empty)
- Ensure reportlab and Pillow are installed

---

## Quick Reference Commands

```bash
# Navigate to project
cd "virtual\Intern Proj"

# Install dependencies
pip install -r requirements.txt

# Run Streamlit app
streamlit run streamlit_app.py

# Run from command line
python seating_arrangement.py --buffer 5 --mode Dense

# Check logs
cat seating_arrangement.log  # Linux/Mac
type seating_arrangement.log  # Windows
```

---

## Project Structure

```
virtual/Intern Proj/
├── streamlit_app.py          # Web interface
├── seating_arrangement.py    # Core logic
├── requirements.txt          # Dependencies
├── input_data_tt.xlsx        # Input file
├── photos/                   # Student photos (ROLL.jpg)
│   ├── 1401ZZ17.jpg
│   └── 1401ZZ18.jpg
├── output/                   # Generated files
│   ├── DD_MM_YYYY/
│   │   └── SESSION/
│   │       ├── YYYY_MM_DD_SESSION_ROOM_SUBCODE.pdf
│   │       └── ...
│   ├── op_overall_seating_arrangement.xlsx
│   └── op_seats_left.xlsx
└── seating_arrangement.log   # Execution log
```

---

## Need Help?

- Check `seating_arrangement.log` for detailed error messages
- Review `errors.txt` in output folder for processing errors
- Verify all input data is correctly formatted
- Ensure all dependencies are installed

---

## Success Checklist

- ✅ Python installed
- ✅ Dependencies installed (`pip install -r requirements.txt`)
- ✅ Input Excel file prepared with all required sheets
- ✅ Photos folder created (optional)
- ✅ Streamlit app running (`streamlit run streamlit_app.py`)
- ✅ Browser opened to `http://localhost:8501`
- ✅ Seating arrangement generated successfully
- ✅ PDF files created in output folder

---

**Happy Generating! 🎉**

