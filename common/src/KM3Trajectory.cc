#include "KM3Trajectory.hh"
#include "G4VisManager.hh"
#include "G4Track.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

KM3Trajectory::KM3Trajectory(const G4Track* track)
    : fTrajectory(new G4Trajectory()), fParticleName (track->GetParticleDefinition()->GetParticleName()) {
    // Store initial point of the trajectory
    //fTrajectoryPoints.push_back(track->GetPosition());
}

KM3Trajectory::~KM3Trajectory() {}

void KM3Trajectory::DrawTrajectory() const {
    G4cout << " ---------------------- test" << G4endl;
    //Check if visualization manager exists
    G4VisManager* visManager = (G4VisManager*)G4VisManager::GetConcreteInstance();
    if (!visManager) return;

    // Create a polyline for trajectory visualization
    G4Polyline polyline;

    // Set color based on particle type
    G4VisAttributes visAttributes;
    if (fParticleName == "gamma") {
        visAttributes.SetColour(G4Colour(1.0, 1.0, 0.0)); // Green for gamma
    } else if (fParticleName == "e-") {
        visAttributes.SetColour(G4Colour(0.0, 0.0, 1.0)); // Blue for electrons
    } else if (fParticleName == "proton") {
        visAttributes.SetColour(G4Colour(1.0, 0.0, 0.0)); // Red for protons
    } else {
        visAttributes.SetColour(G4Colour(0.5, 0.5, 0.5)); // Gray for others
    }
    visAttributes.SetLineWidth(2.0);

    // Apply visualization attributes to the polyline
    polyline.SetVisAttributes(&visAttributes);

    // Add points to the polyline
    // for (const auto& point : fTrajectoryPoints) {
    //     polyline.push_back(G4Point3D(point));
    // }

    // Draw the polyline
    visManager->Draw(polyline);
}
