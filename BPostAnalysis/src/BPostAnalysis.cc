#include "BPostAnalysis.hh"
#include "DAQManager.hh"
#include "OM.hh"
#include "BWaveCatcher.hh"


using namespace tools;

namespace km3net
{
    std::vector<TCanvas*> getNPEPlot(TTree* tree, BAnalysisManager::DetectedEvent* evt, float exposure_time, float t_coin){
        std::vector<TCanvas*> vCanvas;
        float xmin=0.;
        float xmax=200.;
        float unitsPerBin=1.;
        const int nbins= (int)((xmax - xmin)/unitsPerBin);

        float xmin_amp=0.;
        float xmax_amp=2.4;
        float unitsPerBin_amp=10./1e3;
        const int nbins_amp= (int)((xmax_amp - xmin_amp)/unitsPerBin_amp);

        std::cout << "\r\033[45C" << " -- xmin: " << xmin 
                                << " -- xmax: " << xmax 
                                << " -- unitsPerBin: " << unitsPerBin 
                                << " -- nbins: " << nbins 
                                << std::endl;
        std::cout << "\r\033[45C" << " -- xmin_amp: " << xmin_amp 
                                << " -- xmax_amp: " << xmax_amp 
                                << " -- unitsPerBin_amp: " << unitsPerBin_amp 
                                << " -- nbins_amp: " << nbins_amp 
                                << std::endl;

        OM om1 = OM("OMS0321");
        OM om2 = OM("OMS0304");

        BWaveCatcher* wc1 = new BWaveCatcher("wc1");
        wc1->plugOM(0, om1);
        wc1->plugOM(1, om2);


        TH1F* npe_om1 = new TH1F("npe_om1", 
                                        "NPE; Number of PE; Rate for 1 PE per bin [Hz]",
                                        nbins, xmin, xmax
                                    );
        TH1F* npe_om2 = new TH1F("npe_om2", 
                                        "NPE; Number of PE; Rate for 1 PE per bin [Hz]",
                                        nbins, xmin, xmax
                                    );

        TH1F* npeCoin_om1 = new TH1F("npeCoin_om1", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om2 = new TH1F("npeCoin_om2", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
                               
        TH1F* npe_om1_cut = new TH1F("npe_om1_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npe_om2_cut = new TH1F("npe_om2_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om1_cut = new TH1F("npeCoin_om1_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );
        TH1F* npeCoin_om2_cut = new TH1F("npeCoin_om2_cut", 
                                        ("Coincidence #Deltat=" + std::to_string((int)t_coin) + " ns; Number of PE; Rate for 1 PE per bin [Hz]").c_str(),
                                        nbins, xmin, xmax
                                    );


        TH1F* amp_om1 = new TH1F("amp_om1", 
                                        "Amplitude; Amplitude [V]; Rate for 10 mV per bin [Hz]",
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* amp_om2 = new TH1F("amp_om2", 
                                        "Amplitude; Amplitude [V]; Rate for 10 mV per bin [Hz]",
                                        nbins_amp, xmin_amp, xmax_amp
                                    );

        TH1F* ampCoin_om1 = new TH1F("ampCoin_om1", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* ampCoin_om2 = new TH1F("ampCoin_om2", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        
        TH1F* ampCoin_om1_cut = new TH1F("ampCoin_om1_cut", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );
        TH1F* ampCoin_om2_cut = new TH1F("ampCoin_om2_cut", 
                                        ("Amplitude Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; Amplitude [V]; Rate for 10 mV per bin [Hz]").c_str(),
                                        nbins_amp, xmin_amp, xmax_amp
                                    );


        TH2F* npeCoin_oms_cut = new TH2F("npeCoin_oms_cut", 
                                        ("NPE - Coincidence #Deltat"+ std::to_string((int)t_coin) +" ns; nPE OM0; nPE OM1").c_str(),
                                        nbins, xmin, xmax,
                                        nbins, xmin, xmax
                                    );


        for(long long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);
            // apply DAQ
            std::vector<ADC_record> adcs_om1 = wc1->digitizeEvent(0, *evt);
            std::vector<ADC_record> adcs_om2 = wc1->digitizeEvent(1, *evt);

            std::vector<Waveform> wfs_om1 = wc1->waveformFromADC(adcs_om1);
            std::vector<Waveform> wfs_om2 = wc1->waveformFromADC(adcs_om2);

            Waveform wf_om1 = wc1->joinWavefroms(0, wfs_om1);
            Waveform wf_om2 = wc1->joinWavefroms(1, wfs_om2);

            TGraph totA0 = wc1->drawWaveform(wf_om1, wc1->getConfiguration().eventBufferDepth);
            TGraph totA1 = wc1->drawWaveform(wf_om2, wc1->getConfiguration().eventBufferDepth);
        

            double xmin_a0, xmax_a0, ymin_a0, ymax_a0;
            totA0.ComputeRange(xmin_a0, ymin_a0, xmax_a0, ymax_a0);

            double xmin_a1, xmax_a1, ymin_a1, ymax_a1;
            totA1.ComputeRange(xmin_a1, ymin_a1, xmax_a1, ymax_a1);

            amp_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
            amp_om2->Fill(abs(ymin_a1)/1e3, 1/exposure_time);

            int nhits_om1 = 0;
            int nhits_om2 = 0;

            for(auto& hit : evt->hits){
                    if(hit.om_id == 1)
                    nhits_om1++;

                    if(hit.om_id == 2)
                    nhits_om2++;
            }
            npe_om1->Fill(nhits_om1, 1/exposure_time);
            npe_om2->Fill(nhits_om2, 1/exposure_time);

            if(evt->isACoincidence(t_coin)){
                int nhits_om1 = 0;
                int nhits_om2 = 0;

                ampCoin_om1->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
                ampCoin_om2->Fill(abs(ymin_a1)/1e3, 1/exposure_time);

                for(auto& hit : evt->hits){
                    if(hit.om_id == 1)
                    nhits_om1++;

                    if(hit.om_id == 2)
                    nhits_om2++;
                }

                npeCoin_oms_cut->Fill(nhits_om1, nhits_om2, 1/exposure_time);

                npeCoin_om1->Fill(nhits_om1, 1/exposure_time);
                npeCoin_om2->Fill(nhits_om2, 1/exposure_time); 
                if(nhits_om1 >= 25 && nhits_om2 >= 25){ // coicidence and npe cut
                    npeCoin_om1_cut->Fill(nhits_om1, 1/exposure_time);
                    npeCoin_om2_cut->Fill(nhits_om2, 1/exposure_time); 
                    
                    ampCoin_om1_cut->Fill(abs(ymin_a0)/1e3, 1/exposure_time);
                    ampCoin_om2_cut->Fill(abs(ymin_a1)/1e3, 1/exposure_time);
                }         
            }
        }

        TCanvas* canvas = new TCanvas("canvas", "Coincidence", 800, 600);
        canvas->Divide(2);
        canvas->cd(1);
        gPad->SetLogy();
        npeCoin_om1->SetLineColor(1);
        npeCoin_om2->SetLineColor(2);
        npeCoin_om1->Draw("hist");
        npeCoin_om2->Draw("hist same");

        npeCoin_om1->SetStats(0);
        TLegend* leg = new TLegend(.6, .8, .8, .9);
        leg->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg->AddEntry(npeCoin_om2, "OM1 - Lower", "l");
        leg->Draw();

        canvas->cd(2);
        gPad->SetLogy();
        npeCoin_om1_cut->SetLineColor(1);
        npeCoin_om2_cut->SetLineColor(2);
        npeCoin_om1_cut->Draw("hist");
        npeCoin_om2_cut->Draw("hist same");

        npeCoin_om1_cut->SetStats(0);
        TLegend* leg_cut = new TLegend(.6, .8, .8, .9);
        leg_cut->AddEntry(npeCoin_om1_cut, "OM0 - Upper", "l");
        leg_cut->AddEntry(npeCoin_om2_cut, "OM1 - Lower", "l");
        leg_cut->Draw();


        TCanvas* canvas1 = new TCanvas("canvas1", "Amplitude", 800, 600);
        canvas1->Divide(2);
        canvas1->cd(1);

        gPad->SetLogy();
        ampCoin_om1->SetLineColor(1);
        ampCoin_om2->SetLineColor(2);
        ampCoin_om1->Draw("hist");
        ampCoin_om2->Draw("hist same");

        ampCoin_om1->SetStats(0);
        TLegend* leg1 = new TLegend(.6, .8, .8, .9);
        leg1->AddEntry(ampCoin_om1, "OM0 - Upper", "l");
        leg1->AddEntry(ampCoin_om2, "OM1 - Lower", "l");
        leg1->Draw();

        canvas1->cd(2);
        gPad->SetLogy();
        ampCoin_om1_cut->SetLineColor(1);
        ampCoin_om2_cut->SetLineColor(2);
        ampCoin_om1_cut->Draw("hist");
        ampCoin_om2_cut->Draw("hist same");

        ampCoin_om1_cut->SetStats(0);
        TLegend* leg1_cut = new TLegend(.6, .8, .8, .9);
        leg1_cut->AddEntry(ampCoin_om1_cut, "OM0 - Upper", "l");
        leg1_cut->AddEntry(ampCoin_om2_cut, "OM1 - Lower", "l");
        leg1_cut->Draw();




        // plot number of p.e.
        TCanvas* canvas_npe = new TCanvas("canvas_npe", "NPE", 800, 600);
        canvas_npe->SetLogy();
        canvas_npe->SetGrid();
        npe_om1->SetLineColor(1);
        npe_om2->SetLineColor(2);
        npe_om1->Draw("hist");
        npe_om2->Draw("hist same");

        npe_om1->SetStats(0);
        TLegend* leg_npe = new TLegend(.6, .8, .8, .9);
        leg_npe->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg_npe->AddEntry(npe_om2, "OM1 - Lower", "l");
        leg_npe->Draw();

        // plot amplitude without coincidence
        TCanvas* canvas_amp = new TCanvas("canvas_amp", "NPE", 800, 600);
        canvas_amp->SetLogy();
        canvas_amp->SetGrid();
        amp_om1->SetLineColor(1);
        amp_om2->SetLineColor(2);
        amp_om1->Draw("hist");
        amp_om2->Draw("hist same");

        amp_om1->SetStats(0);
        TLegend* leg_amp = new TLegend(.6, .8, .8, .9);
        leg_amp->AddEntry(npeCoin_om1, "OM0 - Upper", "l");
        leg_amp->AddEntry(amp_om1, "OM1 - Upper", "l");
        leg_amp->Draw();

        // plot amplitude with and without coincidence
        TCanvas* canvas_amp_wwCoin = new TCanvas("canvas_amp_wwCoin", "NPE", 800, 600);
        canvas_amp_wwCoin->SetLogy();
        amp_om1->SetLineColor(1);
        amp_om2->SetLineColor(2);
        ampCoin_om1->SetLineColor(3);
        ampCoin_om2->SetLineColor(4);
        amp_om1->Draw("hist");
        amp_om2->Draw("hist same");
        ampCoin_om1->Draw("hist same");
        ampCoin_om2->Draw("hist same");

        amp_om1->SetStats(0);
        TLegend* leg_amp_wwCoin = new TLegend(.6, .8, .8, .9);
        leg_amp_wwCoin->AddEntry(amp_om1, "OM0 - Upper", "l");
        leg_amp_wwCoin->AddEntry(amp_om2, "OM1 - Lower", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om1, "OM0 - Upper - coincidence", "l");
        leg_amp_wwCoin->AddEntry(ampCoin_om2, "OM1 - Lower - coincidence", "l");
        leg_amp_wwCoin->Draw();

        // plot npe with and without coincidence
        TCanvas* canvas_npe_wwCoin = new TCanvas("canvas_npe_wwCoin", "NPE", 800, 600);
        canvas_npe_wwCoin->SetLogy();
        npe_om1->SetLineColor(1);
        npe_om2->SetLineColor(2);
        npeCoin_om1->SetLineColor(3);
        npeCoin_om2->SetLineColor(4);
        npe_om1->Draw("hist");
        npe_om2->Draw("hist same");
        npeCoin_om1->Draw("hist same");
        npeCoin_om2->Draw("hist same");

        npe_om1->SetStats(0);
        TLegend* leg_npe_wwCoin = new TLegend(.6, .8, .8, .9);
        leg_npe_wwCoin->AddEntry(npe_om1, "OM0 - Upper", "l");
        leg_npe_wwCoin->AddEntry(npe_om2, "OM1 - Lower", "l");
        leg_npe_wwCoin->AddEntry(npeCoin_om1, "OM0 - Upper - coincidence", "l");
        leg_npe_wwCoin->AddEntry(npeCoin_om2, "OM1 - Lower - coincidence", "l");
        leg_npe_wwCoin->Draw();


        // plot npe OM1 vs OM2 without coincidence
        TCanvas* canvas_npeOms = new TCanvas("canvas_npeOms", "NPE", 800, 600);
        canvas_npeOms->SetLogy();
        npeCoin_oms_cut->SetLineColor(2);
        npeCoin_oms_cut->Draw("colz");

        npeCoin_oms_cut->SetStats(0);
        // TLegend* leg_amp = new TLegend(.6, .8, .8, .9);
        // leg_amp->AddEntry(npeCoin_om1, "OM", "l");
        // leg_amp->Draw();





        vCanvas.push_back(canvas_npeOms);
        vCanvas.push_back(canvas);
        vCanvas.push_back(canvas1);
        vCanvas.push_back(canvas_amp);
        vCanvas.push_back(canvas_npe);
        vCanvas.push_back(canvas_amp_wwCoin);
        vCanvas.push_back(canvas_npe_wwCoin);
        return vCanvas;

    }

    TCanvas* getDAQParameter(TTree* tree, BAnalysisManager::DetectedEvent* evt, std::string par){
        
        if(par != "Amplitude")
            return new TCanvas;

        OM om1 = OM("OMS0321");
        OM om2 = OM("OMS0304");

        BWaveCatcher* wc1 = new BWaveCatcher("wc1");
        wc1->plugOM(0, om1);
        wc1->plugOM(1, om2);

        for(long long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);

            std::vector<ADC_record> adcs_om1 = wc1->digitizeEvent(0, *evt);
            std::vector<ADC_record> adcs_om2 = wc1->digitizeEvent(1, *evt);

            std::vector<Waveform> wfs_om1 = wc1->waveformFromADC(adcs_om1);
            std::vector<Waveform> wfs_om2 = wc1->waveformFromADC(adcs_om2);

            Waveform wf_om1 = wc1->joinWavefroms(0, wfs_om1);
            Waveform wf_om2 = wc1->joinWavefroms(1, wfs_om2);

            TGraph totA0 = wc1->drawWaveform(wf_om1, wc1->getConfiguration().eventBufferDepth);
            TGraph totA1 = wc1->drawWaveform(wf_om2, wc1->getConfiguration().eventBufferDepth);
        

            double xmin_a0, xmax_a0, ymin_a0, ymax_a0;
            totA0.ComputeRange(xmin_a0, ymin_a0, xmax_a0, ymax_a0);

            double xmin_a1, xmax_a1, ymin_a1, ymax_a1;
            totA1.ComputeRange(xmin_a1, ymin_a1, xmax_a1, ymax_a1);

            if(ymin_a0==0 && ymin_a1)
                std::cout << "\r\033[44C" << "amps: (" << ymin_a0 << "," << ymin_a1 << ")" << std::endl;


        }
        return new TCanvas;
    }
} // namespace km3net
