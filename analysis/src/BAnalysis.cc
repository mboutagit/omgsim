#include "BAnalysis.hh"
#include "ParametrizedGenerator.hh"
#include "BPrimaryGeneratorAction.hh"

#include "KM3EventAction.hh"

#include "G4RunManager.hh"

#include "TTree.h"

namespace lactel
{

    // std::map<float, float> BAnalysis::MeasureParameters::quantum_efficiency = {
    //                         {290., 0.},
    //                         {293., 0.05},
    //                         {295., 0.013},
    //                         {300., 0.033},
    //                         {305., 0.053},
    //                         {310., 0.077},
    //                         {315., 0.107},
    //                         {320., 0.13},
    //                         {325., 0.155},
    //                         {330., 0.177},
    //                         {335., 0.191},
    //                         {340., 0.207},
    //                         {345., 0.217},
    //                         {350., 0.226},
    //                         {355., 0.23},
    //                         {360., 0.233},
    //                         {365., 0.23},
    //                         {370., 0.228},
    //                         {380., 0.229},
    //                         {390., 0.229},
    //                         {400., 0.233},
    //                         {410., 0.231},
    //                         {420., 0.231},
    //                         {430., 0.226},
    //                         {440., 0.221},
    //                         {450., 0.21},
    //                         {460., 0.201},
    //                         {470., 0.189},
    //                         {480., 0.178},
    //                         {490., 0.170},
    //                         {500., 0.164},
    //                         {510., 0.150},
    //                         {520., 0.129},
    //                         {530., 0.101},
    //                         {540., 0.080},
    //                         {550., 0.068},
    //                         {560., 0.059},
    //                         {570., 0.051},
    //                         {580., 0.042},
    //                         {590., 0.034},
    //                         {600., 0.028},
    //                         {620., 0.020},
    //                         {650.,  0.010},
    //                         {680.,  0.}
    //                     };
    // float BAnalysis::MeasureParameters::HV = 1600.;

    BAnalysis::MeasureParameters::MeasureParameters(){
        gain_vs_highVoltage = new TF1("gainVsHv", getGainROOT, 1600, 1900, 0);
        gain_vs_highVoltage->SetTitle("Gain vs. High voltage (OMS0321); HV (V); Gain");

        // int valuesSize = quantum_efficiency.size();
        // std::vector<float> lambda, QE;

        // for(auto& value : quantum_efficiency){
        //     lambda.push_back(value.first);
        //     QE.push_back(value.second);
        // }

        // QEVsLambda_graph = new TGraph(valuesSize, &lambda[0], &QE[0]);
        // QEVsLambda_graph->SetTitle("QE vs. wavelength; #lambda (nm); QE (%)");

        // double* muon_energy;
        // muon_energy[0]=0.1;
        
        // double theta = 60.;
        // double h = 372;

        // double* pars;
        // pars[0]=theta;
        // pars[1]=h;

       // ParametrizedGenerator::MuonFlux(muon_energy, pars);

        muon_flux = new TF1("muons_flux", BPrimaryGeneratorAction::MuonFlux, 1, 10, 100);
        muon_flux->SetParameter(0, 0.0);  
        muon_flux->SetParameter(1, 372 * m); 
        muon_flux->SetTitle("Muon Flux at Altitude h; E_{mu} [GeV]; dN_{mu}/dE d#Omega");

    }

    BAnalysis::MeasureParameters::~MeasureParameters(){
        if(gain_vs_highVoltage)
            delete gain_vs_highVoltage;

        if(QEVsLambda_graph)
            delete QEVsLambda_graph;

        if(muon_flux)
            delete muon_flux;
    };

    float BAnalysis::MeasureParameters::getQE(float lambda){
        return QEVsLambda_graph->Eval(lambda);
    }

    TF1* BAnalysis::MeasureParameters::getGainVsHV(){
        if(gain_vs_highVoltage)
            return gain_vs_highVoltage;
    }

    TF1* BAnalysis::MeasureParameters::getMuonFlux(){
        if(muon_flux)
            return muon_flux;
    }

    TGraph* BAnalysis::MeasureParameters::getQEVsLambda(){
        if(QEVsLambda_graph)
            return QEVsLambda_graph;
    }
    

    BAnalysis::Measure::Measure() : fId(0), fEnergy(0), fNpe(0), fTime(0) {
     //   cout << "\r\033[20C" << "Measure invoked with id: " << fId << endl;
    }

    BAnalysis::Measure::Measure(float energy) : fId(0), fNpe(0), fTime(0) {
        fEnergy = energy;
    }

    BAnalysis::Measure::~Measure(){}
    void BAnalysis::Measure::print(std::ostream& out = std::cout){
        out << "id: " << fId << "\t"
            << "Ene: " << fEnergy << "\t"
            << "NPE: " << fNpe << "\t"
            << "TIME: " << fTime
            << std::endl;
    }


    BAnalysis::BAnalysis(){}
    BAnalysis::~BAnalysis(){
        // if(genTree)
        //     delete genTree;
        
        // if(runTree)
        //     delete genTree;
            
        // if(bEventTree)
        //     delete genTree;
    }
    void BAnalysis::Init(std::string rfn){
        AAAnalysisManager::Init(rfn);
        genTree = new TTree("Gen", "generation parameters");
        runTree = new TTree("Run", "Run parameters");
        detectedEventTree = new TTree("DetectedEvent", "DetectedEvents parameters");
        
        genTree->Branch("gen", &genPars);
        //fTEvent->Branch("measure", &fMeasure);
        detectedEventTree->Branch("DetectedEvent", &fDetectedEvent);
        runTree->Branch("Run", &fRun);
    }

    // void BAnalysis::BeginOfRunAction(const G4Run* run){
    //     fRun.runNb = run->GetRunID();
    //     cout << endl << "\r\033[10C" << "Begin of run: " << run->GetRunID() << endl;
    // }

    // void BAnalysis::EndOfRunAction(const G4Run* run){
    //    // fRun.runNb = run->GetRunID();
    //     runTree->Fill();

    //     fRun.nDetEvent = 0;
    //     fRun.nGenEvent = 0;
    //     fRun.runNb = -1;

    //     // cout << endl << "\r\033[10C" << "End of run: " << run->GetRunID() << endl;
    // }


    void BAnalysis::StartOfEvent(const G4Event* EV){
        AAAnalysisManager::StartOfEvent(EV); // call the base class function first
        
        BPrimaryGeneratorAction::getInstance()->nGen += 1;
        if(fDetectedEvent.nhit > 0)
            BPrimaryGeneratorAction::getInstance()->nDet += 1;

        fDetectedEvent.event_id = 0;
        fDetectedEvent.nhit = 0;
        fDetectedEvent.tmax = 0;
        fDetectedEvent.tmin = 0;
        fDetectedEvent.tota = 0;
        fDetectedEvent.hits.clear();
    }

    void BAnalysis::EndOfEvent(const G4Event* EV) {
        #ifdef DEBUG
            std::cout << "\r\033[10C" << "From : BAnalysis::EndOfEvent(const G4Event* EV)" << std::endl;
        #endif

        fRun.nGenEvent += 1; // add the event to the run 
        // if(fEvent.isDetected == true)
        //     fRun.nDetEvent += 1; // add the event is detected

        AAAnalysisManager::EndOfEvent();
  
        G4PrimaryParticle*  particle = EV->GetPrimaryVertex ()->GetPrimary ();
        genPars.fGenPPDGID = particle->GetPDGcode();
        genPars.fGenEvtID  = EV->GetEventID();    
        genPars.fGenXPos   = EV->GetPrimaryVertex ()->GetPosition().getX() / mm;
        genPars.fGenYPos   = EV->GetPrimaryVertex ()->GetPosition().getY() / mm;
        genPars.fGenZPos   = EV->GetPrimaryVertex ()->GetPosition().getZ() / mm;
        genPars.fGenXDir   = particle->GetMomentum().unit().getX() / mm;
        genPars.fGenYDir   = particle->GetMomentum().unit().getY() / mm;
        genPars.fGenZDir   = particle->GetMomentum().unit().getZ() / mm;
        genPars.fGenTheta  = particle->GetMomentum().theta() / deg;
        genPars.fGenPhi    = particle->GetMomentum().phi() /deg;
        genPars.fGenEnergy = particle->GetTotalEnergy() / MeV;      

        // fDetectedEvent.event_id = EV->GetEventID();
        // fDetectedEvent.event_id = 0;
        // fDetectedEvent.nhit = 0;
        // fDetectedEvent.tmax = 0;
        // fDetectedEvent.tmin = 0;
        // fDetectedEvent.tota = 0;
        // fDetectedEvent.hits.clear();

        // fDetectedEvent.Hit.id = -1;
        // fDetectedEvent.Hit.time = 0;
        // fDetectedEvent.Hit.charge = 0;

        fDetectedEvent.event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        fDetectedEvent.runNb = fRun.runNb; // OK as fRun.runNb is set in the start of the run

        

        if(fDetectedEvent.nhit > 0){    // only detected events
            detectedEventTree->Fill();
            genPars.isDetected = true;
            // std::cout << "\r\033[10C" << "-- Detected event: nhit=" << fDetectedEvent.nhit << std::endl;  
        }
        
        genTree->Fill();
            

        // std::cout << "\r\033[10C" << "End of Event: " << EV->GetEventID() << std::endl;
    }

    BAnalysis::MeasureParameters BAnalysis::getMeasurePars(){
        return fMeasure_parameters;
    }

    BAnalysis::Measure BAnalysis::getMeasure(){
        return fMeasure;
    }


    BAnalysis* BAnalysis::getIt(){
        if(!fManager){
          fManager = new BAnalysis();
        }
        return (BAnalysis*) fManager;
    }

    void BAnalysis::AddDetectedParticule(const KM3TrackInformation* info, unsigned char LIM){
        fMeasure.fEnergy = info->GetOriginalEnergy ();
        fMeasure.fNpe = info->GetOriginalEnergy () /2; 
        fMeasure.fTime = info->GetOriginalEnergy () / 4;
       
        AAAnalysisManager::AddDetectedParticule(info, LIM);
        
        Measure measure;
        measure.fEnergy = info->GetOriginalEnergy ();
        // eventMeasure_map.insert({G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID(), measure});

        fMeasure.fId += 1;
        measure.fId += 1;

        Hit ahit;
        ahit.id += 1;
        ahit.time = info->GetOriginalTime();
        ahit.charge = info->GetOriginalEnergy() / CLHEP::eV;

        fDetectedEvent.hits.push_back(ahit);

        //fDetectedEvent.event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        fDetectedEvent.event_energy += info->GetOriginalEnergy() / CLHEP::eV; 

        fDetectedEvent.nhit += 1;
        if(fDetectedEvent.tmin > info->GetOriginalTime())
            fDetectedEvent.tmin = info->GetOriginalTime();
        
        if(fDetectedEvent.tmax < info->GetOriginalTime())
            fDetectedEvent.tmax = info->GetOriginalTime();

        fDetectedEvent.tota += info->GetOriginalEnergy() / CLHEP::eV;
           //fDetectedEvent.a = info->GetOriginalEnergy() / CLHEP::eV;
        //fDetectedEvent.runNb = fRun.runNb; // OK as fRun.runNb is set in the start of the run
    } 

    void BAnalysis::Write(){
    //     fMeasure_parameters.getGainVsHV()->Write();
    //     fMeasure_parameters.getQEVsLambda()->Write();
    //    if(fMeasure_parameters.getMuonFlux())
    //        fMeasure_parameters.getMuonFlux()->Write();

        // genTree->Write();
        // runTree->Write();
        // if(fRFile) 
        //     genTree->Write();
        //     detectedEventTree->Write();
        
        AAAnalysisManager::Write();
    }

    //ClassImp(lactel::BAnalysis::Measure);
   //ClassImp(BAnalysis);
} // namespace lactel
