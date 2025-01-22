#include "KM3Trajectory.hh"

  //Constructor (initialize based on the track)
KM3Trajectory::KM3Trajectory(const G4Track* track) {
   // Initialization logic if needed
}

KM3Trajectory::~KM3Trajectory() {}

void KM3Trajectory::DrawTrajectory() const
{
    cout << "\r\033[50C" << " Hello from KM3Trajectory" << endl;
    // Access the visualization manager
    G4VisManager* visManager = static_cast<G4VisManager*>(G4VisManager::GetConcreteInstance());
    if (!visManager) return;

    // Create a polyline to represent the trajectory
    G4Polyline polyline;

    // Customize attributes
    G4VisAttributes visAttributes;
    visAttributes.SetLineWidth(2);

    const G4String particleName = "gamma"; // Replace with logic to get the particle name

    // Assign color based on particle type
    if (particleName == "gamma") {
        visAttributes.SetColour(G4Colour(0.0, 1.0, 0.0)); // Green for gamma
    } else if (particleName == "e-") {
        visAttributes.SetColour(G4Colour(0.0, 1.0, 0.0)); // Blue for electrons
    } else if (particleName == "proton") {
        visAttributes.SetColour(G4Colour(0.0, 1.0, 0.0)); // Red for protons
    } else {
        visAttributes.SetColour(G4Colour(0.5, 0.5, 0.5)); // Gray for others
    }

    // Apply attributes
    polyline.SetVisAttributes(&visAttributes);

    // Add trajectory points (example; populate with actual data)
    // For each point, add to the polyline:
    // polyline.push_back(G4Point3D(x, y, z));

    // Draw the trajectory
    visManager->Draw(polyline);
}

    // G4VTrajectory* Clone() const {
    //     return new KM3Trajectory(*this);
    // }