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
tree=file["Run"]

run=tree["Run"].array(library="np")

ng = tree["Run/nGenEvent"].array(library="np")
nd = tree["Run/nDetEvent"].array(library="np")

df=tree.arrays(["Run/nGenEvent", "Run/nDetEvent", "Run/runNb", "Run/detName", "Run/expectedRate", "Run/acceptanceAngle", "Run/generationSurface"], library="pd")
filter=df[df["Run/acceptanceAngle"] > 40]
filter["Run/acceptanceAngle"].plot.hist(bins=100)
plt.show()

print(filter["Run/acceptanceAngle"])

# plt.scatter(ng, nd, s=5, alpha=0.5)
# plt.title("Test Plot")
# plt.ylabel("Number of detected events")
# plt.show()
