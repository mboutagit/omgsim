#include "BDetectorConstruction.hh"
#include "BDetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4PhysicalVolumeStore.hh"
#include "BPrimaryGeneratorAction.hh"


#include "KM3PassiveVolumeMgr.hh"
#include "G4PVPlacement.hh"
#include "KM3OpticalModuleMgr.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "KM3Material.hh"
#include "BLogger.hh"





namespace lactel
{


    BDetectorConstruction::DetectorState BDetectorConstruction::state = BDetectorConstruction::DetectorState::OFF;
    std::set<std::string> BDetectorConstruction::fOMIDs;
    std::map<OM*, G4VPhysicalVolume*> BDetectorConstruction::fOMPhysicalVolumes;

    bool BDetectorConstruction::useDAQ = false;
    float BDetectorConstruction::worldSizeOffset = 5*cm;
    float BDetectorConstruction::acceptanceAngle = 10*deg;
    double BDetectorConstruction::omPos = 44*deg;
    G4ThreeVector BDetectorConstruction::OMPosition = G4ThreeVector(0, 0, 0); // set default OM position 
    std::string  BDetectorConstruction::generationShape = "box"; // set default OM position 
    G4ThreeVector BDetectorConstruction::genSurfPosition = G4ThreeVector(0, 0, .0); // set default SG position 
    G4ThreeVector BDetectorConstruction::genSurfSize = G4ThreeVector(1*m, 1*m, 1*m); // set default GS size 
    bool BDetectorConstruction::autoGenSize = false;

    bool BDetectorConstruction::isAnOM(G4VPhysicalVolume* volume){
        for(auto& ele : fOMPhysicalVolumes){
            if(volume == ele.second)    
                return true;
        }
        return false;
    }

    OM* BDetectorConstruction::getItsOM(G4VPhysicalVolume* volume){
        for(auto& ele : fOMPhysicalVolumes){
            if(volume == ele.second)    
                return ele.first;
        }
        return nullptr;
    }

    G4VPhysicalVolume* BDetectorConstruction::getPhysicalVolumeByName(std::string name){
        for(auto& v : volumesMap)
            if(v.first == name){
               return v.second.first; 
            }
        return nullptr;
    }

    void BDetectorConstruction::getOMIDbyVolumeName(std::string volumeName){
        for(auto& volume : KM3DetectorConstruction::GetIt()->getOMsMap()){
            for(auto& par : volume.second){
                BLOG_INFO(Form("Volumes: om_id: %s", par.om_id.c_str()));
            }
        }
    }
    

    void BDetectorConstruction::getOMsUsedbyVolumeName(std::string volumeName){
        std::set<std::string> oms =  KM3DetectorConstruction::GetIt()->getOMsList();
        for(auto& om : oms){
            BLOG_INFO(Form("Volumes: om_id: %s", om.c_str()));
        }
    }


    // std::string BDetectorConstruction::className = "BDetectorConstruction";
    BDetectorConstruction* BDetectorConstruction::GetIt(){
        if(!fInstance){
            fInstance = new BDetectorConstruction();
        }
        return (BDetectorConstruction*) fInstance;
    }

    BDetectorConstruction::BDetectorConstruction(){
        BLOG_INFO("Construct the Det ..");      
        detectorMessenger = new BDetectorMessenger(this);
        BLOG_INFO(Form("OMPosition: %.2f", getOMPosition()));
        DefineOpticalProperties();
    }

    BDetectorConstruction::~BDetectorConstruction(){

    }  

    void BDetectorConstruction::setTargetPosition(G4ThreeVector vect){
        targetPosition = vect;
    }

    void BDetectorConstruction::setOMPosition(G4ThreeVector vect){
        omPos =  5.1; //vect;
    }    
    
    // G4ThreeVector BDetectorConstruction::getOMPosition(){
    double BDetectorConstruction::getOMPosition(){
        return omPos;
    }

    void BDetectorConstruction::setAcceptanceAngle(float angle){
        acceptanceAngle = angle;
    }

    std::vector<G4VPhysicalVolume*> BDetectorConstruction::getPhysicalVolumesByName(std::string name){
        std::vector<G4VPhysicalVolume*> volumes;
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
            if(pv->GetName().data() == name){
                volumes.push_back(pv);
            }
        }     
        return volumes;               
    }

    std::vector<G4VPhysicalVolume*> BDetectorConstruction::getPhysicalVolumesByNameLike(std::string name){
        std::vector<G4VPhysicalVolume*> volumes;
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
            if(pv->GetName().rfind(name, 0) == 0){
                volumes.push_back(pv);
            }
        }  
        return volumes;                
    }


    void BDetectorConstruction::autoWorldSize(){

        G4ThreeVector min, max;
        float zmax;
        zmax = (*G4PhysicalVolumeStore::GetInstance())[0]->GetTranslation().getZ() + 
                    dynamic_cast<G4Box*> ((*G4PhysicalVolumeStore::GetInstance())[0]->GetLogicalVolume()->GetSolid())->GetZHalfLength(); // initilize

        G4VPhysicalVolume* farVolume;
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){
                        
            if(dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid())){ // just for box kind volumes
                
                if((abs(pv->GetTranslation().getZ()) + abs((dynamic_cast<G4Box*> (pv->GetLogicalVolume()->GetSolid()))->GetZHalfLength())) > abs(zmax)){
                    zmax = (abs(pv->GetTranslation().getZ()) + abs(dynamic_cast<G4Box*> (pv->GetLogicalVolume()->GetSolid())->GetZHalfLength()));
                    farVolume=pv;                     
                }
                
                BLOG_INFO(Form("Check volume %s at (%.2f, %.2f, %2.f) - Half Z size: %.2f", pv->GetName().c_str(),
                                                            pv->GetTranslation().getX(),
                                                            pv->GetTranslation().getY(),
                                                            pv->GetTranslation().getZ(),
                                                            dynamic_cast<G4Box*> (pv->GetLogicalVolume()->GetSolid()) ->GetZHalfLength()
                                                        ));
            }
        }

        double max_gen=0;
        if(generationShape == "disk" ){
            max_gen  = (dynamic_cast<G4Tubs*>  (genSolid))->GetOuterRadius();
        }
        else if(generationShape == "sphere" || generationShape == "hemisphere")  {
            max_gen  = (dynamic_cast<G4Sphere*>  (genSolid))->GetOuterRadius();
        }  

            
                
     
         float autoZ = zmax > max_gen ? zmax : max_gen;  // (abs(zmin) > abs(zmax)) ? 
        //                 (abs(zmin) + worldSizeOffset) : 
        //                 (abs(zmax) + worldSizeOffset);
        dynamic_cast<G4Box*> (solidWorld)->SetXHalfLength(autoZ + worldSizeOffset);
        dynamic_cast<G4Box*> (solidWorld)->SetYHalfLength(autoZ + worldSizeOffset);
        dynamic_cast<G4Box*> (solidWorld)->SetZHalfLength(autoZ + worldSizeOffset);

        BLOG_INFO(Form("zmax: %.2f, world size offset: %.2f, zmin+offset:", zmax, 
                                                                        worldSizeOffset, autoZ));
    }

    void BDetectorConstruction::setTargetType(G4String type){
        std::string functionName="BDetectorConstruction::setTargetType: ";
        GetIt()->detName = type;

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
                std::cerr << functionName << "Check the /KM3/BDet/targetType option. it should be Mxx" << endl;
                exit(-1);
            }
        } 
        else{
            std::cerr << functionName << "Check the /KM3/BDet/targetType option. it should be Mxx" << endl;
            exit(-1);
        } 
    }

    void BDetectorConstruction::setGenerationShape(std::string shape){
        generationShape = shape;
    }

    void BDetectorConstruction::setGenSurfShape(std::string shape){
        // if(! genPhysic)
        //     return;
        G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
        G4Material* vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Vacuum");

        G4double z = 1.;  // Atomic number (Hydrogen)
        G4double am = 1.01 * g/mole;  // Atomic mass
        G4double density = 1.0e-25 * g/cm3;  // Extremely low density
        G4double temperature = 2.73 * kelvin;  // Space temperature
        G4double pressure = 3.0e-18 * pascal;  // Very low pressure

                
        //                ___a__ ___b__
        //      ==========|=====|======| d     gen surface
        //                |     | |c  /
        //            ____|_____| |__/__      b = (2a+c)tan(α)
        //           |          | α /   |       = e*tan(α)
        //           |   water  |--/    | 2a 
        //           |   tank   | /     |
        //           |__________|/      |

        //  (*G4PhysicalVolumeStore::GetInstance())[0]->GetTranslation().getZ()
            
        double radius = genSurfSize.getY();
        BLOG_INFO(Form("AutoGenSize: %.2f", autoGenSize));
        

        G4Material* myVacuum = new G4Material("CustomVacuum", z, am, density, kStateGas, temperature, pressure);
    
        if(shape == "disk"){
            if(! autoGenSize){
                genSolid = new G4Tubs("disk",  genSurfSize.getX(), genSurfSize.getY(), genSurfSize.getZ(), 0, 360*deg);
                AGen = CLHEP::pi * (genSurfSize.getY()*genSurfSize.getY() - genSurfSize.getX()*genSurfSize.getX());
            }else{
                genSolid = new G4Tubs("disk",  genSurfSize.getX(), radius, genSurfSize.getZ(), 0, 360*deg);
                AGen = CLHEP::pi * (radius*radius - genSurfSize.getX()*genSurfSize.getX());
            }
        }
        else if (shape == "sphere"){
            if(! autoGenSize){
                genSolid = new G4Sphere("sphere", genSurfSize.getX(), genSurfSize.getY(), 0, 360*deg, 0, 180*deg);
                AGen = 4 * CLHEP::pi * genSurfSize.getY() * genSurfSize.getY(); // surface 
            }else{
                genSolid = new G4Sphere("sphere", genSurfSize.getX(), radius, 0, 360*deg, 0, 180*deg);
                AGen = 4 * CLHEP::pi * radius * radius; // surface 
            }
        }
        else if (shape == "hemisphere"){
            if(! autoGenSize){
                genSolid = new G4Sphere("hemisphere", genSurfSize.getX(), genSurfSize.getY(), 0, 360*deg, 0, 90*deg);
                AGen = 2 * CLHEP::pi * genSurfSize.getY() * genSurfSize.getY(); // surface 
            }else{
                genSolid = new G4Sphere("hemisphere", genSurfSize.getX(), radius, 0, 360*deg, 0, 90*deg);
                AGen = 2 * CLHEP::pi * radius * radius; // surface 
            }
        }
        else{
            if(! autoGenSize){
                genSolid = new G4Box("genSolid", genSurfSize.getX(), genSurfSize.getY(), genSurfSize.getZ());
                AGen = genSurfSize.getX() * genSurfSize.getY(); // just on x and y
            }else{
                genSolid = new G4Box("genSolid", radius, radius, radius);
                AGen = radius * radius; // just on x and y
            }
        }     
        genLogic = new G4LogicalVolume(genSolid,
                                        myVacuum, 
                                        "genLogic", 0, 0, 0);
        
        genPhysic = new G4PVPlacement(0,             // no rotation
                                    genSurfPosition, // at (0,0,0)
                                    genLogic,        // its logical volume
                                    "genPhysic",     // its name
                                    logicWorld,      // its mother  volume
                                    false,           // no boolean operations
                                    0);              // no field specific to volume  
                                    
        G4VisAttributes* genAttributes = new G4VisAttributes(G4Colour(.7, .0, 0.1, 0.2));
        genAttributes->SetVisibility(true);  // Make the volume visible
        genAttributes->SetForceSolid(true);  // Force solid rendering mode
        genLogic->SetVisAttributes(genAttributes);

        BLOG_INFO("gen volume has been created!");
    }


    void BDetectorConstruction::ReadThisConfiguration (libconfig::Setting* cfg, // reads settings from KM3Det*.dat file
                                                       vector<PositionParameters>&  ConfList) 
    {
        PositionParameters thePar;
        libconfig::Setting
        &sX     = cfg->lookup("X"),
        &sY     = cfg->lookup("Y"),
        &sZ     = cfg->lookup("Z"),
        &sTheta = cfg->lookup("Theta"),
        &sPhi   = cfg->lookup("Phi"),
        &sName  = cfg->lookup("Name"),
        &sID    = cfg->lookup("ID");
        // &sRefName  = cfg->lookup("RefName"); // added to prevent volumes duplication

        libconfig::Setting *sPsi=0;
        if (cfg->exists("Psi"))
            sPsi = &(cfg->lookup("Psi"));

        if (cfg->exists("om_id")){
            libconfig::Setting &som_id = cfg->lookup("om_id");
            thePar.om_id =GetStringParameter  (som_id);
        }


        vector <double>
        vX = GetScalarArrayParameter (sX),
        vY = GetScalarArrayParameter (sY),
        vZ = GetScalarArrayParameter (sZ),
        vTheta = GetScalarArrayParameter (sTheta),
        vPhi  = GetScalarArrayParameter (sPhi),
        vPsi;
        if (sPsi != 0)
            vPsi  = GetScalarArrayParameter (*sPsi);

        if (!vX.size ())
        thePar.X     =GetFloatingParameter (sX);
        if (!vY.size ())
        thePar.Y     =GetFloatingParameter (sY);
        if (!vZ.size ())
        thePar.Z     =GetFloatingParameter (sZ);
        if (!vTheta.size ())
        thePar.Theta =GetFloatingParameter (sTheta);
        if (!vPhi.size ())
        thePar.Phi   =GetFloatingParameter (sPhi);
        if (!vPsi.size () && sPsi != 0)
        thePar.Psi   =GetFloatingParameter (*sPsi);

        bool Multiplicity = vX.size() | vY.size() | vZ.size() | vTheta.size() | vPhi.size () | vPsi.size ();

        thePar.Name  =GetStringParameter   (sName);
        thePar.ID    =GetIntegerParameter  (sID);

        // thePar.RefName  =GetStringParameter   (sName);



        if (cfg->exists("MotherVolume"))
        thePar.MotherVolume=GetStringParameter (cfg->lookup("MotherVolume"));
        if (cfg->exists("BorderSurfaceWith"))
        thePar.BorderSurfaceWith=GetStringParameter (cfg->lookup("BorderSurfaceWith"));
        if (cfg->exists("BorderSurfaceMaterial"))
        thePar.BorderSurfaceMaterial=GetStringParameter (cfg->lookup("BorderSurfaceMaterial"));

        if (Multiplicity)
        {
            unsigned counter = 0;
            bool LetsDoIt=true;
            while (LetsDoIt)
            {
                LetsDoIt=false;
                if (counter < vX.size ())
                {
                    thePar.X     = vX[counter];
                    LetsDoIt=true;
                }

                if (counter < vY.size ())
                {
                    thePar.Y     = vY[counter];
                    LetsDoIt=true;
                }

                if (counter < vZ.size ())
                {
                    thePar.Z     = vZ[counter];
                    LetsDoIt=true;
                }

                if (counter < vTheta.size ())
                {
                    thePar.Theta = vTheta[counter];
                    LetsDoIt=true;
                }

                if (counter < vPhi.size ())
                {
                    thePar.Phi   = vPhi[counter];
                    LetsDoIt=true;
                }
                if (counter < vPsi.size ())
                {
                    thePar.Psi   = vPsi[counter];
                    LetsDoIt=true;
                }

                if (LetsDoIt)
                ConfList.push_back(thePar);

                thePar.ID++;

                counter++;
            }

            return;
        }

        ConfList.push_back(thePar);
    }

    void BDetectorConstruction::ReadThisConfiguration (BLactelDetector& detector){
        BLOG_INFO(Form("Try cell: %d ..", detector.getID()));

        libconfig::Config cfg;
        libconfig::Setting& root = cfg.getRoot();
        libconfig::Setting& volumeDet = root.add("Volume_Det" + std::to_string(detector.getID()) , libconfig::Setting::TypeGroup);
        // volume (cell)
        volumeDet.add("ID", libconfig::Setting::TypeInt) = detector.getID();
        volumeDet.add("Name", libconfig::Setting::TypeString) = detector.getName();
        volumeDet.add("X", libconfig::Setting::TypeFloat) = detector.GetPosition().getX();
        volumeDet.add("Y", libconfig::Setting::TypeFloat) = detector.GetPosition().getX();
        volumeDet.add("Z", libconfig::Setting::TypeFloat) = detector.GetPosition().getY();
        volumeDet.add("Theta", libconfig::Setting::TypeFloat) = detector.GetRotation().getTheta();
        volumeDet.add("Phi", libconfig::Setting::TypeFloat) = detector.GetRotation().getPhi();
        if(detector.getMotherVolName() != "")
                    volumeDet.add("MotherVolume", libconfig::Setting::TypeString) = detector.getMotherVolName();
        try{
               ReadThisConfiguration(&root, fVolumeList[detector.getName()]);
            }
        catch(const std::exception& e){
                std::cerr << e.what() << '\n';
                BLOG_ERROR(e.what());
            }
            
        cfg.write(stdout);


        for(auto& module : detector.GetObjects()){
            for(auto& cell : module.GetObjects()){

                libconfig::Config cfg1;
                libconfig::Setting& root1 = cfg1.getRoot();
                libconfig::Config cfg2;
                libconfig::Setting& root2 = cfg2.getRoot();

                libconfig::Setting& volume = root1.add("Volume" + std::to_string(cell.getID()) , libconfig::Setting::TypeGroup);
                libconfig::Setting& om = root2.add("OM" + std::to_string(cell.getID()) , libconfig::Setting::TypeGroup);

                // volume (cell)
                volume.add("ID", libconfig::Setting::TypeInt) = cell.getID();
                volume.add("Name", libconfig::Setting::TypeString) = cell.getName();
                volume.add("X", libconfig::Setting::TypeFloat) = cell.GetPosition().getX();
                volume.add("Y", libconfig::Setting::TypeFloat) = cell.GetPosition().getX();
                volume.add("Z", libconfig::Setting::TypeFloat) = cell.GetPosition().getY();
                volume.add("Theta", libconfig::Setting::TypeFloat) = cell.GetRotation().getTheta();
                volume.add("Phi", libconfig::Setting::TypeFloat) = cell.GetRotation().getPhi();
                
                if(cell.getVolumeProperties().getMotherVolName() != "")
                    volume.add("MotherVolume", libconfig::Setting::TypeString) = cell.getVolumeProperties().getMotherVolName();
                if(cell.getVolumeProperties().getBorderSurfName() != "")
                    volume.add("BorderSurfaceWith", libconfig::Setting::TypeString) = cell.getVolumeProperties().getBorderSurfName();
                if(cell.getVolumeProperties().getBorderSurfMatName() != "")
                    volume.add("BorderSurfaceMaterial", libconfig::Setting::TypeString) = cell.getVolumeProperties().getBorderSurfMatName();

                // OM 
                om.add("ID", libconfig::Setting::TypeInt) = cell.getID();
                om.add("Name", libconfig::Setting::TypeString) = cell.getOM().getName() + std::to_string(cell.getID());
                om.add("X", libconfig::Setting::TypeFloat) = cell.getOM().getGeometry().getPosition().getX();
                om.add("Y", libconfig::Setting::TypeFloat) = cell.getOM().getGeometry().getPosition().getY();
                om.add("Z", libconfig::Setting::TypeFloat) = cell.getOM().getGeometry().getPosition().getZ();
                om.add("Theta", libconfig::Setting::TypeFloat) = cell.getOM().getGeometry().getRotation().getTheta();
                om.add("Phi", libconfig::Setting::TypeFloat) = cell.getOM().getGeometry().getRotation().getPhi();

                if(cell.getOM().getOMID() != "")
                    om.add("om_id", libconfig::Setting::TypeString) = cell.getOM().getOMID();
                if(cell.getVolumeProperties().getMotherVolName() != "")
                    om.add("MotherVolume", libconfig::Setting::TypeString) = cell.getName();

                cfg1.write(stdout);
                cfg2.write(stdout);

                try
                {
                    ReadThisConfiguration(&root1, fVolumeList[cell.getName()]);
                    ReadThisConfiguration(&root2, fOMList[cell.getName()]);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    BLOG_ERROR(e.what());
                }
            }
        }

    }

    void BDetectorConstruction::BuildDetectorVolumes (BLactelDetector& detector, std::string matName){

        
        for(auto& module : detector.GetObjects()){
            for(auto& cell : module.GetObjects()){
  
                libconfig::Config cfg1;
                libconfig::Setting& root1 = cfg1.getRoot();
            }
        }
    }


    G4VPhysicalVolume* BDetectorConstruction::PlaceThem (KM3ParametrizedVolume* theVolToPlace, PositionParameters& PosPar) // place volume's from KM3Det*.dat 
    {
        cout << "\r\033[20C" << "Placing the detector components" << endl;
        G4LogicalVolume* WhereToPlace=0;

        KM3PassiveVolumeMgr* theVolmgr = 0;
        if (fVolumeList.size ())
        {
            theVolmgr = KM3PassiveVolumeMgr::GetIt ();
        }
        G4RotationMatrix* theRot = new G4RotationMatrix;

            //PMT1 [0.9824,-1.5708]/
            //PMT2[1.2713,-3.1416]
            //PMT13 [1.8703,1.5708]
            double theta = 1.8703;
            double phi = 1.5708;

            G4RotationMatrix* theRotPMT3 = new G4RotationMatrix();
            theRotPMT3->rotateZ(-phi);     //this is done first, counter clockwise, rotates axes too
            theRotPMT3->rotateY(-theta);   //this will be done second
            theRotPMT3->invert();
            //cout<< "inverted rotation " << theRotPMT3->getPhi() << " " << theRotPMT3->getTheta() << " " << theRotPMT3->getPsi() << endl;

        theRot->set(PosPar.Phi, PosPar.Theta, PosPar.Psi);
        G4ThreeVector thePos(PosPar.X, PosPar.Y, PosPar.Z);

        G4LogicalVolume* logicVolume=theVolToPlace->GetLogicalVolume (PosPar.Name);
        if (!logicVolume)
        {
            cerr << "ERROR:KM3DetectorConstruction didn't place " << PosPar.Name <<
            " The corresponding conf file might not exist." << endl;
            return NULL;
        }
        if (theVolmgr && PosPar.MotherVolume.size ())
        {
            WhereToPlace=theVolmgr->GetLogicalVolume (PosPar.MotherVolume);
        }
        else
        {
            // WhereToPlace = logicTarget;
            WhereToPlace = logicWorld;
        }
        if (!WhereToPlace)
        {
            cerr << "ERROR:KM3DetectorConstruction, volume "<< PosPar.MotherVolume<< " can't be found"<< endl;
            return NULL;
        }
        cout << "Placing " << logicVolume->GetName () << " |" << logicVolume << "| in " << WhereToPlace->GetName () << " |" << WhereToPlace <<  "| at " << thePos.x() << " " << thePos.y() << ' ' << thePos.z () << endl;
        const G4String LACTELCELL = "LactelCell";
        if(logicVolume->GetName() == LACTELCELL){
        G4VisAttributes* cellCol = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.2));
        cellCol->SetVisibility(true);
        cellCol->SetForceSolid(true);
        //cellCol->SetForceWireframe(true);  // Render in wireframe mode to emphasize the border
        cellCol->SetForceAuxEdgeVisible(true); 
        logicVolume->SetVisAttributes(cellCol);
        }
        
        std::string name = PosPar.Name.data ();
        if(PosPar.Name == "AntaresOM")
            name = PosPar.om_id;

        BLOG_INFO(Form("Name: ", name.c_str()));

        return new G4PVPlacement(theRot, thePos, logicVolume, name, // add identifier to distinguish volumes
                        WhereToPlace, true, PosPar.ID);

    }


    G4VPhysicalVolume* BDetectorConstruction::Construct (){

        G4double z = 1.;  // Atomic number (Hydrogen)
        G4double a = 1.01 * g/mole;  // Atomic mass
        G4double density = 1.0e-25 * g/cm3;  // Extremely low density
        G4double temperature = 2.73 * kelvin;  // Space temperature
        G4double pressure = 3.0e-18 * pascal;  // Very low pressure

        G4Material* myVacuum = new G4Material("CustomVacuum", z, a, density, kStateGas, temperature, pressure);
        G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

        solidWorld = new G4Box("world", fWorldLength,
                            fWorldLength , fWorldLength);
        
        logicWorld = new G4LogicalVolume(solidWorld,
                                        myVacuum, //TargetMater,  // ideal water
                                        "World", 0, 0, 0);
                                        
        logicWorld->SetVisAttributes(G4VisAttributes::Invisible);

        //G4Rotate3D rotationBouta(G4ThreeVector(1, 0, 0), 45 * deg);
        physiWorld = new G4PVPlacement(0,               // no rotation
                                    G4ThreeVector(), // at (0,0,0)
                                    logicWorld,      // its logical volume
                                    "World",         // its name
                                    0,               // its mother  volume
                                    false,           // no boolean operations
                                    0);              // no field specific to volume
                                        
        G4VisAttributes* worldCol = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9, 0.1));
        G4VisAttributes* genAttributes = new G4VisAttributes(G4Colour(.1, 0.0, 0.7, 0.1));
        G4VisAttributes* targetColor = new G4VisAttributes(G4Colour(0.1, 0.4, 0.0, 0.1));
        G4VisAttributes* genColor = new G4VisAttributes(G4Colour(0.5, 0.1, 0.1, 0.3));

        targetColor->SetVisibility(true);  // Make the volume visible
        targetColor->SetForceSolid(true);  // Force solid rendering mode  
        // logicTarget->SetVisAttributes(targetColor);
        
        genAttributes->SetVisibility(true);  // Make the volume visible
        genAttributes->SetForceSolid(true);  // Force solid rendering mode
        // genLogic->SetVisAttributes(genAttributes);

        worldCol->SetVisibility(true);  // Make the volume visible
        worldCol->SetForceSolid(true);  // Force solid rendering mode
        //worldCol->SetForceWireframe(true); 
        logicWorld->SetVisAttributes(worldCol);

        genColor->SetVisibility(true);  // Make the volume visible
        genColor->SetForceSolid(true);  // Force solid rendering mode


        G4RotationMatrix *targetRot=new G4RotationMatrix(0,0,0);

        targetRot->set (FOrientationPhi, FOrientationTheta, FOrientationPsi);

        BLOG_INFO(Form("Constructing %s ..", fDetectorTypeName.c_str()));

        int waterTankID=0;
        for (auto& VolPar: fVolumeList [fDetectorTypeName])
        { 
            G4VPhysicalVolume* pv = PlaceThem (KM3PassiveVolumeMgr::GetIt (), VolPar);
            fPlacedVolumeDict[VolPar.Name] = pv;
            volumesMap.insert({VolPar.Name, {pv, VolPar}});
            BLOG_INFO(Form("Volume %s placed!", VolPar.Name.c_str()));



            // if(fPlacedVolumeDict[VolPar.Name]->GetName().rfind("OuterCell", 0) == 0){
            //     // fPlacedVolumeDict[VolPar.Name]->SetName("OLC");
            // }
        }

        for (auto& VolPar: fVolumeList [fDetectorTypeName])
        {
            if (VolPar.BorderSurfaceMaterial.size () == 0 || VolPar.BorderSurfaceWith.size () ==0)
                continue;

            if (G4VPhysicalVolume* VolSecond = fPlacedVolumeDict[VolPar.BorderSurfaceWith])
            if (G4VPhysicalVolume* VolBase = fPlacedVolumeDict [VolPar.Name])
                if (G4OpticalSurface* the_surface = KM3Material::GetIt()->GetOpticalSurface (VolPar.BorderSurfaceMaterial))
                {
                    new G4LogicalBorderSurface(VolPar.Name + VolPar.BorderSurfaceWith +
                                                VolPar.BorderSurfaceMaterial + 
                                                "_border", VolBase, VolSecond, the_surface);
                }
        }

        int pmID=0;
        for (auto& PMPar: fPMList [fDetectorTypeName])
        {
            PlaceThem ( KM3PhotoTubeMgr::GetIt (), PMPar);
            BLOG_INFO(Form("PM %s placed!", PMPar.Name.c_str()));
            pmID++;
        }

        int omID=0;
        for (auto& OMPar: fOMList [fDetectorTypeName])
        {
            // ->SetName("OM"+std::to_string(omID));
            fOMNames.insert(OMPar.Name.data());
            fOMIDs.insert(OMPar.om_id.data());
            OM* om = new OM(OMPar.om_id.data());
            om->setID(OMPar.ID);
            fOMPhysicalVolumes.insert({om, PlaceThem ( KM3OpticalModuleMgr::GetIt (), OMPar)});
            BLOG_INFO(Form("OM %s placed!", OMPar.Name.c_str()));

            omID++;
        }

        setGenSurfShape(generationShape);
        G4RotationMatrix* rotZtoUp = new G4RotationMatrix();
        rotZtoUp->rotateX(90 * deg);  // Rotate 90 degrees about the X-axis
        BLOG_INFO("Detector Constructed ..");

        //recalculate the size for the world and generation valume based on the target length (accept 60° events that would hit the target)
        double gen_half_length = 2*target_half_length * tan(acceptanceAngle/rad) + target_half_length; // (c+d)*tg(theta) + d/2
        // AGen = 2*gen_half_length/cm * 2*gen_half_length/cm; // generation volume above the cell cm²

        //BPrimaryGeneratorAction::getInstance()->genArea = (2*gen_half_length) * (2*gen_half_length); // generation area in mm2
        BPrimaryGeneratorAction::getInstance()->genArea = AGen;
        //recalculate the 
    
        dynamic_cast<G4Box*> (solidWorld)->SetXHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (solidWorld)->SetYHalfLength(gen_half_length);
        dynamic_cast<G4Box*> (solidWorld)->SetZHalfLength(5*m);
        
        G4VisAttributes* inner_vol = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9, 0.1));
        G4VisAttributes* outer_vol = new G4VisAttributes(G4Colour(.1, 0.8, 0.2, 0.1));
        G4VisAttributes* lw = new G4VisAttributes(G4Colour(.1, 0.2, 0.8, 0.1));
     
        inner_vol->SetVisibility(true); 
        inner_vol->SetForceSolid(true); 

        outer_vol->SetVisibility(true); 
        outer_vol->SetForceSolid(true);  
        
        lw->SetVisibility(true); 
        lw->SetForceSolid(true);  
  
        float maxX=0, maxY=0, maxZ=0;
        for (auto& pv : *G4PhysicalVolumeStore::GetInstance()){      
            if(pv->GetName().rfind("LACTEL", 0) == 0 || pv->GetName().rfind("LacWater", 0) == 0){
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetXHalfLength(genSurfSize.getY());
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetYHalfLength(genSurfSize.getY());
                dynamic_cast<G4Box*>  (pv->GetLogicalVolume()->GetSolid()) ->SetZHalfLength(2.5*m);
                pv->GetLogicalVolume()->SetVisAttributes(lw);
            }

            if(pv->GetName().rfind("CPPMBlackBox", 0) == 0)
                pv->GetLogicalVolume()->SetVisAttributes(outer_vol);

            if(pv->GetName().rfind("LactelWaterTank", 0) == 0){
                pv->GetLogicalVolume()->SetVisAttributes(outer_vol);
            }

            if(pv->GetName().rfind("IdealWaterVolume", 0) == 0){
                pv->GetLogicalVolume()->SetVisAttributes(inner_vol);
            }
        } 

        BLOG_INFO("Auto world size ..");
        autoWorldSize();

        BLOG_INFO(Form("Detector name: %s", fDetectorTypeName.c_str()));
        std::string detFilePath=(string(PROJECT_SOURCE_DIR)+"/common/data/");
        std::string detFileName="KM3Det" + fDetectorTypeName + ".dat";
        std::string fullDetFilePath=detFilePath + "/" + detFileName;
        BLOG_INFO(Form("looking for the file name in :", (string(PROJECT_SOURCE_DIR)+"/common/data/")));
        if(fs::exists(fullDetFilePath)){
            BLOG_INFO(Form("Detector file found: ", fullDetFilePath));
            detector.read(fullDetFilePath);
            // detector.PrintSummary();
            // detector.dumpDetectorSummary();
        } 
        else{
            BLOG_ERROR(Form("Could not find detfile: ", fullDetFilePath));
            throw std::runtime_error("Could not find detector file.");
        }
            

        return physiWorld;
    }


    void BDetectorConstruction::PrintAllMaterialProperties(ostream& out){
 
        const G4MaterialTable* matTable = G4Material::GetMaterialTable();

        for (size_t i = 0; i < matTable->size(); i++)
        {
            G4Material* mat = (*matTable)[i];

            G4cout << "=================================" << G4endl;
            G4cout << "Material: " << mat->GetName() << G4endl;

            G4MaterialPropertiesTable* mpt = mat->GetMaterialPropertiesTable();

            if (mpt){
                mpt->DumpTable();
            }
            else{
                BLOG_WARNING("No MaterialPropertiesTable");
            }
        }
    }


    ///////////////////////////////// TEST //////////////////////////////////////////////

    void BDetectorConstruction::DefineOpticalProperties(){
        auto air = G4Material::GetMaterial("G4_AIR");

        // Visible range: 300–700 nm → 4.13–1.77 eV
        const G4int nEntries = 6;
        G4double photonEnergy[nEntries] =
        {4.13*eV, 3.10*eV, 2.48*eV, 2.07*eV, 1.90*eV, 1.77*eV};

        // Refractive index is nearly constant for air
        G4double refractiveIndex[nEntries] =
        {1.00029, 1.00029, 1.00029, 1.00029, 1.00029, 1.00029};

        // Absorption length (how far photons travel before being absorbed)
        // Air is very transparent to visible light — hundreds of meters
        G4double absorption[nEntries] =
        {500.*m, 500.*m, 500.*m, 500.*m, 500.*m, 500.*m};

        // Rayleigh scattering length (depends on λ⁴, but we can approximate)
        // If you want a wavelength dependence: λ⁴ scaling
        G4double rayleigh[nEntries] =
        {250.*m, 280.*m, 320.*m, 360.*m, 400.*m, 450.*m};

        // Create the material properties table
        auto airMPT = new G4MaterialPropertiesTable();
        airMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
        airMPT->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);
        airMPT->AddProperty("RAYLEIGH", photonEnergy, rayleigh, nEntries);

        // Attach it to air
        air->SetMaterialPropertiesTable(airMPT);
    }

    void BDetectorConstruction::ReadConfiguration (string ConfigFile) // reads KM3Det*.dat file base infos
    {
        if(ConfigFile.find(fDetectorTypeName)){
            BLOG_INFO(Form("Reading detector from %s ..", ConfigFile.c_str()));
            KM3DetectorConstruction::ReadConfiguration(ConfigFile);
        }else{
            BLOG_ERROR(("Could not find %s", ConfigFile.c_str()));
        }
            
    }


} // namespace lactel

