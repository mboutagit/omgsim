import ROOT
import matplotlib.pyplot as plt
import numpy as py
import pandas as pd
import uproot 


file=uproot.open("default.root")
# tree=file["Run"]

# run=tree["Run"].array(library="np")
detEvents=file["Run"]["Run"]
# print(detEvents["runNb"])

# ng = tree["Run/nGenEvent"].array(library="np")
# nd = tree["Run/nDetEvent"].array(library="np")
# ri = file["Run"]["Run"].arrays(filter_name="*", library="pd")
# runs = pd.DataFrame(file["Run"]["Run"])
dei = file["DetectedEvent"]["DetectedEvent"].arrays(filter_name="*", library="pd")
# print(runInfo.nGenEvent)
# print(dei.columns)



# df=tree.arrays(["Run/nGenEvent", "Run/nDetEvent", "Run/runNb", "Run/detName", "Run/expectedRate", "Run/acceptanceAngle", "Run/generationSurface"], library="pd")
# filter=df[df["Run/acceptanceAngle"] >=0]
# filter["Run/acceptanceAngle"].plot.hist(bins=100)
# plt.show()

# print(filter["Run/acceptanceAngle"])
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')

print(len(dei["hits.charge"]), dei["hits.charge"].shape)
print(len(dei["hits.time"]), dei["hits.time"].shape)

plt.hist(dei["hits.charge"])
# dei.plot.scatter(x="hits.time", y="y_chits.chargeolumn", title="Scatter plot from DataFrame")

plt.show()
# plt.scatter(dei["hits"])
# plt.show()