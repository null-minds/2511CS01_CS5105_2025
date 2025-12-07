import streamlit as st
import pandas as pd
import math, io, zipfile
from datetime import datetime

st.set_page_config(page_title="Groupify Algorithm", layout="wide")
st.title("Groupify Algorithm")
st.markdown("Upload CSV file & number of groups to create balanced student groups.")

# ---------- Helpers ----------
def createBranchFullList(df):
    branches = sorted(df["Branch"].unique())
    branchData, branchFiles = {}, {}
    for b in branches:
        bdf = df[df["Branch"] == b].sort_values("Roll").reset_index(drop=True)
        branchData[b] = bdf
        buf = io.StringIO(); bdf.to_csv(buf, index=False)
        branchFiles[f"{b}.csv"] = buf.getvalue()
    return branchData, branchFiles, branches

def createBranchwiseMix(branchData, branches, nGroups):
    groups = [pd.DataFrame(columns=df.columns) for _ in range(nGroups)]
    iters = {b: 0 for b in branches}
    target = math.ceil(sum(len(branchData[b]) for b in branches) / nGroups)
    for g in range(nGroups):
        while len(groups[g]) < target:
            added = False
            for b in branches:
                if iters[b] < len(branchData[b]):
                    row = branchData[b].iloc[iters[b]]
                    groups[g] = pd.concat([groups[g], row.to_frame().T], ignore_index=True)
                    iters[b] += 1; added = True
                    if len(groups[g]) >= target: break
            if not added: break
    return groups

def createUniformMix(branchData, branches, nGroups):
    total = sum(len(branchData[b]) for b in branches)
    size = math.ceil(total / nGroups)
    sortedBranches = sorted(branches, key=lambda x: len(branchData[x]), reverse=True)
    groups, g = [pd.DataFrame(columns=df.columns) for _ in range(nGroups)], 0
    for b in sortedBranches:
        students, i = branchData[b], 0
        while i < len(students):
            free = size - len(groups[g])
            take = min(free, len(students) - i)
            groups[g] = pd.concat([groups[g], students.iloc[i:i+take]], ignore_index=True)
            i += take; g = (g + 1) % nGroups
    return groups

def createGroupFiles(groups):
    files, stats = {}, []
    for i, g in enumerate(groups, 1):
        if not g.empty:
            g = g.sort_values(['Branch', 'Roll']).reset_index(drop=True)
            buf = io.StringIO(); g.to_csv(buf, index=False)
            files[f"G{i}.csv"] = buf.getvalue()
            d = g["Branch"].value_counts().to_dict(); d.update({"Total": len(g), "Group": f"G{i}"})
            stats.append(d)
    return files, stats

def createCombinedStats(bwStats, uniStats, branches):
    bw, uni = pd.DataFrame(bwStats).fillna(0), pd.DataFrame(uniStats).fillna(0)
    for df in [bw, uni]:
        for c in df.select_dtypes(include="float64"): 
            df[c] = df[c].astype(int)

    def section(df, label): 
        hdr = pd.DataFrame([[label] + branches + ["Total"]], columns=['Group']+branches+['Total'])
        return pd.concat([hdr, df[['Group']+branches+['Total']]], ignore_index=True)

    outLines = []
    for _, row in section(bw, "Mix").iterrows():
        outLines.append(','.join(map(str, row.values)))
    outLines.append("")  # blank line
    for _, row in section(uni, "Uniform").iterrows():
        outLines.append(','.join(map(str, row.values)))
    return "\n".join(outLines)

def createZip(branchFiles, bwFiles, uniFiles, stats):
    buf = io.BytesIO()
    with zipfile.ZipFile(buf, 'w', zipfile.ZIP_DEFLATED) as z:
        for f,c in branchFiles.items(): z.writestr(f"Branch_Full_List/{f}",c)
        for f,c in bwFiles.items():     z.writestr(f"Branchwise_Mix/{f}",c)
        for f,c in uniFiles.items():    z.writestr(f"Uniform_Mix/{f}",c)
        z.writestr("Combined_Stats.csv", stats)
    buf.seek(0); return buf.getvalue()

# ---------- Inputs ----------
st.sidebar.header("Input Parameters")
uploaded = st.sidebar.file_uploader("Upload CSV File", type=['csv'])
nGroups = st.sidebar.number_input("Number of Groups", 1, 50, 5)
process = st.sidebar.button("Process Groups")
if st.sidebar.button("Clear All Data"): st.session_state.clear(); st.rerun()

# ---------- Main ----------
if uploaded or "df" in st.session_state:
    if uploaded:
        try:
            df = pd.read_csv(uploaded)
            if not all(c in df.columns for c in ["Roll","Name","Email"]):
                st.error("Missing required columns."); st.stop()
            df["Branch"] = df["Roll"].astype(str).str[4:6]
            st.session_state.df, st.session_state.filename = df, uploaded.name
        except Exception as e: st.error(f"{e}"); st.stop()

    df = st.session_state.df
    st.info(f"Current file: *{st.session_state.filename}*")

    if process or "results" in st.session_state:
        if process or "results" not in st.session_state:
            with st.spinner("Processing groups..."):
                branchData, branchFiles, branches = createBranchFullList(df)
                bwGroups = createBranchwiseMix(branchData, branches, nGroups)
                uniGroups = createUniformMix(branchData, branches, nGroups)
                bwFiles,bwStats = createGroupFiles(bwGroups)
                uniFiles,uniStats = createGroupFiles(uniGroups)
                stats = createCombinedStats(bwStats, uniStats, sorted(set(df["Branch"])))
                zipData = createZip(branchFiles,bwFiles,uniFiles,stats)
                st.session_state.results = {"bwStats":bwStats,"stats":stats,"zip":zipData,
                                            "branch":branchFiles,"bw":bwFiles,"uni":uniFiles,
                                            "n":nGroups,"total":len(df)}

        r = st.session_state.results
        st.success("Groups created successfully!")

        st.download_button("Download All Files (ZIP)", r["zip"],
                           f"Student_Groups_{datetime.now().strftime('%Y%m%d_%H%M%S')}.zip",
                           "application/zip")

        with st.expander("Individual File Downloads"):
            st.download_button("Combined Statistics", r["stats"], "Combined_Stats.csv","text/csv")
            for label,files in {"Branch Full Lists":r["branch"],"Branchwise Mix":r["bw"],"Uniform Mix":r["uni"]}.items():
                st.write(f"*{label}:*")
                cols = st.columns(min(len(files),5))
                for i,(f,c) in enumerate(files.items()):
                    with cols[i%5]: 
                        st.download_button(f"{f}",c,f,"text/csv",key=f"{label}_{f}")
else:
    st.info("Upload a CSV file to get started")

st.markdown("---\nStudent Grouping System - Streamlit Web App")
