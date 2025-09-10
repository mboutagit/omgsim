#include "BRunAction.hh"

#include "G4DigiManager.hh"
#include "BDigitizerModule.hh"


namespace lactel
{
    BRunAction::BRunAction(){
    }

    BRunAction::~BRunAction(){
        
    }

    void BRunAction::BeginOfRunAction(const G4Run* run){
        KM3RunAction::BeginOfRunAction(run);

        BAnalysis * ana = BAnalysis::getIt();
        ana->fRun.runNb = run->GetRunID();

        ana->fRun.acceptanceAngle = BDetectorConstruction::GetIt()->acceptanceAngle /deg;
        ana->fRun.generationSurface = BDetectorConstruction::GetIt()->AGen;
        
        BPrimaryGeneratorAction::getInstance()->getExpectedFlux(); // just to set up BPrimaryGeneratorAction::muonsFlux first;
        ana->fRun.expectedRate = BPrimaryGeneratorAction::getInstance()->muonsFlux;
        ana->fRun.detName = BDetectorConstruction::GetIt()->detName;

        cout << endl << "\r\033[10C" << "Begin of run: " << run->GetRunID() << endl;
        
        BPrimaryGeneratorAction::getInstance()->thetaMax = BDetectorConstruction::GetIt()->acceptanceAngle;
        BPrimaryGeneratorAction::getInstance()->getExpectedFlux();

        //Digi
         static bool first = true;
        if (first) {
            G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
            digiMan->AddNewModule(new BDigitizerModule("DigitizerModule"));
            first = false;
        }
    }

    void BRunAction::EndOfRunAction(const G4Run* run){
        KM3RunAction::EndOfRunAction(run);

        BAnalysis * ana = BAnalysis::getIt();
        BPrimaryGeneratorAction * bpga = BPrimaryGeneratorAction::getInstance();
        ana->runTree->Fill();
        ana->fTRunInfos->Fill();

        //reset values
        ana->fRun.nDetEvent = 0;
        ana->fRun.nGenEvent = 0;
        ana->fRun.runNb = -1;

        std::cout << "\r\033[10C" << "nGen: " << bpga->nGen << " -- nDet: " << bpga->nDet << " -- Muons flux: " << bpga->muonsFlux << std::endl;
        std::cout << "\r\033[10C" << "Lactel rate for run " << run->GetRunID() << " is: " << bpga->nDet * bpga->muonsFlux / bpga->nGen << " muons/s" << std::endl;
    }
} // namespace lactel
