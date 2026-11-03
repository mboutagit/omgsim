## Output files format
The following trees are created:

- Hit a tree with information about the hits (OMGsim needs to be run with "-fa" full analysis option).
    - VertexX, Y, Z, Z, T, P, DirX, DirY, DirZ, Energy - photon information at the creation vertex.
         - T is theta angle of the OM, P - theta angle of the OM
    - OMHitX, Y, Z, T, P - photon information at arrival to the Optical Module (not filled for now).
    - PhotoX, Y, Z, T, P - photon information at arrival to the Photocathode. 
    - OMID, PMID - IDs of the DOM and PMT which have detected the light.
    - Limit hit limit (select with 2 and 3 to use average QE).
