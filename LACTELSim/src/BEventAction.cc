
#include "BEventAction.hh"
#include "G4DigiManager.hh"
#include "BWaveCatcher.hh"
#include "BRunManager.hh"
#include "BDetectorConstruction.hh"
# include "BLogger.hh"

#include "G4SystemOfUnits.hh"



using namespace km3net;
using namespace tools;
using namespace utils;

namespace lactel
{
    BEventAction::BEventAction(){};
    BEventAction::~BEventAction(){};

    double BEventAction::globalTime = 0. *ns;

    void BEventAction::BeginOfEventAction(const G4Event* anEvent){

        // Total muon rate hitting your generation surface
        G4double muonRate = 500. * hertz;  // <-- YOUR computed rate
        G4double dt = -std::log(G4UniformRand()) / muonRate;
        globalTime += dt;
        // std::cout << "\r\033[20C \033[1;35m" << "Global time: " << globalTime << "\033[0m" << std::endl;

        if(! BDetectorConstruction::GetIt()->IsRunning()){
            // BLogger::getInstance()->log(BLogger::Level::INFO, "Event " +  std::to_string(anEvent->GetEventID()) + " - The Detector is not running ..");
            // std::cout << "\r\033[10C" << "Event " +  std::to_string(anEvent->GetEventID()) + " - The Detector is not running .." << std::endl;
            EndOfEventAction(anEvent);
            return;
        }
        KM3EventAction::BeginOfEventAction(anEvent);
        BAnalysisManager::getIt()->StartOfEvent(anEvent);
    }
    
    void BEventAction::EndOfEventAction(const G4Event* anEvent){
        G4int event_id       = anEvent->GetEventID();
        //analysis
        KM3EventAction::EndOfEventAction (anEvent);

        BAnalysisManager* ana = BAnalysisManager::getIt();

        // for(auto& hit : ana->fDetectedEvent.hits){
        //     // std::cout << "\r\033[10C" << "hit charge: " << hit.charge << std::endl;
        // }

        //Digi
        // G4DigiManager* digiMan = G4DigiManager::GetDMpointer();
        // digiMan->Digitize("DigitizerModule");  
        // BWaveCatcher* wc1 = new BWaveCatcher("Module1_wc"); // default two channels wavecatcher
        // OM om1("OMS0321");
        // OM om2("OMS0302");
        // OM om3("OMS0298"); // for test
        // // om1.uploadOMValuesFormDB();
        // // om2.uploadOMValuesFormDB();
        // wc1->plugOM(0, om1);
        // wc1->plugOM(1, om2);
        // // wc1->getConfiguration().disableAllChannels();
        // // // Waveform wf = wc1->getFakeWaveform(1); // get channel one wavefrom
        // std::vector<ADC_record> oms0321 = wc1->digitizeEvent(0, ana->fDetectedEvent); // digitize on channel 1 wavefrom
        // std::vector<ADC_record> oms0302 = wc1->digitizeEvent(1, ana->fDetectedEvent); // digitize on channel 1 wavefrom
        // std::sort(oms0321.begin(), oms0321.end()); // sort waveform according to timestamp;
        // // for(auto& record : oms0321){
        // //     if(!record.samples.size())
        // //         continue;
        // //     TGraph gr = wc1->drawADCRecord(record);
        // //     gr.SetName(("adc_" + std::to_string(event_id)).c_str());
        // //     // gr.Write();
        // // }
        // std::cout << "\r\033[40C" << "Event: " << anEvent->GetEventID() << std::endl;

        // std::vector<Waveform> wfs_oms0321 = wc1->waveformFromADC(oms0321);
        // std::vector<Waveform> wfs_oms0302 = wc1->waveformFromADC(oms0302);
        
        // Waveform wf0 = wc1->joinWavefroms(0, wfs_oms0321);
        //     TGraph totA0 = wc1->drawWaveform(wf0, wc1->getConfiguration().eventBufferDepth);
        //     totA0.SetName(("totA_evt" + std::to_string(event_id) + "_Channel 0").c_str());
        //     totA0.SetLineColor(1);
        //     if(wf0.samples.size() > 0)
        //         totA0.Write();


        // Waveform wf1 = wc1->joinWavefroms(1, wfs_oms0302);
        // TGraph totA1 = wc1->drawWaveform(wf1, wc1->getConfiguration().eventBufferDepth);
        // totA1.SetName(("totA_evt" + std::to_string(event_id) + "_Channel 1").c_str());
        // totA1.SetLineColor(2);
        // if(wf1.samples.size() > 0)
        //     totA1.Write();

        // double xmin_a0, xmax_a0, ymin_a0, ymax_a0;
        // totA0.ComputeRange(xmin_a0, ymin_a0, xmax_a0, ymax_a0);

        // double xmin_a1, xmax_a1, ymin_a1, ymax_a1;
        // totA1.ComputeRange(xmin_a1, ymin_a1, xmax_a1, ymax_a1);

        // ana->fDetectedEvent.a0=ymin_a0;
        // ana->fDetectedEvent.a1=ymin_a1;

        // std::cout << "\r\033[10C" << "wf0: " << wf0.getAbsoluteMaxAmplitude() << " - " << wf0.signalTime(wc1->getConfiguration().triggerThresholds[0], 1/wc1->getConfiguration().samplingFrequency)[0] 
        //                                                                         << " - " << wf0.signalTime(wc1->getConfiguration().triggerThresholds[0], 1/wc1->getConfiguration().samplingFrequency)[1]
        //                                                                         << " - " << wf0.integrate(1/wc1->getConfiguration().samplingFrequency, 10, 150)  
        //                                                                         << std::endl;
        // std::cout << "\r\033[10C" << "wf1: " << wf1.getAbsoluteMaxAmplitude() << " - " << wf1.signalTime(wc1->getConfiguration().triggerThresholds[1], 1/wc1->getConfiguration().samplingFrequency)[0] 
        //                                                                         << " - " << wf1.signalTime(wc1->getConfiguration().triggerThresholds[1], 1/wc1->getConfiguration().samplingFrequency)[1]  << std::endl;

        // // ana->fDaqEvent.addEvent(0, anEvent->GetEventID(), om1.getOMID(), ymin_a0, 0);
        // // ana->fDaqEvent.addEvent(1, anEvent->GetEventID(), om2.getOMID(), ymin_a1, 0);

        // ana->fDetectedEvent.daqEvent.push_back(BAnalysisManager::DAQEvent(0, anEvent->GetEventID(), 
        //     om1.getOMID(), ymin_a0, BAnalysisManager::Hit::getMeanTimeforOM(ana->fDetectedEvent.hits, 1)));
        // ana->fDetectedEvent.daqEvent.push_back(BAnalysisManager::DAQEvent(1, anEvent->GetEventID(), 
        //     om2.getOMID(), ymin_a1, BAnalysisManager::Hit::getMeanTimeforOM(ana->fDetectedEvent.hits, 2)));

        // ana->fDetectedEvent.checkCoincidences(10.);
        // if(wfs_oms0321.size()){
        // //     Waveform firstWf = *std::min_element(wfs_oms0321.begin(), wfs_oms0321.end());
        // //     Waveform lastWf = *std::max_element(wfs_oms0321.begin(), wfs_oms0321.end());
        // //     // std::cout << "\r\033[10C" << "tmin: " << firstWf.timestamp 
        // //     //                             << " - tmax: " << lastWf.timestamp 
        // //     //                             << " - diff: " << lastWf.timestamp  - firstWf.timestamp 
        // //     //                             << " - max Amp ch0: " << wc1->joinWavefroms(wfs_oms0321).getAbsoluteMaxAmplitude()
        // //     //                             << " - max Amp ch2: " << wc1->joinWavefroms(wfs_oms0302).getAbsoluteMaxAmplitude()
        // //     //                             << " - a0: " << ymin_a0
        // //     //                             << " - a1: " << ymin_a1
        // //     //                             << std::endl;
        

        //     TCanvas* canvas =  new TCanvas(("canvas_Ch0_evt" + std::to_string(event_id)).c_str(), ("Event " + std::to_string(event_id)).c_str());
        //     int i=0;
        //     std::vector<TGraph> graphs;
        //     for(auto& wf : wfs_oms0321){
        //         if(! wf.samples.size())
        //             continue;
        //         TGraph grr = wc1->drawWaveform(wf);
        //         grr.SetName(("wf_" + std::to_string(i) + "-Event_" + std::to_string(event_id)).c_str());
        //         graphs.push_back(grr);
        //         i++;
        //     }
            
        //     bool firstDraw=true;
        //     int c = 1;
        //     for(auto& gr : graphs){
        //         gr.SetLineColor(c);
        //         if(firstDraw){
        //             gr.Draw("APL");
        //             firstDraw=false;
        //         } 
        //         else
        //             gr.Draw("PL");
                
        //         gr.Write();
        //         c++;
        //     }

        //     if(graphs.size())
        //         canvas->Write();

        //     if(canvas)
        //         delete canvas;
        // }

        // if(wfs_oms0302.size()){
        //     TCanvas* canvas1 =  new TCanvas(("canvas_Ch1_evt"+ std::to_string(event_id)).c_str(), ("Event " + std::to_string(event_id)).c_str());
        //     int i=0;
        //     std::vector<TGraph> graphs1;
        //     for(auto& wf : wfs_oms0302){
        //         if(! wf.samples.size())
        //             continue;
        //         TGraph grr = wc1->drawWaveform(wf);
        //         grr.SetName(("wf_" + std::to_string(i) + "-Event_" + std::to_string(event_id)).c_str());
        //         graphs1.push_back(grr);
        //         i++;
        //     }

        //     bool firstDraw=true;
        //     int c1 = 1;
        //     for(auto& gr : graphs1){
        //         gr.SetLineColor(c1);
        //         if(firstDraw){
        //             gr.Draw("APL");
        //             firstDraw=false;
        //         } 
        //         else
        //             gr.Draw("PL");
                
        //         // gr.Write();
        //         c1++;
        //     }

        //     if(graphs1.size())
        //         canvas1->Write();

        //     if(canvas1)
        //         delete canvas1;
        // }

        ana->EndOfEvent(anEvent);   // stop recording
    }

} // namespace lactel
