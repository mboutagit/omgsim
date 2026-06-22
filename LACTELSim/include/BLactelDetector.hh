#ifndef BLACTELDETECTOR_HH
#define BLACTELDETECTOR_HH

#include <iostream>
#include <chrono>
#include <libconfig.h++>


#include "BDetector.hh"
#include "OM.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "utils.hh"

#include "TFile.h"
#include "TTree.h"
#include "TPad.h"
#include "TPoint.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TBox.h"
#include "TPave.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TGraph.h"

#include "BLogger.hh"

#include "BVolumesManager.hh"

#include <sys/types.h>
#include <dirent.h>



namespace lactel
{
    using namespace UTILS;
    using namespace utils;
    using namespace libconfig;
   
    class ElementaryCell : public GeometryObject, public TObject
    {
        private:
            int id = -1;
            std::string name = "IdealWaterVolume";
            OM om;
            int layer = 0; 
            int module;
            VolumeProperties volProp;

        public:
            ElementaryCell();
            ElementaryCell(int uid);
            ElementaryCell(G4ThreeVector pos);
            ElementaryCell(OM& om);
            ElementaryCell(OM om);
            ElementaryCell(G4ThreeVector pos, G4ThreeVector dim);
            ElementaryCell(G4ThreeVector pos, G4ThreeVector dim, OM _om);
            ElementaryCell(int uid, G4ThreeVector pos, G4ThreeVector dim, OM _om, int mdl, int lyr);
            ~ElementaryCell();
            ElementaryCell(const ElementaryCell& other) = default;
            // ElementaryCell(ElementaryCell&& other);

            GeometryObject& getTransform();
            void setName(std::string _name);
            std::string getName();
            int getID();
            OM& getOM();
            void setOM(OM& _om);
            void centerX();
            void centerY();
            void centerZ();
            void center();
            void print(std::ostream& out = std::cout);
            void dump(ostream& out = std::cout);
            void dumpMaterial(ostream& out = std::cout);
            void draw(Option_t* options = "", std::string axis = "XY");

            inline VolumeProperties& getVolumeProperties(){return volProp;}

            inline int getLayer() {return layer;}
            inline void setLayer(int lyr) {layer = lyr;}

            inline int getModule() {return module;}
            inline void setModule(int mdl) {module = mdl;}

            ClassDef(ElementaryCell, 0);
    }; // class ElementaryCell

    class Module : public GeometryObject, public GeometryGrid<ElementaryCell>, public TObject
    {
        protected:
            int id = -1; // no object yet
            std::string name;
            float VInterCellDistance; // vertical Inter Cells Distance 
            int defaultNumberOfCell = 2;

        public:
            Module();
            Module(int uid);
            Module(std::vector<ElementaryCell> cells);
            Module(int uid, std::vector<ElementaryCell> cells);
            Module(std::vector<ElementaryCell> cells, G4ThreeVector spacing);
            Module(int nx, int ny, int nz);
            Module(int nx, int ny, int nz, G4ThreeVector spacing);
            ~Module();
            Module(const Module& other);

            void reset();
            void setName(std::string _name);
            std::string getName();
            void setID(int id);
            int getID();
            void centerCellsX();
            void centerCellsY();
            void centerCellsZ();
            void centerCells();
            void addECell(std::string axis);
            int getNCells();
            int getNLayers(int axis);
            void print(std::ostream& out = std::cout, bool printCells = false);
            void dumpCells(std::ostream& out = std::cout, bool dumpName = false);
            void dumpCellsMaterial(std::ostream& out = std::cout);
            void addNECells(int n, std::string axis);
            void updatePosition();
            void SetPosition(G4ThreeVector pos);
            void updateDimension();
            void updateECellsPosition(G4ThreeVector OldPos, G4ThreeVector NewPos);
            void rebuildGrid();
            const ElementaryCell& getECellsPrototype() const;
            // void setDimension(G4ThreeVector vd);
            void updateGeometry();
            void setDefault();
            void draw(Option_t* options = "", bool _drawCells = false);
            void drawCells(Option_t* options = "");

            ClassDef(Module, 0)

    }; // class Module


    class BLactelDetector : public BDetector, public GeometryObject, public GeometryGrid<Module>, public TObject
    {
        public:
            BLactelDetector();
            BLactelDetector(vector<Module> modules);
            BLactelDetector(vector<Module> modules, G4ThreeVector spacing);
            BLactelDetector(int nx, int ny, int nz);
            BLactelDetector(int nx, int ny, int nz, G4ThreeVector spacing);
            ~BLactelDetector();

            struct VolumeInfo
            {
                std::string groupName;

                std::string name;
                int id = -1;

                double x = 0.0;
                double y = 0.0;
                double z = 0.0;

                double theta = 0.0;
                double phi   = 0.0;

                std::string motherVolume;

                std::string om_id;
            };

            struct PassiveVolumeParameters
            {
                std::string Name;
                std::string VolumeKind;

                //predefined values are optional

                std::string RefVolume;
                std::string OperandVolume;
                double RotTheta;
                double RotPhi;

                double X;
                double Y;
                double Z;

                double RadiusMin;
                double RadiusMax;
                double PhiMin   ;
                double PhiMax   ;
                double ThetaMin ;
                double ThetaMax ;

                double SecondRadiusMin;
                double SecondRadiusMax;

                double ThirdRadiusMax;
                double ZCutMin = 0;
                double ZCutMax = 0;

                //Material
                std::string Material = "G4_AIR";
                std::string SkinMaterial = "";

                void CalculateParameters ()
                {
                }
            };


            static std::string filesPath;

            void print(std::ostream& out = std::cout, bool printModules = false, bool printCells = false);
            void dump(std::ostream& out = std::cout);
            void dumpMaterial(std::ostream& out = std::cout);
            void dumpDetectorSummary(std::ostream& out = std::cout);
            void construct() override;
            void updatePosition();
            void updateDimension();
            void updateGeometry();
            TGraph* drawMZ();
            void drawModules(Option_t* options = "", bool drawModules = false, bool drawCells = false);
            void generateDetectorFiles(bool printSummary = true);
            
            void BuildGrid(Module& prototype);
            void buildMaterial();

            void updateCellsMotherVolume();
            void setCellsRefVolName(std::string vname = "CellRefVolume");
            void setCellsPositionRelativeToDetector();

            inline void clear(){fObjects.clear();}

            const G4ThreeVector getCellsPosition();
            const G4ThreeVector getCellsDimensions();
            int getNModules() const;
            int getNCells() const;

            inline void setMotherVolName(std::string name){motherVolume = name;}
            inline void setBorderSurfName(std::string name){borderSurafce = name;}
            inline void setBorderSurfMatName(std::string name){borderSurfaceMaterial = name;}

            inline const std::string getMotherVolName(){return motherVolume;}
            inline const std::string getBorderSurfName(){return borderSurafce;}
            inline const std::string getBorderSurfMatName(){return borderSurfaceMaterial;}

            void centerOnItsCells(G4ThreeVector margin = G4ThreeVector());

            ElementaryCell readAVolume(libconfig::Setting& set);

            VolumeInfo ReadVolume(const libconfig::Setting& s);
            void PrintVolume(const VolumeInfo& v);
            const libconfig::Setting* FindVolumeByName(const libconfig::Config& cfg, const std::string& volumeName);
            void ReadOMHierarchy(const std::string& detectorFile);

            std::vector<ElementaryCell*> getListOfECells();
            void printListOfECells(std::ostream& out = std::cout);
            std::vector<Module*> getListOfModules();
            void printListOfModules(std::ostream& out = std::cout);
            std::vector<OM*> getListOfOMs();
            void printListOfOMs(std::ostream& out = std::cout);
            ElementaryCell* getOMECell(OM&);
            ElementaryCell* getOMECell(std::string om_id);
            ElementaryCell* getOMECell(int oid);
            Module* getOMModule(OM&);
            Module* getOMModule(std::string om_id);
            OM* getOMByID(int oid) const;
            int getOMIDByRefID(std::string ref);





            // utilities:
            OM readOM(libconfig::Setting& set);

            template<typename T>
            T readAVolume(libconfig::Setting& set){
                T obj;
                obj.setName(set["Name"]);
                
                if constexpr (std::is_same_v<T, ElementaryCell>){
                    std::cout << "\r\033[10C" << "Reading EC .." << std::endl;

                    obj.SetPosition(G4ThreeVector ( static_cast<float>( (int) set["X"]),
                                                    static_cast<float>( (int) set["Y"]),
                                                    static_cast<float> ((int) set["Z"])
                                                    ));
                    if(set.exists("MotherVolume"));
                        obj.getVolumeProperties().setMotherVolName(set["MotherVolume"]);
                    if(set.exists("borderSurafce"));
                        obj.getVolumeProperties().setBorderSurfName(set["borderSurafce"]);
                    if(set.exists("borderSurfaceMaterial"));
                        obj.getVolumeProperties().setBorderSurfMatName(set["borderSurfaceMaterial"]);
                    // obj.GetRotation().setTheta(static_cast<float> ((int) set["Theta"]));
                    // obj.GetRotation().setPhi(static_cast<float> ((int) set["Phi"]));
                }
                if(set.exists("om_id")){
                    std::cout << "\r\033[10C" << "Reading OM: " << (string)set["om_id"] << std::endl;
                    if constexpr (std::is_same_v<T, OM>)
                          obj.setID(set["om_id"]); 
                }  
                     
                   
                return obj;
            }

            ElementaryCell readECell(libconfig::Setting& posSet, libconfig::Config& volConf);

            void read(libconfig::Config& detConf, libconfig::Config& volConf);
            void read(std::string detFile);
            

            G4ThreeVector interModuleDistance = G4ThreeVector(50*cm, 50*cm, 50*cm);
            void setName(std::string _name){name=_name;}
            std::string getName(){return name;}
            int getID(){return id;}

        private:
            static int gID;
            int id;
            std::string motherVolume = "";
            std::string borderSurafce = "";
            std::string borderSurfaceMaterial="";

            ClassDef(BLactelDetector, 0);
    };

} // namespace lactel

#endif