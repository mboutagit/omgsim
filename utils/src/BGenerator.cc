#include "BGenerator.hh"

#include "G4Event.hh"
#include "G4GenericMessenger.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "Randomize.hh"

#include <algorithm>
#include <cmath>
#include <numeric>


// ====================================================================
// Constructor
// ====================================================================

BGenerator::BGenerator()
    : G4VUserPrimaryGeneratorAction(),

      fParticleGun(
          std::make_unique<G4ParticleGun>(1)
      ),

      fParticleNames(),

      fParticleWeights(),

      fMuonChargeRatio(1.27),

      fRealFluxUseChargeRatio(true),

      fEnergyMin(1.0 * GeV),

      fEnergyMax(1.0e4 * GeV),

      fEnergyDistribution(
          EnergyDistribution::RealMuonFlux
      ),

      fPowerLawIndex(2.7),

      fPosition(0., 0., 0.),

      fPositionMin(0., 0., 0.),

      fPositionMax(0., 0., 0.),

      fRandomPosition(false),

      fThetaMin(0. * deg),

      fThetaMax(70. * deg),

      fPhiMin(0. * deg),

      fPhiMax(360. * deg),

      fAngularDistribution(
          AngularDistribution::RealMuonFlux
      ),

      fCosinePower(2.0),

      fMuonFluxScale(1.0),

      fFluxMaximum(1.0),

      fNumberOfParticles(1),

      fMessenger(nullptr)
{
    /*
     * Default:
     *
     *   mu- + mu+
     *
     * with approximately the measured charge ratio
     * N(mu+)/N(mu-) = 1.27.
     */

    AddParticle("mu-", 1.0);

    AddParticle(
        "mu+",
        fMuonChargeRatio
    );

    UpdateFluxMaximum();

    DefineCommands();
}


// ====================================================================
// Destructor
// ====================================================================

BGenerator::~BGenerator()
{
}


// ====================================================================
// Particle configuration
// ====================================================================

void BGenerator::ClearParticles()
{
    fParticleNames.clear();
    fParticleWeights.clear();
}


void BGenerator::AddParticle(
    const G4String& particleName,
    G4double weight)
{
    if (weight <= 0.0)
    {
        G4Exception(
            "BGenerator::AddParticle",
            "InvalidWeight",
            FatalException,
            "Particle weight must be > 0."
        );
    }

    fParticleNames.push_back(
        particleName
    );

    fParticleWeights.push_back(
        weight
    );
}


void BGenerator::SetParticle(
    const G4String& particleName)
{
    ClearParticles();

    AddParticle(
        particleName,
        1.0
    );
}


void BGenerator::SetMuonChargeRatio(
    G4double ratio)
{
    if (ratio <= 0.0)
    {
        G4Exception(
            "BGenerator::SetMuonChargeRatio",
            "InvalidChargeRatio",
            FatalException,
            "Muon charge ratio must be > 0."
        );
    }

    fMuonChargeRatio = ratio;
}


// ====================================================================
// Energy configuration
// ====================================================================

void BGenerator::SetEnergyRange(
    G4double emin,
    G4double emax)
{
    if (emin <= 0.0 || emax <= emin)
    {
        G4Exception(
            "BGenerator::SetEnergyRange",
            "InvalidEnergyRange",
            FatalException,
            "Require 0 < Emin < Emax."
        );
    }

    fEnergyMin = emin;
    fEnergyMax = emax;

    UpdateFluxMaximum();
}


void BGenerator::SetEnergyDistribution(
    EnergyDistribution distribution)
{
    fEnergyDistribution =
        distribution;

    UpdateFluxMaximum();
}


void BGenerator::SetPowerLawIndex(
    G4double index)
{
    if (index <= 0.0)
    {
        G4Exception(
            "BGenerator::SetPowerLawIndex",
            "InvalidIndex",
            FatalException,
            "Power-law index must be > 0."
        );
    }

    fPowerLawIndex = index;
}


// ====================================================================
// Position
// ====================================================================

void BGenerator::SetPosition(
    const G4ThreeVector& position)
{
    fPosition = position;

    fRandomPosition = false;
}


void BGenerator::SetPositionRange(
    const G4ThreeVector& minPosition,
    const G4ThreeVector& maxPosition)
{
    fPositionMin = minPosition;

    fPositionMax = maxPosition;

    fRandomPosition = true;
}


void BGenerator::SetRandomPosition(
    G4bool value)
{
    fRandomPosition = value;
}


// ====================================================================
// Angular configuration
// ====================================================================

void BGenerator::SetThetaRange(
    G4double thetaMin,
    G4double thetaMax)
{
    if (thetaMin < 0.0 ||
        thetaMax > 180.0 * deg ||
        thetaMin >= thetaMax)
    {
        G4Exception(
            "BGenerator::SetThetaRange",
            "InvalidThetaRange",
            FatalException,
            "Invalid theta range."
        );
    }

    fThetaMin = thetaMin;

    fThetaMax = thetaMax;

    UpdateFluxMaximum();
}


void BGenerator::SetPhiRange(
    G4double phiMin,
    G4double phiMax)
{
    if (phiMax <= phiMin)
    {
        G4Exception(
            "BGenerator::SetPhiRange",
            "InvalidPhiRange",
            FatalException,
            "Invalid phi range."
        );
    }

    fPhiMin = phiMin;

    fPhiMax = phiMax;
}


void BGenerator::SetAngularDistribution(
    AngularDistribution distribution)
{
    fAngularDistribution =
        distribution;

    UpdateFluxMaximum();
}


void BGenerator::SetCosinePower(
    G4double power)
{
    if (power < 0.0)
    {
        G4Exception(
            "BGenerator::SetCosinePower",
            "InvalidPower",
            FatalException,
            "Cosine power must be >= 0."
        );
    }

    fCosinePower = power;
}


// ====================================================================
// Muon flux configuration
// ====================================================================

void BGenerator::SetMuonFluxNormalization(
    G4double scale)
{
    if (scale <= 0.0)
    {
        G4Exception(
            "BGenerator::SetMuonFluxNormalization",
            "InvalidScale",
            FatalException,
            "Muon flux scale must be > 0."
        );
    }

    fMuonFluxScale = scale;

    UpdateFluxMaximum();
}


void BGenerator::SetRealFluxUseChargeRatio(
    G4bool value)
{
    fRealFluxUseChargeRatio = value;
}


// ====================================================================
// Multiplicity
// ====================================================================

void BGenerator::SetNumberOfParticles(
    G4int number)
{
    if (number < 1)
    {
        G4Exception(
            "BGenerator::SetNumberOfParticles",
            "InvalidMultiplicity",
            FatalException,
            "Number of particles must be >= 1."
        );
    }

    fNumberOfParticles = number;
}


// ====================================================================
// Modified Gaisser parameterization
// ====================================================================

G4double BGenerator::EffectiveCosTheta(
    G4double theta) const
{
    const G4double c =
        std::cos(theta);

    /*
     * Guan / modified Gaisser atmospheric
     * curvature correction.
     *
     * p1 = 0.102573
     * p2 = 0.068287
     * p3 = 0.958633
     * p4 = 0.0407253
     * p5 = 0.817285
     */

    constexpr G4double p1 = 0.102573;
    constexpr G4double p2 = 0.068287;
    constexpr G4double p3 = 0.958633;
    constexpr G4double p4 = 0.0407253;
    constexpr G4double p5 = 0.817285;

    const G4double numerator =
        c * c
        + p1 * p1
        + p2 * std::pow(c, p3)
        + p4 * std::pow(c, p5);

    const G4double denominator =
        1.0
        + p1 * p1
        + p2
        + p4;

    return std::sqrt(
        numerator / denominator
    );
}


// ====================================================================
// Muon flux
// ====================================================================

G4double BGenerator::MuonFlux(
    G4double energy,
    G4double theta) const
{
    if (energy <= 0.0)
    {
        return 0.0;
    }

    /*
     * Energy expressed in GeV.
     */

    const G4double E =
        energy / GeV;

    /*
     * Effective atmospheric zenith angle.
     */

    const G4double cosThetaStar =
        EffectiveCosTheta(theta);

    /*
     * Modified low-energy Gaisser expression:
     *
     *
     * dN
     * -----
     * dE dOmega
     *
     * =
     *
     * 0.14
     *
     * [
     * E
     * (1 + 3.64 /
     *      (E cos(theta*)^1.29))
     * ]^-2.7
     *
     * x
     *
     * [
     *
     * 1
     * -------------------------------
     * 1 + 1.1 E cos(theta*) / 115
     *
     * +
     *
     * 0.054
     * -------------------------------
     * 1 + 1.1 E cos(theta*) / 850
     *
     * ]
     *
     *
     * Units:
     *
     * cm^-2 s^-1 sr^-1 GeV^-1
     */

    const G4double lowEnergyFactor = 1.0 + 3.64 / ( E * std::pow(cosThetaStar,1.29));
    const G4double spectralTerm = std::pow(E * lowEnergyFactor,-2.7);
    const G4double pionTerm = 1.0 / ( 1.0 + 1.1 * E * cosThetaStar / 115.0);
    const G4double kaonTerm = 0.054 / ( 1.0 + 1.1 * E * cosThetaStar / 850.0 );

    return fMuonFluxScale * 0.14 * spectralTerm * ( pionTerm + kaonTerm );
}


// ====================================================================
// Flux sampling weight
// ====================================================================

G4double BGenerator::GetFluxSamplingWeight(G4double energy, G4double cosTheta) const {
    if (cosTheta <= 0.0){
        return 0.0;
    }

    const G4double theta = std::acos(cosTheta);

    /*
     * We propose:
     *
     *   dE/E
     *
     * and
     *
     *   d(cos theta)
     *
     * uniformly.
     *
     * The target distribution is:
     *
     *   Flux(E,theta) dE dOmega
     *
     * where:
     *
     *   dOmega = dphi d(cos theta)
     *
     * Therefore:
     *
     * target / proposal
     *
     * is proportional to:
     *
     *   Flux(E,theta) * E
     */

    return
        MuonFlux( energy, theta) * energy;
}


// ====================================================================
// Find rejection envelope
// ====================================================================

void BGenerator::UpdateFluxMaximum()
{
    if (fEnergyMin <= 0.0 ||
        fEnergyMax <= fEnergyMin)
    {
        return;
    }

    const G4int nE = 100;

    const G4int nTheta = 100;

    G4double maximum = 0.0;

    const G4double logEmin =
        std::log(fEnergyMin);

    const G4double logEmax =
        std::log(fEnergyMax);

    for (G4int i = 0;
         i < nE;
         ++i)
    {
        const G4double fractionE =
            static_cast<G4double>(i)
            /
            static_cast<G4double>(nE - 1);

        const G4double energy =
            std::exp(
                logEmin
                +
                fractionE *
                (logEmax - logEmin)
            );

        for (G4int j = 0;
             j < nTheta;
             ++j)
        {
            const G4double fractionTheta =
                static_cast<G4double>(j)
                /
                static_cast<G4double>(nTheta - 1);

            const G4double theta =
                fThetaMin
                +
                fractionTheta *
                (
                    fThetaMax
                    -
                    fThetaMin
                );

            const G4double cosTheta =
                std::cos(theta);

            if (cosTheta <= 0.0)
            {
                continue;
            }

            const G4double weight =
                GetFluxSamplingWeight(
                    energy,
                    cosTheta
                );

            maximum =
                std::max(
                    maximum,
                    weight
                );
        }
    }

    /*
     * Safety factor.
     *
     * The grid above is only an estimate of the maximum.
     */

    fFluxMaximum = 1.20 * maximum;

    if (fFluxMaximum <= 0.0) {
        fFluxMaximum = 1.0;
    }
}


// ====================================================================
// Real muon flux sampler
// ====================================================================

void BGenerator::SampleMuonFlux(
    G4double& energy,
    G4double& theta) const
{
    /*
     * Joint sampling from:
     *
     *       dN / dE dOmega
     *
     * using rejection sampling.
     *
     * Proposal:
     *
     *       log-uniform E
     *
     *       uniform cos(theta)
     *
     *       uniform phi
     *
     * Phi does not appear in the flux parameterization,
     * so it is sampled independently.
     */

    constexpr G4int maxTrials = 100000;

    for (G4int trial = 0;
         trial < maxTrials;
         ++trial)
    {
        /*
         * Log-uniform energy proposal.
         */

        const G4double uE =
            G4UniformRand();

        energy =
            std::exp(
                std::log(fEnergyMin)
                +
                uE *
                (
                    std::log(fEnergyMax)
                    -
                    std::log(fEnergyMin)
                )
            );

        /*
         * Uniform in cos(theta).
         *
         * This is uniform in solid angle.
         */

        const G4double cosMin =
            std::cos(fThetaMax);

        const G4double cosMax =
            std::cos(fThetaMin);

        const G4double uTheta =
            G4UniformRand();

        const G4double cosTheta =
            cosMin
            +
            uTheta *
            (
                cosMax - cosMin
            );

        if (cosTheta <= 0.0)
        {
            continue;
        }

        theta =
            std::acos(cosTheta);

        const G4double weight =
            GetFluxSamplingWeight(
                energy,
                cosTheta
            );

        const G4double acceptance =
            weight / fFluxMaximum;

        if (acceptance >= 1.0)
        {
            /*
             * The numerical envelope was too small.
             * Increase it and accept this event.
             */

            fFluxMaximum =
                1.25 * weight;

            return;
        }

        if (G4UniformRand() < acceptance)
        {
            return;
        }
    }

    G4Exception(
        "BGenerator::SampleMuonFlux",
        "FluxSamplingFailure",
        FatalException,
        "Muon flux rejection sampler exceeded maximum number of trials."
    );
}


// ====================================================================
// Energy sampling
// ====================================================================

G4double BGenerator::SampleUniformEnergy() const
{
    return
        fEnergyMin
        +
        G4UniformRand()
        *
        (
            fEnergyMax
            -
            fEnergyMin
        );
}


G4double BGenerator::SamplePowerLawEnergy() const
{
    const G4double gamma =
        fPowerLawIndex;

    const G4double u =
        G4UniformRand();

    if (std::abs(gamma - 1.0) < 1.e-10)
    {
        return
            fEnergyMin
            *
            std::pow(
                fEnergyMax / fEnergyMin,
                u
            );
    }

    const G4double a =
        std::pow(
            fEnergyMin,
            1.0 - gamma
        );

    const G4double b =
        std::pow(
            fEnergyMax,
            1.0 - gamma
        );

    return
        std::pow(
            a + u * (b - a),
            1.0 / (1.0 - gamma)
        );
}


G4double BGenerator::SampleEnergy() const
{
    switch (fEnergyDistribution)
    {
        case EnergyDistribution::Uniform:

            return SampleUniformEnergy();

        case EnergyDistribution::PowerLaw:

            return SamplePowerLawEnergy();

        case EnergyDistribution::RealMuonFlux:
        {
            G4double energy;
            G4double theta;

            SampleMuonFlux(
                energy,
                theta
            );

            return energy;
        }
    }

    return fEnergyMin;
}


// ====================================================================
// Theta sampling
// ====================================================================

G4double BGenerator::SampleUniformTheta() const
{
    return
        fThetaMin
        +
        G4UniformRand()
        *
        (
            fThetaMax
            -
            fThetaMin
        );
}


G4double BGenerator::SampleIsotropicTheta() const
{
    const G4double cosMin =
        std::cos(fThetaMax);

    const G4double cosMax =
        std::cos(fThetaMin);

    const G4double u =
        G4UniformRand();

    const G4double cosTheta =
        cosMin
        +
        u *
        (
            cosMax - cosMin
        );

    return std::acos(cosTheta);
}


G4double BGenerator::SampleCosinePowerTheta() const
{
    const G4double n =
        fCosinePower;

    const G4double xMin =
        std::cos(fThetaMax);

    const G4double xMax =
        std::cos(fThetaMin);

    const G4double u =
        G4UniformRand();

    const G4double x =
        std::pow(
            std::pow(
                xMin,
                n + 1.0
            )
            +
            u *
            (
                std::pow(
                    xMax,
                    n + 1.0
                )
                -
                std::pow(
                    xMin,
                    n + 1.0
                )
            ),
            1.0 / (n + 1.0)
        );

    return std::acos(x);
}


G4double BGenerator::SampleTheta() const
{
    switch (fAngularDistribution)
    {
        case AngularDistribution::UniformTheta:

            return SampleUniformTheta();

        case AngularDistribution::Isotropic:

            return SampleIsotropicTheta();

        case AngularDistribution::CosinePower:

            return SampleCosinePowerTheta();

        case AngularDistribution::RealMuonFlux:
        {
            G4double energy;
            G4double theta;

            SampleMuonFlux(
                energy,
                theta
            );

            return theta;
        }
    }

    return fThetaMin;
}


// ====================================================================
// Position
// ====================================================================

G4ThreeVector BGenerator::SamplePosition() const
{
    if (!fRandomPosition)
    {
        return fPosition;
    }

    const G4double x =
        fPositionMin.x()
        +
        G4UniformRand()
        *
        (
            fPositionMax.x()
            -
            fPositionMin.x()
        );

    const G4double y =
        fPositionMin.y()
        +
        G4UniformRand()
        *
        (
            fPositionMax.y()
            -
            fPositionMin.y()
        );

    const G4double z =
        fPositionMin.z()
        +
        G4UniformRand()
        *
        (
            fPositionMax.z()
            -
            fPositionMin.z()
        );

    return G4ThreeVector(
        x,
        y,
        z
    );
}


// ====================================================================
// Particle
// ====================================================================

G4String BGenerator::SampleParticle() const
{
    if (fParticleNames.empty())
    {
        G4Exception(
            "BGenerator::SampleParticle",
            "NoParticles",
            FatalException,
            "No particles configured."
        );
    }

    if (fParticleNames.size() == 1)
    {
        return fParticleNames.front();
    }

    const G4double totalWeight =
        std::accumulate(
            fParticleWeights.begin(),
            fParticleWeights.end(),
            0.0
        );

    G4double random =
        G4UniformRand()
        *
        totalWeight;

    for (std::size_t i = 0;
         i < fParticleNames.size();
         ++i)
    {
        random -=
            fParticleWeights[i];

        if (random <= 0.0)
        {
            return fParticleNames[i];
        }
    }

    return fParticleNames.back();
}


// ====================================================================
// Direction
// ====================================================================

G4ThreeVector BGenerator::MakeDirection(
    G4double theta,
    G4double phi) const
{
    /*
     * Incoming cosmic-ray muons travel DOWNWARD.
     *
     * Here we assume +Z is upward.
     *
     * Therefore:
     *
     *     pz = -cos(theta)
     */

    return G4ThreeVector(
        std::sin(theta) *
        std::cos(phi),

        std::sin(theta) *
        std::sin(phi),

        -std::cos(theta)
    );
}


// ====================================================================
// Validation
// ====================================================================

void BGenerator::ValidateParameters() const
{
    if (fParticleNames.empty())
    {
        G4Exception(
            "BGenerator::ValidateParameters",
            "NoParticles",
            FatalException,
            "No particles configured."
        );
    }

    if (fEnergyMin <= 0.0 ||
        fEnergyMax <= fEnergyMin)
    {
        G4Exception(
            "BGenerator::ValidateParameters",
            "InvalidEnergy",
            FatalException,
            "Invalid energy range."
        );
    }

    if (fThetaMin < 0.0 ||
        fThetaMax > 180.0 * deg ||
        fThetaMin >= fThetaMax)
    {
        G4Exception(
            "BGenerator::ValidateParameters",
            "InvalidTheta",
            FatalException,
            "Invalid theta range."
        );
    }

    if (fPhiMin >= fPhiMax)
    {
        G4Exception(
            "BGenerator::ValidateParameters",
            "InvalidPhi",
            FatalException,
            "Invalid phi range."
        );
    }
}


// ====================================================================
// GeneratePrimaries
// ====================================================================

void BGenerator::GeneratePrimaries(
    G4Event* event)
{
    ValidateParameters();

    G4ParticleTable* particleTable =
        G4ParticleTable::GetParticleTable();

    /*
     * IMPORTANT:
     *
     * For RealMuonFlux we sample E and theta together.
     */

    for (G4int i = 0;
         i < fNumberOfParticles;
         ++i)
    {
        G4double energy;
        G4double theta;

        // ------------------------------------------------------------
        // Real atmospheric muon flux
        // ------------------------------------------------------------

        if (fEnergyDistribution ==
                EnergyDistribution::RealMuonFlux
            ||
            fAngularDistribution ==
                AngularDistribution::RealMuonFlux)
        {
            SampleMuonFlux(
                energy,
                theta
            );
        }
        else
        {
            energy =
                SampleEnergy();

            theta =
                SampleTheta();
        }

        // ------------------------------------------------------------
        // Phi
        // ------------------------------------------------------------

        const G4double phi =
            SamplePhi();

        // ------------------------------------------------------------
        // Particle species
        // ------------------------------------------------------------

        const G4String particleName =
            SampleParticle();

        G4ParticleDefinition*
            particle =
                particleTable->FindParticle(
                    particleName
                );

        if (!particle)
        {
            G4ExceptionDescription msg;

            msg
                << "Particle '"
                << particleName
                << "' was not found "
                   "in the Geant4 particle table.";

            G4Exception(
                "BGenerator::GeneratePrimaries",
                "ParticleNotFound",
                FatalException,
                msg
            );
        }

        // ------------------------------------------------------------
        // Position
        // ------------------------------------------------------------

        const G4ThreeVector position =
            SamplePosition();

        // ------------------------------------------------------------
        // Direction
        // ------------------------------------------------------------

        const G4ThreeVector direction =
            MakeDirection(
                theta,
                phi
            );

        // ------------------------------------------------------------
        // Particle gun
        // ------------------------------------------------------------

        fParticleGun->SetParticleDefinition(
            particle
        );

        fParticleGun->SetParticleEnergy(
            energy
        );

        fParticleGun->SetParticlePosition(
            position
        );

        fParticleGun->SetParticleMomentumDirection(
            direction
        );

        fParticleGun->GeneratePrimaryVertex(
            event
        );
    }
}


// ====================================================================
// Phi sampling
// ====================================================================

G4double BGenerator::SamplePhi() const
{
    return
        fPhiMin
        +
        G4UniformRand()
        *
        (
            fPhiMax
            -
            fPhiMin
        );
}


// ====================================================================
// Configuration printout
// ====================================================================

void BGenerator::PrintConfiguration()
{
    G4cout
        << G4endl
        << "================ BGenerator ================"
        << G4endl;

    G4cout
        << "Particles:"
        << G4endl;

    for (std::size_t i = 0;
         i < fParticleNames.size();
         ++i)
    {
        G4cout
            << "  "
            << fParticleNames[i]
            << "  weight = "
            << fParticleWeights[i]
            << G4endl;
    }

    G4cout
        << "Energy range: "
        << fEnergyMin / GeV
        << " - "
        << fEnergyMax / GeV
        << " GeV"
        << G4endl;

    G4cout
        << "Theta range: "
        << fThetaMin / deg
        << " - "
        << fThetaMax / deg
        << " deg"
        << G4endl;

    G4cout
        << "Phi range: "
        << fPhiMin / deg
        << " - "
        << fPhiMax / deg
        << " deg"
        << G4endl;

    G4cout
        << "Muon charge ratio: "
        << fMuonChargeRatio
        << G4endl;

    G4cout
        << "============================================"
        << G4endl;
}


// ====================================================================
// Geant4 UI
// ====================================================================

void BGenerator::DefineCommands()
{
    fMessenger->DeclareMethod(
        "energyRange",
        &BGenerator::SetEnergyRange,
        "Set minimum and maximum energy."
    );

    fMessenger->DeclareMethod(
        "thetaRange",
        &BGenerator::SetThetaRange,
        "Set minimum and maximum polar angle."
    );

    fMessenger->DeclareMethod(
        "phiRange",
        &BGenerator::SetPhiRange,
        "Set minimum and maximum azimuthal angle."
    );

    fMessenger->DeclareMethod(
        "position",
        &BGenerator::SetPosition,
        "Set fixed particle position."
    );

    fMessenger->DeclareMethod(
        "positionRange",
        &BGenerator::SetPositionRange,
        "Set random position range."
    );

    fMessenger->DeclareMethod(
        "addParticle",
        &BGenerator::AddParticle,
        "Add a particle species with a relative weight."
    );

    fMessenger->DeclareMethod(
        "particle",
        &BGenerator::SetParticle,
        "Set the particle species."
    );

    fMessenger->DeclareMethod(
        "numberOfParticles",
        &BGenerator::SetNumberOfParticles,
        "Set number of particles per event."
    );

    fMessenger->DeclareMethod(
        "muonChargeRatio",
        &BGenerator::SetMuonChargeRatio,
        "Set the mu+/mu- ratio."
    );

    fMessenger->DeclareMethod(
        "cosinePower",
        &BGenerator::SetCosinePower,
        "Set cosine-power angular exponent."
    );

    fMessenger->DeclareMethod(
        "muonFluxScale",
        &BGenerator::SetMuonFluxNormalization,
        "Set atmospheric muon flux normalization."
    );

    fMessenger->DeclareMethod(
        "randomPosition",
        &BGenerator::SetRandomPosition,
        "Enable or disable random position generation."
    );

    fMessenger->DeclareMethod(
        "print",
        &BGenerator::PrintConfiguration,
        "Print generator configuration."
    );
}