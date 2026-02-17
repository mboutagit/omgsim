#!/usr/bin/env python3

# import ROOT
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import uproot 
import argparse
import sys
import awkward as ak
from pathlib import Path
from scipy import constants as const



def parse_arguments():
    """Parse command-line arguments."""
    parser = argparse.ArgumentParser(
        description="Process a ROOT file using uproot"
    )

    parser.add_argument(
        "root_file",
        type=Path,
        help="Path to the .root file"
    )

    return parser.parse_args()


def check_root_file(path: Path):
    """Check that the ROOT file exists and can be opened by uproot."""
    if not path.exists():
        print(f"ERROR: File does not exist: {path}", file=sys.stderr)
        sys.exit(1)

    if not path.is_file():
        print(f"ERROR: Path is not a file: {path}", file=sys.stderr)
        sys.exit(1)

    if path.suffix != ".root":
        print(f"ERROR: Not a ROOT file: {path}", file=sys.stderr)
        sys.exit(1)

    try:
        root_file = uproot.open(path)
    except Exception as e:
        print(f"ERROR: Cannot open ROOT file: {path}", file=sys.stderr)
        print(f"Reason: {e}", file=sys.stderr)
        sys.exit(1)

    return root_file


def process_root_file(root_file):
    """Main analysis logic."""
    print(f"Successfully opened ROOT file")

    # List top-level keys
    print("File contents:")
    for key in root_file.keys():
        print(f" - {key}")

    # Example: read a TTree
    tree_name = "DetectedEvent"
    if tree_name in root_file:
        tree = root_file[tree_name]
        print(f"Tree '{tree_name}' entries: {tree.num_entries}")

        # Example: read branches as numpy arrays
        arrays = tree.arrays(library="np")
        print(f"Loaded branches: {list(arrays.keys())}")
    else:
        print(f"WARNING: Tree '{tree_name}' not found")

def transfomre_nhit_to_charge(gain, nhit):
    return gain*nhit*const.e
    


def main():
    args = parse_arguments()

    # Early validation
    root_file = check_root_file(args.root_file)

    # Run analysis
    # process_root_file(root_file)

    tree = root_file["DetectedEvent"]

    branches = tree.arrays(["hits.om_id", "hits.charge", "hits.time"],
    library="ak"
    )

    maskom0= branches["hits.om_id"] == 1
    maskom1 = branches["hits.om_id"] == 2

    timemask = branches["hits.time"] < 1e5

    timeom0masks = maskom0 & timemask
    timeom1masks = maskom1 & timemask
    hits_om0 = branches["hits.time"][timeom0masks]
    hits_om1 = branches["hits.time"][timeom1masks]

    sizes_om0 = ak.num(hits_om0)
    sizes_om1 = ak.num(hits_om1)

    masks0 = sizes_om0 < 1e7
    masks1 = sizes_om1 < 1e7

    # for c in hits_om0:
        # print(f"charge: ", c)

    radius = 6.19 # in meters
    gen_surface_m2  = const.pi * radius*radius # pi*R²
    gen_surface_cm2 = gen_surface_m2 * 1e4

    expected_muons_per_m2_per_s = 147 # by m²
    expected_muons_per_cm2_per_s = expected_muons_per_m2_per_s/1e4 # to cm²
    expected_muons_per_gensurf = expected_muons_per_cm2_per_s * gen_surface_cm2

    number_of_generated_muons_per_gensurf = 100000 # 100 000 events
    exposure_time = number_of_generated_muons_per_gensurf / expected_muons_per_gensurf

    w0 = np.ones(len(sizes_om0[masks0])) * 1/exposure_time
    w1 = np.ones(len(sizes_om1[masks1])) * 1/exposure_time

    print(f"expected_muons_per_gensurf: ", expected_muons_per_gensurf)
    print(f"gen_surface_cm2: ", gen_surface_cm2)
    print(f"exposure time in s: ", exposure_time)

    xmin, xmax = 0, 600 # from 0 to 200 PE
    bin_width=1 # 1 PE per bin
    bin_edges = np.arange(xmin, xmax + bin_width, bin_width)  # edges: 0, 0.5, 1.0, ..., 10

    # Compute histogram — only counts inside [xmin, xmax] are used
    hist, edges = np.histogram(sizes_om0[masks0], bins=bin_edges)
    hist1, edges1 = np.histogram(sizes_om1[masks1], bins=bin_edges)


    plt.figure(figsize=(8, 5), layout='constrained')
    # plt.hist([ak.flatten(hits_om0), ak.flatten(hits_om1)],
    plt.hist([sizes_om0[masks0], sizes_om1[masks1]],
        bins=bin_edges,
        weights=[w0, w1],
        histtype="step",
        label=["OM 0 - Upper EC", "OM 1 - Lower EC"],
        alpha = 1,
        lw=1
    )
    plt.xlabel("Number of PE")
    plt.ylabel("Rate per 1 PE bin [Hz]")
    plt.title("")
    plt.grid(True)
    plt.legend()
    plt.yscale('log')
    plt.text(0.80, 0.65,
                "Hits form simualtion\nTrigger: No trigger \nThr: 1PE \nGain: Not applied",
                transform=plt.gca().transAxes,
                bbox=dict(boxstyle='round', facecolor='#55ffff', alpha=0.8)
            )
    plt.show()


if __name__ == "__main__":
    main()
