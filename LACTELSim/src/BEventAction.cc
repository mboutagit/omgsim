
#include "BEventAction.hh"
#include "G4DigiManager.hh"
#include "BWaveCatcher.hh"
#include "BRunManager.hh"

using namespace km3net;
using namespace tools;

namespace lactel
{
    BEventAction::BEventAction(){};
    BEventAction::~BEventAction(){};

    void BEventAction::BeginOfEventAction(const G4Event* anEvent){
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
        BWaveCatcher* wc1 = new BWaveCatcher(); // default two channels wavecatcher
        // OM om1("OMS0321");
        // OM om2("OMS0302");
        // wc1->plugOM(0, om1);
        // wc1->plugOM(1, om2);
        wc1->setInputOffset(1, 400);
        // wc1->startAcquisition();
        // wc1->getConfiguration().disableAllChannels();
        // // Waveform wf = wc1->getFakeWaveform(1); // get channel one wavefrom
        std::vector<ADC_record> oms0321 = wc1->digitizeEvent(0, ana->fDetectedEvent); // digitize on channel 1 wavefrom
        std::vector<ADC_record> oms0302 = wc1->digitizeEvent(1, ana->fDetectedEvent); // digitize on channel 1 wavefrom
        std::sort(oms0321.begin(), oms0321.end()); // sort waveform according to timestamp;
        // for(auto& record : oms0321){
        //     if(!record.samples.size())
        //         continue;
        //     TGraph gr = wc1->drawADCRecord(record);
        //     gr.SetName(("adc_" + std::to_string(event_id)).c_str());
        //     gr.Write();
        // }

        std::vector<Waveform> wfs_oms0321 = wc1->waveformFromADC(oms0321);
        std::vector<Waveform> wfs_oms0302 = wc1->waveformFromADC(oms0302);
        
        TGraph totA0 = wc1->drawWaveform(wc1->joinWavefroms(wfs_oms0321), wc1->getConfiguration().eventBufferDepth);
        totA0.SetName(("tot_Evt" + std::to_string(event_id) + "_Channel 0").c_str());
        totA0.SetLineColor(1);
        totA0.Write();

        TGraph totA1 = wc1->drawWaveform(wc1->joinWavefroms(wfs_oms0302), wc1->getConfiguration().eventBufferDepth);
        totA1.SetName(("tot_Evt" + std::to_string(event_id) + "_Channel 1").c_str());
        totA1.SetLineColor(2);
        totA1.Write();

        if(wfs_oms0321.size()){
            Waveform firstWf = *std::min_element(wfs_oms0321.begin(), wfs_oms0321.end());
            Waveform lastWf = *std::max_element(wfs_oms0321.begin(), wfs_oms0321.end());
            std::cout << "\r\033[10C" << "tmin: " << firstWf.timestamp 
                                        << " - tmax: " << lastWf.timestamp 
                                        << " - diff: " << lastWf.timestamp  - firstWf.timestamp 
                                        << " - max Amp ch0: " << (wc1->joinWavefroms(wfs_oms0321)).getMaxAmplitude()
                                        << " - max Amp ch2: " << wc1->joinWavefroms(wfs_oms0302).getMaxAmplitude()
                                        << std::endl;
        

            int i=0;
            TCanvas* canvas =  new TCanvas(("canvas_" + std::to_string(event_id)).c_str(), ("Event " + std::to_string(event_id)).c_str());
    
            std::vector<TGraph> graphs;
            for(auto& wf : wfs_oms0321){
                if(! wf.samples.size())
                    continue;

                // std::cout << "\r\033[20C" << "Event_" << event_id << " - wf timestamp: " << wf.timestamp << std::endl;

                TGraph grr = wc1->drawWaveform(wf);
                // TGraph* gr = &grr;
                grr.SetName(("wf_" + std::to_string(i) + "-Event_" + std::to_string(event_id)).c_str());
                graphs.push_back(grr);
                i++;
            }
            
            bool firstDraw=true;
            int c = 1;
            for(auto& gr : graphs){
                gr.SetLineColor(c);
                if(firstDraw){
                    gr.Draw("APL");
                    firstDraw=false;
                } 
                else
                    gr.Draw("PL");
                
                gr.Write();
                c++;
            }

            if(graphs.size())
                canvas->Write();

            if(canvas)
                delete canvas;

        }

        ana->EndOfEvent(anEvent);   // stop recording

    }

} // namespace lactel
