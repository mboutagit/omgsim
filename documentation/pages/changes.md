# Changes log

## v3.0

Magor changes:
- Brownian photoelectron production model is added and it is the default option.
- Photon detection is described in the PMTOpticalModel class. The stepping action only checks the electron presence in the PMT vacuum (before the SteppingAction was checking the detection).

Minor changes:
- Hamamatsu R12199-02 glass was created reducing absorption (to almost 0) in the UV part. Previousle the standard Nautilus glass was used for it (and QE could not be tuned with it).
- Eta angle was added as a card for the DOMs rotation in the detector description.
