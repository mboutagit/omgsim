#include "BLactelDetector.hh"

#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TBox.h"
#include "TF1.h"

// #include <vtkSmartPointer.h>
// #include <vtkCubeSource.h>
// #include <vtkPolyDataMapper.h>
// #include <vtkActor.h>
// #include <vtkRenderer.h>
// #include <vtkRenderWindow.h>
// #include <vtkRenderWindowInteractor.h>

#include "BPrimaryGeneratorAction.hh"

// /usr/include/vtk-9.1

using namespace std;
using namespace lactel;

int main(int argc, char* argv[]){

    TApplication app("ROOTApp", &argc, argv);
    
    //Define cells
    ElementaryCell EC1 = ElementaryCell(1, G4ThreeVector(0.*m, 0.*m, -1.5*m),       G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0321"), 1, 1);
    ElementaryCell EC2 = ElementaryCell(2, G4ThreeVector(0.*m, 0.*m, -4*m),         G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0304"), 1, 2);

    ElementaryCell EC3 = ElementaryCell(3, G4ThreeVector(0*m, 2.5*m, -1.5*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0423"), 2, 1);
    ElementaryCell EC4 = ElementaryCell(4, G4ThreeVector(0*m, 2.5*m, -4*m),       G4ThreeVector(2*m, 2*m, 2*m), OM("OMS1060"), 2, 2);

    ElementaryCell EC5 = ElementaryCell(5, G4ThreeVector(0*m, -2.5*m, -1.5*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0311"), 3, 1);
    ElementaryCell EC6 = ElementaryCell(6, G4ThreeVector(0*m, -2.5*m, -4*m),      G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0302"), 3, 2);

    ElementaryCell EC7 = ElementaryCell(7, G4ThreeVector(2.5*m, -2.5*m, -1.5*m),  G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0648"), 4, 1);
    ElementaryCell EC8 = ElementaryCell(8, G4ThreeVector(2.5*m, -2.5*m, -4*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0366"), 4, 2);

    ElementaryCell EC9 = ElementaryCell(9, G4ThreeVector(2.5*m, 0*m, -1.5*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0137"), 5, 1);
    ElementaryCell EC10 = ElementaryCell(10, G4ThreeVector(2.5*m, 0*m, -4*m),      G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0298"), 5, 2);

    ElementaryCell EC11 = ElementaryCell(11, G4ThreeVector(2.5*m, 2.5*m, -1.5*m),   G4ThreeVector(2*m, 2*m, 2*m), OM("OMS1044"), 6, 1);
    ElementaryCell EC12 = ElementaryCell(12, G4ThreeVector(2.5*m, 2.5*m, -4*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0501"), 6, 2);

    ElementaryCell EC13 = ElementaryCell(13, G4ThreeVector(-2.5*m, -2.5*m, -1.5*m), G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0358"), 7, 1);
    ElementaryCell EC14 = ElementaryCell(14, G4ThreeVector(-2.5*m, -2.5*m, -4*m),   G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0319"), 7, 2);

    ElementaryCell EC15 = ElementaryCell(15, G4ThreeVector(-2.5*m, 0*m, -1.5*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0790"), 8, 1);
    ElementaryCell EC16 = ElementaryCell(16, G4ThreeVector(-2.5*m, 0*m, -4*m),      G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0293"), 8, 2);

    ElementaryCell EC17 = ElementaryCell(17, G4ThreeVector(-2.5*m, 2.5*m, -1.5*m),  G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0365"), 9, 1);
    ElementaryCell EC18 = ElementaryCell(18, G4ThreeVector(-2.5*m, 2.5*m, -4*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0299"), 9, 2);

    // define modules
    Module M1 = Module(1, {EC1, EC2});
    Module M2 = Module(2, {EC3, EC4});
    Module M3 = Module(3, {EC5, EC6});
    Module M4 = Module(4, {EC7, EC8});
    Module M5 = Module(5, {EC9, EC10});
    Module M6 = Module(6, {EC11, EC12});
    Module M7 = Module(7, {EC13, EC14});
    Module M8 = Module(8, {EC15, EC16});
    Module M9 = Module(9, {EC17, EC18});

    // Generate lactel prototype Detector file
    BLactelDetector det_prototype({M1, M2, M3, M4, M5, M6, M7, M8, M9},
                            G4ThreeVector(0*cm, 0*cm, 0*cm)
                        );
    det_prototype.setName("LACTEL_9Modules");
    det_prototype.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det_prototype.setCellsPositionRelativeToDetector();
    det_prototype.generateDetectorFiles();
    det_prototype.dumpDetectorSummary();

    // Generate real Detector file
    BLactelDetector det_true({M1}, G4ThreeVector(0*cm, 0*cm, 0*cm));
    det_true.setName("LACTELTrue");
    det_true.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det_true.setCellsPositionRelativeToDetector();
    det_true.generateDetectorFiles();
    det_true.dumpDetectorSummary();



    BLactelDetector det_2M({M1, M2},
                            G4ThreeVector(0*cm, 0*cm, 0*cm)
                        );
    det_2M.setName("LACTEL_2Modules");
    det_2M.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det_2M.setCellsPositionRelativeToDetector();
    det_2M.generateDetectorFiles();
    det_2M.dumpDetectorSummary();

    BLactelDetector det_5M({M1, M4, M6, M7, M9},
                        G4ThreeVector(0*cm, 0*cm, 0*cm)
                        );
    det_5M.setName("LACTEL_5Modules");
    det_5M.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det_5M.setCellsPositionRelativeToDetector();
    det_5M.generateDetectorFiles();
    det_5M.dumpDetectorSummary();

    BLactelDetector det_4M({M4, M6, M7, M9},
                        G4ThreeVector(0*cm, 0*cm, 0*cm)
                        );
    det_4M.setName("LACTEL_4Modules");
    det_4M.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det_4M.setCellsPositionRelativeToDetector();
    det_4M.generateDetectorFiles();
    det_4M.dumpDetectorSummary();



    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;
}