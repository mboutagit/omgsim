#include "BDetectorConstruction.hh"
#include "BDetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4PhysicalVolumeStore.hh"
#include "BPrimaryGeneratorAction.hh"


namespace lactel
{


    float BDetectorConstruction::worldSizeOffset = 20*cm;
    float BDetectorConstruction::acceptanceAngle = 10*deg;

    // std::string BDetectorConstruction::className = "BDetectorConstruction";
    BDetectorConstruction* BDetectorConstruction::GetIt(){
        if(!fInstance){
            fInstance = new BDetectorConstruction();
        }
        return (BDetectorConstruction*) fInstance;
    }

    BDetectorConstruction::BDetectorConstruction(){
        detectorMessenger = new BDetectorMessenger(this);
        cout << "\r\033[20C" << "---- Construct the Det" << endl;
    }

    BDetectorConstruction::~BDetectorConstruction(){

    }  

    void BDetectorConstruction::setTargetPosition(G4ThreeVector vect){
        targetPosition = vect;
    }

    void BDetectorConstruction::setOMPosition(G4ThreeVector vect){
        OMPosition = vect;
    }

    void BDetectorConstruction::setAcceptanceAngle(float angle){
        acceptanceAngle = angle;
    }

    void BDetectorConstruction::autoWorldSize(){
        float zmin, zmax;
        zmin = (*G4PhysicalVolumeStore::GetInstance())[0]->GetTranslation().getZ();
        zmax = (*G4PhysicalVolumeStore::GetInstance())[0]->GetTranslation().getZ();

        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
                        
            if(pv != physiWorld && dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid())){
                if(abs(pv->GetTranslation().getZ()) > zmax){
                    zmax = abs(pv->GetTranslation().getZ()) + 
                            dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->GetZHalfLength();
                     
                }

                std::cout << "\r\033[10C" << "Check Volume: " << pv->GetName() << "  -- zpos: " << pv->GetTranslation().getZ()
                            << " -- halfZ size: " << dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->GetZHalfLength()
                            << std::endl;

                    
            }
        }

        float autoZ = (abs(zmin) > abs(zmax)) ? (abs(zmin) + worldSizeOffset) : (abs(zmax) + worldSizeOffset);
        dynamic_cast<G4Box*> (solidWorld)->SetZHalfLength(autoZ);


        std::cout << "\r\033[10C" << "zmin: " << zmin << " -- zmax: " << zmax << " -- worldsizeOffSet " 
                    << worldSizeOffset << " -- val: " << autoZ << std::endl;
    }

   // G4LogicalVolume* BDetectorConstruction::Construct(const VolumeParameters* = NULL) {return 0;}
    
    G4VPhysicalVolume* BDetectorConstruction::Construct(){

        G4VPhysicalVolume* pv = KM3DetectorConstruction::Construct();

        //recalculate the size for the world and generation valume based on the target length (accept 60° events that would hit the target)
        double gen_half_length = 2*target_half_length * tan(acceptanceAngle/rad) + target_half_length; // (c+d)*tg(theta) + d/2


        // for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
        //     if(pv->GetName() == "LACTEL_Cell_Outer"){
        //         gen_half_length = 2*dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->GetXHalfLength() * tan(acceptanceAngle/rad)
        //                             + dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->GetXHalfLength();
        //     }
        // } 

        BPrimaryGeneratorAction::getInstance()->genArea = (2*gen_half_length) * (2*gen_half_length); // generation area in mm2

        //recalculate the 
        dynamic_cast<G4Box*> (solidWorld)->SetXHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (solidWorld)->SetYHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (solidWorld)->SetZHalfLength(target_half_length);

        dynamic_cast<G4Box*> (genSolid)->SetXHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (genSolid)->SetYHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (genSolid)->SetZHalfLength(0.1*mm);

        // dynamic_cast<G4Box*> (solidTarget)->SetXHalfLength(target_half_length);
        // dynamic_cast<G4Box*> (solidTarget)->SetYHalfLength(target_half_length);
        // dynamic_cast<G4Box*> (solidTarget)->SetZHalfLength(target_half_length);

        // physiTarget->SetTranslation(G4ThreeVector(0, 0, -target_half_length));
        
        G4VisAttributes* inner_vol = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9, 0.1));
        G4VisAttributes* outer_vol = new G4VisAttributes(G4Colour(.1, 0.8, 0.2, 0.1));
     
        inner_vol->SetVisibility(true); 
        inner_vol->SetForceSolid(true); 

        outer_vol->SetVisibility(true); 
        outer_vol->SetForceSolid(true);     
        
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
            //place the OM 50 cm above the target volume
            if(pv->GetName() == "AntaresOM"){
                pv->SetTranslation(G4ThreeVector(0, 0, - target_half_length + 50 *cm));
            }
                
            if(pv->GetName() == "LACTEL_Cell_Outer"){
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetXHalfLength(target_half_length);
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetYHalfLength(target_half_length);
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetZHalfLength(target_half_length);
                pv->SetTranslation(G4ThreeVector(0, 0, - target_half_length));
                pv->GetLogicalVolume()->SetVisAttributes(outer_vol);
            }

            if(pv->GetName() == "LACTEL_Cell_Inner"){
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetXHalfLength(target_half_length - 5*mm ); // 
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetYHalfLength(target_half_length - 5*mm );
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetZHalfLength(target_half_length - 5*mm );
                pv->GetLogicalVolume()->SetVisAttributes(inner_vol);
            }
        } 

        autoWorldSize();

        cout << "\r\033[20C" << "New generation surface for a detector of volume " 
                                << 2*target_half_length/m * 2*target_half_length/m * 2*target_half_length/m 
                                << " m^3 is: "  
                                <<" length = " << 2*gen_half_length/m << " m - s = "
                                << 2*gen_half_length/m * 2*gen_half_length/m << " m² - s = "
                                << 2*gen_half_length/cm * 2*gen_half_length/cm << " cm²"
                                << "\nAcceptance angle in deg: " << acceptanceAngle/deg
                                << endl;
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


} // namespace lactel

