#ifndef BPrimaryGeneratorAction_hh
#define BPrimaryGeneratorAction_hh

#include "KM3PrimaryGeneratorAction.hh"

#include "TFile.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TGraph.h"



using namespace std;
using namespace km3net;


namespace BUnits
{
    static const double rho_air = 1.225; // Air density at sea level (kg/m³)
    static const double H_0 = 8000.0;      // Scale height of atmosphere (m)
    static const double theta_max = 60.0 * (CLHEP::pi / 180.0) * rad; // Max zenith angle in radians

} // namespace BUnits

namespace lactel
{

class BPrimaryGeneratorMessenger;

class BPrimaryGeneratorAction : public KM3PrimaryGeneratorAction
{
private:
    static BPrimaryGeneratorAction* instance;
    enum class gen_mode {GEANT4, GAISSER, CORSIKA, PYTHIA, FILE, OTHER};
    enum class GenParsMode{fixed, integral, random, cos2};

    BPrimaryGeneratorAction();
    BPrimaryGeneratorMessenger* fMessenger;

    
public:

    static BPrimaryGeneratorAction* getInstance();
    BPrimaryGeneratorAction(const BPrimaryGeneratorAction&) = delete;
    BPrimaryGeneratorAction& operator=(const BPrimaryGeneratorAction&) = delete;
    static double MuonFlux(double* energy, double* pars);
    G4ThreeVector GenerateMuonDirection(std::string);
    G4double GenerateMuonEnergy(double Emin, double Emax, float theta = 60);
    void GeneratePrimaries(G4Event* event);
    void init(const gen_mode genmode);
    G4ThreeVector GetRandomPointOnBoxFace(G4VPhysicalVolume* physVol, G4String face);
    static double muon_flux(double E, double theta, double altitude);
    //static double integrate_flux(double altitude, double E_min, double E_max, int E_steps, int theta_steps);
    double integrate_muon_flux(double alt = 327, double Emin = 1*GeV, double Emax = 10*TeV, double Esteps = 1*GeV, 
                                double theta_min = 0.*deg, double theta_max=60.*deg, double theta_steps = 1*deg);
    G4ThreeVector GenerateDirThetaCos2(double theta_min, double theta_max);

    static double integrate_flux(double* pars);

    double GaisserFlux(double E, double theta);
    double IntegrateMuonFlux(double Emin, double Emax, double area_m2 = 1.0, double time_s = 1.0);

    double gaisserSpectrum(double E_GeV); // Gev
    double generateMuonEnergy(double Emin_GeV, double Emax_GeV); // GeV
    double cos2thetaPDF(double theta); // radians
    double generateZenithAngle();

    double gaisser_flux(double E);
    double integrate_gaisser_flux(double E_min, double E_max, int Nbins);
    void getExpectedFlux();

    void print(ostream& out);
    std::string getGeneratorName();
    void write(bool write);

    static bool output_info;

    void SetGenParsMode(std::string mode);
    G4ThreeVector FixedThetaDirection(double theta);

    bool couldHitCube(G4ThreeVector origin, G4ThreeVector direction, G4Box box);

    ofstream gen_data;
    gen_mode generator;
    G4ThreeVector particle_position;
    G4ThreeVector particle_direction;
    G4double particle_energy;
    std::string particle_name = "";
    std::string output_file_name ="generationData.dat";
    uint event_id;
    static bool write_generation_data;

    TH2F* integrate_flux_hist;
    TH2F* nMuons;
    TGraph* nMuonsGr;

    // generation pars
    static float EMax; // maximum energy
    static float EMin; // minimum energy
    static float dE; // energy step
    static float thetaMin; // minimum theta
    static float thetaMax; // maximum theta
    static float phiMin; // minumum phi
    static float phiMax; // maximum phi
    static float dTheta; // theta steps
    static float dPhi; // phi steps
    static float genArea; // generation area time
    static float tExposure; // exposure time
    static float genAltitude; // capture altitude

    double thetaGen;
    double EGen;
    static GenParsMode genParsMode;



    int nGen = 0; // number of generated events
    int nDet = 0; // ,number of detected event (at least one hit in the detector)
    float muonsFlux = 0; // expected muons rate on a given surface for a given energy range and solid angle. unit: muons/cm^2/s
    
    ~BPrimaryGeneratorAction();
};

    

} // namespace lactel

#endif