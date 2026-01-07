import ROOT

# Open the ROOT file
file = ROOT.TFile("results/allmuons_10EvtR_vertical_2-6GeV_r0-20.root")
if not file or file.IsZombie():
    print(f"Error: Could not open file $file")
    exit()

# Get the tree
tree = file.Get("Hits")
if not tree:
    print("Error: Tree 'Hits' not found in the file")
    exit()

# Create a histogram
hist = ROOT.TH1F("hist", "Histogram of XXX;XXX values;Entries", 50, tree.GetMinimum("XXX"), tree.GetMaximum("XXX"))

# Fill histogram from the tree
tree.Draw("XXX >> hist")

# Draw the histogram
canvas = ROOT.TCanvas("canvas", "Canvas", 800, 600)
hist.Draw()

# Show the plot
canvas.Update()
input("Press Enter to exit...")  # Keeps the plot open

# Save histogram as an image
canvas.SaveAs("histogram.png")

# Close the file
file.Close()
