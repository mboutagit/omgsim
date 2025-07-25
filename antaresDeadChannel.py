import cv2
import numpy as np
import pandas as pd
from sklearn.cluster import KMeans
import matplotlib.pyplot as plt
import ROOT

print("Start processing ... ")

# Load image
image = cv2.imread('antares.png')
rgb = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# Resize or crop legend if needed (here we manually define the categories)
legend_colors = {
    "missing": (255, 0, 0),
    "empty": (0, 0, 0),
    "low": (153, 51, 255),
    "high": (102, 204, 255),
    "ok": (255, 153, 51)
}

# Find all keypoints (clover centers) using blob detection
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
_, binary = cv2.threshold(gray, 50, 255, cv2.THRESH_BINARY_INV)
params = cv2.SimpleBlobDetector_Params()
params.filterByArea = True
params.minArea = 30
params.maxArea = 1000
detector = cv2.SimpleBlobDetector_create(params)
keypoints = detector.detect(binary)

# Prepare output
data = []

# Convert image to RGB for color sampling
rgb_image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)

# Map clover positions
for kp in keypoints:
    x, y = int(kp.pt[0]), int(kp.pt[1])
    color = tuple(rgb_image[y, x])  # RGB
    r, g, b = color

    # Estimate line (1–12) and storey (0–25)
    line = int(round((x - 80) / 50)) + 1  # X spacing estimation
    storey = 25 - int(round((y - 50) / 20))  # Y inverted

    # Estimate position: left(0), middle(1), right(2) based on relative x in cluster
    offset = (x - 80) % 50
    if offset < 10:
        pos = 0
    elif offset < 25:
        pos = 1
    else:
        pos = 2

    # Match color to legend category
    matched_category = "unknown"
    for name, ref_color in legend_colors.items():
        if np.linalg.norm(np.array(color) - np.array(ref_color)) < 60:
            matched_category = name
            break

    data.append({
        'line': line,
        'storey': storey,
        'position': pos,
        'category': matched_category,
        'r': r, 'g': g, 'b': b
    })

# Sort data by line, storey, position
df = pd.DataFrame(data)
df = df.sort_values(by=['line', 'storey', 'position'])

# Save
df.to_csv('clover_positions.csv', index=False)
print("Saved to clover_positions.csv")
