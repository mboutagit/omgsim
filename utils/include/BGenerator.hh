#ifndef BGenerator_h
#define BGenerator_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

#include <memory>
#include <vector>

class G4ParticleGun;
class G4GenericMessenger;
class G4Event;


/**
 * BGenerator
 *
 * General-purpose Geant4 primary particle generator.
 *
 * Features:
 *
 *  - one or multiple particle species
 *  - arbitrary Geant4 particle names
 *  - particle weights
 *
 *  - fixed or randomly distributed position
 *
 *  - uniform energy distribution
 *  - power-law energy distribution
 *
 *  - standard angular distributions
 *  - modified Gaisser atmospheric muon flux
 *
 *  - correlated E_mu - theta sampling
 *
 *  - configurable theta and phi ranges
 *
 *  - configurable number of particles/event
 *
 *
 * The modified Gaisser model implemented here is the
 * sea-level atmospheric muon parameterization.
 *
 * Units:
 *
 *   Energy : Geant4 energy units
 *   Length : Geant4 length units
 *   Angle  : Geant4 angle units
 *
 */
class BGenerator : public G4VUserPrimaryGeneratorAction
{
public:

    // ---------------------------------------------------------------
    // Distribution modes
    // ---------------------------------------------------------------

    enum class EnergyDistribution
    {
        Uniform,
        PowerLaw,
        RealMuonFlux
    };

    enum class AngularDistribution
    {
        UniformTheta,
        Isotropic,
        CosinePower,
        RealMuonFlux
    };

    // ---------------------------------------------------------------
    // Constructor / destructor
    // ---------------------------------------------------------------

    BGenerator();

    ~BGenerator() override;

    // ---------------------------------------------------------------
    // Main Geant4 interface
    // ---------------------------------------------------------------

    void GeneratePrimaries(G4Event* event) override;

    // ---------------------------------------------------------------
    // Particle configuration
    // ---------------------------------------------------------------

    void ClearParticles();

    void AddParticle(
        const G4String& particleName,
        G4double weight = 1.0
    );

    void SetParticle(
        const G4String& particleName
    );

    void SetMuonChargeRatio(
        G4double ratio
    );

    // ---------------------------------------------------------------
    // Energy configuration
    // ---------------------------------------------------------------

    void SetEnergyRange(
        G4double emin,
        G4double emax
    );

    void SetEnergyDistribution(
        EnergyDistribution distribution
    );

    void SetPowerLawIndex(
        G4double index
    );

    // ---------------------------------------------------------------
    // Position configuration
    // ---------------------------------------------------------------

    void SetPosition(
        const G4ThreeVector& position
    );

    void SetPositionRange(
        const G4ThreeVector& minPosition,
        const G4ThreeVector& maxPosition
    );

    void SetRandomPosition(
        G4bool value
    );

    // ---------------------------------------------------------------
    // Angular configuration
    // ---------------------------------------------------------------

    void SetThetaRange(
        G4double thetaMin,
        G4double thetaMax
    );

    void SetPhiRange(
        G4double phiMin,
        G4double phiMax
    );

    void SetAngularDistribution(
        AngularDistribution distribution
    );

    void SetCosinePower(
        G4double power
    );

    // ---------------------------------------------------------------
    // Real muon flux
    // ---------------------------------------------------------------

    void SetMuonFluxNormalization(
        G4double scale
    );

    void SetRealFluxUseChargeRatio(
        G4bool value
    );

    // ---------------------------------------------------------------
    // Multiplicity
    // ---------------------------------------------------------------

    void SetNumberOfParticles(
        G4int number
    );

    // ---------------------------------------------------------------
    // Diagnostics
    // ---------------------------------------------------------------

    void PrintConfiguration();

private:

    // ---------------------------------------------------------------
    // Sampling
    // ---------------------------------------------------------------

    G4double SampleEnergy() const;

    G4double SampleTheta() const;

    G4double SamplePhi() const;

    void SampleMuonFlux(
        G4double& energy,
        G4double& theta
    ) const;

    G4double SampleUniformEnergy() const;

    G4double SamplePowerLawEnergy() const;

    G4double SampleUniformTheta() const;

    G4double SampleIsotropicTheta() const;

    G4double SampleCosinePowerTheta() const;

    G4ThreeVector SamplePosition() const;

    G4String SampleParticle() const;

    G4ThreeVector MakeDirection(
        G4double theta,
        G4double phi
    ) const;

    // ---------------------------------------------------------------
    // Modified Gaisser flux
    // ---------------------------------------------------------------

    G4double MuonFlux(
        G4double energy,
        G4double theta
    ) const;

    G4double EffectiveCosTheta(
        G4double theta
    ) const;

    // ---------------------------------------------------------------
    // Rejection sampling support
    // ---------------------------------------------------------------

    void UpdateFluxMaximum();

    G4double GetFluxSamplingWeight(
        G4double energy,
        G4double cosTheta
    ) const;

    // ---------------------------------------------------------------
    // Validation
    // ---------------------------------------------------------------

    void ValidateParameters() const;

    // ---------------------------------------------------------------
    // UI
    // ---------------------------------------------------------------

    void DefineCommands();

private:

    // ---------------------------------------------------------------
    // Geant4 particle gun
    // ---------------------------------------------------------------

    std::unique_ptr<G4ParticleGun> fParticleGun;

    // ---------------------------------------------------------------
    // Particle configuration
    // ---------------------------------------------------------------

    std::vector<G4String> fParticleNames;

    std::vector<G4double> fParticleWeights;

    // mu+ / mu- charge ratio:
    //
    // R = N(mu+) / N(mu-)
    //
    G4double fMuonChargeRatio;

    G4bool fRealFluxUseChargeRatio;

    // ---------------------------------------------------------------
    // Energy
    // ---------------------------------------------------------------

    G4double fEnergyMin;

    G4double fEnergyMax;

    EnergyDistribution fEnergyDistribution;

    G4double fPowerLawIndex;

    // ---------------------------------------------------------------
    // Position
    // ---------------------------------------------------------------

    G4ThreeVector fPosition;

    G4ThreeVector fPositionMin;

    G4ThreeVector fPositionMax;

    G4bool fRandomPosition;

    // ---------------------------------------------------------------
    // Angular range
    // ---------------------------------------------------------------

    G4double fThetaMin;

    G4double fThetaMax;

    G4double fPhiMin;

    G4double fPhiMax;

    AngularDistribution fAngularDistribution;

    G4double fCosinePower;

    // ---------------------------------------------------------------
    // Muon flux
    // ---------------------------------------------------------------

    G4double fMuonFluxScale;

    /*
     * Maximum value used by rejection sampling.
     *
     * The sampling proposal is:
     *
     *       dE/E * d(cos theta)
     *
     * therefore the rejection weight is proportional to:
     *
     *       E * Flux(E,theta)
     */
    mutable G4double fFluxMaximum;

    // ---------------------------------------------------------------
    // Multiplicity
    // ---------------------------------------------------------------

    G4int fNumberOfParticles;

    // ---------------------------------------------------------------
    // UI messenger
    // ---------------------------------------------------------------

    std::unique_ptr<G4GenericMessenger> fMessenger;
};

#endif