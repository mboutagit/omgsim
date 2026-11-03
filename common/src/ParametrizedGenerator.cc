
#include "ParametrizedGenerator.hh"

#include "KM3DetectorConstruction.hh"
#include <KM3PrimaryGeneratorAction.hh>
#include "Randomize.hh"
#include <G4ParticleGun.hh>
#include <G4TransportationManager.hh>
#include <G4PhysicalVolumeStore.hh>

#include <chrono>
#include <ctime>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#elif __linux__
    #include <unistd.h>
#endif

namespace km3net
{    
    ParametrizedGenerator* ParametrizedGenerator::instance = nullptr;

    ParametrizedGenerator* ParametrizedGenerator::getInstance() {
        if (!instance) {  // Only create if not already created
            instance = new ParametrizedGenerator();
        }
        return (ParametrizedGenerator*) instance;
    }

    ParametrizedGenerator::ParametrizedGenerator() : generator(ParametrizedGenerator::gen_mode::GEANT4), particle_name("muons") {
        pga = new KM3PrimaryGeneratorAction();
        init(generator);
    };

    ParametrizedGenerator::ParametrizedGenerator(const gen_mode gen, KM3PrimaryGeneratorAction* pga){
        generator = gen;
        init(gen);
    }

    ParametrizedGenerator::~ParametrizedGenerator(){
        delete pga;
        gen_data.close();
        cout << "gen File colsed correctly" << endl;
    }

    void ParametrizedGenerator::writeGenData(bool wr = true){
        if(! wr )
            return;
        write_generation_data = wr;
        if(!gen_data.is_open()){
            gen_data = ofstream("gen_data.dat");
            printHeader(gen_data);
        }  
    }

    bool ParametrizedGenerator::isWrite(){
        return write_generation_data;
    }

    std::string ParametrizedGenerator::getGeneratorName(){
        switch (generator)
        {
            case  gen_mode::GEANT4:     return "GEANT4";
            case  gen_mode::CORSIKA:    return "CORSIKA";
            case  gen_mode::PYTHIA:     return "PYTHIA";
            case  gen_mode::FILE:       return "FILE";
            case  gen_mode::OTHER:      return "OTHER";
        
            default: return "Unkown generator!";
        }
    }

    void ParametrizedGenerator::print(std::ostream& out = std::cout){
        out << "event id: " << event_id << std::endl;        
        out << "generator: " << getGeneratorName() << std::endl;
        out << "particle name: " << particle_name << std::endl;
        out << "particle position (m): " << particle_position / CLHEP::m << std::endl;
        out << "particle direction (mm): " << particle_direction / CLHEP::mm << std::endl;
        out << "particle theta (deg): " << particle_direction.getTheta() / CLHEP::deg << std::endl;
        out << "particle phi (deg): " << particle_direction.getPhi() / CLHEP::deg << std::endl;
        out << "particle enegry (MeV): " << particle_energy / CLHEP::MeV << std::endl;
        out << std::endl;
    }

    void ParametrizedGenerator::init(const gen_mode gen){
        if(particle_name == ""){
            throw "ParametrizedGenerator::init: Particle name is not set.";
            return;
        }

        generator = gen;

        if(gen == ParametrizedGenerator::gen_mode::GEANT4 && particle_name == "muons"){

            KM3DetectorConstruction* det = KM3DetectorConstruction::GetIt();
            G4VPhysicalVolume* myPhysicVolume;
            G4PhysicalVolumeStore* store = G4PhysicalVolumeStore::GetInstance();
            for (auto physVol : *store) {
                if (physVol->GetName() == "genPhysic") {
                    myPhysicVolume = physVol;  // Found the volume
                }
            }
            if(!myPhysicVolume){
                throw("KM3PrimaryGeneratorAction::InitializeCustumMuonType: Generation physical volume is not defined");
                return;
            }
              
            
            // G4Box* genVolume = dynamic_cast<G4Box*> (det->genSolid);
            // if(!genVolume){
            //   throw("KM3PrimaryGeneratorAction::InitializeCustumMuonType: Generation volume is not defined");
            // }
          
            pga->fTargetRadius = det->GetTargetFullLength();
            pga->fGenVolumeRaduis = det->GetWorldFullLength();
            pga->Navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
            pga->fGunType = 3;
        
            pga->particleGun = new G4ParticleGun(); 
        
            G4ThreeVector surfPoints = GetRandomPointOnBoxFace(myPhysicVolume, "+Z"); //genVolume->GetPointOnSurface()
            G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        
            G4String particleName;
            if(G4UniformRand() > 0.5)
              particleName = "mu+";
            else
              particleName = "mu-";
                    G4ThreeVector direction = GenerateMuonDirection("down-going_cos2");
            if(direction.getZ() > 0)
                direction.setZ(- direction.getZ());
            G4ThreeVector polarization(G4UniformRand(), G4UniformRand(), 0.0); 
            ((G4ParticleGun*)pga->particleGun)->SetParticlePolarization(polarization);
        
            G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
            if(!particle)
              return;
            
            G4double energy = GenerateMuonEnergy(2 * GeV, 6 * GeV, direction.getTheta());

            ((G4ParticleGun*)pga->particleGun)->SetParticleDefinition(particle);
            ((G4ParticleGun*)pga->particleGun)->SetParticlePosition(surfPoints);
            ((G4ParticleGun*)pga->particleGun)->SetParticleEnergy(energy);
            ((G4ParticleGun*)pga->particleGun)->SetParticleMomentumDirection(direction); // Toward the ground        
            
            particle_position = surfPoints;
            particle_direction = direction;
            particle_energy = energy;

            if(write_generation_data)
                print(gen_data);
        }
    }

    bool ParametrizedGenerator::read(){
        cout << "\r\033[20C" << "reading .. " << endl;
    }

    void ParametrizedGenerator::printHeader(ostream& out = std::cout){
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    
        char buffer[1024];

        #ifdef _WIN32
            GetModuleFileNameA(NULL, buffer, sizeof(buffer));
        #elif __linux__
            ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
            if (len != -1) buffer[len] = '\0';  
        #endif

        std::string fullPath(buffer), programName;
        programName = fullPath.substr(fullPath.find_last_of("/\\") + 1);  // Extract file name only
    
        const char* user = std::getenv("USER"); // For Linux/macOS
        if (!user) user = std::getenv("USERNAME"); // For Windows
    
        out << "Execution Date & Time: " << std::ctime(&now_time);
        out << "Program Name: " << programName << "\n";
        out << "User: " << (user ? user : "Unknown") << "\n";
        out << "------------------------------------------- \n\n";
    }

    G4ThreeVector ParametrizedGenerator::GenerateMuonDirection(std::string mode) 
    {
        G4double u = G4UniformRand();
        G4double theta = std::acos(std::pow(u, 1/2));  // cos²(θ) distribution
        G4double  phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth
        
        if(mode == "vertical"){
        theta = CLHEP::pi;  // vertiacal -z
        }
        else if(mode == "down-going"){
        theta = acos(sin((1 - u) * (CLHEP::pi / 2) + u * (CLHEP::pi / 2)));
        }
        else if(mode == "down-going_cos2"){
        theta = - acos(pow(u, 3/2)) - CLHEP::pi/2;
        }

        G4double ux = std::sin(theta) * std::cos(phi);
        G4double uy = std::sin(theta) * std::sin(phi);
        G4double uz = std::cos(theta);

        G4ThreeVector vect = G4ThreeVector(ux, uy, uz);
        return vect;
    }

    G4double ParametrizedGenerator::GenerateMuonEnergy(double min, double max, float theta = 40) {
        float _alpha = 2.7 + (theta < 120 ? 1. : 0.);  
        //float A, B, a, b;
        return std::pow((std::pow(max, 1-_alpha) - std::pow(min, 1-_alpha)) 
                * G4UniformRand() 
                + std::pow(min, 1-_alpha), 1/(1-_alpha));
    }


    G4ThreeVector ParametrizedGenerator::GetRandomPointOnBoxFace(G4VPhysicalVolume* physVol, G4String face) {

        G4LogicalVolume* logVol = physVol->GetLogicalVolume();
        G4VSolid* solid = logVol->GetSolid();

        G4Box* box = dynamic_cast<G4Box*>(solid);
        if (!box) {
            G4cerr << "Error: The given physical volume is not a G4Box!" << G4endl;
            return G4ThreeVector();  // Return an invalid vector
        }

        G4double halfX = box->GetXHalfLength();
        G4double halfY = box->GetYHalfLength();
        G4double halfZ = box->GetZHalfLength();

        G4ThreeVector boxPos = physVol->GetTranslation();

        G4double x, y, z;

        if (face == "+Z") {
            x = (2 * G4UniformRand() - 1) * halfX;
            y = (2 * G4UniformRand() - 1) * halfY;
            z = halfZ;
        } 
        else if (face == "-Z") {
            x = (2 * G4UniformRand() - 1) * halfX;
            y = (2 * G4UniformRand() - 1) * halfY;
            z = -halfZ;
        } 
        else if (face == "+X") {
            x = halfX;
            y = (2 * G4UniformRand() - 1) * halfY;
            z = (2 * G4UniformRand() - 1) * halfZ;
        } 
        else if (face == "-X") {
            x = -halfX;
            y = (2 * G4UniformRand() - 1) * halfY;
            z = (2 * G4UniformRand() - 1) * halfZ;
        } 
        else if (face == "+Y") {
            x = (2 * G4UniformRand() - 1) * halfX;
            y = halfY;
            z = (2 * G4UniformRand() - 1) * halfZ;
        } 
        else if (face == "-Y") {
            x = (2 * G4UniformRand() - 1) * halfX;
            y = -halfY;
            z = (2 * G4UniformRand() - 1) * halfZ;
        } 
        else {
            G4cerr << "Invalid face name! Use +X, -X, +Y, -Y, +Z, -Z." << G4endl;
            return G4ThreeVector();
        }

        // Apply the box's translation (convert to global coordinates)
        return G4ThreeVector(x, y, z) + boxPos;
    }


    // Gaisser's formula modified for altitude
    double ParametrizedGenerator::MuonFlux(double *x, double *par) 
    {
        const double epsilon_pi = 115.0; 
        const double epsilon_K  = 850.0; 
        const double H = 6.3;            
        const double norm = 0.14;        
    
        double E_mu = x[0];   // Muon energy (GeV)
        double theta = par[0]; // Zenith angle (radians)
        double h = par[1];    // Altitude (km)

        double flux = norm * pow(E_mu, -2.7) *
                    (1.0 / (1.1 + (E_mu * cos(theta) / epsilon_pi)) +
                   0.054 / (1.1 + (E_mu * cos(theta) / epsilon_K)));

        flux *= exp(-h / H);  // Apply altitude correction

        return flux;
    }

    void ParametrizedGenerator::GeneratePrimaries(G4Event* event){

        if(fGunType == 3){
            //InitializeCustumMuonType();
            ParametrizedGenerator* pg = ParametrizedGenerator::getInstance();
            if(!pg)
              return;
            pg->setEventID(event->GetEventID());
            //pg->writeGenData(true);
            pg->init(ParametrizedGenerator::gen_mode::GEANT4);
            ((G4ParticleGun*)pg->getPGA()->particleGun)->GeneratePrimaryVertex(event);
    
            return;
          }
          else{
            std::cerr << "ParametrizedGenerator::GeneratePrimaries: fGunType not set!" << endl;
            exit(-1);
          }
    }

} // namespace genPar
