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

        BAnalysisManager * ana = BAnalysisManager::getIt();
        BPrimaryGeneratorAction* bpga = BPrimaryGeneratorAction::getInstance();
        DAQManager* daqManager = DAQManager::getIt();
        if(ana->fRun.runNb == -1)
            ana->fRun.runNb = run->GetRunID();

        // overload : KM3RunAction::BeginOfRunAction(run);
        G4int RunN = ana->fRun.runNb; //aRun->GetRunID();
        //if ( RunN % 1000 == 0 )
        // G4cout << G4endl;

        if(BLogger::getInstance()->getDebugLevel() > 1)
            std::cout << std::endl << "Starting run: " << RunN << std::endl;

        // std::cout << "\r\033[10C" << "--- Logger level: " << BLogger::getInstance()->debugLevel << std::endl;
        // BLogger::getInstance()->log(utils::BLogger::Level::INFO, "Starting run : " + std::to_string(RunN) );

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

        cout << endl << "\r\033[10C" << "Begin of run: " << ana->fRun.runNb << endl;
        
        bpga->thetaMax = BDetectorConstruction::GetIt()->acceptanceAngle;
        bpga->getExpectedFlux();

        //Digi
         static bool first = true;
        if (first) {
            G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
            digiMan->AddNewModule(new BDigitizerModule("DigitizerModule"));
            first = false;
        }

        // BWaveCatcher* wc = new BWaveCatcher();
        // wc->startAcquisition();
    }

    void BRunAction::EndOfRunAction(const G4Run* run){
        // KM3RunAction::EndOfRunAction(run); // not needed does not do anything

        BAnalysisManager * ana = BAnalysisManager::getIt();
        BPrimaryGeneratorAction * bpga = BPrimaryGeneratorAction::getInstance();
        DAQManager* daqManager = DAQManager::getIt();
        BDetectorConstruction* det = BDetectorConstruction::GetIt();

        // det->PrintAllMaterialProperties();

        if(BLogger::getInstance()->getDebugLevel() > 1){
            std::cout << "\r\033[10CRun: " << ana->fRun.runNb << " -- nGen: " << bpga->nGen << " -- nDet: " << bpga->nDet << " -- Muons flux: " << bpga->muonsFlux << std::endl;
            std::cout << "\r\033[10C" << "Lactel rate for run " << ana->fRun.runNb << " is: " << bpga->nDet * bpga->muonsFlux / bpga->nGen << " muons/s" << std::endl;
        }

        if(daqManager->isActive()){
            if(BLogger::getInstance()->getDebugLevel() > 1){
                std::cout << "\r\033[55C \033[1;35m" << "DAQ ON ... Events size: " << daqManager->getEvents().size() << "\033[0m" << std::endl;
                daqManager->dumpEvents(std::cout);
            }

            OM oms0321("OMS0321");
            OM oms0304("OMS0304");
            BWaveCatcher* wc1 = new BWaveCatcher("wc1");
            wc1->plugOM(0, oms0321);
            wc1->plugOM(1, oms0321);
            daqManager->addAWaveCatcher(wc1);
            daqManager->dump();
            std::vector<BAnalysisManager::Hit*> hits = daqManager->getHitsByChannelID(wc1, 0);
            std::vector<BAnalysisManager::Hit*> hits1 = daqManager->getHitsByChannelID(wc1, 1);
            
            if(BLogger::getInstance()->getDebugLevel() > 1){
                std::cout << "\r\033[32C" << "hits vetor is: " << hits.size() << std::endl;
                std::cout << "\r\033[32C" << "hits1 vetor is: " << hits1.size() << std::endl;
                for(auto& hit : hits)
                    hit->dump();
                
                for(auto& hit : hits1)
                    hit->dump();
            }
            
        }
        else{
            if(BLogger::getInstance()->getDebugLevel() > 1)
                std::cout << "\r\033[55C \033[1;31m" << "DAQ OFF ... Events size: " << daqManager->getEvents().size() << "\033[0m" << std::endl;
        }
        // daqManager->getDetectedEvents(ana->getDetectedEventsTree());
        // std::cout << "\r\033[22C" << "detected Events size: " << daqManager->getEvents().size() << std::endl;
        
        // Fill treis; 
        ana->runTree->Fill();
        ana->fTRunInfos->Fill();
        // write data
        ana->Write();
        //reset values
        ana->fRun.nDetEvent = 0;
        ana->fRun.nGenEvent = 0;
        ana->fRun.runNb = -1;
        // ana->fRFile->Close();
    }
} // namespace lactel
