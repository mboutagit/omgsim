#include "BPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "BDetectorConstruction.hh"
#include "Randomize.hh"
#include "G4StateManager.hh"


namespace km3net
{   

    BPrimaryGeneratorAction* BPrimaryGeneratorAction::instance = 0;
    bool BPrimaryGeneratorAction::write_generation_data = false;

    BPrimaryGeneratorAction* BPrimaryGeneratorAction::getInstance(){
        if(!instance){
            instance = new BPrimaryGeneratorAction();
        }
        return (BPrimaryGeneratorAction*) instance;
    }

    BPrimaryGeneratorAction::BPrimaryGeneratorAction() : generator(BPrimaryGeneratorAction::gen_mode::GEANT4),
                                                        particle_name("muons")
    {
       // init(generator);
        if(write_generation_data){
            if(!gen_data.is_open()){
                gen_data = ofstream(output_file_name);
            }
        }
    }

    BPrimaryGeneratorAction::~BPrimaryGeneratorAction(){
        gen_data.close();
    }

    void BPrimaryGeneratorAction::GeneratePrimaries(G4Event* event){
        if(fGunType == 3){
            particle_name ="muons";
            init(BPrimaryGeneratorAction::gen_mode::GEANT4);
            ((G4ParticleGun*)particleGun)->GeneratePrimaryVertex(event);
            return;
        }

        else{
            throw("This type is only for GenType = 3: Please use other type for in /KM3/XP/Type! ");
        }
    }

    double BPrimaryGeneratorAction::MuonFlux(double *x, double *par) 
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

    G4ThreeVector BPrimaryGeneratorAction::GenerateMuonDirection(std::string mode) 
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

    G4double BPrimaryGeneratorAction::GenerateMuonEnergy(double min, double max, float theta = 40) {
        float _alpha = 2.7 + (theta < 120 ? 1. : 0.);  
        //float A, B, a, b;
        return std::pow((std::pow(max, 1-_alpha) - std::pow(min, 1-_alpha)) 
                * G4UniformRand() 
                + std::pow(min, 1-_alpha), 1/(1-_alpha));
    }

    void BPrimaryGeneratorAction::print(std::ostream& out = std::cout){
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

    std::string BPrimaryGeneratorAction::getGeneratorName(){
        switch (generator)
        {
            case  gen_mode::GEANT4:     return "GEANT4";
            case  gen_mode::GAISSER:    return "GAISSER";
            case  gen_mode::CORSIKA:    return "CORSIKA";
            case  gen_mode::PYTHIA:     return "PYTHIA";
            case  gen_mode::FILE:       return "FILE";
            case  gen_mode::OTHER:      return "OTHER";
        
            default: return "Unkown generator!";
        }
    }

    void BPrimaryGeneratorAction::init(const gen_mode gen){
        if(particle_name == ""){
            throw "ParametrizedGenerator::init: Particle name is not set.";
            return;
        }

        generator = gen;

        if(gen == BPrimaryGeneratorAction::gen_mode::GEANT4 && particle_name == "muons"){
            BDetectorConstruction* det = BDetectorConstruction::GetIt();
            if(!det->genPhysic){
                throw("KM3PrimaryGeneratorAction::InitializeCustumMuonType: -- Generation physical volume is not defined");
            }

            fTargetRadius = det->GetTargetFullLength();
            fGenVolumeRaduis = det->GetWorldFullLength();
            Navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
            fGunType = 3;
        
            particleGun = new G4ParticleGun(); 
        
            G4ThreeVector surfPoints = GetRandomPointOnBoxFace(det->genPhysic, "+Z"); //genVolume->GetPointOnSurface()
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
            ((G4ParticleGun*)particleGun)->SetParticlePolarization(polarization);
        
            G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
            if(!particle)
              return;
            
            G4double energy = GenerateMuonEnergy(2 * GeV, 10000 * GeV, direction.getTheta());

            ((G4ParticleGun*)particleGun)->SetParticleDefinition(particle);
            ((G4ParticleGun*)particleGun)->SetParticlePosition(surfPoints);
            ((G4ParticleGun*)particleGun)->SetParticleEnergy(energy);
            ((G4ParticleGun*)particleGun)->SetParticleMomentumDirection(direction); // Toward the ground        
            
            particle_position = surfPoints;
            particle_direction = direction;
            particle_energy = energy;
            altitude = 0.372;
            double ene[]={energy};
            double pars[]={direction.getTheta(), altitude};
            flux = BPrimaryGeneratorAction::MuonFlux(ene, pars);

            // cout << "\r\033[20C" << "Flux expected for theta = " << pars[0] << 
            //                         " and altitude of " << pars[1] << 
            //                         " and Energy of " << energy << 
            //                         " is : " << flux <<
            //                         endl;


            if(write_generation_data)
                print(gen_data);
        }
    }


    G4ThreeVector BPrimaryGeneratorAction::GetRandomPointOnBoxFace(G4VPhysicalVolume* physVol, G4String face) {

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


    void BPrimaryGeneratorAction::write(bool wr){
        write_generation_data = wr;

    }



    // Physical constants
// const double pi = M_PI;
// const double g = 9.81;          // Gravity (m/s²)

// Muon flux parameterization (Gaisser's formula)
double BPrimaryGeneratorAction::muon_flux(double E, double theta, double altitude) {
    double cos_theta = cos(theta);
    double X = BUnits::rho_air * BUnits::H_0 * exp(-altitude / BUnits::H_0) / cos_theta; // Atmospheric depth
    double norm = 0.14;  // Normalization factor for flux (cm⁻² s⁻¹ sr⁻¹ GeV⁻¹)
    double gamma = 2.7;  // Spectral index
    double E_pion = 115.0 / (cos_theta + 0.1 * sqrt(X / BUnits::H_0));  // Pion critical energy
    return norm * pow(E, -gamma) * (1.0 / (1.0 + E / E_pion)); // Differential flux (cm⁻² s⁻¹ sr⁻¹ GeV⁻¹)
}

// integration using trapezoidal rule
double BPrimaryGeneratorAction::integrate_flux(double altitude, double E_min, double E_max, int E_steps, int theta_steps) {
    double dE = (E_max - E_min) / E_steps;
    double dTheta = BUnits::theta_max / theta_steps;
    double total_flux = 0.0;

    for (int i = 0; i < E_steps; i++) {
        double E1 = E_min + i * dE;
        double E2 = E1 + dE;

        for (int j = 0; j < theta_steps; j++) {
            double theta1 = j * dTheta;
            double theta2 = theta1 + dTheta;

            // solid angle element dΩ = sin(theta) dθ dφ
            double dOmega1 = 2 * CLHEP::pi * sin(theta1) * dTheta;
            double dOmega2 = 2 * CLHEP::pi * sin(theta2) * dTheta;

            // Evaluate flux at these points
            double f1 = muon_flux(E1, theta1, altitude) * dOmega1 * dE;
            double f2 = muon_flux(E2, theta2, altitude) * dOmega2 * dE;

            // sum
            total_flux += 0.5 * (f1 + f2);
        }
    }
    
    return total_flux;  // Returns total flux in cm⁻² s⁻¹
}

} // namespace km3net
