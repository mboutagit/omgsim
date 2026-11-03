#include "BAnalysisManager.hh"
#include "ParametrizedGenerator.hh"
#include "BPrimaryGeneratorAction.hh"
#include "BDetectorConstruction.hh"

#include "KM3EventAction.hh"
#include "KM3PhotoTubeMgr.hh"

#include "BHit.hh"

#include "Rtypes.h"

#include "BRunManager.hh"
#include "BEvent.hh"

#include "TTree.h"
#include "DAQManager.hh"

ClassImp(lactel::BAnalysisManager::DAQEvent);

namespace lactel
{
    // std::map<float, float> BAnalysisManager::MeasureParameters::quantum_efficiency = {
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
    // float BAnalysisManager::MeasureParameters::HV = 1600.;

    BAnalysisManager::MeasureParameters::MeasureParameters(){
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

    BAnalysisManager::MeasureParameters::~MeasureParameters(){
        if(gain_vs_highVoltage)
            delete gain_vs_highVoltage;

        if(QEVsLambda_graph)
            delete QEVsLambda_graph;

        if(muon_flux)
            delete muon_flux;
    };

    float BAnalysisManager::MeasureParameters::getQE(float lambda){
        return QEVsLambda_graph->Eval(lambda);
    }

    TF1* BAnalysisManager::MeasureParameters::getGainVsHV(){
        if(gain_vs_highVoltage)
            return gain_vs_highVoltage;
    }

    TF1* BAnalysisManager::MeasureParameters::getMuonFlux(){
        if(muon_flux)
            return muon_flux;
    }

    TGraph* BAnalysisManager::MeasureParameters::getQEVsLambda(){
        if(QEVsLambda_graph)
            return QEVsLambda_graph;
    }
    

    BAnalysisManager::Measure::Measure() : fId(0), fEnergy(0), fNpe(0), fTime(0) {
     //   cout << "\r\033[20C" << "Measure invoked with id: " << fId << endl;
    }

    BAnalysisManager::Measure::Measure(float energy) : fId(0), fNpe(0), fTime(0) {
        fEnergy = energy;
    }

    BAnalysisManager::Measure::~Measure(){}
    void BAnalysisManager::Measure::print(std::ostream& out = std::cout){
        out << "id: " << fId << "\t"
            << "Ene: " << fEnergy << "\t"
            << "NPE: " << fNpe << "\t"
            << "TIME: " << fTime
            << std::endl;
    }


    BAnalysisManager::BAnalysisManager(){
    }
    BAnalysisManager::~BAnalysisManager(){
        // if(genTree)
        //     delete genTree;
        
        // if(runTree)
        //     delete genTree;
            
        // if(bEventTree)
        //     delete genTree;
    }
    void BAnalysisManager::Init(std::string rfn){
        // AAAnalysisManager::Init(rfn);

        if (rfn.size ())
        fRootFileName=rfn;
        BLOG_INFO(Form("Creating an Analysis root files: %s", fRootFileName.c_str()));
        fRFile = new TFile (fRootFileName.data (), "RECREATE");

        fTRunInfos = new TTree("RunInfo","Infos about the simulation.");
        fTRunInfos->Branch("NBTrigg",&fNbTrigg,"NbTrigg/l");
        fTEvent = new TTree("Hit","Hits in a TTree");
        fTEvent->Branch("Vertex",&run_id,"run_id/i:evt_id:VertexX/f:VertexY:VertexZ:VertexT:VertexP:VertexDirX:VertexDirY:VertexDirZ:VertexEnergy:OMHitX:OMHitY:OMHitZ:OMHitT:OMHitP:PhotoX:PhotoY:PhotoZ:PhotoT:PhotoP:OMID/S:PMID");
        fTFastAnalysis = new TTree("FastAnalysis","Data used for very fast analysis.");
        fTFastAnalysis->Branch("PhotonReach",&fNbOM,"NbOM/l:NbPhoto");

        //ANTARES evt-like format output
        if (fEvtOutput) {
            evt_stream.open(evtFileName);
            evt_stream << "start_run: 1" << endl;
            evt_stream << "simul: OMGsim " << GIT_VERSION << "-" << BUILD_DATE << " " << KM3AnalysisManager::currentDateTime() << endl;
        }   

        fTFastAnalysis->Branch ("Detection" ,fNbDetected,"NbDetected[4][100]/i");
        fTEvent->Branch        ("Hit"       ,&fTotalTime,"TotalTime:Energy:Polarity/O:Limit/b");

        genTree = new TTree("Gen", "generation parameters");
        runTree = new TTree("Run", "Run parameters");
        detectedEventTree = new TTree("DetectedEvent", "DetectedEvents parameters");
        DAQEventTree = new TTree("DAQEvent", "DAQ parameters");
        BLactelDetectorTree = new TTree("BLactelDetector", "Detector description");

        BLactelDetectorTree->Branch("LactelDetector", &(BDetectorConstruction::GetIt()->getDetector()) );

        
        genTree->Branch("gen", &genPars);
        //fTEvent->Branch("measure", &fMeasure);
        detectedEventTree->Branch("DetectedEvent", &fDetectedEvent);
        // DAQEventTree->Branch("DAQ Event", &fDaqEvent);
        runTree->Branch("Run", &fRun);
    }

    // void BAnalysisManager::BeginOfRunAction(const G4Run* run){
    //     fRun.runNb = run->GetRunID();
    //     cout << endl << "\r\033[10C" << "Begin of run: " << run->GetRunID() << endl;
    // }

    // void BAnalysisManager::EndOfRunAction(const G4Run* run){
    //    // fRun.runNb = run->GetRunID();
    //     runTree->Fill();

    //     fRun.nDetEvent = 0;
    //     fRun.nGenEvent = 0;
    //     fRun.runNb = -1;

    //     // cout << endl << "\r\033[10C" << "End of run: " << run->GetRunID() << endl;
    // }


    void BAnalysisManager::StartOfEvent(const G4Event* EV){
        AAAnalysisManager::StartOfEvent(EV); // call the base class function first

        fDetectedEvent.event_id = 0;
        fDetectedEvent.nhit = 0;
        fDetectedEvent.tmax = 0;
        fDetectedEvent.tmin = 0;
        fDetectedEvent.tota = 0;
        fDetectedEvent.hits.clear();

        genPars.fGenPPDGID = -99.0;
        genPars.fGenEvtID  = -99.0;
        genPars.fGenXPos   = -99.0;
        genPars.fGenYPos   = -99.0;
        genPars.fGenZPos   = -99.0;
        genPars.fGenXDir   = -99.0;
        genPars.fGenYDir   = -99.0;
        genPars.fGenZDir   = -99.0;
        genPars.fGenTheta  = -99.0;
        genPars.fGenPhi    = -99.0;
        genPars.fGenEnergy = -99.0;
        genPars.isDetected = false;

        nsim+=1;
        nhit = 0;

        fVertexX=0;
        fVertexY=0;
        fVertexZ=0;
        fVertexT=0;
        fVertexP=0;
        fVertexDirX = 0;
        fVertexDirY = 0;
        fVertexDirZ = 0;
        fOMId=-1;
        fPMID=0;

        // fDaqEvent.reset();
        // fDetectedEvent.daqEvent.clear();
    }

    void BAnalysisManager::EndOfEvent(const G4Event* EV) {

        fRun.nGenEvent += 1; 
        
        BPrimaryGeneratorAction::getInstance()->nGen += 1;
        
        if(fDetectedEvent.nhit > 0){
            BPrimaryGeneratorAction::getInstance()->nDet += 1;
            fRun.nDetEvent += 1; 
        }

        // add the event to the run 
        // if(fEvent.isDetected == true)
        //     fRun.nDetEvent += 1; // add the event is detected

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
        genPars.fGenEnergy = particle->GetTotalEnergy() / GeV; 
        // genPars.fGenEnergy = BPrimaryGeneratorAction::getInstance()->particle_energy / GeV; 
        
        /////////////////////////////////
        
        const G4ThreeVector& vertexVect= EV->GetPrimaryVertex ()->GetPosition ();
        const G4ThreeVector& vertexDir = EV->GetPrimaryVertex ()->GetPrimary ()->GetMomentumDirection ();
        
        run_id = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
        evt_id = EV->GetEventID();
        fVertexX = vertexVect.x ();
        fVertexY = vertexVect.y ();
        fVertexZ = vertexVect.z ();
        fVertexT = vertexVect.theta ();
        fVertexP = vertexVect.phi ();

        fVertexDirX = vertexDir.x ();
        fVertexDirY = vertexDir.y ();
        fVertexDirZ = vertexDir.z ();

        fVertexEnergy = EV->GetPrimaryVertex ()->GetPrimary ()->GetKineticEnergy ();

        /////////////////////////////////

        fDetectedEvent.event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        fDetectedEvent.runNb = fRun.runNb; // OK as fRun.runNb is set in the start of the run
        fDetectedEvent.zenith =  BDetectorConstruction::GetIt()->acceptanceAngle;
        // fDetectedEvent.OMPosition = BDetectorConstruction::GetIt()->OMPosition;


        if(fDetectedEvent.nhit > 0){    // only detected events
            // fill tmin and tmax for the event
            float mint = fDetectedEvent.hits[0].t_tts;
            float maxt = fDetectedEvent.hits[0].t_tts;
            for(auto& hit : fDetectedEvent.hits){
                if(hit.t_tts < mint)
                    mint = hit.t_tts;
                if(hit.t_tts > maxt)
                    maxt = hit.t_tts;
            }
            fDetectedEvent.tmin = mint;
            fDetectedEvent.tmax = maxt;
            // fill the tota for the event


            detectedEventTree->Fill();
            if(DAQ::DAQManager::getIt()->isActive())
                DAQ::DAQManager::getIt()->addEvent(&fDetectedEvent);
            genPars.isDetected = true;
            ndet+=1;
        }

        BLOG_DEBUG(Form("Filling genTree: Event %d", EV->GetEventID()));
        genTree->Fill();

        BLOG_INFO(Form("End of event %d", EV->GetEventID()));
    }

    BAnalysisManager::MeasureParameters BAnalysisManager::getMeasurePars(){
        return fMeasure_parameters;
    }

    BAnalysisManager::Measure BAnalysisManager::getMeasure(){
        return fMeasure;
    }


    BAnalysisManager* BAnalysisManager::getIt(){
        if(!fManager){
          fManager = new BAnalysisManager();
        }
        return (BAnalysisManager*) fManager;
    }

    void BAnalysisManager::AddDetectedParticule(const KM3TrackInformation* info, unsigned char LIM){
        
        AAAnalysisManager::AddDetectedParticule(info, LIM);

        fMeasure.fEnergy = info->GetOriginalEnergy ();
        fMeasure.fNpe = info->GetOriginalEnergy () /2; 
        fMeasure.fTime = info->GetOriginalEnergy () / 4;
        
        Measure measure;
        measure.fEnergy = info->GetOriginalEnergy ();

        fMeasure.fId += 1;
        measure.fId += 1;

        fHits.id = nhit;
        fHits.time = info->GetOriginalTime();
        fHits.charge = info->GetOriginalEnergy() / CLHEP::eV;

        fHits.vrx = info->GetVertexPosition().getX();
        fHits.vry = info->GetVertexPosition().getY();
        fHits.vrz = info->GetVertexPosition().getZ();

        fDetectedEvent.hits.push_back(fHits);
        // BHit ahit;
        // ahit.SetEdep(info->GetOriginalEnergy() / CLHEP::eV);
        // ahit.SetTime(info->GetOriginalTime());

        // BEvent* event = BRunManager::GetRunManager()->GetCurrentEvent();

        // G4Event* event = BRunManager::GetRunManager()->GetEventManager()->GetConstCurrentEvent();
        // BEvent* myEvent = static_cast<BEvent*> (event);

        //fDetectedEvent.event_id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        fDetectedEvent.event_energy += info->GetOriginalEnergy() / CLHEP::eV; 

        fDetectedEvent.nhit += 1;
        if(fDetectedEvent.tmin > info->GetOriginalTime())
            fDetectedEvent.tmin = info->GetOriginalTime();
        
        if(fDetectedEvent.tmax < info->GetOriginalTime())
            fDetectedEvent.tmax = info->GetOriginalTime();

        fDetectedEvent.tota += info->GetOriginalEnergy() /eV;

        nhit+=1;
    } 

    void BAnalysisManager::Write(){
        if (fRFile == NULL) {
            G4cout << "WARNING: fRFile not found." << G4endl;
        return;
        }

        fRFile->Write ();
        fRFile->Close ();
        delete (fRFile);
        fRFile=0;
        cout << "Analysis file was normaly closed." << endl;
    }

 void BAnalysisManager::AddPhotoTubeEntranceInfo (const G4Step* STEP)
  {
    if (fNbPhoto % 500 ==0)
      {
       cout << "\r\033[20C" << fNbPhoto << " in pmt ";
       cout.flush ();
      }
    fNbPhoto++;
    if (fRFile == NULL)
      return;
    if (fFastAnalysisOnly)
      return;

    /*
     * More flexible and robust way of PMT number search introduced (scroll through all volumes).
     * Get OMid here was added here.
     */
    const G4VTouchable*   touchpost = STEP->GetPostStepPoint ()->GetTouchable ();
    for (int ihist = 0; ihist < touchpost->GetHistoryDepth (); ihist++) {
        G4String volname = touchpost->GetVolume(ihist)->GetName();
        if (volname.find(KM3PhotoTubeMgr::fPMTContainerName) != std::string::npos) {
             fPMID=touchpost->GetCopyNumber(ihist); // OK for OM and DOM

        } else if (KM3DetectorConstruction::fOMNames.find(volname) != KM3DetectorConstruction::fOMNames.end()) {
             fOMId=touchpost->GetCopyNumber(ihist);
        }
    }

    const G4StepPoint* prestep = STEP->GetPreStepPoint();
    const G4ThreeVector& globalPoint = prestep->GetPosition();
    const G4VTouchable*   touch = prestep->GetTouchable();
    const G4ThreeVector& localpoint=touch->GetHistory ()->GetTopTransform().TransformPoint(globalPoint);
    // const G4ThreeVector norm=prestep->GetPhysicalVolume()->GetLogicalVolume ()->GetSolid ()->SurfaceNormal( localpoint );

    fPhotoX=localpoint.x ();
    fPhotoY=localpoint.y ();
    fPhotoZ=localpoint.z ();
    fPhotoT=localpoint.theta ();
    fPhotoP=localpoint.phi ();

    fHits.pm_x=localpoint.x ();
    fHits.pm_y=localpoint.y ();
    fHits.pm_z=localpoint.z ();
    fHits.pm_t=localpoint.theta ();
    fHits.pm_p=localpoint.phi ();
  }

  //Vladimir: This function is outdated and not used currently. OMid is added at AddPhotoTubeEntranceInfo.
  void BAnalysisManager::AddOMEntranceInfo (const G4Step* STEP)
  {
    const G4StepPoint* prestep = STEP->GetPreStepPoint();
    G4String prestepName = prestep->GetPhysicalVolume()->GetName();

    // if (prestepName != "Target")
    //   {
    //     return;
    //   }

    const G4StepPoint* poststep = STEP->GetPostStepPoint();
    G4String poststepName=poststep->GetPhysicalVolume()->GetName();

    if (fNbOM % 10000 == 0)
      {
        cout << endl;
        cout << "\r\033[40C" << fNbOM << " in OM ";
        cout << endl;
        cout.flush ();
      }
    fNbOM++;
    if (fRFile == NULL)
      return;
    if (fFastAnalysisOnly)
      return;

    const G4ThreeVector& globalPoint = poststep->GetPosition();
    const G4VTouchable*   touch = poststep->GetTouchable();
    const G4ThreeVector& localpoint=touch->GetHistory ()->GetTopTransform().TransformPoint(globalPoint);

    fHits.om_x = globalPoint.x ();
    fHits.om_y = globalPoint.y ();
    fHits.om_z = globalPoint.z ();
    fHits.om_t = globalPoint.theta ();
    fHits.om_p = globalPoint.phi ();
  }

    /**
     * @brief temp function to compute coinsidence by locally applying tts to hits
     * @remark This is a temporary function used for testing
     * @param ctime, the coincidence time
     */
    bool BAnalysisManager::DetectedEvent::isACoincidence(float ctime){
        bool coincidence = false;
        double sigma_tts = OM::TTSDist_(OM::rng_)/2.355; // FWHM /2*sqrt(2*ln(2))
        double t_jutter = 0.;
        std::normal_distribution<> TTSFDist_(t_jutter, sigma_tts*sigma_tts);  // sample from TTS FWHM dist. with time jutter
        for(auto& hita: hits){
            for(auto& hitb : hits){
                if(hita.om_id == hitb.om_id) // check on different OMs 
                    continue;
                
                double hitatime = hita.time + TTSFDist_(OM::rng_); // hit time + tts in ns
                double hitbtime = hitb.time + TTSFDist_(OM::rng_); // hit time + tts in ns
                if(abs(hitatime- hitbtime) < ctime){ // coincidence
                    coincidence = true;
                    break;
                }
            }
            if(coincidence)
                break;
        }
        return coincidence;
    }

    bool BAnalysisManager::DetectedEvent::checkCoincidence(float ctime){ // default coincidence time of 16 ns
        return false;
    }


    // /**
    //  * @brief temp function to compute coinsidence by locally applying tts to hits
    //  */
    // bool BAnalysisManager::DetectedEvent::isACoincidence(float ctime, float thr){
    //     BLactelDetector& det = BDetectorConstruction::GetIt()->getDetector();
    //     bool coincidence = false;
    //     for(auto& hita: daqEvents){
    //         for(auto& hitb : daqEvents){
    //             if( hita.om_id == hitb.om_id) // check different OMs on the same module 
    //                 continue;
                
    //             if(abs(hita.t - hitb.t) < ctime && abs(hita.amp) >= thr && abs(hitb.amp) >= thr){ // coincidence
    //                 coincidence = true;
    //                 break;
    //             }
    //         }
    //         if(coincidence)
    //             break;
    //     }
    //     return coincidence;
    // }



} // namespace lactel
