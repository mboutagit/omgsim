import numpy as np
import matplotlib
import uproot, awkward as ak
matplotlib.use('Qt5Agg')  # or 'TkAgg' if using Tkinter
import matplotlib.pyplot as plt

import ROOT
ROOT.gSystem.Load("liblibOMG.so")

import cppyy
# cppyy.set_debug(True)

# Open the ROOT file
# file = ROOT.TFile.Open("M2_30000evt_1run_worldMat.root")  # Replace with your filename

# # List contents of the file
# print("File contents:")
# file.ls()

file = uproot.open("/home/bouta-mohammed/work/cppm/lactel/trunk/build/M2_30000evt_1run_worldMat.root")
tree = file["Gen"]

# Suppose your tree has branches: "px", "py", "pz", "time"
# xpos = tree["gen/fGenXPos"].array("np")
# ypos = tree["gen/fGenYPos"].array("np")
zpos = tree["gen/fGenZPos"].array()
tmax = tree["Event/tmax"].array()

xpos_ak = tree["gen/fGenXPos"].array(library="ak")
ypos_ak = tree["gen/fGenYPos"].array(library="ak")

xpos = ak.to_numpy(ak.flatten(xpos_ak))
ypos = ak.to_numpy(ak.flatten(ypos_ak))

plt.hist2d(xpos, ypos, bins=50)
plt.show()

# Get the TTree
# tree = file.Get("Gen")  # Replace with actual TTree name

# Print all branch names
# print("\nBranches:")
# tree.GetListOfBranches().Print()

# xpos = []
# ypos = []

# h2 = ROOT.TH2F("h2", "2D Hist", 100, -8000, 8000, 100, -8000, 8000)
# for i in range(tree.GetEntries()):
#     tree.GetEntry(i)

#     xpos.append(tree.gen.fGenXPos)
#     ypos.append(tree.gen.fGenYPos)
#     # if(tree.Event.isDetected):
    #     print(f"xpos: {tree.Event.isDetected}")
    # h2.Fill(tree.gen.fGenXPos, tree.gen.fGenYPos)


    
# h2.Draw()

# Show a few entries using tree.Show()
# print("\nShowing first 5 entries with tree.Show():")
# for i in range(min(5, tree.GetEntries())):
#     print(f"\n--- Entry {i} ---")
#     tree.Show(i)

# Access and print values from branches manually
# print("\nReading entries manually:")
# for i in range(min(5, tree.GetEntries())):
#     tree.GetEntry(i)
#     print(f"Entry {i}:")

    # Replace with your actual branch names
    # try:
    #     print(f"  branch1 = {tree.gen.fGenXPos}")
    #     # print(f"  branch2 = {tree.Event}")
    # except AttributeError:
    #     print("  Branch names 'branch1' and 'branch2' not found. Replace with actual names.")

# Close the file
# ROOT.gPad.Update()
# ROOT.gApplication.Run()
file.Close()
