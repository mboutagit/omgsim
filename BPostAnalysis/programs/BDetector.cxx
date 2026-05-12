#include "BLactelDetector.hh"

#include <iostream>
#include "TApplication.h"
#include "TCanvas.h"
#include "TGeoManager.h"
#include "TGeoMaterial.h"
#include "TGeoMedium.h"
#include "TBox.h"
#include "TF1.h"

#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include "BPrimaryGeneratorAction.hh"

// /usr/include/vtk-9.1

using namespace std;
using namespace lactel;

int main(int argc, char* argv[]){

    

    TApplication app("ROOTApp", &argc, argv);
 
    TCanvas canvas("canvas", "myCanvas", 1000, 800);
    canvas.Divide(2, 3);

    ElementaryCell cell;
    std::cout << "\r\033[10C" << "position: " << cell.getTransform().GetPosition() << std::endl;
    std::cout << "\r\033[10C" << "dimension" << cell.getTransform().GetDimensions() << std::endl;
    std::cout << "\r\033[10C" << "OM: " << cell.getOM() << std::endl;
    // cell.setPosition(G4ThreeVector(2000, 0, -2000));
    canvas.cd(1);
    gPad->DrawFrame(-5000, -5000, 5000, 5000);
    gPad->SetGridx();
    gPad->SetGridy();
    cell.draw();
    canvas.cd(2);
    gPad->DrawFrame(-5000, -5000, 5000, 5000);
    gPad->SetGridx();
    gPad->SetGridy();
    cell.print();
    cell.getTransform().SetPosition(G4ThreeVector(-4000, -2000, 0));
    cell.getTransform().SetDimensions(G4ThreeVector(2000, 2000, 2000));
    cell.print();
    cell.draw();

    canvas.cd(3);
    Module module;
    gPad->DrawFrame(-10000, -10000, 10000, 10000);
    gPad->SetGridx();
    gPad->SetGridy();
    module.addECell("x");
    module.addECell("x");
    module.addECell("x");
    module.addECell("x");
    // module.setInterCellDistanceZ();
    std::cout << "\r\033[10C" << ">>>> Start Check" << std::endl;
    module.print(std::cout, true);
    module.reset();
    module.print(std::cout, true);
    module.setDefault();
    module.print(std::cout, true);
    // module.SetPosition(G4ThreeVector(1500));
    module.print(std::cout, true);
    std::cout << "\r\033[10C" << "<<<< End Check" << std::endl;
    // module.centerCells();
    // module.setInterCellDistanceY(100*cm);
// module.draw();
    module.addECell("x");
    module.addECell("x");
    module.addECell("x");
    module.addECell("x");
    // module.SetSpacing(G4ThreeVector(module.GetDimensions().getX()+50*cm, 0, module.GetDimensions().getZ()+50*cm));
    module.SetCounts(3, 3, 2);
    module.SetSpacing(G4ThreeVector(cell.GetDimensions().getX()+50*cm, cell.GetDimensions().getY()+50*cm, cell.GetDimensions().getZ()+50*cm));
    module.SetOrigin(G4ThreeVector(0, 0, -2400*mm));
    module.BuildGrid(cell);
    // module.updateGeometry();
    module.CenterGrid();
    // module.SetPosition(G4ThreeVector(4000, 0, -2250));
    // module.setInterCellDistanceX();
    // module.CenterZ();
    module.print();
    module.draw("", true);
    module.print(std::cout, true);
    
    module.setName("Module1");

    // std::ofstream detFile("KM3Det" + module.getName() + ".dat", std::ios::out);
    // std::ofstream matFile("KM3Vol" + module.getName() + ".dat", std::ios::out);
    // module.dumpCells(detFile, true);
    // module.dumpCellsMaterial(matFile);
    // detFile.close();
    // matFile.close();


     canvas.cd(4);
    gPad->DrawFrame(-10000, -10000, 10000, 10000);
    gPad->SetGridx();
    gPad->SetGridy();
    Module module2;
    module2.SetCounts(1, 1, 2);
    module2.SetSpacing(G4ThreeVector(cell.GetDimensions().getX()+50*cm, cell.GetDimensions().getY()+50*cm, cell.GetDimensions().getZ()+50*cm));
    module2.SetOrigin(G4ThreeVector(0, 0, -2400*mm));
    module2.BuildGrid(cell);
    module2.print(std::cout, true);


    canvas.cd(5);
    gPad->DrawFrame(-10000, -10000, 10000, 10000);
    gPad->SetGridx();
    gPad->SetGridy();

    BLactelDetector detector;
    detector.SetCounts(3, 3, 1);
    // module2.SetSpacing(G4ThreeVector(module2.GetDimensions().getX()+500*cm, module2.GetDimensions().getY()+50*cm, module2.GetDimensions().getZ()+50*cm));
    // detector.SetOrigin(G4ThreeVector(0, 0, -3400*mm));
    detector.BuildGrid(module2);
    // detector.updateGeometry();
    detector.drawModules("", 1, 0);
    detector.print(std::cout, 1, 1);

    canvas.cd(6);
    gPad->DrawFrame(-5000, -5000, 5000, 5000);
    gPad->SetGridx();
    gPad->SetGridy();


//Define cells
ElementaryCell EC1 = ElementaryCell(G4ThreeVector(0*m, 0*m, -1.5*m),       G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0321"));
ElementaryCell EC2 = ElementaryCell(G4ThreeVector(0*m, 0*m, -4*m),         G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0304"));

ElementaryCell EC3 = ElementaryCell(G4ThreeVector(0*m, 2.5*m, -1.5*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0423"));
ElementaryCell EC4 = ElementaryCell(G4ThreeVector(0*m, 2.5*m, -4*m),       G4ThreeVector(2*m, 2*m, 2*m), OM("OMS1060"));

ElementaryCell EC5 = ElementaryCell(G4ThreeVector(0*m, -2.5*m, -1.5*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0311"));
ElementaryCell EC6 = ElementaryCell(G4ThreeVector(0*m, -2.5*m, -4*m),      G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0302"));

ElementaryCell EC7 = ElementaryCell(G4ThreeVector(2.5*m, -2.5*m, -1.5*m),  G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0648"));
ElementaryCell EC8 = ElementaryCell(G4ThreeVector(2.5*m, -2.5*m, -4*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0366"));

ElementaryCell EC9 = ElementaryCell(G4ThreeVector(2.5*m, 0*m, -1.5*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0137"));
ElementaryCell EC10 = ElementaryCell(G4ThreeVector(2.5*m, 0*m, -4*m),       G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0298"));

ElementaryCell EC11 = ElementaryCell(G4ThreeVector(2.5*m, 2.5*m, -1.5*m),   G4ThreeVector(2*m, 2*m, 2*m), OM("OMS1044"));
ElementaryCell EC12 = ElementaryCell(G4ThreeVector(2.5*m, 2.5*m, -4*m),     G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0501"));

ElementaryCell EC13 = ElementaryCell(G4ThreeVector(-2.5*m, -2.5*m, -1.5*m), G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0358"));
ElementaryCell EC14 = ElementaryCell(G4ThreeVector(-2.5*m, -2.5*m, -4*m),   G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0319"));

ElementaryCell EC15 = ElementaryCell(G4ThreeVector(-2.5*m, 0*m, -1.5*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0790"));
ElementaryCell EC16 = ElementaryCell(G4ThreeVector(-2.5*m, 0*m, -4*m),      G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0293"));

ElementaryCell EC17 = ElementaryCell(G4ThreeVector(-2.5*m, 2.5*m, -1.5*m),  G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0365"));
ElementaryCell EC18 = ElementaryCell(G4ThreeVector(-2.5*m, 2.5*m, -4*m),    G4ThreeVector(2*m, 2*m, 2*m), OM("OMS0299"));

// define modules
Module M1 = Module({EC1, EC2});
Module M2 = Module({EC3, EC4});
Module M3 = Module({EC5, EC6});
Module M4 = Module({EC7, EC8});
Module M5 = Module({EC9, EC10});
Module M6 = Module({EC11, EC12});
Module M7 = Module({EC13, EC14});
Module M8 = Module({EC15, EC16});
Module M9 = Module({EC17, EC18});

//define Detector
BLactelDetector det({M1, M2, M3, M4, M5, M6, M7, M8, M9},
                            G4ThreeVector(0*cm, 0*cm, 0*cm)
                        );
    det.drawModules("", 0, 1);
    det.print(std::cout, 1, 1);
    det.setName("LacWater");
    // std::ofstream file("testdet.det", std::ios::out);
    // det.dump(file);
    // file.close();
    det.centerOnItsCells(G4ThreeVector(2*m, 2*m, 2*m));
    det.setCellsPositionRelativeToDetector();

    // std::ofstream detFile("KM3Det" + det.getName() + ".dat", std::ios::out);
    // std::ofstream matFile("KM3Vol" + det.getName() + ".dat", std::ios::out);
    // det.dump(detFile);
    // det.dumpMaterial(matFile);
    // detFile.close();
    // matFile.close();
    det.generateDetectorFiles();



BLactelDetector det1;
det1.read("../common/data/KM3DetLacWater.dat");
det1.print(std::cout, true, true);
    
    // TBox *box = new TBox(-2000, -2000, -2000, 2000);
    // box->SetFillColor(2);          // fill color
    // box->SetLineColor(1);

    // box->Draw();


    // // Create cube
    // auto cube = vtkSmartPointer<vtkCubeSource>::New();d

    // // Mapper
    // auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // mapper->SetInputConnection(cube->GetOutputPort());

    // // Actor
    // auto actor = vtkSmartPointer<vtkActor>::New();
    // actor->SetMapper(mapper);

    // // Renderer
    // auto renderer = vtkSmartPointer<vtkRenderer>::New();
    // renderer->AddActor(actor);
    // renderer->SetBackground(0.1, 0.2, 0.3);

    // // Window
    // auto window = vtkSmartPointer<vtkRenderWindow>::New();
    // window->AddRenderer(renderer);
    // window->SetSize(800, 600);

    // // Interactor
    // auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    // interactor->SetRenderWindow(window);

    // window->Render();
    // interactor->Start();


    
    
    // TF1* flux = new TF1("flux", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 10, 5e3, 1);
    // TF1* flux56 = new TF1("flux56", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 10, 5e3, 1);
    // TF1* flux75 = new TF1("flux75", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::gaisser_flux(x, pars);}, 10, 5e3, 1);
    // TF1* modFlux = new TF1("modFlux", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 10, 5e3, 1);
    // TF1* modFlux30 = new TF1("modFlux30", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 10, 5e3, 1);
    // TF1* modFlux60 = new TF1("modFlux60", [](double* x, double* pars){ return pow(x[0], 2.7) *lactel::BPrimaryGeneratorAction::ModifiedFormula(x, pars);}, 10, 5e3, 1);

    // flux->SetParameter(0, 0.);
    // modFlux->SetParameter(0, 0.);
    // flux56->SetParameter(0, 56);
    // modFlux60->SetParameter(0, 75);

    // modFlux30->SetParameter(0, 56);
    // modFlux60->SetParameter(0, 75);


    // modFlux->SetLineColor(4);

    // modFlux30->SetLineColor(4);
    // modFlux30->SetLineStyle(2);

    // modFlux60->SetLineColor(4);
    // modFlux60->SetLineStyle(7);

    // flux->Draw();
    // modFlux->Draw("same");
    // modFlux30->Draw("same");
    // modFlux60->Draw("same");



    cout << "Info: Waiting for user action! ..." << endl;
    app.Run();

    return 0;
}