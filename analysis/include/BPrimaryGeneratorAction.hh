#ifndef BPrimaryGeneratorAction_hh
#define BPrimaryGeneratorAction_hh

#include "KM3PrimaryGeneratorAction.hh"


using namespace std;


namespace BUnits
{
    static const double rho_air = 1.225; // Air density at sea level (kg/m³)
    static const double H_0 = 8000.0;      // Scale height of atmosphere (m)
    static const double theta_max = 60.0 * (CLHEP::pi / 180.0); // Max zenith angle in radians

} // namespace BUnits

namespace km3net
{

class BPrimaryGeneratorAction : public KM3PrimaryGeneratorAction
{
private:
    static BPrimaryGeneratorAction* instance;
    enum class gen_mode {GEANT4, GAISSER, CORSIKA, PYTHIA, FILE, OTHER};
    BPrimaryGeneratorAction();

public:

    static BPrimaryGeneratorAction* getInstance();
    BPrimaryGeneratorAction(const BPrimaryGeneratorAction&) = delete;
    BPrimaryGeneratorAction& operator=(const BPrimaryGeneratorAction&) = delete;
    static double MuonFlux(double* energy, double* pars);
    G4ThreeVector GenerateMuonDirection(std::string);
    G4double GenerateMuonEnergy(double, double, float);
    void GeneratePrimaries(G4Event*);
    void init(const gen_mode);
    G4ThreeVector GetRandomPointOnBoxFace(G4VPhysicalVolume* physVol, G4String face);
    static double muon_flux(double E, double theta, double altitude);
    static double integrate_flux(double altitude, double E_min, double E_max, int E_steps, int theta_steps);

    void print(ostream& out);
    std::string getGeneratorName();
    void write(bool);

    static bool output_info;

    



    ofstream gen_data;
    gen_mode generator;
    G4ThreeVector particle_position;
    G4ThreeVector particle_direction;
    G4double particle_energy;
    std::string particle_name = "";
    std::string output_file_name ="generationData.dat";
    uint event_id;
    static bool write_generation_data;

    float altitude;
    float flux;
    
    ~BPrimaryGeneratorAction();
};

    

} // namespace km3net

#endif