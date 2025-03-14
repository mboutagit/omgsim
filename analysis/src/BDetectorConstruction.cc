#include "BDetectorConstruction.hh"
#include "BDetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4PhysicalVolumeStore.hh"
#include "BPrimaryGeneratorAction.hh"


namespace km3net
{

    std::string BDetectorConstruction::className = "BDetectorConstruction";
    BDetectorConstruction* BDetectorConstruction::GetIt(){
        if(!fInstance){
            fInstance = new BDetectorConstruction();
        }
        return (BDetectorConstruction*) fInstance;
    }

    BDetectorConstruction::BDetectorConstruction(){
        detectorMessenger = new BDetectorMessenger(this);
        cout << "\r\033[20C" << "---- Construction the Det" << endl;
    }

    BDetectorConstruction::~BDetectorConstruction(){

    }  

    void BDetectorConstruction::setTargetPosition(G4ThreeVector vect){
        targetPosition = vect;
    }

   // G4LogicalVolume* BDetectorConstruction::Construct(const VolumeParameters* = NULL) {return 0;}
    
    G4VPhysicalVolume* BDetectorConstruction::Construct(){

        G4VPhysicalVolume* pv = KM3DetectorConstruction::Construct();
        dynamic_cast<G4Box*> (solidTarget)->SetXHalfLength(target_half_length);
        dynamic_cast<G4Box*> (solidTarget)->SetYHalfLength(target_half_length);
        dynamic_cast<G4Box*> (solidTarget)->SetZHalfLength(target_half_length);

        double length = target_half_length + 2*target_half_length * tan(CLHEP::pi /3.);
        
        // cout << "\r\033[20C" << "New generation surface for a detector of volume " 
        //                         << 2*target_half_length/m * 2*target_half_length/m * 2*target_half_length/m 
        //                         << " m^3 is: " 
        //                         << length/m * length/m << " m²"
        //                         << "\n rho_air: " << BUnits::rho_air
        //                         << "\n H_0: " << BUnits::H_0
        //                         << "\n theta_max: " << BUnits::theta_max
        //                         << endl;

        // double ene[]={10*GeV};
        // double pars[]={60, 372};
        // cout << "\r\033[20C" << "Flux expected: " 
        //     << BPrimaryGeneratorAction::integrate_flux(372*m, 1*GeV, 100*GeV, 100, 100) * m/cm * m/cm * 60 * 60 * 19.9282
        //     << endl;

        dynamic_cast<G4Box*> (genSolid)->SetXHalfLength(length);
        dynamic_cast<G4Box*> (genSolid)->SetYHalfLength(length);
        dynamic_cast<G4Box*> (genSolid)->SetZHalfLength(target_half_length);

        dynamic_cast<G4Box*> (solidWorld)->SetXHalfLength(length);
        dynamic_cast<G4Box*> (solidWorld)->SetYHalfLength(length);
        dynamic_cast<G4Box*> (solidWorld)->SetZHalfLength(target_half_length*2);

        physiTarget->SetTranslation(G4ThreeVector(0, 0, -target_half_length));
        genPhysic->SetTranslation(G4ThreeVector(0, 0, -target_half_length));
        
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
            if(pv->GetName() == "AntaresOM")
                pv->SetTranslation(G4ThreeVector(0, 0, - target_half_length + 50 *cm));
        } 
        return pv;
    }

    void BDetectorConstruction::setTargetType(G4String type){
        std::string functionName="BDetectorConstruction::setTargetType: ";
        if(type.length() <= 3){
            if(type.substr(0, 1) == "M"){
                try
                {
                    float length = std::stoi(type.substr(1, type.length() - 1));
                    GetIt()->target_half_length = length * CLHEP::m / 2.;
                }
                catch(const std::exception& e)
                {      
                    std::cerr << functionName << e.what() << '\n';
                }       
            }
            else{
                std::cerr << functionName << "Check the /KM3/BDet/targetType option. it shoulb be Mxx" << endl;
                exit(-1);
            }
        } 
        else{
            std::cerr << functionName << "Check the /KM3/BDet/targetType option. it shoulb be Mxx" << endl;
            exit(-1);
        } 
    }


} // namespace km3net

