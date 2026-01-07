import ROOT
ROOT.gSystem.Load("libBAnalysis.so")

# Open the ROOT file
file = ROOT.TFile("default.root", "READ")
if file.IsZombie():
    print("Error opening file!")
    exit()

# Get the tree
tree = file.Get("Gen")
if not tree:
    print("Tree 'Gen' not found in file!")
    exit()

# Loop over entries in the tree
num_entries = tree.GetEntries()
print(f"Number of entries: {num_entries}")

tree.Print()

# for i in range(num_entries):
#     tree.GetEntry(i)
    
#     # Accessing leaves from the 'gen' branch
#     try:
#         gen_branch = getattr(tree, "gen")
#         for j in range(gen_branch.GetEntries()):
#             print(f"gen[{j}] = {gen_branch[j]}")
#     except AttributeError:
#         print("Branch 'gen' not found or does not contain indexed values")
    
#     # Accessing leaves from the 'event' branch
#     try:
#         event_branch = getattr(tree, "Event")
#         print(f"event = {event_branch}")
#     except AttributeError:
#         print("Branch 'Event' not found")

# Close the file
file.Close()
