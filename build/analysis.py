# import ROOT
import matplotlib.pyplot as plt
import numpy as py
import pandas as pd
import uproot 


file=uproot.open("default5runs10000evtsPerRun_all.root")
tree=file["Run"]
tree1=file["DetectedEvent"]

run=tree["Run"].array(library="np")
events = tree1.arrays(
    filter_name="DetectedEvent.*",
    how="zip",
)
# detEvt=tree["DetectedEvent"].array(library="np")
# test=tree1["DetectedEvent"].array(library="np")
# detEvents=file["Run"]
print(run["runNb"])
print(run["nGenEvent"])
print(run["nDetEvent"])
# print(run["nDetEvent"]/run["nGenEvent"]*100)
# tree1.show()
# print(events["DetectedEvent.nhit"])
# print(events["DetectedEvent.event_id"])

# ng = tree["Run/nGenEvent"].array(library="np")
# nd = tree["Run/nDetEvent"].array(library="np")
# ri = file["Run"]["Run"].arrays(filter_name="*", library="pd")
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

# print(len(dei["hits.charge"]), dei["hits.charge"].shape)
# print(len(dei["hits.time"]), dei["hits.time"].shape)

# plt.hist(run["nDetEvent"])
fig, ax = plt.subplots(figsize=(5, 2.7))
ax.scatter(run["runNb"],run["nDetEvent"]/run["nGenEvent"]*100, s=50, facecolor='C0', edgecolor='k')
ax.set_xlabel("run number")
ax.set_ylabel("Detection efficiency (%)")
# dei.plot.scatter(x="hits.time", y="y_chits.chargeolumn", title="Scatter plot from DataFrame")

plt.show()
# plt.scatter(run["nGenEvent"])
# plt.show()