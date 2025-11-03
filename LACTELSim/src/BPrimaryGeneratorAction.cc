#include "BPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4TransportationManager.hh"
#include "BDetectorConstruction.hh"
#include "Randomize.hh"
#include "G4StateManager.hh"
#include "G4OpticalPhoton.hh"

#include "BPrimaryGeneratorMessenger.hh"

namespace lactel
{   

    BPrimaryGeneratorAction* BPrimaryGeneratorAction::instance = 0;
    
    bool BPrimaryGeneratorAction::write_generation_data = false;

    BPrimaryGeneratorAction::GenParsMode BPrimaryGeneratorAction::genParsMode=GenParsMode::integral;

    float BPrimaryGeneratorAction::EMin        = 1 *GeV; // maximum energy
    float BPrimaryGeneratorAction::EMax        = 1 *TeV; // minimum energy
    float BPrimaryGeneratorAction::dE          = .1 *GeV; // energy step
    float BPrimaryGeneratorAction::thetaMin    = 0.*deg; // minimum theta
    float BPrimaryGeneratorAction::thetaMax    = 60 *deg; //60. *deg;; // maximum theta
    float BPrimaryGeneratorAction::phiMin      = 0. *deg; // minumum phi
    float BPrimaryGeneratorAction::phiMax      = 360. *deg; // maximum phi
    float BPrimaryGeneratorAction::dTheta      = .5 *deg; // theta steps
    float BPrimaryGeneratorAction::dPhi        = .5 *deg; // phi steps
    float BPrimaryGeneratorAction::genArea     = 1 *m2; // generation area time
    float BPrimaryGeneratorAction::tExposure   = 1 *s; // exposure time
    float BPrimaryGeneratorAction::genAltitude = 372. *meter; // generation altitude for the primaries

    BPrimaryGeneratorAction* BPrimaryGeneratorAction::getInstance(){
        if(!instance){
            instance = new BPrimaryGeneratorAction();
        }
        return (BPrimaryGeneratorAction*) instance;
    }

    void BPrimaryGeneratorAction::initMuons (){
      if (fGunType)
        throw ("Two guntype init in the mac file !!");

      particleGun = new G4ParticleGun();
      fGunType = 3;
    }

    void BPrimaryGeneratorAction::initTest (){
      if (fGunType)
        throw ("Two guntype init in the mac file !!");

      particleGun = new G4ParticleGun();
      fGunType = 4;
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

        fMessenger = new BPrimaryGeneratorMessenger(this);

        double Emin = 10*GeV, Emax=10*TeV, dE = 500*MeV;
        double Tmin = 0 * deg, Tmax = 70 *deg, dT = .1 * deg;
        double alt = 274 * m;

        TH2F* hisET = new TH2F("hisET", "Muons flux; log10(E) (GeV); #theta (Deg)", 500, log10(Emin/GeV), log10(Emax/GeV), 200, Tmin/deg, Tmax/deg);

        vector<double> angles = {0*deg, 10*deg , 20*deg , 30*deg, 40*deg, 50*deg, 60*deg};
        vector<TGraph*> FEGraphes;

        for(auto& angle : angles){
            TGraph* gr = new TGraph();
            gr->SetTitle(("gr_" + std::to_string((int)(angle/deg)) + "; Energy (GeV); Flux").c_str());
            gr->SetName(("gr_" + std::to_string((int)(angle/deg))).c_str());

            int p = 0;
            std::cout << "\r\033[10C" << "energy min in GeV: " << Emin/GeV << " angle in rad: " << angle/rad << std::endl;
            for(double E = Emin; E <= Emax; E+=dE){
                gr->SetPoint(p, E/GeV, muon_flux(E/GeV, angle/rad, alt/m) * 100 * 100 );
                p++;
            }
            // FEGraphes.push_back(gr);
            gr->Write();
        }



        for(double E = Emin; E <= Emax; E+=dE){
            for(double T = Tmin; T <= Tmax; T+=dT){
                hisET->Fill(log10(E/GeV), T/deg, muon_flux(E/GeV, T/rad, alt/m));
            }
        }
        hisET->Write();
       
    }

    BPrimaryGeneratorAction::~BPrimaryGeneratorAction(){
        // if(integrate_flu_hist && gFile && gFile->IsWritable())
        //     integrate_flux_hist->Write();
            
        if(nMuons && gFile && gFile->IsWritable())
            nMuons->Write();

        if(nMuonsGr && gFile && gFile->IsWritable())
            nMuonsGr->Write();
    
        gen_data.close();
    }

    void BPrimaryGeneratorAction::SetGenParsMode(std::string mode){
        if(mode == "fixed"){
            genParsMode = GenParsMode::fixed;
            std::cout << "\r\033[10C" << ">>>> GenParsMode is: " << "fixed" << std::endl;
        }
        else if(mode == "integral"){
            genParsMode = GenParsMode::integral;        
            std::cout << "\r\033[10C" << ">>>> GenParsMode is: " << "integral" << std::endl;
        }
        else if(mode == "random"){
            genParsMode = GenParsMode::random;
            std::cout << "\r\033[10C" << ">>>> GenParsMode is: " << "random" << std::endl;
        }
        else if(mode == "cos2"){
            genParsMode = GenParsMode::cos2;
            std::cout << "\r\033[10C" << ">>>> GenParsMode is: " << "cos2" << std::endl;
        }
        else{
            std::cerr << "\r\033[10C" << "Unkown generation parameter mode " << mode << std::endl;
        }

    }

    void BPrimaryGeneratorAction::GeneratePrimaries(G4Event* event){
        if(fGunType == 3){
            particle_name ="muons";
            init(BPrimaryGeneratorAction::gen_mode::GEANT4);
            ((G4ParticleGun*)particleGun)->GeneratePrimaryVertex(event);
            return;
        }
        else if(fGunType == 4){
            // std::cout << "\r\033[10C" << "\033[1;35mInfo: Using costum particle gun. \033[0m" << std::endl;
            
             G4ParticleGun* gun = new G4ParticleGun(1);

            // // Define the optical photon
            // G4ParticleDefinition* photon = G4OpticalPhoton::Definition();
            // gun->SetParticleDefinition(photon);

            // // ---- Laser-like properties ----
            // gun->SetParticleEnergy(3.5 * eV);               // ~620 nm (red laser)
            // gun->SetParticlePosition(G4ThreeVector(0., 0., 0.)); // source position
            // gun->SetParticleMomentumDirection(G4ThreeVector((0.01 + 0.01 * G4UniformRand()), 
            //                                                     (0.01 + 0.02 * G4UniformRand()),
            //                                                      -(0.9 + 0.1 * G4UniformRand()))); // +z direction (collimated)

            // // Set linear polarization (unit vector orthogonal to momentum)
            // G4ThreeVector pol(1., 0., 0.); // polarization along x-axis
            // gun->SetParticlePolarization(pol);

            // // Generate the event
            // gun->GeneratePrimaryVertex(event);
            // ((G4ParticleGun*)particleGun)->GeneratePrimaryVertex(event);

            G4ThreeVector hemisphereCenter(0., 0., -650.*mm);  // center position in world coordinates
            G4double R = 0.5 * m;                           // radius of hemisphere


            G4double theta = 3.145*G4UniformRand(); //;std::acos(G4UniformRand());     // [0, π/2] for upper hemisphere
            G4double phi   = 2.0 * CLHEP::pi * G4UniformRand();

            G4double x = R * std::sin(theta) * std::cos(phi);
            G4double y = R * std::sin(theta) * std::sin(phi);
            G4double z = R * std::cos(theta);                // upper hemisphere (+z)

            // Shift by the hemisphere’s origin
            G4ThreeVector position = hemisphereCenter + G4ThreeVector(x, y, z);

        
            G4ThreeVector direction = (hemisphereCenter - position).unit();

            gun->SetParticleDefinition(G4OpticalPhoton::Definition());
            gun->SetParticleEnergy((1.77 + 4.13 * G4UniformRand()) * eV);
            gun->SetParticlePosition(position);
            gun->SetParticlePolarization(G4ThreeVector(1, 0, 0));
            gun->SetParticleMomentumDirection(direction);

            gun->GeneratePrimaryVertex(event);


            return;
        }
        else{
            std::cerr << "\r\033[10C" << "\033[1;33mWarning: Usage of GunType = " << static_cast<int> (fGunType) 
                                        << ". This type is not suitable for BA Type simulations. \033[0m" << std::endl;
            KM3PrimaryGeneratorAction::GeneratePrimaries(event);
        }
    }

    void BPrimaryGeneratorAction::getExpectedFlux(){
        double nMuons = integrate_muon_flux(genAltitude, EMin, EMax, dE, thetaMin, thetaMax, dTheta); // muon/second/cm^2
        std::cout << "Estimated muon rate per " << genArea/cm2 << " cm2 per " << tExposure/s << " seconds: " 
                    << std::setprecision(4) << nMuons * genArea/cm2 * tExposure/s
                    <<  "\nExpected muons /cm2: "
                    << std::setprecision(4) <<   nMuons

                    << std::endl;

        muonsFlux = nMuons * genArea/cm2 * tExposure/s;
    }

    G4ThreeVector BPrimaryGeneratorAction::GenerateDirThetaCos2(double theta_min, double theta_max) {
        double theta, y;
        double ymax = std::pow(std::cos(theta_min), 2); // peak value of cos²(theta) in the range
        double  phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth
    
        do {
            theta = G4UniformRand() * (theta_max - theta_min) + theta_min;
            y = G4UniformRand() * ymax;
        } while (y > std::pow(std::cos(theta), 2));

        double ux = std::sin(theta) * std::cos(phi);
        double uy = std::sin(theta) * std::sin(phi);
        double uz = std::cos(theta);

        G4ThreeVector vect = G4ThreeVector(ux, uy, uz);
        return vect;
    }

    G4ThreeVector BPrimaryGeneratorAction::GenerateMuonDirection(std::string mode) 
    {
        double u = G4UniformRand();
        double theta = std::acos(std::pow(u, 1/2));  // cos²(θ) distribution
        double  phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth
        
        if(mode == "vertical"){
            theta = CLHEP::pi;  // vertiacal -z
        }
        else if(mode == "down-going"){
            theta = acos(sin((1 - u) * (CLHEP::pi / 2) + u * (CLHEP::pi / 2)));
        }
        else if(mode == "down-going_cos2"){
            theta = - acos(pow(u, 3/2)) - CLHEP::pi/2;
        }
        else if(mode == "angle"){
            theta = thetaMax;
        }

        G4double ux = std::sin(theta) * std::cos(phi);
        G4double uy = std::sin(theta) * std::sin(phi);
        G4double uz = std::cos(theta);

        G4ThreeVector vect = G4ThreeVector(ux, uy, uz);
        return vect;


    }

    G4ThreeVector BPrimaryGeneratorAction::FixedThetaDirection(double theta){
        double  phi = 2 * CLHEP::pi * G4UniformRand(); // Uniform in azimuth
        return G4ThreeVector(std::sin(theta) * std::cos(phi), 
                                std::sin(theta) * std::sin(phi),
                                std::cos(theta)
                            );
    }

    G4double BPrimaryGeneratorAction::GenerateMuonEnergy(double min, double max, float theta) {
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


    bool BPrimaryGeneratorAction::couldHitCube(G4ThreeVector origin, G4ThreeVector direction, G4Box box){
        double hx = box.GetXHalfLength();
        double hy = box.GetYHalfLength();
        double hz = box.GetZHalfLength();

        // Define min/max bounds for each axis
        double xmin = -hx, xmax = hx;
        double ymin = -hy, ymax = hy;
        double zmin = -hz, zmax = hz;

        // Compute intersections on X-axis
        double t1x = (xmin - origin.x()) / direction.x();
        double t2x = (xmax - origin.x()) / direction.x();
        double tnear_x = std::min(t1x, t2x);
        double tfar_x = std::max(t1x, t2x);

        // Compute for Y-axis
        double t1y = (ymin - origin.y()) / direction.y();
        double t2y = (ymax - origin.y()) / direction.y();
        double tnear_y = std::min(t1y, t2y);
        double tfar_y = std::max(t1y, t2y);

        // Compute for Z-axis
        double t1z = (zmin - origin.z()) / direction.z();
        double t2z = (zmax - origin.z()) / direction.z();
        double tnear_z = std::min(t1z, t2z);
        double tfar_z = std::max(t1z, t2z);

        // Determine the entry/exit points
        double t_entry = std::max({tnear_x, tnear_y, tnear_z});
        double t_exit  = std::min({tfar_x, tfar_y, tfar_z});

        // Intersection condition
        return (t_entry <= t_exit) && (t_exit >= 0);    
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

            fGunType = 3;
            particleGun = new G4ParticleGun(); 
            G4ThreeVector surfPoints = GetRandomPointOnBoxFace(det->genPhysic, "+Z"); //genVolume->GetPointOnSurface()

            G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        
            G4String particleName;
            if(G4UniformRand() > 0.5)
              particleName = "mu+";
            else
              particleName = "mu-";

            particleName="gamma";

            double Emin = 1.0;     // GeV
            double Emax = 1e4;     // GeV (10 TeV)
            G4ThreeVector direction;
            G4double energy;


            if(genParsMode == GenParsMode::cos2){
                direction = GenerateMuonDirection("down-going_cos2");
                energy = GenerateMuonEnergy(Emin * GeV, Emax * GeV, direction.getTheta());
            }
            else if(genParsMode == GenParsMode::integral){
                direction = GenerateMuonDirection("down-going_cos2");
                energy = GenerateMuonEnergy(Emin * GeV, Emax * GeV, direction.getTheta());
            }
            else if(genParsMode == GenParsMode::fixed){
                energy = EGen;
                direction = FixedThetaDirection(thetaGen);
            }
                
            

            if(direction.getZ() > 0)
                direction.setZ(- direction.getZ());
            G4ThreeVector polarization(G4UniformRand(), G4UniformRand(), 0.0); 
            ((G4ParticleGun*)particleGun)->SetParticlePolarization(polarization);
        
            G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
            if(!particle)
              return;
            
            

            ((G4ParticleGun*)particleGun)->SetParticleDefinition(particle);
            ((G4ParticleGun*)particleGun)->SetParticlePosition(surfPoints);
            ((G4ParticleGun*)particleGun)->SetParticleEnergy(energy);
            ((G4ParticleGun*)particleGun)->SetParticleMomentumDirection(direction); // Toward the ground        
            
            particle_position = surfPoints;
            particle_direction = direction;
            particle_energy = energy;

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

        // cout << "\r\033[10C" << "Half size: " << "(" << halfX << ", " << halfY << ", " << halfZ << ")" << endl;

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
        // cout << "\r\033[10C" << "Found pos: " << G4ThreeVector(x, y, z) << " - Trans pos: " << boxPos << endl;
        return G4ThreeVector(x, y, z) + boxPos;
    }


    void BPrimaryGeneratorAction::write(bool wr){
        write_generation_data = wr;

    }


        // Muon flux parameterization (Gaisser's formula)
    double BPrimaryGeneratorAction::muon_flux(double E, double theta, double altitude) { // E in GeV, theta in rad and alt in m
        // double cos_theta = cos(theta/rad);
        // double X = BUnits::rho_air * BUnits::H_0 * exp(-altitude/m / BUnits::H_0/m) / cos_theta; // Atmospheric depth
        // double norm = 0.14;  // Normalization factor for flux (cm⁻² s⁻¹ sr⁻¹ GeV⁻¹)
        // double gamma = 2.7;  // Spectral index
        // double E_pion = 115.0 / (cos_theta + 0.1 * sqrt(X / BUnits::H_0));  // Pion critical energy
        // return norm * pow(E, -gamma) * (1.0 / (1.0 + E / E_pion)); // Differential flux (cm⁻² s⁻¹ sr⁻¹ GeV⁻¹)

        double cost = std::cos(theta);
        double term1 = 1.0 / (1.0 + 1.1 * E * cost / 115.0);
        double term2 = 0.054 / (1.0 + 1.1 * E * cost / 850.0);
        const double H = 6.3 * km; // in km   // Atmospheric scale height
        return 0.14 * std::pow(E, -2.7) * (term1 + term2) * exp(-altitude / H/m); // muons/cm²/s/sr/GeV * altitude correction
    }

    // integration using trapezoidal rule
    double BPrimaryGeneratorAction::integrate_muon_flux(double alt, double Emin, double Emax, double Esteps, 
                                                        double theta_min, double theta_max, double theta_steps) {
        
        alt /= m;
        Emin /= GeV;
        Emax /= GeV;
        Esteps /= GeV;
        theta_min /= rad;
        theta_max /= rad;
        theta_steps /= rad;

        std::cout << "BPrimaryGeneratorAction::integrate_flux: " 
                            << "\n Altitude in m: " << alt
                            << "\n E_min in GeV: " << Emin
                            << "\n E_max in GeV: " << Emax
                            << "\n E_steps in GeV: " << Esteps
                            << "\n Theta_min in deg: " << theta_min /deg
                            << "\n Theta_max in deg: " << theta_max /deg
                            << "\n Theta_steps in deg: " << theta_steps /deg
                            << "\n (Emax - Emin) /Esteps: " << (Emax - Emin) /Esteps
                            << "\n (theta_max - theta_min)/theta_steps: " << (theta_max - theta_min)/theta_steps
                            << std::endl;
        // Emin + dE + dE +... = Emax => Emin + n*dE = Emax =>  n*dE = Emax -Emin => n = (Emax-Emin)/dE 
        double Ensteps = (Esteps > 0) ? (Emax - Emin) / Esteps : Emax; // 
        double thetansteps = (theta_steps > 0) ? (theta_max - theta_min)/theta_steps : theta_max/deg;

        integrate_flux_hist = new TH2F("integtared_flux", "Integrated muon flux; E(GeV); #theta (Deg)",
                                    (int) Ensteps, (int) Emin, Emax,
                                    (int) thetansteps, (int) theta_min/deg, theta_max/deg
                                    // 500, 1e-7, 1e1
                                    );

        nMuons = new TH2F("nMuons", "Integrated muon flux #theta [0-60 deg]; log_{10}(E[GeV]); log_{10} # muons",
                        (int) Ensteps, log10(Emin)-1, log10(Emax)+1,
                        500, -7, 3
                    );

        //nMuonsGr = new TGraph((int) Ensteps);


        double dE = (Emax - Emin) / Ensteps;
        double dTheta = (theta_max - theta_min) / thetansteps;
        //double dTheta = BUnits::theta_max / Theta_steps;
        double total_flux = 0.0;
        
        std::vector<double> dffv, enev;

        for (int i = 0; i < Ensteps; i++) {
            double E1 = Emin + i * dE;
            double E2 = (E1 + dE);

            float dff=0;

            for (int j = 0; j < thetansteps; j++) {
                double theta1 = theta_min + j * dTheta;
                double theta2 = theta1 + dTheta;

                // solid angle element dOmega = sin(theta) dphi dtheta (phi is isotropic)
                float phi = 2 * CLHEP::pi; // phi = 2*pi (isotropic azimuth)
                double dOmega1 = phi* sin(theta1) * dTheta; 
                double dOmega2 = phi* sin(theta2) * dTheta; 

                // Evaluate flux at these points
                double f1 = muon_flux(E1, theta1, alt) * dOmega1 * dE;
                double f2 = muon_flux(E2, theta2, alt) * dOmega2 * dE;

                // double f1 = GaisserFlux(E1/GeV, theta1/rad) * dOmega1 * dE/GeV;
                // double f2 = GaisserFlux(E2/GeV, theta2/rad) * dOmega2 * dE/GeV;

                // sum
                total_flux += 0.5 * (f1 + f2) ;
                dff += 0.5 * (f1 + f2);

                nMuons->Fill( log10((E1 + E2)/2.), log10(dff));
                // nMuonsGr->SetPoint(i, (E1/GeV + E2/GeV)/2., dff);

                integrate_flux_hist->Fill((E1 + E2)/2., (theta2/deg + theta1/deg)/2, 0.5 * (f1 + f2));
                 if(false)
                    cout << "\r\033[10C" << "TF for E = " << ((E1+E2)/2.) << " GeV - and T = " 
                            << (theta2+theta1)/2 / deg 
                            << " deg = " << total_flux
                            << " -- df: " << 0.5 * (f1 + f2)
                            << " -- i = " << i << " -- j = " << j 
                            << " -- Enst = " << Ensteps << " -- tnst = " << thetansteps 
                            << " -- th1 = " << theta1/deg << " -- th2 = " << theta2/deg
                            << endl;
            }

            dffv.push_back(dff);
            enev.push_back((E1 + E2)/2.);
            
        }

        nMuonsGr = new TGraph((int) Ensteps, &enev[0], &dffv[0]);
        nMuonsGr->SetName("graph");
        nMuonsGr->Write();
        return total_flux;  // Returns total flux in cm⁻² s⁻¹ 
    }


    // integration using trapezoidal rule
    double BPrimaryGeneratorAction::integrate_flux(double *pars) {
        double E_min, E_max, E_steps, theta_steps, altitude;
        E_min = pars[0];
        E_max = pars[1];
        E_steps = pars[2];
        theta_steps = pars[3];
        altitude = pars[4];
        
        std::cout << "BPrimaryGeneratorAction::integrate_flux: " 
        << "\n Altitude in m: " << altitude / m
        << "\n E_min in GeV: " << E_min / GeV
        << "\n E_max in GeV: " << E_max / GeV
        << "\n E_steps in GeV: " << E_steps / GeV
        << "\n theta_steps in deg: " << theta_steps / deg
        << std::endl;

                            
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
                // double f1 = muon_flux(E1, theta1, altitude) * dOmega1 * dE;
                // double f2 = muon_flux(E2, theta2, altitude) * dOmega2 * dE;
            
                double Ene1[] = {E1};
                double par1[] = {theta1, altitude};

                double Ene2[] = {E2};
                double par2[] = {theta2, altitude};
                
                double f1 = MuonFlux(Ene1, par1) * dOmega1 * dE;
                double f2 = MuonFlux(Ene2, par2) * dOmega2 * dE;


                // sum
                total_flux += 0.5 * (f1 + f2);
            }
        }
        
        return total_flux;  // Returns total flux in cm⁻² s⁻¹
    }

    // double muFlux(double* x, double* pars, int thetamin, double thetamax){
        
    //     double GFlux = 0;
    //     for(int i = thetamin; i <= thetamax; i++){
    //         double cosTheta = std::cos(theta);
    //         double term1 = 1.0 / (1.0 + 1.1 * x[0] * cosTheta / 115.0);
    //         double term2 = 0.054 / (1.0 + 1.1 * x[0] * cosTheta / 850.0);
    //         GFlux += 0.14 * std::pow(x[0], -2.7) * (term1 + term2); // muons/cm²/s/sr/GeV
    //     }
    //     return GFlux;
    // }

    double BPrimaryGeneratorAction::MuonFlux(double *energy, double *par) 
    {
    // cout << "\r\033[5C" << "---- BPrimaryGeneratorAction::MuonFlux: " << energy[0] << " - " << par[0] << " - " << par[1] << endl;
        const double epsilon_pi = 115.0; 
        const double epsilon_K  = 850.0; 
        const double H = 6.3; // in km   // Atmospheric scale height

        const double norm = 0.14;        

        double E_mu = energy[0];   // Muon energy (GeV)
        double theta = par[0]; // Zenith angle (radians)
        double h = par[1] / km ;    // Altitude (km)
        double flux = norm * pow(E_mu, -2.7) *
                    (1.0 / (1.1 + (E_mu * cos(theta) / epsilon_pi)) +
                0.054 / (1.1 + (E_mu * cos(theta) / epsilon_K)));

        flux *= exp(-h / H);  // Apply altitude correction

        return flux;
    }


    //////////////////////////////////

    double BPrimaryGeneratorAction::GaisserFlux(double E, double theta) {
        double cosTheta = std::cos(theta);
        double term1 = 1.0 / (1.0 + 1.1 * E * cosTheta / 115.0);
        double term2 = 0.054 / (1.0 + 1.1 * E * cosTheta / 850.0);
        return 0.14 * std::pow(E, -2.7) * (term1 + term2); // muons/cm²/s/sr/GeV
    }

    double BPrimaryGeneratorAction::IntegrateMuonFlux(double Emin, double Emax, double area_m2, double time_s) {
        const int nThetaSteps = 100;
        const int nESteps = 100;
        const double dTheta = (CLHEP::pi / 2.0 )/ nThetaSteps;
        const double dE = (Emax - Emin) / nESteps;

        double totalFlux = 0.0;

        for (int i = 0; i < nThetaSteps; ++i) {
            double theta = i * dTheta + dTheta / 2.0;
            double sinTheta = std::sin(theta);

            for (int j = 0; j < nESteps; ++j) {
                double E = Emin + j * dE + dE / 2.0;
                double dNdEdOmega = GaisserFlux(E, theta);
                //totalFlux += dNdEdOmega * sinTheta * dE * dTheta;
                totalFlux += dNdEdOmega * sinTheta * dE * dTheta * 2.0 * CLHEP::pi; // include azimuth

            }
        }

        // Convert area to cm²
        double area_cm2 = area_m2 * 1e4;

        return totalFlux * area_cm2 * time_s; // Total number of muons
    }

    // Power-law muon spectrum: dN/dE ∝ E^{-gamma}
    double BPrimaryGeneratorAction::gaisserSpectrum(double E) {
        return pow(E, -2.7);
    }

    // Function to generate a muon energy in [Emin, Emax] (GeV)
    double BPrimaryGeneratorAction::generateMuonEnergy(double Emin, double Emax) {
        double E, f, fmax;
        fmax = gaisserSpectrum(Emin);  // spectrum is highest at Emin

        do {
            E = Emin + (Emax - Emin) * ((double) rand() / RAND_MAX);
            f = gaisserSpectrum(E);
        } while (((double) rand() / RAND_MAX) * fmax > f);

        return E;
    }

    // Probability density function: P(θ) ∝ cos²(θ)
    double BPrimaryGeneratorAction::cos2thetaPDF(double theta) {
        return pow(cos(theta), 2);
    }

    // Generate a zenith angle θ ∈ [0, π/2] with cos²(θ) distribution
    double BPrimaryGeneratorAction::generateZenithAngle() {
        double theta, f, fmax = 1.0; // cos²(0) = 1 is the max

        do {
            theta = ((double) rand() / RAND_MAX) * (M_PI / 2.0); // Uniform in [0, π/2]
            f = cos2thetaPDF(theta);
        } while (((double) rand() / RAND_MAX) > f / fmax);

        return theta; // In radians
    }

    double BPrimaryGeneratorAction::gaisser_flux(double E) {
        double cos_theta = 1.0; // vertical
        double term1 = 1.0 / (1.0 + 1.1 * E * cos_theta / 115.0);
        double term2 = 0.054 / (1.0 + 1.1 * E * cos_theta / 850.0);
        return 0.14 * pow(E, -2.7) * (term1 + term2);
    }
    
    double BPrimaryGeneratorAction::integrate_gaisser_flux(double E_min, double E_max, int Nbins) {
        double logE_min = std::log10(E_min);
        double logE_max = std::log10(E_max);
        double dlogE = (logE_max - logE_min) / Nbins;
    
        double total_flux = 0.0;
    
        for (int i = 0; i < Nbins; ++i) {
            double logE1 = logE_min + i * dlogE;
            double logE2 = logE_min + (i + 1) * dlogE;
            double E1 = pow(10, logE1);
            double E2 = pow(10, logE2);
            double dE = E2 - E1;
    
            double flux = gaisser_flux((E1 + E2) / 2.0); // midpoint value
            total_flux += flux * dE;
        }
    
        return total_flux; // [cm^-2 s^-1 sr^-1]
    }

} // namespace lactel
