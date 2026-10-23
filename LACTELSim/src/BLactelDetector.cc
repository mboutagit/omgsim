#include "BLactelDetector.hh"


namespace lactel
{
    ElementaryCell::ElementaryCell(): GeometryObject(G4ThreeVector(), G4ThreeVector(2*m, 2*m, 2*m)){}
    ElementaryCell::ElementaryCell(int uid): id(uid), GeometryObject(G4ThreeVector(), G4ThreeVector(2*m, 2*m, 2*m)){}
    ElementaryCell::ElementaryCell(G4ThreeVector pos) : ElementaryCell(){SetPosition(pos);}
    ElementaryCell::ElementaryCell(OM& _om) : ElementaryCell(){om=_om;}
    ElementaryCell::ElementaryCell(OM _om) : ElementaryCell() {om = _om;}
    ElementaryCell::ElementaryCell(G4ThreeVector pos, G4ThreeVector dim): ElementaryCell(pos){SetDimensions(dim);}
    ElementaryCell::ElementaryCell(G4ThreeVector pos, G4ThreeVector dim, OM _om): ElementaryCell(pos, dim){ om = _om;}
    ElementaryCell::ElementaryCell(int uid, G4ThreeVector pos, G4ThreeVector dim, OM _om, int mdl, int lyr): ElementaryCell(pos, dim){ 
        id = uid;
        om = _om;
        module = mdl;
        layer = lyr;
    }
    ElementaryCell::~ElementaryCell(){}

    GeometryObject& ElementaryCell::getTransform(){
        return static_cast<GeometryObject&>(*this);
    }

    void ElementaryCell::setName(std::string _name){name=_name;}
    std::string ElementaryCell::getName(){return name;}
    int ElementaryCell::getID(){return id;}
    OM& ElementaryCell::getOM(){return om;}
    void ElementaryCell::setOM(OM& _om){om = _om;}

    void ElementaryCell::setOMPosition(G4ThreeVector new_pos){
        om.setPoition(new_pos);
    }

    void ElementaryCell::centerX(){
        fPosition = G4ThreeVector(0, fPosition.getY(), fPosition.getZ());
    }

    void ElementaryCell::centerY(){
        fPosition = G4ThreeVector(fPosition.getX(), 0, fPosition.getZ());
    }

    void ElementaryCell::centerZ(){
        fPosition = G4ThreeVector(fPosition.getX(), fPosition.getY(), 0);
    }
    void ElementaryCell::center(){  
                    centerX(); 
                    centerY(); 
                    centerZ();
                }

    void ElementaryCell::print(std::ostream& out){
        out << "Cell: "      << id        << " - "
            << "Name: "      << name      << " - "
            << "OM: "        << om        << " - "
            << "Module: "    << module        << " - "
            << "Layer: "     << layer        << " - "
            << "position: "  << fPosition<< " - "
            << "dimension: " << fDimensions << " - "
            << std::endl;
    }

    /**
     * @brief dump into an ostream the volumes for a given cell including its associated OM. This would help to construct the detector.
     * @param out an ostream where to output the cell material
     */
    void ElementaryCell::dump(ostream& out){
        // volumes

        //Container
        out << "\nVolumeWT" + std::to_string(id) + ": # Water tank volume\n{\n";
        out << "\tName = \"LactelWaterTank" + std::to_string(id) + "\";\n";
        out << "\tID   = " + std::to_string(id) + ";\n";
        out << "\tX      = " + std::to_string(fPosition.getX()) + ";\n";
        out << "\tY      = " + std::to_string(fPosition.getY()) + ";\n";
        out << "\tZ      = " + std::to_string(fPosition.getZ()) + "; # in mm\n";
        out << "\tModule  = " + std::to_string(module) + "; #Module number;\n";
        out << "\tLayer  = " + std::to_string(layer) + "; #Layer number;\n";
        out << "\tTheta  = 0.00; #3.14;\n";
        out << "\tPhi    = 0.00;\n";
        out << "\tMotherVolume = \"" + volProp.motherVolume + "\";\n";
        out << "}\n";

        // Water
        out << "\nVolumeWInside" + std::to_string(id) + ": # Water inside the water tank\n{\n";
        out << "\tName = \"" + name + "_" + std::to_string(id) + "\";\n";
        out << "\tID   = " + std::to_string(id) + ";\n";
        out << "\tX      = 0.00;\n";
        out << "\tY      = 0.00;\n";
        out << "\tZ      = 0.00; # in mm\n";
        out << "\tTheta  = 0.00; #3.14;\n";
        out << "\tPhi    = 0.00;\n";
        out << "\tMotherVolume = \"LactelWaterTank" + std::to_string(id) + "\";\n";
        out << "}\n";
        
        // OM
        out << "\nOM" + std::to_string(id) + ": # OM inside the water volume\n{\n";
        out << "\tName = \"AntaresOM\";\n";
        out << "\tID   = " + std::to_string(id) + ";\n";
        out << "\tom_id = \"" + getOM().getOMID() + "\";\n";
        out << "\tX      = " + std::to_string(getOM().getPosition().getX()) + ";\n";
        out << "\tY      = " + std::to_string(getOM().getPosition().getY()) + ";\n";
        out << "\tZ      = " + std::to_string(getOM().getPosition().getZ()) + "; # in mm\n";
        out << "\tTheta  = 0.00; #3.14;\n";
        out << "\tPhi    = 0.00;\n";
        out << "\tMotherVolume =\"" + name + "_" + std::to_string(id) + "\";\n";
        out << "}\n";
    }

    /**
     * @brief dump into an ostream the meterial for a given cell. This would help to construct the detector.
     * @param out an ostream where to output the cell material
     */
    void ElementaryCell::dumpMaterial(ostream& out){

        out <<  name + "_" + std::to_string(id) + ":\n{\n";
        out << "\tName = \"" + name + "_" + std::to_string(id) + "\";\n";
        out << "\tPrefix = \"IWV\";\n";
        out << "\tMaterial = \"IdealWater\";\n";
        out << "\tVolumeKind = \"Box\";\n";
        out << "\tX = 1000; // Half dim 2X2X2 m \n";
        out << "\tY = 1000;\n";
        out << "\tZ = 1000;\n";
        out << "\tCheckOverlaps = false;\n";
        out << "}\n";

        out << "LactelWaterTank" + std::to_string(id) + ":\n{\n";
        out << "\tName= \"LactelWaterTank" + std::to_string(id) + "\";\n";
        out << "\tPrefix = \"LWT1\"; # Lactel Weter Tank \n";
        out << "\tMaterial = \"G4_POLYPROPYLENE\"; \n";
        out << "\tVolumeKind = \"Box\"; \n";
        out << "\tX = 1005; \n";
        out << "\tY = 1005; \n";
        out << "\tZ = 1005; \n";
        out << "\tRotTheta = 0; \n";
        out << "\tRotPhi = 0; \n";
        out << "\tCheckOverlaps = true;\n";
        // out << "\tRefVolume = \"OuterCell\"; \n";
        // out << "\tOperandVolume = \"InnerCell\"; \n";
        // out << "\tCheckOverlaps = false; \n";
        out << "\tSkinMaterial = \"LactelWaterTankSurf\"; \n";
        out << "}\n\n";
    }


    void ElementaryCell::draw(Option_t* options, std::string axis){
        // double x1, x2, y1, y2;
        // if(axis == "XY" || axis == "Xy" || axis == "Xy" || axis == "xy"){
        //     double x1 = GetPosition().getX() - GetDimensions().getX()/2.;
        //     double x2 = GetPosition().getX() + GetDimensions().getX()/2.;
        //     double y1 = GetPosition().getY() - GetDimensions().getY()/2.;
        //     double y2 = GetPosition().getY() - GetDimensions().getY()/2.;
        // }
        // else if(axis == "XZ" || axis == "Xz" || axis == "xZ" || axis == "xz"){
        //     double x1 = GetPosition().getX() - GetDimensions().getX()/2.;
        //     double x2 = GetPosition().getX() + GetDimensions().getX()/2.;
        //     double y1 = GetPosition().getZ() - GetDimensions().getZ()/2.;
        //     double y2 = GetPosition().getZ() - GetDimensions().getZ()/2.;
        // }
        // else if(axis == "YZ" || axis == "Yz" || axis == "yZ" || axis == "yz"){
        //     double x1 = GetPosition().getY() - GetDimensions().getY()/2.;
        //     double x2 = GetPosition().getY() + GetDimensions().getY()/2.;
        //     double y1 = GetPosition().getZ() - GetDimensions().getZ()/2.;
        //     double y2 = GetPosition().getZ() - GetDimensions().getZ()/2.;
        // }
        // else{
        //     BLogger::getInstance()->log(BLogger::Level::ERROR, "ElementaryCell::draw: Unkown axis!");
        // }
        TPave* cellPave = new TPave(    GetPosition().getX() - GetDimensions().getX()/2.,
                                        GetPosition().getY() - GetDimensions().getY()/2.,
                                        GetPosition().getX() + GetDimensions().getX()/2.,
                                        GetPosition().getY() + GetDimensions().getY()/2.,
                                        1, 
                                        ""
                                    );
        cellPave->SetName(("Cell_" + std::to_string(id)).c_str());
        cellPave->SetFillColorAlpha(4, 0.4);
        cellPave->Draw(options);
    }




///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                   Module Class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////
    

    Module::Module(): GeometryObject(G4ThreeVector(0, 0, 0), G4ThreeVector(2*m, 2*m, 4.5*m)), GeometryGrid(1, 1, 2, G4ThreeVector(0, 0, VInterCellDistance)){
        updateGeometry();
    };

    Module::Module(int uid): id(uid), GeometryObject(G4ThreeVector(0, 0, 0), G4ThreeVector(2*m, 2*m, 4.5*m)), GeometryGrid(1, 1, 2, G4ThreeVector(0, 0, VInterCellDistance)) {}

    Module::Module(std::vector<ElementaryCell> cells){
        addObjects(cells);
    }

    Module::Module(int uid, std::vector<ElementaryCell> cells){
        id = uid;
        addObjects(cells);
    }

    Module::Module(std::vector<ElementaryCell> cells, G4ThreeVector spacing){
        addObjects(cells);
        SetSpacing(spacing);
    }

    Module::Module(int nx, int ny, int nz): Module(){
        SetSpacing(G4ThreeVector(.5*m));
        updateGeometry();
    };
    Module::Module(int nx, int ny, int nz, G4ThreeVector spacing): Module(){
        SetSpacing(spacing);
        updateGeometry();
    };
    Module::~Module(){};

    Module::Module(const Module& other){
        *this = other;
    }

    void Module::reset(){
        GeometryGrid::Clear();
    }

    void Module::setName(std::string _name){name=_name;}
    std::string Module::getName(){return name;}
    int Module::getID(){return id;}

    void Module::centerCellsX(){
        for(auto& cell : GetObjects())
            cell.centerX();
        updateGeometry();
    }
    void Module::centerCellsY(){
        for(auto& cell : GetObjects())
            cell.centerY();
        updateGeometry();
    }
    void Module::centerCellsZ(){
        for(auto& cell : GetObjects())
            cell.centerZ();
        updateGeometry();
    }
    void Module::centerCells(){
        for(auto& cell : GetObjects())
            cell.getTransform().SetPosition(G4ThreeVector());
        updateGeometry();
    }

    void Module::addECell(std::string axis){
        if(axis == "X" || axis == "x")
            fNx++;
        else if(axis == "Y" || axis == "y")
            fNy++;
        else if(axis == "Z" || axis == "z")
            fNz++;
        else
            BLogger::getInstance()->log(BLogger::Level::WARNING, "Module::addCell: Unkown axis " + axis);
        updateGeometry();
    }

    // void Module::addECell(ElementaryCell& cell){
    //     GetObjects().push_back(cell);
    //     updateGeometry();
    // }

    int Module::getNCells(){
        return GetTotal();
    }

    int Module::getNLayers(int axis){
        switch (axis)
        {
            case 1:
                GetNx();
                break;
            case 2:
                GetNy();
                break;
            case 3:
                GetNz();
                break;
            
            default:
                std::cout << "\r\033[10C" << "Error axis" << std::endl;
                break;
        }

        return getNCells();
    }

    void Module::print(std::ostream& out, bool printCells){
        std::cout << "Module: " << id 
                            << " - name: " << name 
                            << " - pos: " <<  GetPosition()
                            << " - dim: " <<  GetDimensions()
                            << std::endl; 
        if(printCells){
            if(! GetObjects().size()){
                out << "\tModule " << id << " has no cells." << endl;
                return;
            }
                
            for(auto& cell : GetObjects()){
                cell.print(out);
            }
        }    
    }

    /**
     * @brief loops over the module cells and dump into an ostream the cell volumes including the associated OM.
     * @param out an ostream where to output the cell material
     * @param dumpName a boolean to print the module name first.
     */
    void Module::dumpCells(std::ostream& out, bool dumpName){

        if(dumpName)
            out << "Name = \"" << getName() << "\"\n\n" << std::endl;

        for(auto& cell : GetObjects())
            cell.dump(out);
    }

    /**
     * @brief loops over the module cells Material and dump into an ostream. This would preapre .dat file for detector volumes.
     * @param out an ostream where to output the cell material
     */
    void Module::dumpCellsMaterial(std::ostream& out){
        for(auto& cell : GetObjects())
            cell.dumpMaterial(out);
    }

    void Module::addNECells(int n, std::string axis){
        for(int i = 1; i<=n; i++)
            addECell(axis);
        updateGeometry();
    }

    void Module::setID(int uid){
        id = uid;
    }

    std::vector<ElementaryCell>& Module::getElementaryCells(){
        return GetObjects();
    }



    void Module::updatePosition(){
        G4ThreeVector pcum;
        if(! GetObjects().size())
            return;

        G4ThreeVector dup = GetObjects()[0].GetDimensions();
        G4ThreeVector minPos = GetObjects()[0].GetPosition(), 
                        maxPos = GetObjects()[0].GetPosition();
        G4ThreeVector minDim = GetObjects()[0].GetDimensions(), 
                        maxDim = GetObjects()[0].GetDimensions();

        for (auto& cell : GetObjects()){
            pcum += cell.GetPosition();
            minPos = std::min(minPos, cell.GetPosition());
            maxPos = std::max(maxPos, cell.GetPosition());

            minDim = std::min(minDim, cell.GetDimensions());
            maxDim = std::max(maxDim, cell.GetDimensions());

            std::cout << "\r\033[10C" << "cell: " << cell.getID() << 
                                        " - Pos: " << cell.GetPosition() <<
                                        " - minPos: " << minPos <<
                                        " - maxPos: " << maxPos <<
                                        " - minDim: " << minDim <<
                                        " - maxDim: " << maxDim <<
                                        std::endl;
        }
        SetPosition((maxPos+maxDim/2 - minPos+minDim/2)/GetObjects().size());
    }

    void Module::SetPosition(G4ThreeVector pos){
        updateECellsPosition(GetPosition(), pos); // descplace cells before chaniging the old pos (GetPosition())
        GeometryObject::SetPosition(pos); 
    }

    void Module::updateDimension(){
        if(getNCells() == 0) // no cell yet
            return;
        G4ThreeVector dup = GetObjects()[0].GetDimensions();
        G4ThreeVector minPos = GetObjects()[0].GetPosition(), 
                        maxPos = GetObjects()[0].GetPosition();
        G4ThreeVector minDim = GetObjects()[0].GetDimensions(), 
                        maxDim = GetObjects()[0].GetDimensions();

        for (auto& cell : GetObjects()){
            minPos = std::min(minPos, cell.GetPosition());
            maxPos = std::max(maxPos, cell.GetPosition());

            minDim = std::min(minDim, cell.GetDimensions());
            maxDim = std::max(maxDim, cell.GetDimensions());
        }

        std::cout << "\r\033[10C" << "Old Dim: " << GetDimensions() 
                                    << " -- New Dim: " << maxPos + maxDim - (minPos - minDim)
                                    << std::endl;
        SetDimensions((maxPos + maxDim )/2. + (minPos + minDim)/2. );
    }


    void Module::updateECellsPosition(G4ThreeVector OldPos, G4ThreeVector NewPos){
        for(auto& cell : GetObjects()){
            G4ThreeVector tv = NewPos - OldPos;
            cell.Translate(tv);
        }
    }

    void Module::rebuildGrid(){

    }


    // void Module::setDimension(G4ThreeVector vd){ // imension is controlled by cells
    //     dimension = vd;
    // }

    void Module::updateGeometry(){
        updatePosition();
        updateDimension();

    }
    

    void Module::setDefault(){
        reset();
        updateGeometry();
        // position.setZ();
    }

    const ElementaryCell& Module::getECellsPrototype() const {
        if(! GetObjects().size()){
            BLogger::getInstance()->log(BLogger::Level::WARNING, "Module::getECellsPrototype: No object has been assigned yet.");
            return ElementaryCell();
        }
        return GetObjects()[0]; // return the first object as prototype;
    }


    void Module::draw(Option_t* options, bool _drawCells){
        TPave* modulePave = new TPave(  GetPosition().getX() -  GetDimensions().getX()/2.,
                                        GetPosition().getY() -  GetDimensions().getY()/2.,
                                        GetPosition().getX() +  GetDimensions().getX()/2.,
                                        GetPosition().getY() +  GetDimensions().getY()/2.,
                                    1, 
                                    ""
                                );
        modulePave->SetName("Module_" + id);
        modulePave->SetFillColorAlpha(6, 0.4);
        modulePave->Draw(options);
        modulePave->SetFillColorAlpha(id = (id == 0) ? id+1 : id, 0.4);

        if(_drawCells)
                drawCells("same");
    }

    void Module::drawCells(Option_t* options){
        if(! GetObjects().size())
            return;
        const int size = GetObjects().size();
        TPave* cellBox[size];
        int color = 1;
        int iter_cell = 0;


        for(int itr = 0; itr < GetObjects().size(); itr++){
            ElementaryCell& cell = GetObjects()[itr];
            cellBox[itr] = new TPave(   cell.getTransform().GetPosition().getX() - cell.getTransform().GetDimensions().getX()/2.,
                                        cell.getTransform().GetPosition().getY() - cell.getTransform().GetDimensions().getY()/2.,
                                        cell.getTransform().GetPosition().getX() + cell.getTransform().GetDimensions().getX()/2.,
                                        cell.getTransform().GetPosition().getY() + cell.getTransform().GetDimensions().getY()/2.,
                                        1, 
                                        ""
                                    );
            cellBox[itr]->SetName(("Module_" + std::to_string(id) + "-Cell_" + std::to_string(cell.getID())).c_str());
            cellBox[itr]->SetFillColorAlpha(cell.getID()+1, 0.4);
            cellBox[itr]->Draw(options);
            color++;

        }
    }


///////////////////////////////////////////////////////////////////////////////////////////////////////
//
//                                   BLactelDetector Class
//
///////////////////////////////////////////////////////////////////////////////////////////////////////

    int BLactelDetector::gID = 0;

    BLactelDetector::BLactelDetector(): id(gID), GeometryObject(G4ThreeVector(0, 0, 0), G4ThreeVector(2*m, 2*m, 4.5*m)), 
                                        GeometryGrid(3, 3, 1, G4ThreeVector(50*cm, 50*cm, 50*cm)) 
                                        {gID++;}
    BLactelDetector::BLactelDetector(int nx, int ny, int nz): id(gID), GeometryObject(G4ThreeVector(0, 0, 0), 
                                        G4ThreeVector(2*m, 2*m, 4.5*m)), 
                                        GeometryGrid(nx, ny, nz, G4ThreeVector(50*cm, 50*cm, 50*cm))
                                        {gID++;}
    BLactelDetector::BLactelDetector(int nx, int ny, int nz, G4ThreeVector spacing): id(gID), 
                                        GeometryObject(G4ThreeVector(0, 0, 0), G4ThreeVector(2*m, 2*m, 4.5*m)), 
                                        GeometryGrid(nx, ny, nz, spacing)
                                        {gID++;}
    
    BLactelDetector::BLactelDetector(vector<Module> modules) : id(gID){
        addObjects(modules);
    }

    BLactelDetector::BLactelDetector(vector<Module> modules, G4ThreeVector spacing) : id(gID){
        addObjects(modules);
        SetSpacing(spacing);
    }

    BLactelDetector::~BLactelDetector(){};

    std::string BLactelDetector::filesPath = (string(PROJECT_SOURCE_DIR)+"/common/data/");

    void BLactelDetector::BuildGrid(Module& prototype){
        GeometryGrid::BuildGrid(prototype);
        updateCellsMotherVolume();
    }

    void BLactelDetector::print(std::ostream& out, bool printModules, bool printCells){
        out     << string(25, '+') 
                << " Detector: " << getName() 
                << " - id: " << getID() 
                << " - Pos: " << GetPosition()
                << " - Dim: " << GetDimensions()
                << string(25, '+') 
                << endl << endl;

        if(printModules)
            for(auto& module : GetObjects())
                module.print(out, printCells);
        if(!printModules && printCells)
            for(auto& module : GetObjects())
                for(auto& cell : module.GetObjects())
                    cell.print(out);
    }

    void BLactelDetector::dump(std::ostream& out){

        out << "\n# Main volume (detector name): " + getName() + "\n";
        out << "Name= \"" << getName() << "\";\n\n" << std::endl;

        out << "VolumeDet" + std::to_string(id) + ":\n{\n";
        out << "\t Name = \"" + name + "\";\n";
        out << "\t ID = " + std::to_string(id) + "\n";
        out << "\t X = " + std::to_string(GetPosition().getX()) + "; # generic size \n";
        out << "\t Y = " + std::to_string(GetPosition().getY()) + ";\n";
        out << "\t Z = " + std::to_string(GetPosition().getZ()) + ";\n";
        out << "\t Theta = " + std::to_string(GetRotation().getTheta()) + ";\n";
        out << "\t Phi = " + std::to_string(GetRotation().getPhi()) + ";\n";
        out << "}\n\n";

        out << "# Module and Cells\n\n";

        for(auto mod : GetObjects())
            mod.dumpCells(out);
    }

    void BLactelDetector::dumpMaterial(std::ostream& out){

        out << "# Main Volume: Detector\n\n";
        out <<  name + ":\n{\n";
        out << "\tName = \"" + name + "\";\n";
        out << "\tPrefix = \"MainVol\";\n";
        out << "\tMaterial = \"IdealWater\";\n";
        out << "\tVolumeKind = \"Box\";\n";
        out << "\tX = 1.0; // Half dim 2X2X2 m \n";
        out << "\tY = 1.0;\n";
        out << "\tZ = 1.0;\n";
        out << "\tCheckOverlaps = false;\n";
        out << "}\n";


        out << "# Module and Cells Materials\n\n";
        for(auto mod : GetObjects())
            mod.dumpCellsMaterial(out);
    }

    void BLactelDetector::dumpDetectorSummary(std::ostream& out){
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        int m=1, c=1; // counters
        out << "# Generated using: BLactelDetector::dump function. \
                \n# Date: " <<  std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S")                
                << "\n# Detector Name: " << name
                << "\n# Detector ID: " << id
                << "\n# Number of Modules: " << GetObjects().size()
                << "\n# Description: \n";
                for(auto& mod : GetObjects()){
                    out << "# " << m << string(10, '-') << "Module: " << mod.getID() << " - Name: " << mod.getName() << "\n";
                    for (auto& cell : mod.GetObjects()){
                        out << "#" << string(10, '-') << c << string(10, '-') << "cell: " << cell.getID() << " - Name: " << cell.getName() 
                                                        << " - Position: " << cell.GetPosition()
                                                        << " - Dimensions: " << cell.GetDimensions()
                                                        << " - Assigned OM: " << cell.getOM().getOMID() 
                                                        << " - OM pos: " << cell.getOM().position 
                                                        << "\n";
                    c++;
                    } 
                m++;   
                }
        out << "\n\n";
    }


    void BLactelDetector::generateDetectorFiles(bool printSummary){

        std::string matFileName = filesPath + "/KM3Vol" + name + ".dat";
        std::string detFileName = filesPath + "/KM3Det" + name + ".dat";
        std::ofstream detFile(detFileName, std::ios::out);
        std::ofstream matFile(matFileName, std::ios::out);

        updateCellsMotherVolume();

        if(printSummary){
            dumpDetectorSummary(detFile);
            dumpDetectorSummary(matFile);
        }

        detFile << "# This part is used to construct the detector. Please do not remove it\n";
        detFile << "Config:\n{\n";
        detFile << "\t detectorFile = \"" + detFileName + "\";\n";
        detFile << "\t materialFile = \"" + matFileName + "\";\n";
        detFile << "}\n\n";

        dump(detFile);
        dumpMaterial(matFile);

        detFile.close();
        matFile.close();

        // TFile file((getName() + ".root").c_str(), "recreate");
        // TTree tree(getName().data(), "detector file");
        // tree.SetBranchAddress("detector", this);
        // tree.Write();
        // file.Close();
    }

    void BLactelDetector::construct() {
        for(auto& module : GetObjects())
            BuildGrid(module);
    }

    void BLactelDetector::updatePosition(){
        GetGridCenter();
    }
    void BLactelDetector::updateDimension(){
        GetGridDimensions();
    }
    void BLactelDetector::drawModules(Option_t* options, bool drawMod, bool drawCells){
        
        TPave* DetPave = new TPave(GetPosition().getX() - GetDimensions().getX()/2.,
                                    GetPosition().getY() - GetDimensions().getY()/2.,
                                    GetPosition().getX() + GetDimensions().getX()/2.,
                                    GetPosition().getY() + GetDimensions().getY()/2.,
                                    1, 
                                    ""
                                );
        DetPave->SetName("Detector_" + id);
        DetPave->SetFillColorAlpha(5, 0.4);
        DetPave->Draw(options);

        if(drawMod)
            for(auto& mod : GetObjects())
                    mod.draw("same", drawCells);
        if(! drawMod && drawCells)
            for(auto& mod : GetObjects())
                for(auto& cell : mod.GetObjects())
                        cell.draw("same");
    }

    TGraph* BLactelDetector::drawMZ(){
        TGraph* gr = new TGraph();
        int nc = 0;
        for(auto& m : GetObjects()){
            for(auto& cell : m.GetObjects()){
                gr->SetPoint(nc, m.getID(), cell.getLayer());
                nc++;
            }
        }
        gr->SetTitle(
                        ("Detector: " + name + 
                            "; Module; Layer").data()
                    );
        return gr;
    }

    
    /**
     * @brief returns number of modules in this detector
     * @see @class Module
     * @example int nModules = detector.getNModules();
     */
    int BLactelDetector::getNModules() const {
        return GetObjects().size();
    }

    /**
     * @brief returns the number of cells used in that detector regardless the number of modules.
     */
    int BLactelDetector::getNCells() const {
        int nCells = 0;
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects())
                nCells++;
        return nCells;
    }


    /**
     * @brief returns the OM of a given id 
     * @param oid, the OM identifier in the detector int not string
     */
    OM* BLactelDetector::getOMByID(int oid) const{

    }

    int BLactelDetector::getOMIDByRefID(std::string ref){
        for(auto& om : getListOfOMs())
            if(om->om_id == ref)
                return om->id;
        return -99;
    }

    /**
     * @brief return a vector of cell that are in this detector
     */
    std::vector<ElementaryCell*> BLactelDetector::getListOfECells(){
        std::vector<ElementaryCell*> cells;
        for(auto& m : getListOfModules())
            for(auto& c : m->GetObjects())
                cells.push_back(&c);
        return cells;
    }

    void BLactelDetector::printListOfECells(std::ostream& out){
        for(auto& c : getListOfECells())
            c->print(out);
    }

    std::vector<Module*> BLactelDetector::getListOfModules(){
        std::vector<Module*> modules;
        for(auto& m : GetObjects())
            modules.push_back(&m);
        return modules;
    }

    void BLactelDetector::printListOfModules(std::ostream& out){
        for(auto& m : GetObjects())
            m.print(out);
    }

    std::vector<OM*> BLactelDetector::getListOfOMs(){
        std::vector<OM*> OMs;
        for(auto& c : getListOfECells())
            OMs.push_back(&c->getOM());
        return OMs;
    }

    void BLactelDetector::printListOfOMs(std::ostream& out){
        for(auto& c : getListOfECells())
            c->getOM().dump(out);
    }

    ElementaryCell* BLactelDetector::getOMECell(OM& om){
        for(auto& c : getListOfECells()){
            if(c->getOM() == om)
                return c;
        }
        return nullptr;
    }

    ElementaryCell* BLactelDetector::getOMECell(std::string om_id){
        for(auto& c : getListOfECells()){
            if(c->getOM().getOMID() == om_id)
                return c;
        }
        return nullptr;
    }

    /**
     * @brief get the Elementary cell Correspoding to a given OM id
     * @param oid: OM id 
     * @return ElementaryCell*
     * @see BLactelDetector
     */
    ElementaryCell* BLactelDetector::getOMECell(int oid){
        for(auto& c : getListOfECells()){
            // std::cout << "\r\033[10C" << "checking cell: " << c->getID() 
            //                             << " - with OM: " << c->getOM().getOMID() 
            //                             << " - with OM iid: " << c->getOM().getID() 
            //                             << std::endl;
            if(c->getOM().getID() == oid)
                return c;
        }
        return nullptr;
    }


    Module* BLactelDetector::getOMModule(OM& om){
        for(auto& m : getListOfModules()){
            for(auto& c : getListOfECells())
                if(c->getOM() == om)
                    return m;
        }
        return nullptr;
    }

    Module* BLactelDetector::getOMModule(std::string om_id){
        for(auto& m : getListOfModules()){
            for(auto& c : getListOfECells())
                if(c->getOM().getOMID() == om_id)
                    return m;
        }
        return nullptr;
    }

    void BLactelDetector::updateGeometry(){
        updatePosition();
        updateDimension();
    }

    void BLactelDetector::updateCellsMotherVolume(){
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects())
                cell.getVolumeProperties().setMotherVolName(getName());
    }

    void BLactelDetector::setCellsRefVolName(std::string vname){
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects())
                cell.setName(vname + "_" + std::to_string(cell.getID()));
    }

    void BLactelDetector::buildMaterial(){

    }

    const G4ThreeVector BLactelDetector::getCellsPosition(){
        if(! getNCells())
            return G4ThreeVector();
        G4ThreeVector posMin = GetObjects()[0].GetObjects()[0].GetPosition();
        G4ThreeVector posMax = GetObjects()[0].GetObjects()[0].GetPosition();

        int nCells = 0;
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects()){
                if(posMin.getX() > cell.GetPosition().getX()) posMin.setX(cell.GetPosition().getX());
                if(posMin.getY() > cell.GetPosition().getY()) posMin.setY(cell.GetPosition().getY());
                if(posMin.getZ() > cell.GetPosition().getZ()) posMin.setZ(cell.GetPosition().getZ());

                if(posMax.getX() < cell.GetPosition().getX()) posMax.setX(cell.GetPosition().getX());
                if(posMax.getY() < cell.GetPosition().getY()) posMax.setY(cell.GetPosition().getY());
                if(posMax.getZ() < cell.GetPosition().getZ()) posMax.setZ(cell.GetPosition().getZ());
                nCells++;
            }
        return (posMin + posMax)/2.;
    }


    const G4ThreeVector BLactelDetector::getCellsDimensions(){
        if(! getNCells())
            return G4ThreeVector();

        G4ThreeVector posMin = GetObjects()[0].GetObjects()[0].GetPosition();
        G4ThreeVector posMax = GetObjects()[0].GetObjects()[0].GetPosition();

        G4ThreeVector dimMin = GetObjects()[0].GetObjects()[0].GetDimensions();
        G4ThreeVector dimMax = GetObjects()[0].GetObjects()[0].GetDimensions();

        int nCells = 0;
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects()){
                if(posMin.getX() > cell.GetPosition().getX()) posMin.setX(cell.GetPosition().getX());
                if(posMin.getY() > cell.GetPosition().getY()) posMin.setY(cell.GetPosition().getY());
                if(posMin.getZ() > cell.GetPosition().getZ()) posMin.setZ(cell.GetPosition().getZ());

                if(posMax.getX() < cell.GetPosition().getX()) posMax.setX(cell.GetPosition().getX());
                if(posMax.getY() < cell.GetPosition().getY()) posMax.setY(cell.GetPosition().getY());
                if(posMax.getZ() < cell.GetPosition().getZ()) posMax.setZ(cell.GetPosition().getZ());

                if(dimMin.getX() > cell.GetDimensions().getX()) dimMin.setX(cell.GetDimensions().getX());
                if(dimMin.getY() > cell.GetDimensions().getY()) dimMin.setY(cell.GetDimensions().getY());
                if(dimMin.getZ() > cell.GetDimensions().getZ()) dimMin.setZ(cell.GetDimensions().getZ());

                if(dimMax.getX() < cell.GetDimensions().getX()) dimMax.setX(cell.GetDimensions().getX());
                if(dimMax.getY() < cell.GetDimensions().getY()) dimMax.setY(cell.GetDimensions().getY());
                if(dimMax.getZ() < cell.GetDimensions().getZ()) dimMax.setZ(cell.GetDimensions().getZ());
                nCells++;
            }
        return posMax + dimMax - (posMin - dimMin);        
    }

    void BLactelDetector::centerOnItsCells(G4ThreeVector margin){
        SetPosition(getCellsPosition());
        SetDimensions(getCellsDimensions() + margin);
    }

    /**
     * @brief This function center the cells into the detector Volume. It should be use after centerOnItsCells function.
     * consider using change of references instead.
     */
    void BLactelDetector::setCellsPositionRelativeToDetector(){
        for(auto& module : GetObjects())
            for(auto& cell : module.GetObjects()){
                cell.SetPosition(cell.GetPosition() - GetPosition()); 
            }
    }

    // /**
    //  * @brief Read a detector from a detector file
    //  * @param fileName the name of file to read from.
    //  */
    // void BLactelDetector::read(std::string fileName){

    //     struct dirent *de=NULL;
    //     DIR *d=NULL;
    //     d=opendir(filesPath.data ());
    //     if(d == NULL)
    //     {
    //         cerr << "Couldn't open directory" << filesPath << endl;
    //         return;
    //     }

    //     libconfig::Config cfg;
    //     cfg.setAutoConvert (true);

    //     try {
    //             cfg.readFile(fileName.c_str());
    //         }
    //         catch(const FileIOException &e) {
    //             std::cerr << "BLactelDetector::read: I/O error while reading the file" << "\n";
    //             return;
    //         }
    //         catch(const ParseException &e) {
    //             std::cerr << "BLactelDetector::read: Parse error at " << e.getFile()
    //                     << ":" << e.getLine() << " - " << e.getError() << "\n";
    //             return;
    //         }

    //     try {
    //         // Access values

    //         // std::string detPattern="VolumeDet";
    //         // std::string Module="Volume";
    //         // std::string Cell="VolumeEC";

    //         // libconfig::Setting det;
    //         // libconfig::Setting& Cell();
    //         // libconfig::Setting& Module();

    //         libconfig::Setting& rootStg=cfg.getRoot();

    //         std::string detfName = rootStg.lookup("Config.detectorFile");
    //         std::string matfName = rootStg.lookup("Config.materialFile");

    //         if(detfName == "" || matfName == ""){
    //             BLogger::getInstance()->log(BLogger::Level::ERROR, "BLactelDetector::read: Could not find configuration.");
    //             return;
    //         }
    //         else{
    //             BLogger::getInstance()->log(BLogger::Level::INFO, 
    //                 "BLactelDetector::read: config found: \n\tDetector file name: " + detfName + \
    //                     "\n\tMaterial file name: " + matfName  + "\n"
    //             );
    //         }

    //         libconfig::Config cfgMat;
    //         std::string matFilePath = matfName;
    //         cfgMat.readFile(matFilePath.data());   
    //         libconfig::Setting& matRootStg=cfgMat.getRoot();          


    //         std::string DetName;

    //         if (!rootStg.lookupValue ("Name", DetName)){
    //             BLogger::getInstance()->log(BLogger::Level::ERROR, "Could not find detector name");
    //             return;
    //         }
    //         else{
    //             BLogger::getInstance()->log(BLogger::Level::INFO, "Detector name found: " + DetName);
    //         }

    //         read(cfg, cfgMat);

    //         // int configFileSize=rootStg.getLength ();
    //         // for (int cfIt=0; cfIt < configFileSize; cfIt++)
    //         // {
    //         //     if (! rootStg[cfIt].isGroup ())
    //         //         continue;
 
    //         //     string currentName=rootStg[cfIt].getName ();
    //         //     libconfig::Setting& currentSetting = cfg.lookup(currentName);
                
    //         //     std::vector<OM> oms;
    //         //     std::vector<ElementaryCell> cells;

    //         //     // read detector informations
    //         //     if (currentName.find("VolumeDet") != string::npos){
    //         //         // std::cout << "\r\033[10C" << "Det Volume found: " << currentName << std::endl;
    //         //         currentSetting.lookupValue("Name", name);
    //         //         // currentSetting.lookupValue("ID", id); // this should be auto
    //         //         float x, y, z, t, p;
    //         //         currentSetting.lookupValue("X", x);
    //         //         currentSetting.lookupValue("Y", y);
    //         //         currentSetting.lookupValue("Z", z);
    //         //         currentSetting.lookupValue("Theta", t);
    //         //         currentSetting.lookupValue("Phi", p);
    //         //         SetPosition(G4ThreeVector(x, y, z));
    //         //     }

    //         //     // read water tanks (ECells) information
    //         //     else if (currentName.find("VolumeWT") != string::npos){
    //         //         // std::cout << "\r\033[10C" << "Cell found: " << currentName << std::endl;
    //         //         cells.push_back(readECell(currentSetting, cfgMat));
    //         //     }

    //         //     // read OMs information
    //         //     else if (currentName.find("OM") != string::npos){
    //         //         // currentSetting.lookupValue("Name", name);
    //         //         // currentSetting.lookupValue("ID", id);
    //         //         // float x, y, z, t, p;
    //         //         // currentSetting.lookupValue("X", x);
    //         //         // currentSetting.lookupValue("Y", y);
    //         //         // currentSetting.lookupValue("Z", z);
    //         //         // currentSetting.lookupValue("Theta", t);
    //         //         // currentSetting.lookupValue("Phi", p);
    //         //         // SetPosition(G4ThreeVector(x, y, z));
    //         //     }    
    //         // }
    //     }
    //     catch(const SettingNotFoundException &e) {
    //         std::cerr << "Missing setting: " << e.getPath() << "\n";
    //     }
    // }

    OM BLactelDetector::readOM(libconfig::Setting& set){
        
        OM om;
        om.setName(set["Name"]);
        om.setPoition(G4ThreeVector (static_cast<float>( (int) set["X"]),
                                            static_cast<float>( (int) set["Y"]),
                                            static_cast<float> ((int) set["Z"])
                                            )
                    );
        om.getGeometry().getRotation().setTheta(static_cast<float> ((int) set["Theta"]));
        om.getGeometry().getRotation().setPhi(static_cast<float> ((int) set["Phi"]));
        if(set.exists("MotherVolume"));
            om.getVolumeProperties().setMotherVolName(set["MotherVolume"]);
        if(set.exists("borderSurafce"));
            om.getVolumeProperties().setBorderSurfName(set["borderSurafce"]);
        if(set.exists("borderSurfaceMaterial"));
            om.getVolumeProperties().setBorderSurfMatName(set["borderSurfaceMaterial"]);
        
        return om;
    }



    ElementaryCell BLactelDetector::readECell(libconfig::Setting& posSet, libconfig::Config& volConf){
        std::string tname;
        int tid;
        float x, y, z, t, p;
        ElementaryCell cell;

        posSet.lookupValue("Name", tname);
        posSet.lookupValue("ID", tid);
        posSet.lookupValue("X", x);
        posSet.lookupValue("Y", y);
        posSet.lookupValue("Z", z);
        posSet.lookupValue("Theta", t);
        posSet.lookupValue("Phi", p);

        cell.setName(tname);
        cell.SetPosition(G4ThreeVector(x, y, z));
        if(volConf.exists(tname))
            BLogger::getInstance()->log(BLogger::Level::WARNING, tname + " does not exists!"); 
            
        libconfig::Setting& root = volConf.getRoot();
        libconfig::Setting& volume = root.lookup(tname);
        if(! volume.isGroup())
            BLogger::getInstance()->log(BLogger::Level::WARNING, tname + " group not found!");
            

        G4ThreeVector cellDim = G4ThreeVector(static_cast<float>((int)volume["X"]), 
                                                static_cast<float>((int)volume["Y"]), 
                                                static_cast<float>((int)volume["Z"]));
        cell.SetDimensions(2*cellDim); // *2: remember cellDim contains half length;
        cell.print();
        return cell;
    }

    ElementaryCell BLactelDetector::readAVolume(libconfig::Setting& set){
        ElementaryCell obj;
        obj.setName(set["Name"]);
        std::cout << "\r\033[10C" << "Reading EC .." << std::endl;
        obj.SetPosition(G4ThreeVector ( static_cast<float>( (int) set["X"]),
                                            static_cast<float>( (int) set["Y"]),
                                            static_cast<float> ((int) set["Z"])
                                            ));
        if(set.exists("MotherVolume"))
            obj.getVolumeProperties().setMotherVolName((string)set["MotherVolume"]);
        if(set.exists("borderSurface"))
            obj.getVolumeProperties().setBorderSurfName((string)set["borderSurface"]);
        if(set.exists("borderSurfaceMaterial"))
            obj.getVolumeProperties().setBorderSurfMatName((string)set["borderSurfaceMaterial"]);
        // obj.GetRotation().setTheta(static_cast<float> ((int) set["Theta"]));
        // obj.GetRotation().setPhi(static_cast<float> ((int) set["Phi"]));
        if(set.exists("om_id"))
            obj.getOM().setID((string)set["om_id"]);

        return obj;
    }

    void BLactelDetector::read(libconfig::Config& detConf, libconfig::Config& volConf){
        libconfig::Setting& detSet = detConf.getRoot();
        size_t detFileSize = detSet.getLength();

        std::string detName;
        if(detSet.exists("Name"))
            std::string detName = detSet["Name"];
        else    
            BLogger::getInstance()->log(BLogger::Level::WARNING, "Could not find the detector Volume.");

        for(int i = 0; i < detFileSize; i++){
            if(! detSet[i].isGroup())
                continue;
            libconfig::Setting& currDetSet = detSet[i];
            std::string detSetName = currDetSet.getName();

            OM om;
            ElementaryCell cell;
            if(detSetName.find("OM") != string::npos){
                // om = readAVolume<OM>(currDetSet);
                cell = readAVolume(currDetSet);
                std::cout << "\r\033[10C" << "found and read OM: " << cell.getOM().getOMID()
                                        << " - Its mother: " << cell.getVolumeProperties().getMotherVolName()
                                        << " - Name: " << cell.getName()
                                        << std::endl;
                std::string omMotherVolumeName = cell.getVolumeProperties().getMotherVolName();
                ElementaryCell waterVolume;
                if(omMotherVolumeName != "" && omMotherVolumeName.find("IdealWaterVolume") != string::npos){  
                    std::cout << "\r\033[10C" << " Check its Mother found: " << (string)detSet.lookup(omMotherVolumeName) << std::endl;
                    if(detSet.exists(omMotherVolumeName)){
                        std::cout << "\r\033[10C" << " --- its Mother found: " << (string)detSet.lookup(omMotherVolumeName) << std::endl;
                        libconfig::Setting& waterSet = detSet.lookup(omMotherVolumeName);
                        std::cout << "\r\033[10C" << "its Mother found: " << (string)detSet.lookup(omMotherVolumeName) << std::endl;
                        if(waterSet.isGroup()){
                            if(waterSet.exists("MotherVolume")){
                                std::string tankVolumeName = waterSet.lookup("MotherVolume");
                                libconfig::Setting& tankSet = detSet.lookup(tankVolumeName);
                                ElementaryCell tankVolume = readAVolume<ElementaryCell>(tankSet);
                                tankVolume.setOM(om);
                                tankVolume.print();
                            }
                        }
                    }
                }  
            } 
        }

    }



    // void BLactelDetector::print(std::ostream& out = std::cout, bool printCells = false){
    //     out     << string(25, '+') 
    //             << " BLactelDetector " 
    //             << " Pos: " << GetPosition()
    //             << " Dim: " << GetDimensions()
    //             << string(25, '+') 
    //             << endl << endl;

    //     for(auto& module : GetObjects())
    //         module.print(out, printCells);
    // }

    // virtual void BLactelDetector::construct() override {
    //     std::cout << "\r\033[10C" << "Construct" << std::endl;
    // }

    // void BLactelDetector::setInterModuleDistance(G4ThreeVector dv = interModuleDistance){
    //     setInterModuleDistanceX(dv.getX());
    //     setInterModuleDistanceY(dv.getY());
    //     setInterModuleDistanceZ(dv.getZ());
    //     update();
    // }

    // void BLactelDetector::setInterModuleDistanceX(int d = interModuleDistance.getX()){
    //     for(int i = 0; i < GetObjects().size()-1; i++){
    //         GetObjects()[i+1].Translate(GetObjects()[i].GetPosition().getX() + GetObjects()[i].GetDimensions().getX() + d, 0, 0);
    //     }   
    //     update();
    // }
    // void BLactelDetector::setInterModuleDistanceY(int d = interModuleDistance.getY()){
    //     for(int i = 0; i < GetObjects().size()-1; i++){
    //         // GetObjects()[i+1].Translate(0, GetObjects()[i].GetPosition().getY() + GetObjects()[i].GetDimensions().getY()/2 + d, 0);
    //         GetObjects()[i+1].Translate(0, GetObjects()[i].GetPosition().getY() + GetObjects()[i].GetDimensions().getY() + d, 0);                            
    //     }
    //     update();
    // }
    // void BLactelDetector::setInterModuleDistanceZ(int d = interModuleDistance.getZ()){
    //     for(int i = 0; i < GetObjects().size()-1; i++){
    //         // GetObjects()[i+1].Translate(0, 0, GetObjects()[i].GetPosition().getZ() + GetObjects()[i].GetDimensions().getZ()/2 + d);
    //         GetObjects()[i+1].Translate(0, 0, GetObjects()[i].GetPosition().getZ() + GetObjects()[i].GetDimensions().getZ() + d);
    //     }
    //     update();
    // }

    // void BLactelDetector::updatePosition(){
    //     G4ThreeVector pcum;
    //     for (size_t i = 0; i < modules.GetObjects().size(); i++)
    //     {
    //         pcum += modules.GetObjects()[i].GetPosition();
    //     }
    //     SetPosition(pcum/modules.GetObjects().size());
    // }

    // void BLactelDetector::updateDimension(){
    //     if(modules.GetObjects().size() == 0) // no cell yet
    //         return;

    //     G4ThreeVector dup = modules.GetObjects()[0].GetDimensions();
    //     for (size_t i = 0; i < modules.GetObjects().size(); i++)
    //     {
    //         if(abs(dup.getX()) < abs(modules.GetObjects()[i].GetPosition().getX() + modules.GetObjects()[i].GetDimensions().getX()))
    //             dup.setX(modules.GetObjects()[i].GetPosition().getX() + modules.GetObjects()[i].GetDimensions().getX());

    //         if(abs(dup.getY()) < abs(modules.GetObjects()[i].GetPosition().getY() + modules.GetObjects()[i].GetDimensions().getY()))
    //             dup.setY(modules.GetObjects()[i].GetPosition().getY() + modules.GetObjects()[i].GetDimensions().getY());

    //         if(abs(dup.getZ()) < abs(modules.GetObjects()[i].GetPosition().getZ() + modules.GetObjects()[i].GetDimensions().getZ()))
    //             dup.setZ(modules.GetObjects()[i].GetPosition().getZ() + modules.GetObjects()[i].GetDimensions().getZ());

    //     }
    //     SetDimensions(dup);
    // }

    // void BLactelDetector::update(){
    //             updatePosition();
    //             updateDimension();
    //         }

    // void BLactelDetector::drawModules(Option_t* options = "", bool drawCells = false){
    //     int color = 1;
    //     const int size = modules.GetObjects().size();
    //     TPave * moduleBox[size];
    //     for(int itr = 0; itr < modules.GetObjects().size(); itr++){
    //             Module module = modules.GetObjects()[itr];
    //             moduleBox[itr] = new TPave( module.GetPosition().getX() - module.GetDimensions().getX(),
    //                                         module.GetPosition().getZ() - module.GetDimensions().getZ(),
    //                                         module.GetPosition().getX() + module.GetDimensions().getX(),
    //                                         module.GetPosition().getZ() + module.GetDimensions().getZ(),
    //                                         1, 
    //                                         ""
    //                                     );
    //             moduleBox[itr]->SetName(("Detector_" + std::to_string(id) + "-Module_" + std::to_string(module.getID())).c_str());
    //             moduleBox[itr]->SetFillColorAlpha(color, 0.4);
    //             moduleBox[itr]->Draw(options);
    //             color++;
    //             if(drawCells)
    //                 module.drawCells();
    //         }
    // }


    //======================================================
    // Read one libconfig group into VolumeInfo
    //======================================================
    BLactelDetector::VolumeInfo BLactelDetector::ReadVolume(const libconfig::Setting& s)
    {
        BLactelDetector::VolumeInfo v;

        v.groupName = s.getName();

        if(s.exists("Name"))
            v.name = (const char*)s["Name"];

        if(s.exists("ID"))
            v.id = s["ID"];

        if(s.exists("X"))
            v.x = s["X"];

        if(s.exists("Y"))
            v.y = s["Y"];

        if(s.exists("Z"))
            v.z = s["Z"];

        if(s.exists("Theta"))
            v.theta = s["Theta"];

        if(s.exists("Phi"))
            v.phi = s["Phi"];

        if(s.exists("MotherVolume"))
            v.motherVolume = (const char*)s["MotherVolume"];

        if(s.exists("om_id"))
            v.om_id = (const char*)s["om_id"];

        return v;
    }  

    //======================================================
    // Print volume info
    //======================================================
    void BLactelDetector::PrintVolume(const BLactelDetector::VolumeInfo& v)
    {
        cout << "----------------------------------" << endl;
        cout << "Group Name     : " << v.groupName << endl;
        cout << "Name           : " << v.name << endl;
        cout << "ID             : " << v.id << endl;
        cout << "Position       : (" << v.x << ", "
                                    << v.y << ", "
                                    << v.z << ")" << endl;

        cout << "Theta          : " << v.theta << endl;
        cout << "Phi            : " << v.phi << endl;

        cout << "MotherVolume   : " << v.motherVolume << endl;

        if(!v.om_id.empty())
            cout << "OM ID          : " << v.om_id << endl;
    }

    //======================================================
    // Find a group whose Name == volumeName
    //======================================================
    const libconfig::Setting* BLactelDetector::FindVolumeByName(const libconfig::Config& cfg,
                                    const std::string& volumeName)
    {
        const libconfig::Setting& root = cfg.getRoot();

        for(int i = 0; i < root.getLength(); ++i)
        {
            const libconfig::Setting& s = root[i];

            if(s.getType() != libconfig::Setting::TypeGroup)
                continue;

            if(!s.exists("Name"))
                continue;

            std::string name = (const char*)s["Name"];
            if(name == volumeName)
                return &s;
        }

        return nullptr;
    }

    //======================================================
    // Main function requested
    //======================================================
    void BLactelDetector::ReadOMHierarchy(const std::string& detectorFile)
    {
        libconfig::Config cfg;

        try
        {
            cfg.readFile(detectorFile.c_str());
        }
        catch(const FileIOException& e)
        {
            cerr << "File I/O error" << endl;
            return;
        }
        catch(const ParseException& e)
        {
            cerr << "Parse error at "
                << e.getFile() << ":"
                << e.getLine()
                << " - " << e.getError()
                << endl;

            return;
        }

        const libconfig::Setting& root = cfg.getRoot();

        //--------------------------------------------------
        // Read detector name
        //--------------------------------------------------
        std::string detectorName;

        if(root.exists("Name"))
            detectorName = (const char*)root["Name"];

        cout << "Detector Name = " << detectorName << endl;


        //--------------------------------------------------
        // Loop over all groups
        //--------------------------------------------------
        for(int i = 0; i < root.getLength(); ++i)
        {
            const libconfig::Setting& group = root[i];

            if(group.getType() != libconfig::Setting::TypeGroup)
                continue;

            std::string groupName = group.getName();

            //--------------------------------------------------
            // Keep only groups containing "OM"
            //--------------------------------------------------
            if(groupName.find("OM") == std::string::npos)
                continue;

            cout << endl;
            cout << "==================================" << endl;
            cout << "Processing OM group: "
                << groupName << endl;

            //--------------------------------------------------
            // Read OM
            //--------------------------------------------------
            BLactelDetector::VolumeInfo current = ReadVolume(group);

            PrintVolume(current);

            //--------------------------------------------------
            // Walk through mother volumes
            //--------------------------------------------------
            std::string mother = current.motherVolume;

            while(true)
            {
                //--------------------------------------------------
                // Stop condition
                //--------------------------------------------------
                if(mother == detectorName)
                {
                    cout << endl;
                    cout << "Reached detector volume: "
                        << detectorName << endl;

                    break;
                }
                
                //--------------------------------------------------
                // Find mother volume group
                //--------------------------------------------------
                const libconfig::Setting* motherGroup = FindVolumeByName(cfg, mother);

                if(!motherGroup)
                {
                    cout << "Cannot find volume with Name = "
                        << mother << endl;

                    break;
                }
      
                //--------------------------------------------------
                // Read mother volume
                //--------------------------------------------------
                BLactelDetector::VolumeInfo motherVolume =
                    ReadVolume(*motherGroup);

                PrintVolume(motherVolume);

                //--------------------------------------------------
                // Continue climbing hierarchy
                //--------------------------------------------------
                mother = motherVolume.motherVolume;
            }
        }
    }

    //======================================================
    // Read a detector from file
    //======================================================

    /**
     * Routine to read a detector from a file.
     * @brief read a BLactelDetector from a file
     * @param detFile is the name of the file containing the detector description in a specefic format (libcinfig style)
     */

    void BLactelDetector::read(std::string detFile) {
        libconfig::Config cfg;
        libconfig::Config cfgMat; // materila conf

        fObjects.clear();

        std::vector<ElementaryCell> cells;
        try{
            cfg.readFile(detFile.c_str());
        }
        catch(const FileIOException& e){
            cerr << "File I/O error" << endl;
            return;
        }
        catch(const ParseException& e){
            cerr << "Parse error at "
                << e.getFile() << ":"
                << e.getLine()
                << " - " << e.getError()
                << endl;

            return;
        }

        const libconfig::Setting& root = cfg.getRoot();

        std::string detectorName;

        if(root.exists("Name"))
            detectorName = (const char*)root["Name"];

        cout << "Reading detector " << detectorName << "..." << endl;

        

        
        if(root.exists("Config")){
            const libconfig::Setting& conf = cfg.lookup("Config");
            if(conf.getType() == libconfig::Setting::TypeGroup){
                std::string materialFile = conf["materialFile"];
                BLogger::getInstance()->log(BLogger::Level::INFO, "BLactelDetector::read(): material file Found: " + materialFile);
                try{
                    cfgMat.readFile(materialFile.c_str());
                }
                catch(const FileIOException& e){
                    cerr << "File I/O error" << endl;
                    return;
                }
                catch(const ParseException& e){
                    cerr << "Parse error at "
                        << e.getFile() << ":"
                        << e.getLine()
                        << " - " << e.getError()
                        << endl;

                    return;
                }
            }                
            else{
                std::cout << "\r\033[10C" << "Issue occured while reading materialFile" << std::endl;
                return;
            }
        }
        else{
            std::cout << "\r\033[10C" << "Issue when looking for Config group" << std::endl;
            return;
        }



        for(int i = 0; i < root.getLength(); ++i)
        {
            const libconfig::Setting& group = root[i];

            if(group.getType() != libconfig::Setting::TypeGroup)
                continue;

            std::string groupName = group.getName();

            if(groupName.find("OM") == std::string::npos)
                continue;

            BLactelDetector::VolumeInfo current = ReadVolume(group);
            std::string mother = current.motherVolume;

            while(true)
            {
                //--------------------------------------------------
                // Stop condition
                //--------------------------------------------------
                const libconfig::Setting* motherGroup;
                if(mother == detectorName)
                {
                    std::string lastMVName = motherGroup->lookup("Name");
                    cout << endl;
                    cout << "Reached detector volume: "
                        << detectorName 
                        << " Looking for : " << lastMVName
                        << endl;

                    if(cfgMat.exists(lastMVName)){
                        const libconfig::Setting& matVolume = cfgMat.lookup(lastMVName);
                        if(matVolume.getType() == libconfig::Setting::TypeGroup){
                            G4ThreeVector pos((float)motherGroup->lookup("X"),
                                                (float)motherGroup->lookup("Y"),
                                                (float)motherGroup->lookup("Z")
                                            );
                            G4ThreeVector dim(static_cast<float>((int)matVolume["X"]),
                                                static_cast<float>((int)matVolume["Y"]),
                                                static_cast<float>((int)matVolume["Z"])
                                            );
                            OM om = OM((string) group["om_id"]);
                            int module = (int)motherGroup->lookup("Module");
                            int layer = (int)motherGroup->lookup("Layer");
                            int id = (int)motherGroup->lookup("ID");
                            om.setID(id);
                            cells.push_back(ElementaryCell(id, pos, dim, om, module, layer));
                        }
                    }

                    break;
                }

                motherGroup = FindVolumeByName(cfg, mother);

                if(!motherGroup)
                {
                    cout << "Cannot find volume with Name = "
                        << mother << endl;

                    break;
                }

                BLactelDetector::VolumeInfo motherVolume =
                    ReadVolume(*motherGroup);
                mother = motherVolume.motherVolume;
            }
        }

        setName(detectorName);
        SetPosition(G4ThreeVector()); // set position at center

        std::map<int, std::vector<ElementaryCell>> modulesMap;
        for(auto& cell : cells){
            modulesMap[cell.getModule()].push_back(cell);
        }
        
        for(auto& module : modulesMap){
            Module mod;
            for(auto& cell : module.second){
                mod.setID(cell.getModule());
                mod.addObject(cell);
            } 
            addObject(mod);   
        }
    }

} // namespace lactel
