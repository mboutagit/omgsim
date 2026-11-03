#include "BRunAction.hh"

#include "G4DigiManager.hh"
#include "BDigitizerModule.hh"
#include "BWaveCatcher.hh"
#include "DAQManager.hh"

#include "G4VVisManager.hh"
#include "G4UImanager.hh"

#include "BLogger.hh"

using namespace tools;
using namespace DAQ;
using namespace utils;
namespace lactel
{
    BRunAction::BRunAction(){
    }

    BRunAction::~BRunAction(){
        
    }

    void BRunAction::BeginOfRunAction(const G4Run* run){
        BLOG_INFO(Form("Staring run %s ..", run->GetRunID()));

        BAnalysisManager * ana = BAnalysisManager::getIt();
        BPrimaryGeneratorAction* bpga = BPrimaryGeneratorAction::getInstance();
        DAQManager* daqManager = DAQManager::getIt();
        if(ana->fRun.runNb == -1)
            ana->fRun.runNb = run->GetRunID();

        G4int RunN = ana->fRun.runNb; //aRun->GetRunID();

        if (G4VVisManager::GetConcreteInstance())
        {
            G4UImanager* UI = G4UImanager::GetUIpointer();
            // UI->ApplyCommand("/vis/clear/view");
            // UI->ApplyCommand("/vis/draw/current");
        }

        if (KM3AnalysisManager::fEvtOutput) {
            KM3AnalysisManager::evt_stream << "norma: 0 " << run->GetNumberOfEventToBeProcessed() << std::endl;
            KM3AnalysisManager::evt_stream << "end_event:" << std::endl;
        }
        // overload : KM3RunAction::BeginOfRunAction(run);

        ana->Init(("run_" + std::to_string(ana->fRun.runNb) + ".root").c_str());
        bpga->init();
        daqManager->init();

        // ana->fRFile = new TFile(("run_" + std::to_string(run->GetRunID()) + ".root").c_str(), "recreate");
        //ana->fRun.runNb = run->GetRunID();

        ana->fRun.acceptanceAngle = BDetectorConstruction::GetIt()->acceptanceAngle /deg;
        ana->fRun.generationSurface = BDetectorConstruction::GetIt()->AGen/cm2; // in cm2
        
        bpga->getExpectedFlux(); // just to set up BPrimaryGeneratorAction::muonsFlux first;
        ana->fRun.expectedRate = bpga->muonsFlux;
        ana->fRun.detName = BDetectorConstruction::GetIt()->detName;
        bpga->thetaMax = BDetectorConstruction::GetIt()->acceptanceAngle;

        cout << endl << "\r\033[10C" << "Begin of run: " << ana->fRun.runNb << endl;
        
        //Digi
        //  static bool first = true;
        // if (first) {
        //     G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
        //     digiMan->AddNewModule(new BDigitizerModule("DigitizerModule"));
        //     first = false;
        // }

        // BWaveCatcher* wc = new BWaveCatcher();
        // wc->startAcquisition();
    }

    void BRunAction::EndOfRunAction(const G4Run* run){
        // KM3RunAction::EndOfRunAction(run); // not needed does not do anything

        BAnalysisManager * ana = BAnalysisManager::getIt();
        BPrimaryGeneratorAction * bpga = BPrimaryGeneratorAction::getInstance();
        DAQManager* daqManager = DAQManager::getIt();
        BDetectorConstruction* det = BDetectorConstruction::GetIt();

        BLOG_INFO(Form("Run: %d, nGen: %d, nDet: %d", ana->fRun.runNb,
                                                                    bpga->nGen,
                                                                    bpga->nDet
                                                                ));

        if(daqManager->isActive()){

            BLOG_INFO(Form("DAQ ON ... Events size: %d", daqManager->getEvents().size()));
            if(BLogger::getInstance()->debugLevel > 2)
                daqManager->dumpEvents(*BLogger::getInstance()->out);
            BLOG_WARNING("This functionality is not implimented yet !");
        }
        else{
            BLOG_INFO("DAQ is OFF");
        }
        
        BLOG_INFO("Writing information to file ..");

        // Fill treis; 
        ana->runTree->Fill();
        ana->fTRunInfos->Fill();
        ana->detectedEventTree->Fill();
        // write data
        ana->Write();
        ana->BLactelDetectorTree->Write();
        //reset values
        ana->fRun.nDetEvent = 0;
        ana->fRun.nGenEvent = 0;
        ana->fRun.runNb = -1;
        // ana->fRFile->Close();
        BLOG_INFO(Form("Ending run %d", run->GetRunID()));
    }
} // namespace lactel
