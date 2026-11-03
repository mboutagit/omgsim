import ROOT
import matplotlib.pyplot as plt
import numpy as py
import pandas as pd
import uproot 


# amount=3
# print(f"the amount is amount", amount)
# his = ROOT.TH1F("his", "test Hist", 100, 0, 100)
# file=ROOT.TFile("/home/bouta-mohammed/work/cppm/lactel/trunk/build/results/last/all_M2.root", "OLD")
# tree=file.Get("Run")
# tree.ls()
# tree.Print()
# tree.Draw("nDetEvent")


file=uproot.open("/home/bouta-mohammed/work/cppm/lactel/trunk/build/results/last/all_M2.root")
# tree=file["Run"]

# run=tree["Run"].array(library="np")
detEvents=file["Run"]["Run"]
# print(detEvents["runNb"])

# ng = tree["Run/nGenEvent"].array(library="np")
# nd = tree["Run/nDetEvent"].array(library="np")
ri = file["Run"]["Run"].arrays(filter_name="*", library="pd")
# runs = pd.DataFrame(file["Run"]["Run"])
# dei = file["DetectedEvent"]["DetectedEvent"].arrays(filter_name="*", library="pd")
# print(runInfo.nGenEvent)
# print(dei.columns)



# df=tree.arrays(["Run/nGenEvent", "Run/nDetEvent", "Run/runNb", "Run/detName", "Run/expectedRate", "Run/acceptanceAngle", "Run/generationSurface"], library="pd")
# filter=df[df["Run/acceptanceAngle"] >=0]
# filter["Run/acceptanceAngle"].plot.hist(bins=100)
# plt.show()

# print(filter["Run/acceptanceAngle"])
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

plt.scatter(ri["acceptanceAngle"], ri["expectedRate"])
plt.show()
# plt.scatter(dei["hits"])
# plt.show()