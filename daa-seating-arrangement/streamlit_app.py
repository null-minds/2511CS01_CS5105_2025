import streamlit as st
import pandas as pd
import os
import sys
from seating_arrangement import SeatingArrangement
import logging

# Configure page
st.set_page_config(
    page_title="Seating Arrangement System",
    page_icon="🪑",
    layout="wide"
)

# Custom CSS
st.markdown("""
    <style>
    .main-header {
        font-size: 2.5rem;
        font-weight: bold;
        color: #1f77b4;
        text-align: center;
        margin-bottom: 2rem;
    }
    .success-box {
        padding: 1rem;
        border-radius: 0.5rem;
        background-color: #d4edda;
        border: 1px solid #c3e6cb;
        margin: 1rem 0;
    }
    .error-box {
        padding: 1rem;
        border-radius: 0.5rem;
        background-color: #f8d7da;
        border: 1px solid #f5c6cb;
        margin: 1rem 0;
    }
    </style>
""", unsafe_allow_html=True)

st.markdown('<div class="main-header">Seating Arrangement Management System</div>', unsafe_allow_html=True)

# Sidebar for inputs
st.sidebar.header("Configuration")

# File upload
uploaded_file = st.sidebar.file_uploader(
    "Upload Input Excel File",
    type=['xlsx'],
    help="Upload the input_data_tt.xlsx file"
)

# Buffer input
buffer = st.sidebar.number_input(
    "Buffer",
    min_value=0,
    max_value=20,
    value=5,
    step=1,
    help="Buffer value for capacity reduction"
)

# Mode selection
mode = st.sidebar.selectbox(
    "Allocation Mode",
    ["Dense", "Sparse"],
    help="Dense: Fill rooms to capacity. Sparse: Fill 50% capacity per subject"
)

# Output directory
output_dir = st.sidebar.text_input(
    "Output Directory",
    value="output",
    help="Directory where output files will be saved"
)

# Main content area
tab1, tab2, tab3 = st.tabs(["Upload & Run", "View Results", "About"])

with tab1:
    st.header("Upload Input File and Generate Seating Arrangement")
    
    if uploaded_file is not None:
        # Save uploaded file temporarily
        input_path = "temp_input.xlsx"
        with open(input_path, "wb") as f:
            f.write(uploaded_file.getbuffer())
        
        st.success(f"File uploaded: {uploaded_file.name}")
        
        # Display file info
        try:
            excel_data = pd.read_excel(input_path, sheet_name=None)
            st.info(f"Sheets found: {', '.join(excel_data.keys())}")
            
            # Show preview of timetable
            if 'in_timetable' in excel_data:
                st.subheader("Timetable Preview")
                st.dataframe(excel_data['in_timetable'].head(), use_container_width=True)
        except Exception as e:
            st.error(f"Error reading file: {str(e)}")
        
        # Run button
        if st.button("Generate Seating Arrangement", type="primary", use_container_width=True):
            with st.spinner("Processing seating arrangement..."):
                try:
                    # Create system instance
                    system = SeatingArrangement(
                        input_file=input_path,
                        buffer=buffer,
                        mode=mode
                    )
                    
                    # Load data
                    with st.status("Loading data...", expanded=True) as status:
                        system.load_data()
                        status.update(label="Data loaded successfully", state="complete")
                    
                    # Process timetable
                    with st.status("Processing timetable...", expanded=True) as status:
                        system.process_timetable()
                        status.update(label="Timetable processed successfully", state="complete")
                    
                    # Generate output files
                    with st.status("Generating output files...", expanded=True) as status:
                        system.generate_output_files(output_dir=output_dir)
                        status.update(label="Output files generated successfully", state="complete")
                    
                    st.success("Seating arrangement generation completed successfully!")
                    
                    # Show summary
                    st.subheader("Summary")
                    col1, col2, col3, col4 = st.columns(4)
                    
                    with col1:
                        st.metric("Total Allocations", len(system.allocations))
                    
                    with col2:
                        total_students = sum(len(alloc['students']) for alloc in system.allocations)
                        st.metric("Total Students Allocated", total_students)
                    
                    with col3:
                        unique_rooms = len(set(alloc['room'] for alloc in system.allocations))
                        st.metric("Rooms Used", unique_rooms)
                    
                    with col4:
                        st.metric("Errors", len(system.errors))
                    
                    # Show errors if any
                    if system.errors:
                        st.warning("Some errors were detected. Check the errors section below.")
                        with st.expander("View Errors"):
                            for error in system.errors:
                                st.text(error)
                    
                    # Store results in session state
                    st.session_state['system'] = system
                    st.session_state['output_dir'] = output_dir
                    st.session_state['generated'] = True
                    
                except Exception as e:
                    st.error(f"Error: {str(e)}")
                    st.exception(e)
    else:
        st.info("Please upload an input Excel file to begin")
        
        # Check if default file exists
        default_file = "input_data_tt.xlsx"
        if os.path.exists(default_file):
            st.info(f"Default file '{default_file}' found. You can use it or upload a new one.")
            
            # Option to use default file
            if st.button("Use Default File", use_container_width=True):
                input_path = default_file
                st.success(f"Using default file: {input_path}")
                
                # Display file info
                try:
                    excel_data = pd.read_excel(input_path, sheet_name=None)
                    st.info(f"Sheets found: {', '.join(excel_data.keys())}")
                    
                    # Show preview of timetable
                    if 'in_timetable' in excel_data:
                        st.subheader("Timetable Preview")
                        st.dataframe(excel_data['in_timetable'].head(), use_container_width=True)
                except Exception as e:
                    st.error(f"Error reading file: {str(e)}")
                
                # Run button
                if st.button("Generate Seating Arrangement", type="primary", use_container_width=True, key="run_default"):
                    with st.spinner("Processing seating arrangement..."):
                        try:
                            # Create system instance
                            system = SeatingArrangement(
                                input_file=input_path,
                                buffer=buffer,
                                mode=mode
                            )
                            
                            # Load data
                            with st.status("Loading data...", expanded=True) as status:
                                system.load_data()
                                status.update(label="Data loaded successfully", state="complete")
                            
                            # Process timetable
                            with st.status("Processing timetable...", expanded=True) as status:
                                system.process_timetable()
                                status.update(label="Timetable processed successfully", state="complete")
                            
                            # Generate output files
                            with st.status("Generating output files...", expanded=True) as status:
                                system.generate_output_files(output_dir=output_dir)
                                status.update(label="Output files generated successfully", state="complete")
                            
                            st.success("Seating arrangement generation completed successfully!")
                            
                            # Show summary
                            st.subheader("Summary")
                            col1, col2, col3, col4 = st.columns(4)
                            
                            with col1:
                                st.metric("Total Allocations", len(system.allocations))
                            
                            with col2:
                                total_students = sum(len(alloc['students']) for alloc in system.allocations)
                                st.metric("Total Students Allocated", total_students)
                            
                            with col3:
                                unique_rooms = len(set(alloc['room'] for alloc in system.allocations))
                                st.metric("Rooms Used", unique_rooms)
                            
                            with col4:
                                st.metric("Errors", len(system.errors))
                            
                            # Show errors if any
                            if system.errors:
                                st.warning("Some errors were detected. Check the errors section below.")
                                with st.expander("View Errors"):
                                    for error in system.errors:
                                        st.text(error)
                            
                            # Store results in session state
                            st.session_state['system'] = system
                            st.session_state['output_dir'] = output_dir
                            st.session_state['generated'] = True
                            
                        except Exception as e:
                            st.error(f"Error: {str(e)}")
                            st.exception(e)

with tab2:
    st.header("View Generated Results")
    
    if st.session_state.get('generated', False):
        output_dir = st.session_state.get('output_dir', 'output')
        
        # Check if output directory exists
        if os.path.exists(output_dir):
            st.success(f"Output directory found: {output_dir}")
            
            # List output files
            st.subheader("Generated Files")
            
            # Overall files
            overall_file = os.path.join(output_dir, "op_overall_seating_arrangement.xlsx")
            seats_file = os.path.join(output_dir, "op_seats_left.xlsx")
            
            if os.path.exists(overall_file):
                st.success("Overall Seating Arrangement file generated")
                try:
                    df_overall = pd.read_excel(overall_file)
                    st.dataframe(df_overall, use_container_width=True)
                    
                    # Download button
                    with open(overall_file, "rb") as f:
                        st.download_button(
                            label="Download Overall Seating Arrangement",
                            data=f,
                            file_name="op_overall_seating_arrangement.xlsx",
                            mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
                        )
                except Exception as e:
                    st.error(f"Error reading file: {str(e)}")
            
            if os.path.exists(seats_file):
                st.success("Seats Left file generated")
                try:
                    df_seats = pd.read_excel(seats_file)
                    st.dataframe(df_seats, use_container_width=True)
                    
                    # Download button
                    with open(seats_file, "rb") as f:
                        st.download_button(
                            label="Download Seats Left Summary",
                            data=f,
                            file_name="op_seats_left.xlsx",
                            mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"
                        )
                except Exception as e:
                    st.error(f"Error reading file: {str(e)}")
            
            # Show individual room files
            st.subheader("Individual Room Files")
            
            # Get all date folders
            date_folders = [d for d in os.listdir(output_dir) 
                          if os.path.isdir(os.path.join(output_dir, d)) and d != '__pycache__']
            
            if date_folders:
                selected_date = st.selectbox("Select Date", sorted(date_folders))
                date_path = os.path.join(output_dir, selected_date)
                
                sessions = [s for s in os.listdir(date_path) 
                           if os.path.isdir(os.path.join(date_path, s))]
                
                if sessions:
                    selected_session = st.selectbox("Select Session", sessions)
                    session_path = os.path.join(date_path, selected_session)
                    
                    # File type filter
                    file_type = st.radio("File Type", ["Excel (.xlsx)", "PDF (.pdf)", "All Files"], horizontal=True)
                    
                    # Get files based on filter
                    all_files = [f for f in os.listdir(session_path) if f.endswith(('.xlsx', '.pdf'))]
                    if file_type == "Excel (.xlsx)":
                        room_files = [f for f in all_files if f.endswith('.xlsx')]
                    elif file_type == "PDF (.pdf)":
                        room_files = [f for f in all_files if f.endswith('.pdf')]
                    else:
                        room_files = all_files
                    
                    if room_files:
                        selected_file = st.selectbox("Select Room File", sorted(room_files))
                        file_path = os.path.join(session_path, selected_file)
                        
                        try:
                            # Show file preview based on type
                            if selected_file.endswith('.xlsx'):
                                df_room = pd.read_excel(file_path)
                                st.dataframe(df_room, use_container_width=True)
                                
                                # Download buttons
                                col1, col2 = st.columns(2)
                                
                                with col1:
                                    # Excel download button
                                    with open(file_path, "rb") as f:
                                        st.download_button(
                                            label=f"Download Excel: {selected_file}",
                                            data=f,
                                            file_name=selected_file,
                                            mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
                                            use_container_width=True
                                        )
                                
                                with col2:
                                    # PDF download button
                                    pdf_file_path = file_path.replace('.xlsx', '.pdf')
                                    if os.path.exists(pdf_file_path):
                                        with open(pdf_file_path, "rb") as f:
                                            st.download_button(
                                                label=f"Download PDF: {selected_file.replace('.xlsx', '.pdf')}",
                                                data=f,
                                                file_name=selected_file.replace('.xlsx', '.pdf'),
                                                mime="application/pdf",
                                                use_container_width=True
                                            )
                                    else:
                                        st.info("PDF not yet generated")
                            
                            elif selected_file.endswith('.pdf'):
                                # PDF file selected
                                st.info("PDF file selected. Click download to view.")
                                
                                # Download buttons
                                col1, col2 = st.columns(2)
                                
                                with col1:
                                    # PDF download button
                                    with open(file_path, "rb") as f:
                                        st.download_button(
                                            label=f"Download PDF: {selected_file}",
                                            data=f,
                                            file_name=selected_file,
                                            mime="application/pdf",
                                            use_container_width=True
                                        )
                                
                                with col2:
                                    # Excel download button (if exists)
                                    excel_file_path = file_path.replace('.pdf', '.xlsx')
                                    if os.path.exists(excel_file_path):
                                        with open(excel_file_path, "rb") as f:
                                            st.download_button(
                                                label=f"Download Excel: {selected_file.replace('.pdf', '.xlsx')}",
                                                data=f,
                                                file_name=selected_file.replace('.pdf', '.xlsx'),
                                                mime="application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
                                                use_container_width=True
                                            )
                                    else:
                                        st.info("Excel file not found")
                        except Exception as e:
                            st.error(f"Error reading file: {str(e)}")
        else:
            st.warning(f"Output directory '{output_dir}' not found. Please generate seating arrangement first.")
    else:
        st.info("Please generate seating arrangement first in the 'Upload & Run' tab")

with tab3:
    st.header("About Seating Arrangement System")
    
    st.markdown("""
    ### Features
    
    - **Smart Allocation**: Allocates largest courses first for optimal room usage
    - **Building Optimization**: Prefers same building (B1/B2) for courses
    - **Adjacent Rooms**: Tries to use adjacent rooms for better movement
    - **Clash Detection**: Automatically detects and reports student clashes
    - **Flexible Modes**: 
      - **Dense**: Fills rooms to effective capacity
      - **Sparse**: Fills 50% of effective capacity per subject
    
    ### Input File Structure
    
    The input Excel file should contain the following sheets:
    
    1. **in_timetable**: Exam schedule with Date, Day, Morning, and Evening columns
    2. **in_course_roll_mapping**: Maps courses to student roll numbers
    3. **in_roll_name_mapping**: Maps roll numbers to student names
    4. **in_room_capacity**: Contains room numbers and their capacities
    
    ### Output Files
    
    - **Individual Room Files**: dd_mm_yyyy_subcode_classroom_session.xlsx in organized folders
    - **Overall Seating Arrangement**: Summary of all allocations
    - **Seats Left**: Summary of allocated and vacant seats per room
    
    ### Usage
    
    1. Upload your input Excel file
    2. Set buffer and mode (Dense/Sparse)
    3. Click "Generate Seating Arrangement"
    4. View and download results from the "View Results" tab
    """)
    
    st.markdown("---")
    st.markdown("**Version**: 1.0.0")
    st.markdown("**Built with**: Python, Streamlit, Pandas, OpenPyXL")