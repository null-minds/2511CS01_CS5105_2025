#!/usr/bin/env python3
"""
Seating Arrangement System for Exam Scheduling
This script generates seating arrangements based on timetable and room capacity.
"""

import pandas as pd
import numpy as np
import os
import sys
import logging
import argparse
from datetime import datetime
from collections import defaultdict
from typing import Dict, List, Tuple, Set
import traceback
from reportlab.lib import colors
from reportlab.lib.pagesizes import letter, A4
from reportlab.lib.units import inch, cm
from reportlab.platypus import SimpleDocTemplate, Table, TableStyle, Paragraph, Spacer, Image, PageBreak
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.enums import TA_CENTER, TA_LEFT
from PIL import Image as PILImage
from io import BytesIO

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler('seating_arrangement.log'),
        logging.StreamHandler(sys.stdout)
    ]
)

logger = logging.getLogger(__name__)


class SeatingArrangement:
    def __init__(self, input_file: str, buffer: int, mode: str):
        """
        Initialize the seating arrangement system.
        
        Args:
            input_file: Path to input Excel file
            buffer: Buffer value for capacity reduction
            mode: 'Sparse' or 'Dense' mode
        """
        self.input_file = input_file
        self.buffer = buffer
        self.mode = mode.strip().lower()
        
        if self.mode not in ['sparse', 'dense']:
            raise ValueError("Mode must be 'Sparse' or 'Dense'")
        
        # Data storage
        self.timetable = None
        self.course_roll_mapping = None
        self.roll_name_mapping = None
        self.room_capacity = None
        
        # Allocation tracking
        self.allocations = []  # List of (date, day, course, room, students, session)
        self.room_usage = defaultdict(int)  # Track how many students allocated per room
        
        # Error tracking
        self.errors = []
        
    def load_data(self):
        """Load all data from Excel file."""
        try:
            logger.info(f"Loading data from {self.input_file}")
            
            # Read all sheets
            excel_data = pd.read_excel(self.input_file, sheet_name=None)
            
            # Load timetable
            self.timetable = excel_data['in_timetable'].copy()
            self.timetable['Date'] = pd.to_datetime(self.timetable['Date'])
            logger.info(f"Loaded timetable with {len(self.timetable)} rows")
            
            # Load course-roll mapping
            self.course_roll_mapping = excel_data['in_course_roll_mapping'].copy()
            self.course_roll_mapping['rollno'] = self.course_roll_mapping['rollno'].astype(str).str.strip()
            self.course_roll_mapping['course_code'] = self.course_roll_mapping['course_code'].astype(str).str.strip()
            logger.info(f"Loaded course-roll mapping with {len(self.course_roll_mapping)} entries")
            
            # Load roll-name mapping
            self.roll_name_mapping = excel_data['in_roll_name_mapping'].copy()
            self.roll_name_mapping['Roll'] = self.roll_name_mapping['Roll'].astype(str).str.strip()
            self.roll_name_mapping['Name'] = self.roll_name_mapping['Name'].astype(str).str.strip()
            logger.info(f"Loaded roll-name mapping with {len(self.roll_name_mapping)} entries")
            
            # Load room capacity
            self.room_capacity = excel_data['in_room_capacity'].copy()
            self.room_capacity['Room No.'] = self.room_capacity['Room No.'].astype(str).str.strip()
            logger.info(f"Loaded room capacity with {len(self.room_capacity)} rooms")
            
            # Create name mapping dictionary for quick lookup
            self.name_dict = dict(zip(
                self.roll_name_mapping['Roll'],
                self.roll_name_mapping['Name']
            ))
            
            logger.info("Data loaded successfully")
            
        except Exception as e:
            error_msg = f"Error loading data: {str(e)}\n{traceback.format_exc()}"
            logger.error(error_msg)
            self.errors.append(error_msg)
            raise
    
    def get_students_for_course(self, course_code: str) -> List[str]:
        """Get list of students enrolled in a course."""
        course_code = course_code.strip()
        students = self.course_roll_mapping[
            self.course_roll_mapping['course_code'] == course_code
        ]['rollno'].tolist()
        return [str(s).strip() for s in students]
    
    def get_effective_capacity(self, room_cap: int) -> int:
        """
        Calculate effective capacity based on buffer and mode.
        
        Args:
            room_cap: Original room capacity
            
        Returns:
            Effective capacity after applying buffer and mode
        """
        # Apply buffer
        effective = room_cap - self.buffer
        
        # Apply mode
        if self.mode == 'sparse':
            effective = effective // 2
        
        return max(0, effective)
    
    def get_room_block(self, room_no: str) -> str:
        """Extract block (B1/B2) from room number."""
        room_str = str(room_no).strip()
        if room_str.startswith('B-'):
            return 'B2'
        elif any(room_str.startswith(prefix) for prefix in ['6', '8', '9', '10', 'LT', 'R']):
            return 'B1'
        return 'B1'  # Default
    
    def get_room_number_value(self, room_no: str) -> int:
        """Extract numeric value from room number for sorting."""
        room_str = str(room_no).strip()
        # Extract numbers from room string
        import re
        numbers = re.findall(r'\d+', room_str)
        if numbers:
            return int(numbers[0])
        return 0
    
    def check_clashes(self, courses: List[str], date: str, session: str) -> bool:
        """
        Check for clashes between courses in the same slot.
        
        Args:
            courses: List of course codes
            date: Exam date
            session: Morning or Evening
            
        Returns:
            True if clash detected, False otherwise
        """
        if len(courses) <= 1:
            return False
        
        course_students = {}
        for course in courses:
            students = set(self.get_students_for_course(course))
            course_students[course] = students
        
        # Check intersections
        clash_found = False
        for i, course1 in enumerate(courses):
            for course2 in courses[i+1:]:
                intersection = course_students[course1] & course_students[course2]
                if intersection:
                    clash_found = True
                    clash_msg = f"CLASH DETECTED on {date} ({session}): {course1} and {course2} have common students: {', '.join(sorted(intersection))}"
                    logger.warning(clash_msg)
                    print(clash_msg)
                    self.errors.append(clash_msg)
        
        return clash_found
    
    def get_available_rooms(self, date: datetime, session: str, preferred_block: str = None) -> pd.DataFrame:
        """Get available rooms for allocation, sorted by preference."""
        rooms_df = self.room_capacity.copy()
        rooms_df['effective_capacity'] = rooms_df['Exam Capacity'].apply(self.get_effective_capacity)
        rooms_df['block'] = rooms_df['Room No.'].apply(self.get_room_block)
        rooms_df['room_num'] = rooms_df['Room No.'].apply(self.get_room_number_value)
        
        # Calculate available capacity for each room
        available_caps = []
        for _, room_row in rooms_df.iterrows():
            room_no = str(room_row['Room No.']).strip()
            room_key = (str(date.date()), session, room_no)
            current_usage = self.room_usage.get(room_key, 0)
            effective_cap = room_row['effective_capacity']
            available = max(0, effective_cap - current_usage)
            available_caps.append(available)
        
        rooms_df['available'] = available_caps
        rooms_df = rooms_df[rooms_df['available'] > 0].copy()
        
        # Sort: prefer same block, larger capacity, smaller room numbers (adjacent)
        if preferred_block:
            rooms_df['block_match'] = (rooms_df['block'] == preferred_block).astype(int)
            rooms_df = rooms_df.sort_values(
                by=['block_match', 'effective_capacity', 'room_num'],
                ascending=[False, False, True]
            )
        else:
            rooms_df = rooms_df.sort_values(
                by=['effective_capacity', 'room_num'],
                ascending=[False, True]
            )
        
        return rooms_df
    
    def allocate_course_to_rooms(self, course_code: str, students: List[str], 
                                  date: datetime, day: str, session: str) -> bool:
        """
        Allocate students of a course to rooms.
        Optimizes for: same building, adjacent rooms, minimal room count.
        
        Args:
            course_code: Course code
            students: List of student roll numbers
            date: Exam date
            day: Day of week
            session: Morning or Evening
            
        Returns:
            True if allocation successful, False otherwise
        """
        if not students:
            logger.warning(f"No students found for course {course_code}")
            return True
        
        remaining_students = students.copy()
        allocated_rooms = []  # Track rooms used for this course
        course_block = None
        
        while remaining_students:
            # Get available rooms, preferring same block if we've started allocating
            available_rooms = self.get_available_rooms(date, session, course_block)
            
            if available_rooms.empty:
                # Try other block if no rooms in current block
                if course_block:
                    available_rooms = self.get_available_rooms(date, session, None)
                    course_block = None  # Reset to allow any block
            
            if available_rooms.empty:
                break
            
            # Select best room
            best_room = available_rooms.iloc[0]
            room_no = str(best_room['Room No.']).strip()
            room_block = best_room['block']
            available = int(best_room['available'])
            
            # Set course block if first allocation
            if course_block is None:
                course_block = room_block
            
            # Prefer same block, but allow if necessary
            if course_block and room_block != course_block:
                # Check if we can still use this block (only if no other option)
                if len(available_rooms[available_rooms['block'] == course_block]) > 0:
                    continue
            
            # Allocate students to this room
            to_allocate = min(available, len(remaining_students))
            allocated_students = remaining_students[:to_allocate]
            remaining_students = remaining_students[to_allocate:]
            
            # Update room usage
            room_key = (str(date.date()), session, room_no)
            self.room_usage[room_key] = self.room_usage.get(room_key, 0) + to_allocate
            
            # Record allocation
            self.allocations.append({
                'date': date,
                'day': day,
                'course': course_code,
                'room': room_no,
                'students': allocated_students,
                'session': session
            })
            
            allocated_rooms.append(room_no)
            logger.info(f"Allocated {to_allocate} students of {course_code} to {room_no} on {date.date()} ({session})")
        
        if remaining_students:
            error_msg = f"Cannot allocate all students for {course_code} on {date.date()} ({session}). {len(remaining_students)} students remaining. Total capacity insufficient."
            logger.error(error_msg)
            self.errors.append(error_msg)
            print(f"ERROR: {error_msg}")
            return False
        
        return True
    
    def process_timetable(self):
        """Process the entire timetable and allocate students."""
        logger.info("Starting timetable processing")
        
        for _, row in self.timetable.iterrows():
            date = row['Date']
            day = str(row['Day']).strip()
            
            # Process Morning session
            morning_courses = str(row['Morning']).strip()
            if morning_courses and morning_courses.upper() != 'NO EXAM':
                courses = [c.strip() for c in morning_courses.split(';') if c.strip()]
                if courses:
                    # Check for clashes
                    self.check_clashes(courses, str(date.date()), 'Morning')
                    
                    # Get course sizes
                    course_sizes = {}
                    for course in courses:
                        students = self.get_students_for_course(course)
                        course_sizes[course] = len(students)
                    
                    # Sort courses by size (largest first)
                    sorted_courses = sorted(course_sizes.items(), key=lambda x: x[1], reverse=True)
                    
                    # Allocate largest courses first
                    for course_code, _ in sorted_courses:
                        students = self.get_students_for_course(course_code)
                        self.allocate_course_to_rooms(course_code, students, date, day, 'Morning')
            
            # Process Evening session
            evening_courses = str(row['Evening']).strip()
            if evening_courses and evening_courses.upper() != 'NO EXAM':
                courses = [c.strip() for c in evening_courses.split(';') if c.strip()]
                if courses:
                    # Check for clashes
                    self.check_clashes(courses, str(date.date()), 'Evening')
                    
                    # Get course sizes
                    course_sizes = {}
                    for course in courses:
                        students = self.get_students_for_course(course)
                        course_sizes[course] = len(students)
                    
                    # Sort courses by size (largest first)
                    sorted_courses = sorted(course_sizes.items(), key=lambda x: x[1], reverse=True)
                    
                    # Allocate largest courses first
                    for course_code, _ in sorted_courses:
                        students = self.get_students_for_course(course_code)
                        self.allocate_course_to_rooms(course_code, students, date, day, 'Evening')
        
        logger.info("Timetable processing completed")
    
    def generate_output_files(self, output_dir: str = 'output'):
        """Generate all output Excel files."""
        try:
            logger.info(f"Generating output files in {output_dir}")
            
            # Create output directory structure
            os.makedirs(output_dir, exist_ok=True)
            
            # Group allocations by date and session
            by_date_session = defaultdict(lambda: defaultdict(list))
            
            for alloc in self.allocations:
                date_str = alloc['date'].strftime('%d_%m_%Y')
                session = alloc['session']
                by_date_session[date_str][session].append(alloc)
            
            # Generate individual room files
            for date_str, sessions in by_date_session.items():
                date_folder = os.path.join(output_dir, date_str)
                for session in ['Morning', 'Evening']:
                    session_folder = os.path.join(date_folder, session)
                    os.makedirs(session_folder, exist_ok=True)
                    
                    # Group by course and room
                    course_room_allocations = defaultdict(list)
                    for alloc in sessions.get(session, []):
                        key = (alloc['course'], alloc['room'])
                        course_room_allocations[key].extend(alloc['students'])
                    
                    # Create file for each course-room combination
                    for (course, room), students in course_room_allocations.items():
                        # Parse date to get YYYY_MM_DD format for PDF
                        try:
                            date_obj = datetime.strptime(date_str, '%d_%m_%Y')
                            date_yyyy_mm_dd = date_obj.strftime('%Y_%m_%d')
                        except Exception as e:
                            logger.warning(f"Error parsing date {date_str}: {str(e)}")
                            date_yyyy_mm_dd = date_str
                        
                        # Format room number - add R prefix if not present
                        room_formatted = str(room).strip()
                        if not room_formatted.startswith('R') and not room_formatted.startswith('B-'):
                            # Check if it's a numeric room
                            if room_formatted.isdigit() or (room_formatted[0].isdigit() if room_formatted else False):
                                room_formatted = f"R{room_formatted}"
                        
                        # Excel filename (keep original format)
                        filename = f"{date_str}_{course}_{room}_{session.lower()}.xlsx"
                        filepath = os.path.join(session_folder, filename)
                        
                        # PDF filename (YYYY_MM_DD_SESSION_ROOM_SUBCODE.pdf)
                        # Format: 2025_10_31_Morning_R104_CE1101.pdf
                        pdf_filename = f"{date_yyyy_mm_dd}_{session}_{room_formatted}_{course}.pdf"
                        pdf_filepath = os.path.join(session_folder, pdf_filename)
                        
                        # Create DataFrame with student details
                        data = []
                        for roll in students:
                            name = self.name_dict.get(roll, 'Unknown Name')
                            data.append({
                                'Roll': roll,
                                'Student Name': name,
                                'Signature': ''
                            })
                        
                        # Add empty rows and TA/Invigilator rows
                        data.append({'Roll': '', 'Student Name': '', 'Signature': ''})
                        for i in range(1, 6):
                            data.append({'Roll': f'TA{i}', 'Student Name': '', 'Signature': ''})
                        for i in range(1, 6):
                            data.append({'Roll': f'Invigilator{i}', 'Student Name': '', 'Signature': ''})
                        
                        df = pd.DataFrame(data)
                        
                        # Create header
                        header_text = f"Course: {course} | Room: {room} | Date: {date_str.replace('_', '-')} | Session: {session}"
                        
                        # Write to Excel with custom header
                        with pd.ExcelWriter(filepath, engine='openpyxl') as writer:
                            # Write data starting from row 1
                            df.to_excel(writer, index=False, sheet_name='Sheet1', startrow=1, header=False)
                            
                            # Get workbook and worksheet
                            workbook = writer.book
                            worksheet = writer.sheets['Sheet1']
                            
                            # Add header row at top
                            worksheet.insert_rows(0)
                            worksheet.merge_cells('A1:C1')
                            worksheet['A1'] = header_text
                            from openpyxl.styles import Font
                            worksheet['A1'].font = Font(bold=True)
                            
                            # Add column headers
                            worksheet['A2'] = 'Roll'
                            worksheet['B2'] = 'Student Name'
                            worksheet['C2'] = 'Signature'
                        
                        logger.info(f"Created file: {filepath}")
                        
                        # Generate PDF version with new format
                        try:
                            self.generate_pdf_for_room(course, room, date_str, session, students, pdf_filepath, photos_dir='photos')
                        except Exception as e:
                            error_msg = f"Error generating PDF {pdf_filename}: {str(e)}"
                            logger.error(error_msg)
                            self.errors.append(error_msg)
                            # Continue with other files
            
            # Generate overall seating arrangement file
            overall_data = []
            for alloc in self.allocations:
                roll_list = ';'.join(alloc['students'])
                overall_data.append({
                    'Date': alloc['date'],
                    'Day': alloc['day'],
                    'course_code': alloc['course'],
                    'Room': alloc['room'],
                    'Allocated_students_count': len(alloc['students']),
                    'Roll_list (semicolon separated_)': roll_list
                })
            
            overall_df = pd.DataFrame(overall_data)
            overall_file = os.path.join(output_dir, 'op_overall_seating_arrangement.xlsx')
            
            with pd.ExcelWriter(overall_file, engine='openpyxl') as writer:
                # Write data starting from row 2 (after headers)
                overall_df.to_excel(writer, index=False, sheet_name='Sheet1', startrow=2, header=False)
                
                workbook = writer.book
                worksheet = writer.sheets['Sheet1']
                
                # Write header row 0
                worksheet['A1'] = 'Seating Plan'
                # Write header row 1
                worksheet['A2'] = 'Date'
                worksheet['B2'] = 'Day'
                worksheet['C2'] = 'course_code'
                worksheet['D2'] = 'Room'
                worksheet['E2'] = 'Allocated_students_count'
                worksheet['F2'] = 'Roll_list (semicolon separated_)'
                worksheet['G2'] = 'Subject may repeat if large num of stud are there'
            
            logger.info(f"Created overall file: {overall_file}")
            
            # Generate seats left file
            seats_data = []
            for _, room_row in self.room_capacity.iterrows():
                room_no = str(room_row['Room No.']).strip()
                capacity = int(room_row['Exam Capacity'])
                block = self.get_room_block(room_no)
                
                # Calculate total allocated
                total_allocated = 0
                for alloc in self.allocations:
                    if alloc['room'] == room_no:
                        total_allocated += len(alloc['students'])
                
                vacant = capacity - total_allocated
                
                seats_data.append({
                    'Room No.': room_no,
                    'Exam Capacity': capacity,
                    'Block': block,
                    'Alloted': total_allocated,
                    'Vacant (B-C)': vacant
                })
            
            seats_df = pd.DataFrame(seats_data)
            seats_file = os.path.join(output_dir, 'op_seats_left.xlsx')
            
            with pd.ExcelWriter(seats_file, engine='openpyxl') as writer:
                seats_df.to_excel(writer, index=False, sheet_name='Sheet1', startrow=1, header=False)
                
                workbook = writer.book
                worksheet = writer.sheets['Sheet1']
                
                # Write header row
                worksheet['A1'] = 'Room No.'
                worksheet['B1'] = 'Exam Capacity'
                worksheet['C1'] = 'Block'
                worksheet['D1'] = 'Alloted'
                worksheet['E1'] = 'Vacant (B-C)'
            
            logger.info(f"Created seats left file: {seats_file}")
            
            # Write errors to file
            if self.errors:
                error_file = os.path.join(output_dir, 'errors.txt')
                with open(error_file, 'w') as f:
                    f.write('\n'.join(self.errors))
                logger.info(f"Errors written to {error_file}")
            
            logger.info("Output files generated successfully")
            
        except Exception as e:
            error_msg = f"Error generating output files: {str(e)}\n{traceback.format_exc()}"
            logger.error(error_msg)
            self.errors.append(error_msg)
            raise
    
    def get_student_photo(self, roll: str, photos_dir: str = 'photos') -> Image:
        """
        Get student photo or placeholder.
        
        Args:
            roll: Student roll number
            photos_dir: Directory containing student photos
            
        Returns:
            Image object (photo or placeholder)
        """
        try:
            photo_path = os.path.join(photos_dir, f"{roll}.jpg")
            if os.path.exists(photo_path):
                # Load and resize photo
                pil_img = PILImage.open(photo_path)
                # Resize to fit card (approximately 1.0x1.6 inches)
                pil_img.thumbnail((120, 192), PILImage.Resampling.LANCZOS)
                
                # Convert to bytes
                img_buffer = BytesIO()
                pil_img.save(img_buffer, format='JPEG')
                img_buffer.seek(0)
                
                return Image(img_buffer, width=1.0*inch, height=1.6*inch)
            else:
                # Create placeholder with camera icon
                return self._create_photo_placeholder()
        except Exception as e:
            logger.warning(f"Error loading photo for roll {roll}: {str(e)}")
            return self._create_photo_placeholder()
    
    def _create_photo_placeholder(self) -> Table:
        """Create a photo placeholder with camera icon matching the image format."""
        try:
            styles = getSampleStyleSheet()
            
            # Create placeholder with camera icon and text
            # Match the format: camera icon on top, "No Image" and "Available" below
            placeholder_style_normal = ParagraphStyle(
                'PlaceholderNormal',
                parent=styles['Normal'],
                fontSize=8,
                textColor=colors.black,
                alignment=TA_CENTER,
                fontName='Helvetica'
            )
            
            placeholder_style_icon = ParagraphStyle(
                'PlaceholderIcon',
                parent=styles['Normal'],
                fontSize=16,
                textColor=colors.black,
                alignment=TA_CENTER,
                fontName='Helvetica'
            )
            
            text_placeholder = Table([
                [Paragraph('📷', placeholder_style_icon)],
                [Paragraph('No Image', placeholder_style_normal)],
                [Paragraph('Available', placeholder_style_normal)]
            ], colWidths=[1.0*inch], rowHeights=[0.5*inch, 0.35*inch, 0.35*inch])
            
            text_style = TableStyle([
                ('ALIGN', (0, 0), (-1, -1), 'CENTER'),
                ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                ('BACKGROUND', (0, 0), (-1, -1), colors.HexColor('#F5F5F5')),
                ('LEFTPADDING', (0, 0), (-1, -1), 0),
                ('RIGHTPADDING', (0, 0), (-1, -1), 0),
                ('TOPPADDING', (0, 0), (-1, -1), 0),
                ('BOTTOMPADDING', (0, 0), (-1, -1), 0),
            ])
            text_placeholder.setStyle(text_style)
            
            return text_placeholder
        except Exception as e:
            logger.warning(f"Error creating photo placeholder: {str(e)}")
            # Return simple text as fallback
            return Paragraph("No Image<br/>Available", getSampleStyleSheet()['Normal'])
    
    def _create_student_card(self, roll: str, name: str, photos_dir: str = 'photos') -> Table:
        """
        Create a student card with photo and information.
        
        Args:
            roll: Student roll number
            name: Student name
            photos_dir: Directory containing photos
            
        Returns:
            Table representing student card
        """
        try:
            # Get photo or placeholder
            photo = self.get_student_photo(roll, photos_dir)
            
            # Format name - handle missing names
            if not name or name.strip() == '' or name == 'Unknown Name':
                display_name = "(name not found)"
            else:
                display_name = name
            
            # Create student info section with proper formatting
            styles = getSampleStyleSheet()
            name_style = ParagraphStyle(
                'NameStyle',
                parent=styles['Normal'],
                fontSize=10,
                fontName='Helvetica-Bold',
                textColor=colors.black,
                alignment=TA_LEFT,
                leftIndent=8
            )
            roll_style = ParagraphStyle(
                'RollStyle',
                parent=styles['Normal'],
                fontSize=10,
                fontName='Helvetica-Bold',
                textColor=colors.black,
                alignment=TA_LEFT,
                leftIndent=8
            )
            sign_style = ParagraphStyle(
                'SignStyle',
                parent=styles['Normal'],
                fontSize=10,
                fontName='Helvetica-Bold',
                textColor=colors.black,
                alignment=TA_LEFT,
                leftIndent=8
            )
            
            info_data = [
                [Paragraph(display_name, name_style)],
                [Paragraph(f"Roll: {roll}", roll_style)],
                [Paragraph(f"Sign: _______________", sign_style)]
            ]
            
            # Adjust info table to fit in card (info_width = 1.4 inch)
            info_table = Table(info_data, colWidths=[1.4*inch], rowHeights=[0.5*inch, 0.4*inch, 0.4*inch])
            info_style = TableStyle([
                ('ALIGN', (0, 0), (-1, -1), 'LEFT'),
                ('VALIGN', (0, 0), (-1, -1), 'TOP'),
                ('LEFTPADDING', (0, 0), (-1, -1), 0),
                ('RIGHTPADDING', (0, 0), (-1, -1), 0),
                ('TOPPADDING', (0, 0), (-1, -1), 8),
                ('BOTTOMPADDING', (0, 0), (-1, -1), 0),
                ('BACKGROUND', (0, 0), (-1, -1), colors.white),
            ])
            info_table.setStyle(info_style)
            
            # Combine photo and info in a card with vertical divider
            # Adjust widths to fit properly in grid (total ~2.5 inch per card)
            photo_width = 1.0*inch
            info_width = 1.4*inch
            card_data = [[photo, info_table]]
            card_table = Table(card_data, colWidths=[photo_width, info_width], rowHeights=[1.6*inch])
            card_style = TableStyle([
                # Outer border - thick black border
                ('GRID', (0, 0), (-1, -1), 2, colors.black),
                # Vertical divider between photo and info - thick line
                ('LINEAFTER', (0, 0), (0, -1), 2, colors.black),
                ('VALIGN', (0, 0), (-1, -1), 'TOP'),
                ('LEFTPADDING', (0, 0), (-1, -1), 0),
                ('RIGHTPADDING', (0, 0), (-1, -1), 0),
                ('TOPPADDING', (0, 0), (-1, -1), 0),
                ('BOTTOMPADDING', (0, 0), (-1, -1), 0),
            ])
            card_table.setStyle(card_style)
            
            return card_table
        except Exception as e:
            logger.error(f"Error creating student card for {roll}: {str(e)}")
            # Return a simple fallback card
            fallback_data = [[f"Roll: {roll}", f"Name: {name or '(name not found)'}"]]
            return Table(fallback_data, colWidths=[2.35*inch, 2.35*inch])
    
    def generate_pdf_for_room(self, course: str, room: str, date_str: str, session: str, 
                              students: List[str], output_path: str, photos_dir: str = 'photos'):
        """
        Generate a PDF attendance sheet for a specific room seating arrangement.
        Format: IITP Attendance System with grid layout of student cards.
        
        Args:
            course: Course code
            room: Room number
            date_str: Date string (dd_mm_yyyy format)
            session: Morning or Evening
            students: List of student roll numbers
            output_path: Path where PDF will be saved
            photos_dir: Directory containing student photos
        """
        try:
            # Parse date to get day name
            try:
                date_obj = datetime.strptime(date_str, '%d_%m_%Y')
                day_name = date_obj.strftime('%A')
                date_display = date_obj.strftime('%d-%m-%Y')
            except Exception as e:
                logger.warning(f"Error parsing date {date_str}: {str(e)}")
                day_name = "Unknown"
                date_display = date_str.replace('_', '-')
            
            # Create PDF document with proper margins
            doc = SimpleDocTemplate(output_path, pagesize=letter,
                                   leftMargin=0.4*inch, rightMargin=0.4*inch,
                                   topMargin=0.4*inch, bottomMargin=0.4*inch)
            elements = []
            
            # Define styles
            styles = getSampleStyleSheet()
            title_style = ParagraphStyle(
                'TitleStyle',
                parent=styles['Heading1'],
                fontSize=18,
                textColor=colors.black,
                spaceAfter=12,
                alignment=TA_CENTER,
                fontName='Helvetica-Bold'
            )
            
            header_style = ParagraphStyle(
                'HeaderStyle',
                parent=styles['Normal'],
                fontSize=10,
                textColor=colors.black,
                alignment=TA_LEFT,
                fontName='Helvetica',
                spaceAfter=4
            )
            
            # Title
            elements.append(Paragraph("IITP Attendance System", title_style))
            elements.append(Spacer(1, 0.15*inch))
            
            # Header information - match exact format from image
            header_info = f"Date: {date_display} ({day_name}) | Shift: {session} | Room No: {room} | Student count: {len(students)}"
            elements.append(Paragraph(header_info, header_style))
            
            # Subject line - format like "Subject: Mathematics - II (MA102)"
            subject_info = f"Subject: {course} | Stud Present: | Stud Absent:"
            elements.append(Paragraph(subject_info, header_style))
            elements.append(Spacer(1, 0.15*inch))
            
            # Create student grid (5 rows x 3 columns = 15 students per page)
            students_per_page = 15
            students_per_row = 3
            rows_per_page = 5
            
            for page_start in range(0, len(students), students_per_page):
                page_students = students[page_start:page_start + students_per_page]
                
                # Create grid rows
                grid_rows = []
                for row_idx in range(rows_per_page):
                    row_students = page_students[row_idx * students_per_row:(row_idx + 1) * students_per_row]
                    row_cards = []
                    
                    for student_roll in row_students:
                        name = self.name_dict.get(student_roll, '')
                        card = self._create_student_card(student_roll, name, photos_dir)
                        row_cards.append(card)
                    
                    # Fill empty cells if row is not full
                    while len(row_cards) < students_per_row:
                        # Create empty card matching the format
                        empty_photo = Table([['']], colWidths=[1.0*inch], rowHeights=[1.6*inch])
                        empty_info = Table([['']], colWidths=[1.4*inch], rowHeights=[1.6*inch])
                        empty_card = Table([[empty_photo, empty_info]], colWidths=[1.0*inch, 1.4*inch], rowHeights=[1.6*inch])
                        empty_style = TableStyle([
                            ('GRID', (0, 0), (-1, -1), 2, colors.black),
                            ('LINEAFTER', (0, 0), (0, -1), 2, colors.black),
                        ])
                        empty_card.setStyle(empty_style)
                        row_cards.append(empty_card)
                    
                    grid_rows.append(row_cards)
                
                # Create grid table
                grid_data = []
                for row_cards in grid_rows:
                    grid_data.append(row_cards)
                
                # Calculate proper column widths for 3 columns on letter size page
                # Page width: 8.5 inch, margins: 0.4*2 = 0.8 inch, usable: 7.7 inch
                # With spacing between cards: 7.7 / 3 = ~2.57 inch per card
                card_width = 2.5*inch
                grid_table = Table(grid_data, colWidths=[card_width, card_width, card_width], 
                                  rowHeights=[1.6*inch] * rows_per_page)
                grid_style = TableStyle([
                    ('VALIGN', (0, 0), (-1, -1), 'TOP'),
                    ('LEFTPADDING', (0, 0), (-1, -1), 2),
                    ('RIGHTPADDING', (0, 0), (-1, -1), 2),
                    ('TOPPADDING', (0, 0), (-1, -1), 2),
                    ('BOTTOMPADDING', (0, 0), (-1, -1), 2),
                ])
                grid_table.setStyle(grid_style)
                elements.append(grid_table)
                
                # Add invigilator section
                elements.append(Spacer(1, 0.25*inch))
                
                # Invigilator header
                invigilator_header_style = ParagraphStyle(
                    'InvigilatorHeader',
                    parent=styles['Normal'],
                    fontSize=10,
                    textColor=colors.black,
                    alignment=TA_LEFT,
                    fontName='Helvetica',
                    spaceAfter=6
                )
                elements.append(Paragraph("Invigilator Name & Signature", invigilator_header_style))
                
                invigilator_data = [
                    ['SI No.', 'Name', 'Signature'],
                    ['1', '', ''],
                    ['2', '', ''],
                    ['3', '', ''],
                    ['4', '', ''],
                    ['5', '', '']
                ]
                invigilator_table = Table(invigilator_data, colWidths=[0.8*inch, 3.2*inch, 3.7*inch], 
                                         rowHeights=[0.35*inch] * 6)
                invigilator_style = TableStyle([
                    ('BACKGROUND', (0, 0), (-1, 0), colors.HexColor('#E0E0E0')),
                    ('TEXTCOLOR', (0, 0), (-1, 0), colors.black),
                    ('ALIGN', (0, 0), (0, -1), 'CENTER'),  # Center SI No.
                    ('ALIGN', (1, 0), (-1, -1), 'LEFT'),    # Left align Name and Signature
                    ('FONTNAME', (0, 0), (-1, 0), 'Helvetica-Bold'),
                    ('FONTSIZE', (0, 0), (-1, -1), 10),
                    ('GRID', (0, 0), (-1, -1), 1, colors.black),
                    ('VALIGN', (0, 0), (-1, -1), 'MIDDLE'),
                    ('LEFTPADDING', (0, 0), (-1, -1), 6),
                    ('RIGHTPADDING', (0, 0), (-1, -1), 6),
                    ('TOPPADDING', (0, 0), (-1, -1), 6),
                    ('BOTTOMPADDING', (0, 0), (-1, -1), 6),
                ])
                invigilator_table.setStyle(invigilator_style)
                elements.append(invigilator_table)
                
                # Add page break if more students
                if page_start + students_per_page < len(students):
                    elements.append(PageBreak())
            
            # Build PDF
            doc.build(elements)
            logger.info(f"Created PDF file: {output_path}")
            
        except Exception as e:
            error_msg = f"Error generating PDF for {course} {room}: {str(e)}\n{traceback.format_exc()}"
            logger.error(error_msg)
            self.errors.append(error_msg)
            # Don't raise - continue with other files
            raise


def main():
    """Main function to run the seating arrangement system."""
    parser = argparse.ArgumentParser(
        description='Generate seating arrangement for exams'
    )
    parser.add_argument(
        '--input',
        type=str,
        default='input_data_tt.xlsx',
        help='Input Excel file path (default: input_data_tt.xlsx)'
    )
    parser.add_argument(
        '--buffer',
        type=int,
        required=True,
        help='Buffer value for capacity reduction'
    )
    parser.add_argument(
        '--mode',
        type=str,
        required=True,
        choices=['Sparse', 'Dense', 'sparse', 'dense'],
        help='Allocation mode: Sparse or Dense'
    )
    parser.add_argument(
        '--output',
        type=str,
        default='output',
        help='Output directory (default: output)'
    )
    
    args = parser.parse_args()
    
    try:
        # Create seating arrangement system
        system = SeatingArrangement(
            input_file=args.input,
            buffer=args.buffer,
            mode=args.mode
        )
        
        # Load data
        system.load_data()
        
        # Process timetable
        system.process_timetable()
        
        # Generate output files
        system.generate_output_files(output_dir=args.output)
        
        logger.info("Seating arrangement generation completed successfully!")
        print("\nSeating arrangement generation completed successfully!")
        
    except Exception as e:
        error_msg = f"Fatal error: {str(e)}\n{traceback.format_exc()}"
        logger.error(error_msg)
        print(f"\nFATAL ERROR: {error_msg}")
        sys.exit(1)


if __name__ == '__main__':
    main()

