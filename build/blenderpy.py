#import subprocess, sys, os

#python_exec = sys.executable  # This is Blender's Python
#subprocess.call([python_exec, "-m", "ensurepip"])
#subprocess.call([python_exec, "-m", "pip", "install", "--upgrade", "pip"])
#subprocess.call([python_exec, "-m", "pip", "install", "uproot"])

import sys
print(sys.executable)
print(sys.path)

import bpy
import mathutils
import uproot
#create a cube of size 2x2x2
#bpy.ops.mesh.primitive_cube_add(size=2, location=(0, 0, -1))

file = uproot.open("/home/bouta-mohammed/work/cppm/lactel/trunk/build/M4_1000evt_1run_worldMat.root")
tree = file["Gen"]

# Suppose your tree has branches: "px", "py", "pz", "time"
px_array = (tree["gen/fGenXPos"].array() /1000) [tree["Event/isDetected"].array() == 1]
py_array = (tree["gen/fGenYPos"].array() / 1000) [tree["Event/isDetected"].array() == 1]
pz_array = (tree["gen/fGenZPos"].array() / 1000) [tree["Event/isDetected"].array() == 1]

pxd_array = (tree["gen/fGenXDir"].array()) [tree["Event/isDetected"].array() == 1]
pyd_array = (tree["gen/fGenYDir"].array()) [tree["Event/isDetected"].array() == 1]
pzd_array = (tree["gen/fGenZDir"].array()) [tree["Event/isDetected"].array() == 1]

time_array = tree["Event/tmax"].

# Create a new collection for the lines
coll = bpy.data.collections.new("GenSegments")
bpy.context.scene.collection.children.link(coll)

len=2

for i, (px, py, pz, pxd, pyd, pzd) in enumerate(zip(px_array, py_array, pz_array, pxd_array, pyd_array, pzd_array)):
    start = mathutils.Vector((px, py, pz))
    end   = mathutils.Vector((px + len*pxd, py + len*pyd, pz + len*pzd))

    # Create mesh with two points and one edge
    verts = [start, end]
    edges = [(0, 1)]
    faces = []

    mesh = bpy.data.meshes.new(f"seg_mesh_{i}")
    mesh.from_pydata(verts, edges, faces)
    mesh.update()

    obj = bpy.data.objects.new(f"seg_{i}", mesh)
    coll.objects.link(obj)
    
    
import math

def create_arrow(axis, color):
    bpy.ops.mesh.primitive_cylinder_add(
        radius=0.02, depth=2, location=(0, 0, 1)
    )
    arrow = bpy.context.object
    arrow.name = f"{axis}_axis"

    # Rotate & move depending on axis
    if axis == 'X':
        arrow.rotation_euler = (0, math.radians(90), 0)
        arrow.location = (1, 0, 0)
    elif axis == 'Y':
        arrow.rotation_euler = (math.radians(-90), 0, 0)
        arrow.location = (0, 1, 0)
    elif axis == 'Z':
        arrow.location = (0, 0, 1)

    # Add color
    mat = bpy.data.materials.new(name=f"{axis}_Mat")
    mat.diffuse_color = color
    arrow.data.materials.append(mat)
    
create_arrow('X', (1, 0, 0, 1))
create_arrow('Y', (0, 1, 0, 1))
create_arrow('Z', (0, 0, 1, 1))